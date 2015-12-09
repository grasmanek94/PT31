#include "pixylocationprovider.h"

PixyLocationProvider::PixyLocationProvider()
{
}

PixyLocationProvider::~PixyLocationProvider()
{
    Stop();
}

bool PixyLocationProvider::RegisterSignature(int id, int sig1, int sig2)
{
    std::map<int, CCSignature>::iterator it;

    it = registeredIds.find(id);
    if (it != registeredIds.end())
    {
        return false;
    }

    CCSignature newsig;
    newsig.sig1 = sig1;
    newsig.sig2 = sig2;

    registeredIds.insert(std::pair<int, CCSignature>(id, newsig));

    return true;
}

void PixyLocationProvider::RemoveSignature(int id)
{
    std::map<int, CCSignature>::iterator it;

    it = registeredIds.find(id);
    registeredIds.erase(it);
}

void PixyLocationProvider::Start()
{
    if (!running)
    {
        running = true;
        stop = false;

        int status = pixy_init();

        if (status != 0)
        {
            throw std::runtime_error("Error initializing pixy");
        }

        Run();
    }
}

void PixyLocationProvider::Stop()
{
    if (running)
    {
        stop = true;
        running = false;
        pixy_close();
    }
}

void PixyLocationProvider::Run()
{
    while (!stop)
    {
    }
}

/*void* receiveLocations(void* instance)
{
    PixyLocationProvider* pixy = (PixyLocationProvider*)instance;

    int status = pixy_init();

    if (status != 0)
    {
        throw std::runtime_error(
                    "error initialising pixy");
    }

    while (!pixy->mustStop())
    {
        std::map<int, CCSignature>* registeredIds =
                pixy->getRegisteredIds();

        if (!pixy_blocks_are_new())
        {
            // nothing to report
            continue;
        }

        struct Block blocks[BLOCK_BUFFER_SIZE];
        int new_blocks = pixy_get_blocks(BLOCK_BUFFER_SIZE, &blocks[0]);

        if (new_blocks < 0)
        {
            // error or no blocks?
            continue;
        }

        for (int i = new_blocks; i > 0; i--)
        {

        }
    }

    pixy_close();

    return NULL;
}*/
