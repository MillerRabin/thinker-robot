#include "armQueue.h"
#include "armParams.h"

//--------ArmQueueItem------------

ArmQueueItem::ArmQueueItem(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, const double clawAngle, const unsigned int iterations, const unsigned int postDelay, const unsigned int iterationDelay) :     
    shoulderZAngle(shoulderZAngle),
    shoulderYAngle(shoulderYAngle),            
    elbowYAngle(elbowYAngle),
    wristYAngle(wristYAngle),
    clawXAngle(clawXAngle),
    clawAngle(clawAngle),
    iterations(iterations),
    postDelay(postDelay),
    iterationDelay(iterationDelay),
    valid(isValid())
{};

const bool ArmQueueItem::isValid() {
    return (!isnan(shoulderZAngle)  &&
            !isnan(shoulderYAngle)  &&
            !isnan(elbowYAngle)     &&
            !isnan(wristYAngle)     &&
            !isnan(clawXAngle)      &&
            !isnan(clawAngle)       &&
            (postDelay > 0)         &&
            (postDelay < 10000)     &&
            (iterationDelay > 1)    &&
            (iterationDelay <= 100) &&
            (iterations > 0)        &&
            (iterations <= 100));
}

//----------ArmQueue-----------
ArmQueue::ArmQueue() :     
    syncSemaphore(xSemaphoreCreateBinary())
{    
    //xSemaphoreGive(syncSemaphore);
}

unsigned int ArmQueue::inc(const unsigned int val) {
    unsigned int res = val + 1;
    if (res > (COMMAND_QUEUE_SIZE - 1)) 
        res = 0;
    return res;
}

const bool ArmQueue::isFull() {
    const unsigned int test = inc(tail);
    if (test == head) return true;
    return false;
}

ArmOperationResult ArmQueue::enqueue(
    const double shoulderYAngle, 
    const double shoulderZAngle, 
    const double elbowYAngle, 
    const double wristYAngle, 
    const double clawXAngle, 
    const double clawAngle, 
    const unsigned int iterations,
    const unsigned int postDelay,
    const unsigned int iterationDelay
) {    
    if (isFull()) {        
        return ERROR_COMMAND_QUEUE_IS_FULL;    
    }
    ArmQueueItem* item = new ArmQueueItem(
                                shoulderYAngle, 
                                shoulderZAngle, 
                                elbowYAngle, 
                                wristYAngle, 
                                clawXAngle, 
                                clawAngle, 
                                iterations,
                                postDelay,
                                iterationDelay
                         );                             
    if (!item->valid) {                
        delete item;        
        return ERROR_COMMAND_QUEUE_ITEM_IS_INVALID;
    }
    
        
    portENTER_CRITICAL(&qMux);
        storage[tail] = item;
        tail = inc(tail);    
    portEXIT_CRITICAL(&qMux);
    
    xSemaphoreGive(syncSemaphore);
    return ARM_OPERATION_SUCCESS;                    
}

ArmQueueItem* ArmQueue::dequeue() {    
    if (head == tail)
        xSemaphoreTake(syncSemaphore, portMAX_DELAY);    
        
    portENTER_CRITICAL(&qMux);
    ArmQueueItem* res = storage[head];    
    head = inc(head);
    portEXIT_CRITICAL(&qMux);
        
    if (head == tail) {    
        xSemaphoreTake(syncSemaphore, portMAX_DELAY);            
    } else {
        xSemaphoreGive(syncSemaphore);
    }        
    return res;
}

const unsigned int ArmQueue::size() {      
    if (tail > head) 
        return COMMAND_QUEUE_SIZE - (COMMAND_QUEUE_SIZE - tail + head);    
    if (tail < head) 
        return COMMAND_QUEUE_SIZE - head + tail;
    return 0;
}