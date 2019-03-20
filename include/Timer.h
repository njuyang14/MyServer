#ifndef _TIMER_H_
#define _TIEMR_H_

#include "Channel.h"
#include <memory>
#include <queue>

class Timer {
public:
    Timer(std::shared_ptr<Channel> channel, int timeout);
    ~Timer();
    void update();
    bool isOutOfTime();
    size_t getExpiredTime() const
    { 
        return expiredTime_; 
    } 
private:
    size_t expiredTime_;
    std::shared_ptr<Channel> channel_;
};

struct TimerCmp {
    bool operator()(std::shared_ptr<Timer> &a, std::shared_ptr<Timer> &b) const
    {
        return a->getExpiredTime() > b->getExpiredTime();
    }
};

class TimerQueue {
public:
    TimerQueue();
    ~TimerQueue();
    void addTimer(std::shared_ptr<Channel> channel, int timeout);
    void handleExpiredEvent();
private:
    std::priority_queue<std::shared_ptr<Timer>, std::vector<std::shared_ptr<Timer> >, TimerCmp> timerQueue;
};

#endif