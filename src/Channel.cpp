#include "Channel.h"
#include "EventLoop.h"
#include "Util.h"
#include "Epoll.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

Channel::Channel(EventLoop *loop, int fd, int listenFd) : loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(0),
    listenFd_(listenFd)
{
}

Channel::~Channel()
{}

void Channel::update()
{
    loop_->updateChannel(std::shared_ptr<Channel>(this));
}

void Channel::handleEvent()
{
    if ((listenFd_ == fd_) && (events_ & (EPOLLIN))) {
        if (connectCallBack_ != NULL) {
            connectCallBack_();
        }
    }
    else if (events_ & (EPOLLIN | EPOLLPRI | EPOLLHUP)) {
        if (readCallBack_ != NULL) {
            readCallBack_();
        }
    }
    else if (events_ & (EPOLLOUT)) {
        if (writeCallBack_ != NULL) {
            writeCallBack_();
        }
    }
    //...
}

void Channel::handleDefaultConnEvent()
{
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(struct sockaddr_in));
    socklen_t clientAddrLen = sizeof(clientAddr);
    int acceptFd = accept(listenFd_, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (acceptFd == -1) {
        perror("accept error!\n");
    }
    std::shared_ptr<Channel> newConn(new Channel(loop_, acceptFd, listenFd_));
    newConn->setReadCallBack(std::bind(&Channel::handleDefaultReadEvent, this));
    newConn->setEvents(EPOLLIN | EPOLLPRI);
    loop_->addToPoller(std::shared_ptr<Channel>(this));
    printf("Channel::handleDefaultConnEvent success!\n");
}

void Channel::handleDefaultReadEvent()
{
    char buf[0xFFFF];
    ssize_t n = readn(fd_, buf, sizeof(buf));
    if (n > 0) {
        printf("read msg: %s", buf);
        setReadCallBack(std::bind(&Channel::handleDefaultWriteEvent, this));
        setEvents(EPOLLOUT);
        loop_->modToPoller(std::shared_ptr<Channel>(this));
    }
    printf("Channel::handleDefaultReadEvent success!\n");
    //TODO:异常处理
}

void Channel::handleDefaultWriteEvent()
{
    char *buf = "Hello client";
    ssize_t n = writen(fd_, buf, sizeof(buf));
    if (n > 0) {
        printf("write msg: %s", buf);
        setReadCallBack(std::bind(&Channel::handleDefaultWriteEvent, this));
        setEvents(EPOLLIN | EPOLLPRI);
        loop_->modToPoller(std::shared_ptr<Channel>(this));
    }
    printf("Channel::handleDefaultWriteEvent success!\n");
    //TODO:异常处理
}