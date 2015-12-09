#include "pixylocationprovider.h"

PixyLocationProvider::PixyLocationProvider()
{
}

PixyLocationProvider::~PixyLocationProvider()
{
    Stop();
}

bool PixyLocationProvider::RegisterSignature(int id, int sig)
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

void PixyLocationProvider::RemoveSignature(int id)
{
    std::map<int, int>::iterator it;

    it = registeredIds.find(id);
    registeredIds.erase(it);
    it = reverseIds.find(id);
    reverseIds.erase(it);
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
        if (!pixy_blocks_are_new())
        {
            continue;
        }

        struct Block blocks[BLOCK_BUFFER_SIZE];
        int newcount = pixy_get_blocks(BLOCK_BUFFER_SIZE, &blocks[0]);

        if (newcount < 0)
        {
            continue;
        }

        std::map<int, int>::iterator it;
        std::vector<int> processed;
        // loop backwards as the most recent
        // signature is at the end
        for (int i = newcount - 1; i > 0; i--)
        {
            Block current = blocks[i];
            if (current.type != PIXY_BLOCKTYPE_COLOR_CODE)
            {
                continue;
            }

            bool isprocessed = false;
            for (int j = 0; j < processed.size(); j++)
            {
                if (current.signature == processed.at(j))
                {
                    isprocessed = true;
                    break;
                }
            }

            if (isprocessed)
            {
                continue;
            }


        }
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
