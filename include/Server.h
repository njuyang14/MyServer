#ifndef _SERVER_H_
#define _SERVER_H_

#include "EventLoop.h"
#include "Channel.h"

class Server {
public:
    Server(EventLoop *loop, int port);
    ~Server() {};
    EventLoop *getLoop()
    {
        return loop_;
    }
    void start();
private:
    EventLoop *loop_;
    bool started_;
    int port_;
    int listenFd_;
    std::shared_ptr<Channel> acceptChannel_;
    static const int MAX_FD = 100000;
};

#endif