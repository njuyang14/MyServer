#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <vector>
#include <map>
#include <memory>
#include <sys/epoll.h>

#include "Channel.h"

class EventLoop;

class Epoll {
public:
    typedef std::vector<std::shared_ptr<Channel> > ChannelList;
    typedef std::vector<struct epoll_event> EventList;
    typedef std::map<int, std::shared_ptr<Channel> > ChannelMap;

    Epoll();
    ~Epoll();

    ChannelList poll();

    void epollAdd(Channel *req, int timeout);
    void epollMod(std::shared_ptr<Channel> req, int timeout);
    void epollDel(std::shared_ptr<Channel> req);

    void updateChannel(Channel *channel);

private:
    static const int MAX_FD = 100000;
    EventList events_;
    int epollFd_; // epoll_create()
    ChannelMap channelMap_; // <FD, Channel>
};

#endif