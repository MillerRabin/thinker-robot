#ifndef arm_queue_h
#define arm_queue_h

#include "Arduino.h"
#include "armParams.h"

//------ArmQueueItem------

class ArmQueueItem {
    private:
        const bool isValid();
    public:
        const double shoulderZAngle;
        const double shoulderYAngle;
        const double elbowYAngle;
        const double wristYAngle;         
        const double clawXAngle;
        const double clawAngle; 
        const unsigned int iterations;        
        const bool valid;
        ArmQueueItem(const double shoulderYAngle = NAN, const double shoulderZAngle = NAN, const double elbowYAngle = NAN, const double wristYAngle = NAN, const double clawXAngle = NAN, const double clawAngle = NAN, const unsigned int iterations = 100);
};

class ArmQueue {
    private:
        ArmQueueItem* storage[COMMAND_QUEUE_SIZE];
        unsigned int head = 0;
        unsigned int tail = 0;
        unsigned int inc(const unsigned int val);        
    public: 
        const bool isFull();
        const unsigned int size();
        ArmOperationResult enqueue(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, const double clawAngle, const unsigned int iterations);
        ArmQueueItem* dequeue();
};

#endif