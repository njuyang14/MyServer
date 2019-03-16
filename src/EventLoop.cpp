#include "EnventLoop.h"

#include <stdio.h>

__thread EventLoop *t_loopInThisThread = 0;

EventLoop::EventLoop() : looping_(false),
    threadId_(0)
{
    if (t_loopInThisThread != 0) {
        printf("thread has eventloop already!\n");
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{
    printf("this is main loop, pid=%lu\n",threadId_);
}