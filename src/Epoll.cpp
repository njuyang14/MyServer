#include "Epoll.h"
#include "Timer.h"

const int EVENTS_NUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll() : events_(EVENTS_NUM),
    timerQueue_(new TimerQueue())
{
    epollFd_ = epoll_create1(EPOLL_CLOEXEC);
}

Epoll::~Epoll()
{
}

Epoll::ChannelList Epoll::poll()
{
    int numEvents = epoll_wait(epollFd_, 
                                &(*events_.begin()), 
                                events_.size(),
                                EPOLLWAIT_TIME);
    printf("Epoll::poll event num = %d!\n", numEvents);
    ChannelList actChannel;
    for (int i = 0; i < numEvents; i++) {
        std::shared_ptr<Channel> channel = channelMap_[events_[i].data.fd];

        channel->setRevents(events_[i].events);
        actChannel.push_back(std::shared_ptr<Channel>(channel));
    }
    return actChannel;
}

void Epoll::updateChannel(std::shared_ptr<Channel> channel)
{
    epollAdd(channel, 5);
    printf("update interested event\n");
}

void Epoll::epollAdd(std::shared_ptr<Channel> req, int timeout)
{
    int fd = req->getFd();
    struct epoll_event event;
    event.data.fd = fd; // channel的描述符
    event.events = req->getEvents(); // channel的事件

    if (req != NULL) {
        channelMap_.insert(std::pair<int, std::shared_ptr<Channel> >(fd, req));
        timerQueue_->addTimer(req, timeout);
    }

    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        perror("epoll_add error\n");
        channelMap_.erase(fd);
    }
    return;
}

void Epoll::epollMod(std::shared_ptr<Channel> req, int timeout)
{
    int fd = req->getFd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = req->getEvents();

    if (req != NULL) {
        channelMap_.insert(std::pair<int, std::shared_ptr<Channel> >(fd, req));
        timerQueue_->addTimer(req, timeout);
    }

    if(epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0)
    {
        perror("epoll_add error\n");
        channelMap_.erase(fd);
    }
    return;
}

void Epoll::handleExpired()
{
    timerQueue_->handleExpiredEvent();
}