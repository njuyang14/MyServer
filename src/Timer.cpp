#include "Timer.h"
#include <sys/time.h>

Timer::Timer(std::shared_ptr<Channel> channel, int timeout)
{
    channel_ = channel;

    struct timeval now;
    gettimeofday(&now, NULL);
    expiredTime_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
}

Timer::~Timer()
{}

void Timer::update()
{}

bool Timer::isOutOfTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    size_t curTime = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
    if (curTime < expiredTime_) {
        return false;
    } else {
        return true;
    }
}

TimerQueue::TimerQueue()
{}

TimerQueue::~TimerQueue()
{}

void TimerQueue::addTimer(std::shared_ptr<Channel> channel, int timeout)
{
    timerQueue_.push(std::shared_ptr<Timer>(new Timer(channel, timeout)));
}

void TimerQueue::handleExpiredEvent()
{
    while (!timerQueue_.empty()) {
        if (timerQueue_.top()->isOutOfTime()) {
            timerQueue_.pop();
        } else {
            break; // 堆顶没有超时事件则堆中都没有
        }
    }
}