#ifndef IPCPOS_H
#define IPCPOS_H

#include <iostream>

#include <string>
#include <new>
#include <semaphore.h>
#include <pthread.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

#include <JumpPointSearch/JPS.hxx>
#include <shm/SharedMemoryHelper.hxx>

template <size_t robots = 2, typename pos_type = JPS::Position>
class IPCPos
{
private:
	pos_type* pos_shared_memory;
	sem_t* memory_prepare_semaphore;
	int shm_fd;
	std::string queue_name;
	int deletion_fd_protection;

public:
	IPCPos(/*const std::string& queue_name*/)
		:	pos_shared_memory(NULL),
			memory_prepare_semaphore(SEM_FAILED),
			shm_fd(-1),
			queue_name(/*queue_name*/"InterProcessPositioning"),
			deletion_fd_protection(-1)
	{
		deletion_fd_protection = open(("/tmp/deletion_fd_protection.ipc_lockcheck." + queue_name).c_str(), O_CREAT | O_RDWR);
		if (deletion_fd_protection == -1)
		{
			throw std::exception(/*"Cannot access critical lock file"*/);
		}

		if (flock(deletion_fd_protection, LOCK_EX | LOCK_NB) == 0)
		{
			//we have the only and only lock now so this below should generrally NOT be able to fail
			flock(deletion_fd_protection, LOCK_UN);
			if (flock(deletion_fd_protection, LOCK_SH | LOCK_NB) != 0)
			{
				throw std::exception(/*"Cannot access critical lock file"*/);
			}

			shm_unlink(queue_name.c_str());
			sem_unlink(queue_name.c_str());
		}
		else
		{
			if (flock(deletion_fd_protection, LOCK_SH | LOCK_NB) != 0)
			{
				throw std::exception(/*"Cannot access critical lock file"*/);
			}
		}

		my_PrepSem(queue_name, &memory_prepare_semaphore, 1);

		sem_wait(memory_prepare_semaphore);

		pos_shared_memory = my_shm_create<pos_type*>(queue_name, robots * sizeof(pos_type), shm_fd);
		if (!pos_shared_memory)
		{
			pos_shared_memory = my_shm_open<pos_type*>(queue_name, shm_fd);
			if (!pos_shared_memory)
			{
				throw std::exception(/*"Cannot access shared memory"*/);
			}
		}
		else
		{
			memset(pos_shared_memory, 0, robots * sizeof(pos_type));
		}

		sem_post(memory_prepare_semaphore);
	}

	bool Get(size_t robot_id, pos_type& pos) const
	{
		if (robot_id >= robots)
		{
			return false;
		}
		 
		pos = pos_shared_memory[robot_id];
		return true;
	}

	bool Set(size_t robot_id, const pos_type& pos)
	{
		if (robot_id >= robots)
		{
			return false;
		}

		pos_shared_memory[robot_id] = pos;
		return true;
	}

	~IPCPos()
	{
		sem_close(memory_prepare_semaphore);
		close(shm_fd);

		if (deletion_fd_protection != -1)
		{
			flock(deletion_fd_protection, LOCK_UN); // release shared lock
			close(deletion_fd_protection);
		}
	}
};

#endif