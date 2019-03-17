#include "Epoll.h"

const int EVENTS_NUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll() : events_(EVENTS_NUM)
{
    epollFd_ = epoll_create1(EPOLL_CLOEXEC);
}

Epoll::~Epoll()
{}

Epoll::ChannelList Epoll::poll()
{
    int numEvents = epoll_wait(epollFd_, 
                                &(*events_.begin()), 
                                events_.size(),
                                EPOLLWAIT_TIME);
    ChannelList actChannel;
    for (int i = 0; i < numEvents; i++) {
        std::shared_ptr<Channel> channel = std::shared_ptr<Channel>((Channel *)events_[i].data.ptr);

        channel->setRevents(events_[i].events);
        actChannel.push_back(channel);
    }
    return actChannel;
}