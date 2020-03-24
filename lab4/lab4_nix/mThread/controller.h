#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <pthread.h>
#include <iostream>
#include <string.h>
#include <vector>

#include "esync.h"
#include "child.h"
#include "conio.h"

using namespace std;

typedef struct {
	int pipe_write;
	pthread_mutex_t* saccess;
} ControllerArgs;

void* controller_main(void* lpParam);