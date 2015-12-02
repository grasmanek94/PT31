#include "pixylocationprovider.h"

PixyLocationProvider::PixyLocationProvider()
{
    int status = pixy_init();

    if (status != 0)
    {
        throw std::runtime_error("Error initializing pixy");
    }
}

PixyLocationProvider::~PixyLocationProvider()
{
    if (running)
    {
        stop();
    }
}

bool PixyLocationProvider::registerSignature(int id, int sig1, int sig2)
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

void PixyLocationProvider::removeSignature(int id)
{
    std::map<int, CCSignature>::iterator it;

    it = registeredIds.find(id);
    registeredIds.erase(it);
}

void PixyLocationProvider::start()
{
    if (!running)
    {
        int status = pthread_create(&receiveThread, NULL, receiveLocations, this);
        if (status)
        {
            throw std::runtime_error(
                        "Error creating receive thread");
        }

        running = true;
    }
}

void PixyLocationProvider::stop()
{
    if (running)
    {
        stopReceiving = true;
        int status = pthread_join(receiveThread, NULL);
        if (status)
        {
            throw std::runtime_error(
                        "Error joining receive thread");
        }
        stopReceiving = false;
        running = false;
    }
}

bool PixyLocationProvider::mustStop()
{
    return stopReceiving;
}

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

void* receiveLocations(void* instance)
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
}
