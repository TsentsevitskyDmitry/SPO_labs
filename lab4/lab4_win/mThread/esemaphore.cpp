#ifdef __linux__

#include "esemaphore.h"

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
 };

int GetValSemaphore(int sem_id)
{
    semctl(sem_id, 0, GETVAL, 0);
}

int CreateSemaphore(int privacy, int def_val, string skey)
{
	std::hash<skey> hash_fn;
	int ikey = hash_fn(str);
    key_t skey = ftok("/tmp", ikey);
    int sem_id = semget(skey, 1, privacy);
    semun arg;
    arg.val = def_val;
    semctl(sem_id, 0, SETALL, arg);
    return sem_id;
}

int DisposeSemaphore(int sem_id)
{
    union semun
    {
        int              val;
        struct semid_ds *buf;
        unsigned short  *array;
        struct seminfo  *__buf;
    } semunion;

    semctl(sem_id, 0, IPC_RMID, semunion);
}


int WaitSemaphore(int sem_id, int num)
{
    struct sembuf buf;
    buf.sem_op = -1;
    buf.sem_flg = IPC_NOWAIT;
    buf.sem_num = 0;
    return semop(sem_id, &buf, 1);
}

int ReleaseSemaphore(int sem_id, int num)
{
    struct sembuf buf;
    buf.sem_op = 1;
    buf.sem_flg = IPC_NOWAIT;
    buf.sem_num = 0;
    return semop(sem_id, &buf, 1);
}

#endif

