#ifndef PIXYLOCATIONPROVIDER_H
#define PIXYLOCATIONPROVIDER_H

#include <stdio.h>
#include <stdexcept>
#include <map>
#include <pthread.h>
#include <pixy.h>

#include "ipcpos.h"

#define BLOCK_BUFFER_SIZE 25

struct Location
{
    int x;
    int y;
};

struct CCSignature
{
    int sig1;
    int sig2;
};

void* receiveLocations(void*);
int dec2oct(int);

class PixyLocationProvider
{
private:
    std::map<int, CCSignature> registeredIds;

    bool running;
    volatile bool stopReceiving;
    pthread_t receiveThread;

public:
    IPCPos ipcPos;

    PixyLocationProvider();
    ~PixyLocationProvider();
    bool registerSignature(int id, int sig1, int sig2);
    void removeSignature(int id);
    void start();
    void stop();
    bool mustStop();

    std::map<int, CCSignature>* getRegisteredIds();
};

#endif // PIXYLOCATIONPROVIDER_H
