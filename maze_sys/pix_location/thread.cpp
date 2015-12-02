#include "thread.h"

Thread::Thread(void* (*method)(void*))
{
    threadMethod = method;
}

Thread::~Thread()
{
    threadMethod = NULL;
}

Thread::Start(void* args)
{

}

