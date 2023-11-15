#include "engineHandler.h"

void EngineHandler::sendSuccess(AsyncWebServerRequest *request, Position pos, ArmDetectorsData data, std::string strategyType) {
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();

  root["strategy-type"] = strategyType.c_str();
  if (pos.getLastError() == ARM_OPERATION_SUCCESS) {
    JsonObject &pAngles = root.createNestedObject("target-physical-angles");
    pAngles["claw-angle"] = pos.claw.getAngle();
    pAngles["claw-angle-x"] = pos.claw.getXAngle();
    pAngles["claw-angle-y"] = pos.claw.getYAngle();
    pAngles["claw-angle-z"] = pos.claw.getZAngle();
    pAngles["wrist-angle-x"] = pos.wrist.getXAngle();
    pAngles["wrist-angle-y"] = pos.wrist.getYAngle();
    pAngles["wrist-angle-z"] = pos.wrist.getZAngle();
    pAngles["elbow-angle-x"] = pos.elbow.getXAngle();
    pAngles["elbow-angle-y"] = pos.elbow.getYAngle();
    pAngles["elbow-angle-z"] = pos.elbow.getZAngle();
    pAngles["shoulder-angle-x"] = pos.shoulder.getXAngle();
    pAngles["shoulder-angle-y"] = pos.shoulder.getYAngle();
    pAngles["shoulder-angle-z"] = pos.shoulder.getZAngle();

    JsonObject &lAngles = root.createNestedObject("target-logical-angles");
    lAngles["claw-angle"] = pos.claw.getAngle();
    lAngles["claw-angle-x"] = float(pos.claw.getXRad() / PI) * 180;
    lAngles["claw-angle-y"] = float(pos.claw.getYRad() / PI) * 180;
    lAngles["claw-angle-z"] = float(pos.claw.getZRad() / PI) * 180;
    lAngles["wrist-angle-y"] = float(pos.wrist.YRad / PI) * 180;
    lAngles["elbow-angle-y"] = float(pos.elbow.YRad / PI) * 180;
    lAngles["shoulder-angle-y"] = float(pos.shoulder.YRad / PI) * 180;
    lAngles["shoulder-angle-z"] = float(pos.shoulder.ZRad / PI) * 180;

    JsonObject &positions = root.createNestedObject("target-position");
    positions["claw-x"] = pos.getX();
    positions["claw-y"] = pos.getY();
    positions["claw-z"] = pos.getZ();
    positions["wrist-x"] = pos.shoulder.x + pos.elbow.x + pos.wrist.x;
    positions["wrist-y"] = pos.shoulder.y + pos.elbow.y + pos.wrist.y;
    positions["wrist-z"] = pos.shoulder.z + pos.elbow.z + pos.wrist.z + BASE_HEIGHT;
    positions["elbow-x"] = pos.shoulder.x + pos.elbow.x;
    positions["elbow-y"] = pos.shoulder.y + pos.elbow.y;
    positions["elbow-z"] = pos.shoulder.z + pos.elbow.z + BASE_HEIGHT;
    positions["shoulder-x"] = pos.shoulder.x;
    positions["shoulder-y"] = pos.shoulder.y;
    positions["shoulder-z"] = pos.shoulder.z + BASE_HEIGHT;

    JsonObject &detectors = root.createNestedObject("detectors");
    detectors["baseMPUError"] = data.baseMPUError;
    detectors["clawRangeError"] = data.clawRangeError;
    detectors["baseGValues-x"] = data.baseGValues.x;
    detectors["baseGValues-y"] = data.baseGValues.y;
    detectors["baseGValues-z"] = data.baseGValues.z;
    detectors["baseGyroValues-x"] = data.baseGyroValues.x;
    detectors["baseGyroValues-y"] = data.baseGyroValues.y;
    detectors["baseGyroValues-z"] = data.baseGyroValues.z;
    detectors["baseMagValues-x"] = data.baseMagValues.x;
    detectors["baseMagValues-y"] = data.baseMagValues.y;
    detectors["baseMagValues-z"] = data.baseMagValues.z;
    detectors["baseMPUTemperature"] = data.baseMPUTemperature;
    detectors["baseResultantG"] = data.baseResultantG;
    detectors["baseAngles-x"] = data.baseAngles.x;
    detectors["baseAngles-y"] = data.baseAngles.y;
    detectors["baseAngles-z"] = data.baseAngles.z;
    detectors["clawRange"] = data.clawRange;
    detectors["baseAzimuth"] = data.getBaseAzimuth();
  }

  ArmOperationResult res = pos.getLastError();
  JsonObject &stats = root.createNestedObject("stats");
  stats["arm-operation-queued"] = ArmEngines::queue.size();

  if (res != ARM_OPERATION_SUCCESS)
  {
    JsonObject &error = root.createNestedObject("error");
    error["code"] = (int)res;
    error["text"] = pos.getLastErrorText().c_str();
  }

  root.printTo(*response);
  request->send(response);
}

void EngineHandler::sendError(AsyncWebServerRequest *request, const char *message) {
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["error"] = message;
  root.printTo(*response);
  request->send(response);
}

EngineHandler::EngineHandler() {
  engineHandler = new AsyncCallbackJsonWebHandler("/move", [this](AsyncWebServerRequest *request, JsonVariant &json) mutable {    
    try {
      JsonObject& jsonObj = json.as<JsonObject>();
      Position pos = armEngines.set(jsonObj);
      ArmDetectorsData data = armDetectors.getData();
      sendSuccess(request, pos, data, "Direct move");
    } catch (const std::exception& e) {
      sendError(request, e.what());
    } catch (...) {
      sendError(request, "Unknown Error");
    } 
  });

  positionHandler = new AsyncCallbackJsonWebHandler("/position", [this](AsyncWebServerRequest *request, JsonVariant &json) mutable {    
    try {
      JsonObject& jsonObj = json.as<JsonObject>();        
      Position pos = armEngines.getPosition();

      const float clawX = ArmEngines::getfloatDef(jsonObj, "claw-x", pos.getX());
      const float clawY = ArmEngines::getfloatDef(jsonObj, "claw-y", pos.getY());
      const float clawZ = ArmEngines::getfloatDef(jsonObj, "claw-z", pos.getZ());
      const float clawRadX = getRad(ArmEngines::getfloat(jsonObj, "claw-angle-x"));
      const float clawRadY = getRad(ArmEngines::getfloat(jsonObj, "claw-angle-y"));
      const float clawRadZ = getRad(ArmEngines::getfloat(jsonObj, "claw-angle-z"));
      const float clawRad = getRad(ArmEngines::getfloat(jsonObj, "claw-angle"));

      Target target = Target({clawX}, {clawY}, {clawZ}, {clawRadX}, {clawRadY}, {clawRadZ});

      Strategy moveStrategy(
        pos,
        target,
        ArmEngines::getUintDef(jsonObj, "clawAngle", pos.claw.getAngle()),        
        ArmEngines::getUintDef(jsonObj, "speed", DEFAULT_SPEED),
        ArmEngines::getUintDef(jsonObj, "post-delay", DEFAULT_POST_DELAY)            
      );            
      Position rPos = armEngines.applyStrategy(moveStrategy);
      ArmDetectorsData data = armDetectors.getData();
      sendSuccess(request, rPos, data, moveStrategy.getType());
    } catch (const std::exception& e) {
      sendError(request, e.what());
    } catch (...) {
      sendError(request, "Unknown Error");
    } 
  });

  Server.addHandler(engineHandler);
  Server.addHandler(positionHandler);
}

EngineHandler::~EngineHandler() {
  delete engineHandler;
  delete positionHandler;
}