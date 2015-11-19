#include "lib/PathProcessor/PathProcessor.hxx"

typedef PathProcessor<2048, 128> MyProcessor;

int main()
{
	MyProcessor* processor = new MyProcessor();

	processor->Run();

	delete processor;

	return 0;
}
