#ifdef __linux__

#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>

const int TRUE = 1
const int FALSE = 0
const int SEMAPHORE_ALL_ACCESS = IPC_CREAT | 0777;

int GetValSemaphore(int sem_id);

int CreateSemaphore(int privacy, int def_val, string key);
int DisposeSemaphore(int sem_id);

int WaitSemaphore(int sem_id, int num);
int ReleaseSemaphore(int sem_id, int num);

#endif