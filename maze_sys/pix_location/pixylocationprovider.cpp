#include "pixylocationprovider.h"

PixyLocationProvider::PixyLocationProvider() {}

PixyLocationProvider::~PixyLocationProvider()
{
    Stop();
}

int PixyLocationProvider::dec2oct(int num)
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

void PixyLocationProvider::OnStart()
{
    int status = pixy_init();

    if (status != 0)
    {
        throw std::runtime_error("Error initializing pixy");
    }
}

void PixyLocationProvider::OnStop()
{
    pixy_close();
}

void PixyLocationProvider::DoStep()
{
    if (!pixy_blocks_are_new())
    {
        return;
    }

    struct Block blocks[BLOCK_BUFFER_SIZE];
    int newcount = pixy_get_blocks(BLOCK_BUFFER_SIZE, &blocks[0]);

    if (newcount < 0)
    {
        return;
    }

    std::map<int, int>::iterator it;
    std::vector<int> processed;
    // loop backwards as the most recent
    // signature is at the end
    for (int i = newcount - 1; i >= 0; i--)
    {
        Block current = blocks[i];
        int currentsig = dec2oct(current.signature);
        if (current.type != PIXY_BLOCKTYPE_COLOR_CODE)
        {
            continue;
        }

        bool isprocessed = false;
        for (unsigned int j = 0; j < processed.size(); j++)
        {
            if (currentsig == processed.at(j))
            {
                isprocessed = true;
                break;
            }
        }

        if (isprocessed)
        {
            continue;
        }

        it = reverseIds.find(currentsig);
        if (it != reverseIds.end())
        {
            Location loc;
            loc.x = current.x;
            loc.y = current.y;
            ipcPos.set(it->second, loc);
            processed.push_back(it->first);
        }
    }
}
