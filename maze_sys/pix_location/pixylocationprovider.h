#ifndef PIXYLOCATIONPROVIDER_H
#define PIXYLOCATIONPROVIDER_H

#include <stdio.h>
#include <stdexcept>
#include <map>
#include <vector>
#include <pixy.h>

#include "../common/LocationProvider/LocationProvider.h"

#define BLOCK_BUFFER_SIZE 25

class PixyLocationProvider : public LocationProvider
{
private:
    int dec2oct(int num);

    void DoStep() override;
    void OnStart() override;
    void OnStop() override;

public:
    PixyLocationProvider();
    ~PixyLocationProvider();
};

#endif // PIXYLOCATIONPROVIDER_H
