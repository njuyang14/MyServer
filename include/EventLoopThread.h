#ifndef _EVENT_LOOP_THREAD_H_
#define _EVENT_LOOP_THREAD_H_

#include "EventLoop.h"
#include "Thread.h"
#include "MutexLock.h"
#include "Condition.h"

class EventLoopThread {
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop *startLoop();

private:
    void threadFunc();
    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};

#endif
