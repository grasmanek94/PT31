#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

enum ThreadState
{
    Stopped,
    Running
};

class Thread
{
private:
    ThreadState threadState;
    pthread_t threadHandle;
    void* (*threadMethod)(void*);

public:
    Thread();
    ~Thread();

    void Start(void* args);
    void Abort();
    void Join();

    ThreadState GetThreadState() { return threadState; }
};

#endif // THREAD_H
