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
    IPCPos(){}

    void set(int robot_id, Location pos)
    {
        std::cout << robot_id
                  << " is now at "
                  << "("
                  << pos.x
                  << ", "
                  << pos.y
                  << ")"
                  << std::endl;
    }
};

#endif // IPCPOS_H
