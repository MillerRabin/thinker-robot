#ifndef target_h
#define target_h

class TargetRange {
  public:
    const double min;
    const double max;
    TargetRange(const double min, const double max) : min(min), max(max) {};
};



class Target {
  private:

  public:
    Target(            
      TargetRange xRad,
      TargetRange yRad,
      TargetRange zRad,
      const double x,
      const double y,
      const double z
    );
};

#endif