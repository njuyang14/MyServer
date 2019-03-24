#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <vector>
#include <map>
#include <memory>
#include <sys/epoll.h>

#include "Channel.h"

class TimerQueue;
class EventLoop;

class Epoll {
public:
    typedef std::vector<std::shared_ptr<Channel> > ChannelList;
    typedef std::vector<struct epoll_event> EventList;
    typedef std::map<int, std::shared_ptr<Channel> > ChannelMap;

    Epoll();
    ~Epoll();

    ChannelList poll();

    void epollAdd(std::shared_ptr<Channel> req, int timeout);
    void epollMod(std::shared_ptr<Channel> req, int timeout);
    void epollDel(std::shared_ptr<Channel> req);

    void updateChannel(std::shared_ptr<Channel> channel);
    void handleExpired();

private:
    static const int MAX_FD = 100000;
    EventList events_;
    int epollFd_; // epoll_create()
    ChannelMap channelMap_; // <FD, Channel>
    std::shared_ptr<TimerQueue> timerQueue_;
};

#endif