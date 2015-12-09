#include <server/Server.hxx>

int main()
{
	Server* svr = new Server();
	while (true)
	{
		svr->Tick();
	}
	return 0;
}
