#include "rwlib.h"

DWORD result(RWSettings *settings)
{
	GetOverlappedResult(settings->hFile, &settings->overlapped, &settings->RWBytes, FALSE);
	return settings->RWBytes;
}

void read(RWSettings *settings)
{   
	if (settings->RWBytes) settings->overlapped.Offset += settings->RWBytes; // if previous read was successful
	else settings->overlapped.Offset = 0;

	ResetEvent(settings->successEvent);
	settings->overlapped.hEvent = settings->successEvent;
	ReadFile(settings->hFile, settings->buffer, settings->bufferLen, NULL, &settings->overlapped);
}
void write(RWSettings *settings)
{
	GetOverlappedResult(settings->hFile, &settings->overlapped, &settings->RWBytes, FALSE);

	if (settings->RWBytes) settings->overlapped.Offset += settings->RWBytes; // if previous read was successful
	else settings->overlapped.Offset = 0;

	ResetEvent(settings->successEvent);
	settings->overlapped.hEvent = settings->successEvent;
	WriteFile(settings->hFile, settings->buffer, settings->bufferLen, NULL, &settings->overlapped);
}
