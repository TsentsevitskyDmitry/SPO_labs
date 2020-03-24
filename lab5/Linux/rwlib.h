#pragma once
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct RWSettings
{
	int hFile;
	int bufferLen;
	char* buffer;
  	struct aiocb aiocbStruct;
	int RWBytes;
} RWSettings;