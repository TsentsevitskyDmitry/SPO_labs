#include <sys/shm.h>
#include <sys/sem.h>
#include <wait.h>
#include <pthread.h>
#include <iostream>
#include <string.h>

#include "esync.h"
#include "conio.h"

using namespace std;

typedef struct {
	int pipe_write;
	pthread_mutex_t* saccess;
	string syncName;
	string unicString;
} ChildArgs;

void* child_main(void* lpParam);