#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include <pthread.h>

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    bool isInLoopThread()
    {
        return threadId_ == 0;
    }
private:
    bool looping_;
    const pthread_t threadId_;
};

#endif