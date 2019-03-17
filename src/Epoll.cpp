#include "Epoll.h"

const int EVENTS_NUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll() : events_(EVENTS_NUM)
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
    ChannelList actChannel;
    for (int i = 0; i < numEvents; i++) {
        // std::shared_ptr<Channel> channel = std::shared_ptr<Channel>((Channel *)events_[i].data.ptr);
        Channel *channel = static_cast<Channel *>(events_[i].data.ptr);

        channel->setRevents(events_[i].events);
        actChannel.push_back(std::shared_ptr<Channel>(channel));
    }
    return actChannel;
}

void Epoll::updateChannel(Channel *channel)
{
    epollAdd(channel, 5);
    printf("update interested event\n");
}

void Epoll::epollAdd(Channel *req, int timeout)
{
    int fd = req->getFd();
    struct epoll_event event;
    event.data.fd = fd; // channel的描述符
    event.events = req->getEvents(); // channel的事件

    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        perror("epoll_add error");
    }
    return;
}