#ifndef target_h
#define target_h

  class Range {
    public: 
      const float from;
      const float to;
      Range(const float from, const float to) : from(from), to(to) {};
      Range(const float value) : from(value), to(value) {};
      const float middle();
  };

  class Target {
    public:        
      Range x;
      Range y;
      Range z;
      Range angleX;
      Range angleY;
      Range angleZ;
      Target(Range x, 
             Range y, 
             Range z, 
             Range angleX, 
             Range angleY, 
             Range angleZ) : x(x), y(y), z(z), angleX(angleX), angleY(angleY), angleZ(angleZ) {};
  };
#endif