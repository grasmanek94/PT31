#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

#include "NetworkServer.hxx"
#include "enet/enetpp.hxx"


int main(int argc, char** argv)
{
	NetServer server = new NetServer();
	while (server)
	{
		switch (server->Receive())
		{
			
		}
	}
	return 0;
}
