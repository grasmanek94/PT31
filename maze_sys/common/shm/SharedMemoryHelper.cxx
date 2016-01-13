#include <string>
#include <semaphore.h>

#include "SharedMemoryHelper.hxx"

void my_PrepSem(const std::string& name, sem_t** semaphore, int initial)
{
	*semaphore = sem_open(name.c_str(), O_CREAT | O_EXCL, 0777, initial);

	if (*semaphore == SEM_FAILED)
	{
		*semaphore = sem_open(name.c_str(), 0);
		if (*semaphore == SEM_FAILED)
		{
			throw std::exception(/*"Cannot access semaphore"*/);
		}
	}
}