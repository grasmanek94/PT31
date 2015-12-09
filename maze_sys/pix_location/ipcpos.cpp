#include "ipcpos.h"

IPCPos::IPCPos()
{

}

IPCPos::set(int robot_id, Location pos)
{\
    std::cout << robot_id
              << " is now at "
              << "("
              << pos.x
              << ", "
              << pos.y
              << ")"
              << std::endl;
}

