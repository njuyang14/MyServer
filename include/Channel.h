#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <functional>
#include <sys/epoll.h>

class EventLoop;

class Channel {
public:
    typedef std::function<void()> EventCallBack;
    Channel(EventLoop *eventLoop, int fd);
    ~Channel();

    void handleEvent();
    void setReadCallBack(const EventCallBack &func)
    {
        readCallBack_ = func;
    }
    void setWriteCallBack(const EventCallBack &func)
    {
        writeCallBack_ = func;
    }
    void setErrorCallBack(const EventCallBack &func)
    {
        errorCallBack_ = func;
    }

    int getFd() const
    {
        return fd_;
    }

    int getEvents() const
    {
        return events_;
    }

    void setRevents(int revents)
    {
        revents_ = revents;
    }

    void enableReading()
    {
        events_ |= EPOLLIN|EPOLLPRI;
        update();
    }

    //for Poller
    int getIndex() const
    {
        return index_;
    }
    void setIndex(int index)
    {
        index_ = index;
    }

    EventLoop *getLoop() const
    {
        return loop_;
    }
private:
    EventLoop *loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;

    void update();
    EventCallBack readCallBack_;
    EventCallBack writeCallBack_;
    EventCallBack errorCallBack_;
};
#endif