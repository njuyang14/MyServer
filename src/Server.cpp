#include "Server.h"
#include "Util.h"

Server::Server(EventLoop *loop, int port) : loop_(loop),
    started_(false),
    port_(port),
    listenFd_(socket_bind_listen(port_)),
    acceptChannel_(new Channel(loop_, listenFd_, listenFd_))
{
    if (setSocketNonBlocking(listenFd_) < 0)
    {
        perror("set socket non block failed");
        abort();
    }
}

void Server::start()
{
    // 监听listenFd_, 注册进epoll
    acceptChannel_->setConnCallBack(std::bind(&Channel::handleDefaultConnEvent, acceptChannel_));
    acceptChannel_->setEvents(EPOLLIN | EPOLLPRI);
    loop_->addToPoller(acceptChannel_);
    started_ = true;
}


