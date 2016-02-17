#ifndef LOCATIONPROVIDER_H
#define LOCATIONPROVIDER_H

#include <map>

#include "ipcpos.h"

class LocationProvider
{
private:
    bool running;
    bool stop;

    void Run();

protected:
    std::map<int, int> registeredIds;
    std::map<int, int> reverseIds;
    IPCPos ipcPos;

    virtual void DoStep() = 0;
    virtual void OnStart() = 0;
    virtual void OnStop() = 0;

public:
    LocationProvider();
    virtual ~LocationProvider();

    bool RegisterSignature(int id, int sig);
    void RemoveSignature(int id);
    void Start();
    void Stop();
};

#endif
