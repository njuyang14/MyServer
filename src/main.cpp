#include"EnventLoop.h"

#include "pthread.h"


int main(int argc, char *argv[])
{
    EventLoop mainLoop;
    mainLoop.loop();
    return 0;
}