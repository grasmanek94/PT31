#include "lib/PathProcessor/PathProcessor.hxx"

int main()
{
	PathProcessor* processor = new PathProcessor();

	processor->Run();

	delete processor;

	return 0;
}
