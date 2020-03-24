#include "esync.h"

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
 };

int ValueSemaphore(int sem_id)
{
    return semctl(sem_id, 0, GETVAL, 0);
}

int CreateSemaphore(string name)
{
	std::hash<std::string> hash_fn;
	int ikey = hash_fn(name);
    key_t kkey = ftok("/tmp", ikey);
    int sem_id = semget(kkey, 1, IPC_CREAT | 0777);
    semun arg;
    arg.val = 0;
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