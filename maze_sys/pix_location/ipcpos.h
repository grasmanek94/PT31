#ifndef IPCPOS_H
#define IPCPOS_H

#include <iostream>

struct Location
{
    int x;
    int y;
};

class IPCPos
{
public:
    IPCPos();
    void set(int robot_id, Location pos);
};

#endif // IPCPOS_H
