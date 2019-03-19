#include "EnventLoop.h"
#include "Channel.h"

#include <pthread.h>
#include <sys/timerfd.h>
#include <string.h>

EventLoop *g_loop;

void timeOut()
{
    printf("Time out!\n");
    g_loop->quit();
}

int main(int argc, char *argv[])
{
    EventLoop mainLoop;
    g_loop = &mainLoop;

    int timeFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel *channel = new Channel(&mainLoop, timeFd);
    channel->setReadCallBack(timeOut);
    channel->enableReading();
    
    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    timerfd_settime(timeFd, 0, &howlong, NULL);

    mainLoop.loop();
    return 0;
}