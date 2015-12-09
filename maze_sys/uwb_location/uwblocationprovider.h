#ifndef UWBLOCATIONPROVIDER_H
#define UWBLOCATIONPROVIDER_H

#include "../common/LocationProvider/LocationProvider.h"

class UwbLocationProvider : LocationProvider
{
private:
    void DoStep() override;
    void OnStart() override;
    void OnStop() override;

public:
    UwbLocationProvider();
    ~UwbLocationProvider();
};

#endif // UWBLOCATIONPROVIDER_H
