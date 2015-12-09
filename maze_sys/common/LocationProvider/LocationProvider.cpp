#include "LocationProvider.h"

LocationProvider::LocationProvider(){}

LocationProvider::~LocationProvider()
{
    Stop();
}

bool LocationProvider::RegisterSignature(int id, int sig)
{
    std::map<int, int>::iterator it;

    it = registeredIds.find(id);
    if (it != registeredIds.end())
    {
        return false;
    }

    registeredIds.insert(std::pair<int, int>(id, sig));
    reverseIds.insert(std::pair<int, int>(sig, id));

    return true;
}

void LocationProvider::RemoveSignature(int id)
{
    std::map<int, int>::iterator it;

    it = registeredIds.find(id);
    registeredIds.erase(it);
    it = reverseIds.find(id);
    reverseIds.erase(it);
}

void LocationProvider::Start()
{
    if (!running)
    {
        OnStart();

        running = true;
        stop = false;

        Run();
    }
}

void LocationProvider::Stop()
{
    if (running)
    {
        stop = true;
        running = false;
        OnStop();
    }
}

void LocationProvider::Run()
{
    while (!stop)
    {
        DoStep();
    }
}
