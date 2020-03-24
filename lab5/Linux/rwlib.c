#include "rwlib.h"


int Read(RWSettings *settings)
{
  settings->aiocbStruct.aio_buf = settings->buffer;
  settings->aiocbStruct.aio_offset += settings->RWBytes;
  settings->aiocbStruct.aio_fildes = settings->hFile;
  settings->aiocbStruct.aio_nbytes = settings->bufferLen;
  aio_read(&settings->aiocbStruct);
  
  while(aio_error(&settings->aiocbStruct) == EINPROGRESS) 
  	usleep(10000);
  settings->RWBytes = aio_return(&settings->aiocbStruct);
     
  return settings->RWBytes;
}

int Write(RWSettings *settings)
{
  settings->aiocbStruct.aio_buf = settings->buffer;
  settings->aiocbStruct.aio_offset += settings->RWBytes;
  settings->aiocbStruct.aio_fildes = settings->hFile;
  settings->aiocbStruct.aio_nbytes = settings->bufferLen;

  aio_write(&settings->aiocbStruct);
  
  while(aio_error(&settings->aiocbStruct) == EINPROGRESS) 
    usleep(10000);
  settings->RWBytes = aio_return(&settings->aiocbStruct);
     
  return settings->RWBytes;
}


/*
 EINPROGRESS - возвращает AIO_ERROR(3)  
 0 - успех 
 EOVERFLOW - конец файла 
 
 
 */