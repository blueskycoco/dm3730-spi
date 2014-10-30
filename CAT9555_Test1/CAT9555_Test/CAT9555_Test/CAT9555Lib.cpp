#include "stdafx.h"
#include "CAT9555Lib.h"
#include <winioctl.h>
#define IOCTL_CAT9555_WRITE       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0320, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CAT9555_READ       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0321, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CAT95552_WRITE       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0322, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CAT95552_READ       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0323, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CAT95553_WRITE       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0324, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CAT95553_READ       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0325, METHOD_BUFFERED, FILE_ANY_ACCESS)

int Cat9555::SetCAT9555(int channel,unsigned char port,unsigned char level)
{
	if(channel==0) h= CreateFile(TEXT("CAT1:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);
	
	if(channel==1) h= CreateFile(TEXT("CTA1:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);

	if(channel==2) h= CreateFile(TEXT("ACT1:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);
	
	if(h == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (L"open gpio device failed\r\n"));
		exit(-1);
	}
	if(channel==0) DeviceIoControl(h, IOCTL_CAT9555_WRITE, &port, sizeof(unsigned char), &level, sizeof(unsigned char), NULL, NULL);
	
	if(channel==1) DeviceIoControl(h, IOCTL_CAT95552_WRITE, &port, sizeof(unsigned char), &level, sizeof(unsigned char), NULL, NULL);
	
	if(channel==2) DeviceIoControl(h, IOCTL_CAT95553_WRITE, &port, sizeof(unsigned char), &level, sizeof(unsigned char), NULL, NULL);
	CloseHandle(h);
	return 1;
}

int Cat9555::GetCAT9555(int channel,unsigned char port,unsigned char *level)
{
	if(channel==0) h= CreateFile(TEXT("CAT1:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);
	
	if(channel==1) h= CreateFile(TEXT("CTA1:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);

	if(channel==2) h= CreateFile(TEXT("ACT1:"), (GENERIC_READ|GENERIC_WRITE), (FILE_SHARE_READ|FILE_SHARE_WRITE), 0, OPEN_EXISTING, 0, 0);
	
	if(h == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (L"open gpio device failed\r\n"));
		exit(-1);
	}
	if(channel==0) DeviceIoControl(h, IOCTL_CAT9555_READ, &port, sizeof(unsigned char), level, sizeof(unsigned char), NULL, NULL);
	
	if(channel==1) DeviceIoControl(h, IOCTL_CAT95552_READ, &port, sizeof(unsigned char), level, sizeof(unsigned char), NULL, NULL);

	if(channel==2) DeviceIoControl(h, IOCTL_CAT95553_READ, &port, sizeof(unsigned char), level, sizeof(unsigned char), NULL, NULL);
	
	CloseHandle(h);
	return 1;
}
