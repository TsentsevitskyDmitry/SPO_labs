#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>

int GetValSemaphore(int sem_id);

int CreateSemaphore(key_t key);
int OpenSemaphore(key_t key);
int DisposeSemaphore(int sem_id);

int WaitSemaphore(int sem_id, int num);
int ReleaseSemaphore(int sem_id, int num);