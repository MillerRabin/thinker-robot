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
        const double clawZAngle;
        const double clawAngle; 
        const unsigned int iterations;
        const unsigned int postDelay;
        const unsigned int iterationDelay;
        const bool valid;
        
        ArmQueueItem(
            const double shoulderYAngle = NAN, 
            const double shoulderZAngle = NAN, 
            const double elbowYAngle = NAN, 
            const double wristYAngle = NAN, 
            const double clawXAngle = NAN, 
            const double clawZAngle = NAN, 
            const double clawAngle = NAN, 
            const unsigned int iterations = DEFAULT_ITERATIONS, 
            const unsigned int postDelay = DEFAULT_POST_DELAY,
            const unsigned int iterationDelay = DEFAULT_ITERATION_DELAY
        );
};

class ArmQueue {
    private:
        ArmQueueItem* storage[COMMAND_QUEUE_SIZE];
        unsigned int head = 0;
        unsigned int tail = 0;
        unsigned int inc(const unsigned int val);     
        const volatile SemaphoreHandle_t syncSemaphore;           
        portMUX_TYPE qMux = portMUX_INITIALIZER_UNLOCKED;
    public: 
        ArmQueue();
        const bool isFull();
        const unsigned int size();
        ArmOperationResult enqueue(
            const double shoulderYAngle, 
            const double shoulderZAngle, 
            const double elbowYAngle, 
            const double wristYAngle, 
            const double clawXAngle, 
            const double clawZAngle, 
            const double clawAngle, 
            const unsigned int iterations, 
            const unsigned int postDelay, 
            const unsigned int iterationDelay);
        ArmQueueItem* dequeue();        
};

#endif