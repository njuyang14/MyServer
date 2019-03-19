#include "EnventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "Util.h"
#include <stdio.h>
#include <sys/eventfd.h>

__thread EventLoop *t_loopInThisThread = NULL;

int createEventfd()
{
    int eventFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (eventFd < 0) {
        printf("fail in eventfd!\n");
        // abort();
    }
    return eventFd;
}

EventLoop::EventLoop() : looping_(false),
    quit_(true),
    threadId_(0),
    poller_(new Epoll()),
    callingPendingFunctor_(false),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_))
{
    if (t_loopInThisThread != NULL) {
        printf("thread has eventloop already!\n");
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    if (looping_ == true) {
        printf("EventLoop stops error!\n");
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
        doPendingFunctors();
    }
    
    printf("EventLoop stop, pid=%lu\n", threadId_);
    looping_ = false;
}

void EventLoop::doPendingFunctors()
{
    std::vector<std::function<void()>> functors;
    callingPendingFunctor_ = true;
    {
        // MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctorList_);
    }

    for (int i = 0; i < functors.size(); i++) {
        functors[i]();
    }
    callingPendingFunctor_ = false;
}

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

void EventLoop::quit()
{
    quit_ = true;
    wakeup();
}

void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
    
}

void EventLoop::runInLoop(const std::function<void()> &func)
{
    if (isInLoopThread()) {
        func();
    } else {
        queueInLoop(func);
    }
}

void EventLoop::queueInLoop(const std::function<void()> &func)
{
    {
        // MutexLockGuard lock(mutex_);
        pendingFunctorList_.push_back(func);
    }

    if (!isInLoopThread() || callingPendingFunctor_) {
        wakeup();
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = writen(wakeupFd_, (char*)(&one), sizeof one);
    if (n != sizeof one)
    {
        printf("EventLoop::wakeup() writes %d bytes instead of 8", n);
    }
}

