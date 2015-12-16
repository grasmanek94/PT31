#include "pixylocationprovider.h"

#define BLOCK_BUFFER_SIZE 25

int main()
{
    PixyLocationProvider pix;

    pix.RegisterSignature(1, 12);
    pix.RegisterSignature(2, 34);

    pix.Start();
}
