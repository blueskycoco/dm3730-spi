#include "stdafx.h"
#include "AT88SCLib.h"
#include <winioctl.h>
#define IOCTL_AT88SC_WRITE       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0322, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_AT88SC_READ       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0323, METHOD_BUFFERED, FILE_ANY_ACCESS)
    
#define IOCTL_AT88SC_READ_ID       \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0324, METHOD_BUFFERED, FILE_ANY_ACCESS)
	
typedef struct {
	unsigned char *data;
	unsigned char g[8];
	unsigned char pw[3];
	unsigned int addr;
	unsigned int size;
}at88,*pat88;
at88 g_at88;
int AT88SC::ConfigKey(unsigned char *G,unsigned char *pw)
{
	int i=0;
	for(i=0;i<8;i++)
		g_at88.g[i]=G[i];
	for(i=0;i<3;i++)
		g_at88.pw[i]=pw[i];
	return 1;
}
int AT88SC::Write(int addr,unsigned char *data,int len)
{
	h= CreateFile(TEXT("ATS:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);
	
	if(h == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (L"open at88sc device failed\r\n"));
		exit(-1);
	}
	g_at88.addr=addr;
	g_at88.size=len;
	g_at88.data=(unsigned char *)malloc(len*sizeof(unsigned char));
	memcpy(g_at88.data,data,len);
	DeviceIoControl(h, IOCTL_AT88SC_WRITE, NULL, 0, &g_at88, sizeof(g_at88), NULL, NULL);
	free(g_at88.data);
	CloseHandle(h);
	return 1;
}

int AT88SC::Read(int addr,unsigned char *data,int len)
{
	h= CreateFile(TEXT("ATS:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);
	
	if(h == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (L"open at88sc device failed\r\n"));
		exit(-1);
	}
	g_at88.addr=addr;
	g_at88.size=len;
	g_at88.data=data;
	DeviceIoControl(h, IOCTL_AT88SC_READ, NULL, 0, &g_at88, sizeof(g_at88), NULL, NULL);
	CloseHandle(h);
	return 1;
}
int AT88SC::ReadID(unsigned char *id)
{
	h= CreateFile(TEXT("ATS:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);
	
	if(h == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (L"open at88sc device failed\r\n"));
		exit(-1);
	}
	DeviceIoControl(h, IOCTL_AT88SC_READ_ID, NULL, 0, &id, sizeof(unsigned char)*8, NULL, NULL);
	CloseHandle(h);
	return 1;
}
