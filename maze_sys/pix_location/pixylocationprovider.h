#ifndef PIXYLOCATIONPROVIDER_H
#define PIXYLOCATIONPROVIDER_H

#include <stdio.h>
#include <stdexcept>
#include <map>
#include <vector>
#include <pthread.h>
#include <pixy.h>

#include "ipcpos.h"

#define BLOCK_BUFFER_SIZE 25

inline int dec2oct (int num)
{
    int total = 0;
    int power = 1;

    while(num > 0)
    {
        total += power * (num % 8);
        num /= 8;
        power *= 10;
    }

    return total;
}

class PixyLocationProvider
{
private:
    std::map<int, int> registeredIds;
    std::map<int, int> reverseIds;
    IPCPos ipcPos;

    bool running;
    bool stop;

    void Run();

public:
    PixyLocationProvider();
    ~PixyLocationProvider();
    bool RegisterSignature(int id, int sig);
    void RemoveSignature(int id);
    void Start();
    void Stop();
};

#endif // PIXYLOCATIONPROVIDER_H
