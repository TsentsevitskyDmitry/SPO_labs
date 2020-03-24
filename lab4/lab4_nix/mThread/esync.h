#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
#include <iostream>
#include <string.h>

using namespace std;

int ValueSemaphore(int sem_id);

int CreateSemaphore(string key);
int DisposeSemaphore(int sem_id);

int WaitSemaphore(int sem_id, int num);
int ReleaseSemaphore(int sem_id, int num);