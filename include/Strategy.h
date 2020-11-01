#ifndef strategy_h
#define strategy_h


#include <Position.h>
#include <Arduino.h>

class EngineControl {
    public:
        EngineControl(unsigned int engine, unsigned int angle);
        unsigned int engine;
        unsigned int angle;
};

class Strategy {
    private:
        void case1(Position pos, double x);
    public:
        Strategy(Position pos, double x, double y, double z);
        std::vector<EngineControl> sequence;       
};

#endif
