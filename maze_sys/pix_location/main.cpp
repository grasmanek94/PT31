#include <iostream>
#include <pixy.h>
#include <boost/thread.hpp>

#define BLOCK_BUFFER_SIZE 25

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

int main()
{
    int status = pixy_init();

    if (status != 0)
    {
        std::cout << "error init pixy" << std::endl;
    }

    while (true)
    {
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

        for (int i = 0; i < new_blocks; i++)
        {
            std::cout << dec2oct(blocks[i].signature)
                      << " x: "
                      << blocks[i].x
                      << " y: "
                      << blocks[i].y
                      << std::endl;
        }

        std::cout << std::endl;

        boost::this_thread::sleep(
                    boost::posix_time::milliseconds(250));
    }
}

