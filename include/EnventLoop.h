#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include <pthread.h>
#include <memory>
#include <vector>

class Epoll;
class Channel;

class EventLoop {
public:
    typedef std::vector<std::shared_ptr<Channel> > ChannelList;
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    bool isInLoopThread()
    {
        return threadId_ == 0;
    }

    void updateChannel(Channel *channel);
    // void removeChannel(Channel *channel){}

    static EventLoop *getEventLoopOfCurrentThread();
private:
    bool looping_;
    bool quit_;
    const pthread_t threadId_;
    std::shared_ptr<Epoll> poller_;
    ChannelList activeChannel_;
};

#endif