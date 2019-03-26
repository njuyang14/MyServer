#include "Server.h"
#include "Util.h"

Server::Server(EventLoop *loop, int port) : loop_(loop),
    started_(false),
    port_(port),
    listenFd_(socket_bind_listen(port_)),
    acceptChannel_(new Channel(loop, listenFd_))
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

    started_ = true;
}