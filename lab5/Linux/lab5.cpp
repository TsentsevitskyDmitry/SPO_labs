#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <aio.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "rwlib.h"

using namespace std;

typedef struct asyncBuffer
{
	char* buffer;
	int bytes;
} asyncBuffer;

void* ReaderThread(void* laBuff);
void* WriterThread(void* laBuff);
void handleError(string err);

void *RWlib;
const int MAX_READ_LEN = 32;
const int MAX_PATH = 256;
const char* OUTPUT_FILE_PATH = "./out.txt";

pthread_mutex_t readComplete;
pthread_mutex_t writeComplete;

int main()
{
  RWlib = dlopen("./rwlib.so", RTLD_NOW);
  if(!RWlib) handleError("Library Error!");

  	pthread_mutex_init(&readComplete, NULL);
	pthread_mutex_init(&writeComplete, NULL);

	pthread_mutex_lock(&readComplete);
	pthread_mutex_unlock(&writeComplete);

	asyncBuffer aBuff;
	aBuff.buffer = new char[MAX_READ_LEN];
  	aBuff.bytes = 0;
  
  pthread_t hThread_read, hThread_write;
    
  pthread_create(&hThread_read,NULL,ReaderThread, (void *)&aBuff);
  pthread_create(&hThread_write,NULL,WriterThread, (void *)&aBuff);
 
  pthread_join(hThread_read, NULL);
  pthread_join(hThread_write, NULL);
  
  pthread_mutex_destroy(&readComplete);
  pthread_mutex_destroy(&writeComplete);
  return 0;
}

void handleError(string err)
{
	cout << "Error: " << err << endl;
	cin.ignore();
	cin.get();
	exit(-1);
}

void* ReaderThread(void *laBuff)
{
  	int (*Read) (struct RWSettings *) = (int(*)(struct RWSettings*)) dlsym(RWlib,"Read");
  	
  	asyncBuffer* aBuff = (asyncBuffer*)laBuff;
	RWSettings Rsettings;
	memset(&Rsettings, 0 , sizeof(struct RWSettings));
	Rsettings.buffer = aBuff->buffer;
	Rsettings.bufferLen = MAX_READ_LEN;
	Rsettings.aiocbStruct.aio_sigevent.sigev_notify = SIGEV_NONE;

	string folder;
	cout << "Input directory with files to combine: ";
	cin >> folder;

	DIR *dir;
	char sourceFilePath[MAX_PATH];
	dir = opendir(folder.c_str());
	if(!dir) handleError("OpenDir ERROR");
	dirent* item;
	item = readdir(dir);
	item = readdir(dir);
	if (!item) handleError("Empty folder");

	while (2 * 2 == 4)
	{
		pthread_mutex_lock(&writeComplete);

		if (!aBuff->bytes)
		{
			strcpy(sourceFilePath, folder.c_str()); 
			strcat(sourceFilePath, "/");
			strcat(sourceFilePath, item->d_name);  

			Rsettings.RWBytes = 0;
			Rsettings.aiocbStruct.aio_offset = 0;
			Rsettings.hFile = open(sourceFilePath, O_RDONLY);	
				cout << "Opening file: " << sourceFilePath << endl;
		}

		aBuff->bytes = Read(&Rsettings);	// Прочитать из файла

		if (!aBuff->bytes) // читать больше нечего
		{
			close(Rsettings.hFile);
			item = readdir(dir);

			if(item && !strcmp(item->d_name, ".."))
				item = readdir(dir);

			if (!item) 
			{
				aBuff->bytes = -1;
				pthread_mutex_unlock(&readComplete);
				break;
			}
			pthread_mutex_unlock(&writeComplete);
		}
		else
			//cout << "read: " << aBuff->buffer << endl;
			pthread_mutex_unlock(&readComplete);
	}

	closedir(dir);
	return 0; 
}

void* WriterThread(void * laBuff)
{
	int (*Write) (struct RWSettings *) = (int(*)(struct RWSettings*))dlsym(RWlib,"Write");  

  	asyncBuffer* aBuff = (asyncBuffer*)laBuff;
	RWSettings Wsettings;
	memset(&Wsettings, 0 , sizeof(struct RWSettings));
	Wsettings.buffer = aBuff->buffer;
	Wsettings.aiocbStruct.aio_sigevent.sigev_notify = SIGEV_NONE;
	Wsettings.hFile = open(OUTPUT_FILE_PATH, O_WRONLY | O_CREAT |O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
 
 	while(1)
	 {   
	    pthread_mutex_lock(&readComplete);

	    if (aBuff->bytes == -1)
	    	break;

		Wsettings.bufferLen = aBuff->bytes;
		Write(&Wsettings);
    
    	pthread_mutex_unlock(&writeComplete);
	
	}

	pthread_mutex_unlock(&readComplete);
	close(Wsettings.hFile);
	return NULL; 
}