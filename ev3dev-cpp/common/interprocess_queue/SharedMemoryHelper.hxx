#ifndef SHAREDMEMORYHELPER_H
#define SHAREDMEMORYHELPER_H

#include <string>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

template <typename T>
T my_shm_create(const std::string& shm_name, size_t size, int& shm_fd)
{
	shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0777);
	if (shm_fd == -1)
	{
		return NULL;
	}

	int rtnval = ftruncate(shm_fd, size);
	if (rtnval != 0)
	{
		return NULL;
	}

	T shm_addr = (T)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_addr == MAP_FAILED)
	{
		return NULL;
	}

	return (shm_addr);
}

template <typename T>
T my_shm_open(const std::string& shm_name, int& shm_fd)
{
	shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0777);
	if (shm_fd == -1)
	{
		return NULL;
	}

	size_t size = lseek(shm_fd, 0, SEEK_END);

	T shm_addr = (T)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_addr == MAP_FAILED)
	{
		return NULL;
	}

	return (shm_addr);
}

#endif