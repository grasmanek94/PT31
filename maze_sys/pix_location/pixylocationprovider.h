#ifndef PIXYLOCATIONPROVIDER_H
#define PIXYLOCATIONPROVIDER_H

#include <stdio.h>
#include <stdexcept>
#include <map>
#include <pthread.h>
#include <pixy.h>

#include "ipcpos.h"

#define BLOCK_BUFFER_SIZE 25

struct CCSignature
{
    int sig1;
    int sig2;
};

int dec2oct (int num)
{
    int total = 0,power=1;
    while(num > 0)
    {
        total += power * (num % 8) ; // 2 if decimal to binary
        num /= 8; // 2 if decimal to binary
        power *= 10;
    }
    return total;
}

class PixyLocationProvider
{
private:
    std::map<int, CCSignature> registeredIds;
    IPCPos ipcPos;

    bool running;
    bool stop;

    void Run();

public:
    PixyLocationProvider();
    ~PixyLocationProvider();
    bool RegisterSignature(int id, int sig1, int sig2);
    void RemoveSignature(int id);
    void Start();
    void Stop();
};

#endif // PIXYLOCATIONPROVIDER_H
