#include <unistd.h>
#include <server/Server.hxx>

int main()
{
	Server* svr = new Server();
	while (true)
	{
		svr->Tick();
		usleep(5000);
	}
	return 0;
}
