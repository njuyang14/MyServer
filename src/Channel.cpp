#include "Channel.h"
#include "EnventLoop.h"

Channel::Channel(EventLoop *loop, int fd) : loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(0)
{
}

Channel::~Channel()
{}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent()
{
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLHUP)) {
        if (readCallBack_ != NULL) {
            readCallBack_();
        }
    }
    //...
}