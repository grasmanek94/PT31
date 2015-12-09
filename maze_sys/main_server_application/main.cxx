#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

#include "NetworkServer.h"
#include "enet/enetpp.hxx"


int main(int argc, char** argv)
{
	NetworkServer server = new NetworkServer( serverAddress);
	
	return 0;
}
