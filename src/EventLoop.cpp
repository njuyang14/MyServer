#include "EnventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <stdio.h>

__thread EventLoop *t_loopInThisThread = NULL;

EventLoop::EventLoop() : looping_(false),
    quit_(true),
    threadId_(0),
    poller_(new Epoll())
{
    if (t_loopInThisThread != NULL) {
        printf("thread has eventloop already!\n");
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    if (looping_ == false) {
        printf("EventLoop stops error!");
    }
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    looping_ = true;
    quit_ = false;
    printf("this is main loop, pid=%lu\n", threadId_);
    while (quit_ != true) {
        activeChannel_.clear();
        activeChannel_ = poller_->poll();
        for (ChannelList::iterator it = activeChannel_.begin();
            it != activeChannel_.end(); it++) {
            (*it)->handleEvent();
        }
    }

    printf("EventLoop stop, pid=%lu\n", threadId_);
    looping_ = false;
}

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

void EventLoop::quit()
{
    quit_ = true;
    // wakeup();
}

void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
    
}

