#include "AT88SC.h"
#include "ceddkex.h"
#include "s1.h"
#define debug 0
#define DEVICE_TYPE_AT88SC0104C 0
#define DEVICE_TYPE_AT88SC0204C 1
#define DEVICE_TYPE_AT88SC0404C 2
#define DEVICE_TYPE_AT88SC0808C 3
#define DEVICE_TYPE_AT88SC1616C 4
#define DEVICE_TYPE_AT88SC3216C 5
#define DEVICE_TYPE_AT88SC6416C 6
#define DEVICE_TYPE_AT88SC12816C 7
#define DEVICE_TYPE_AT88SC25616C 8
int one_page_max=0;//one page size
int one_userzone_max=0;//one user zone size
int userzone_num=0;//userzone num
unsigned char chip_info[24];
unsigned char chip_at88sc0104c[10]={0x3b,0xb2,0x11,0x00,0x10,0x80,0x00,0x01,0x10,0x10};
unsigned char chip_at88sc0204c[10]={0x3b,0xb2,0x11,0x00,0x10,0x80,0x00,0x02,0x20,0x20};
unsigned char chip_at88sc0404c[10]={0x3b,0xb2,0x11,0x00,0x10,0x80,0x00,0x04,0x40,0x40};
unsigned char chip_at88sc0808c[10]={0x3b,0xb2,0x11,0x00,0x10,0x80,0x00,0x08,0x80,0x60};
unsigned char chip_at88sc1616c[10]={0x3b,0xb2,0x11,0x00,0x10,0x80,0x00,0x16,0x16,0x80};
unsigned char chip_at88sc3216c[10]={0x3b,0xb3,0x11,0x00,0x00,0x00,0x00,0x32,0x32,0x10};
unsigned char chip_at88sc6416c[10]={0x3b,0xb3,0x11,0x00,0x00,0x00,0x00,0x64,0x64,0x40};
unsigned char chip_at88sc12816c[10]={0x3b,0xb3,0x11,0x00,0x00,0x00,0x01,0x28,0x28,0x60};
unsigned char chip_at88sc25616c[10]={0x3b,0xb3,0x11,0x00,0x00,0x00,0x02,0x56,0x58,0x60};

BOOL ReadReg(UINT8* data,UINT8 size);
BOOL WriteReg(UINT8* value,UINT8 size);

BOOL Virtual_Alloc()
{
	memset(chip_info,0xff,24);
	if(get_config(chip_info))
	{
		if(memcmp(chip_info,chip_at88sc0104c,10)==0)
		{
			one_page_max=16;
			one_userzone_max=32;
			userzone_num=4;
		}
		else if(memcmp(chip_info,chip_at88sc0204c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=64;
			userzone_num=4;
		}		
		else if(memcmp(chip_info,chip_at88sc0404c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=128;
			userzone_num=4;
		}		
		else if(memcmp(chip_info,chip_at88sc0808c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=128;
			userzone_num=8;
		}		
		else if(memcmp(chip_info,chip_at88sc1616c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=128;
			userzone_num=16;
		}		
		else if(memcmp(chip_info,chip_at88sc3216c,10)==0)
		{
			one_page_max=64;	
			one_userzone_max=256;
			userzone_num=16;
		}		
		else if(memcmp(chip_info,chip_at88sc6416c,10)==0)
		{
			one_page_max=64;	
			one_userzone_max=512;
			userzone_num=16;
		}		
		else if(memcmp(chip_info,chip_at88sc12816c,10)==0)
		{
			one_page_max=128;	
			one_userzone_max=1024;
			userzone_num=16;
		}		
		else if(memcmp(chip_info,chip_at88sc25616c,10)==0)
		{
			one_page_max=128;	
			one_userzone_max=2048;
			userzone_num=16;
		}		
		else
		{
			RETAILMSG(1,(TEXT("UnKnown at88sc chip\r\n")));
			return FALSE;
		}
		
	}
	else
	{
		RETAILMSG(1,(TEXT("at88sc bus error\r\n")));
	}
		return FALSE;
	
	return TRUE;
}
void Virtual_Realease()
{
}


	BOOL WINAPI  
DllEntry(HANDLE	hinstDLL, 
		DWORD dwReason, 
		LPVOID /* lpvReserved */)
{
	switch(dwReason)
	{
		case DLL_PROCESS_ATTACH:
			DEBUGREGISTER((HINSTANCE)hinstDLL);
			return TRUE;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
#ifdef UNDER_CE
		case DLL_PROCESS_EXITING:
			break;
		case DLL_SYSTEM_STARTED:
			break;
#endif
	}
	return TRUE;
}

BOOL ATS_Deinit(DWORD hDeviceContext)
{
	return TRUE;
} 
DWORD ATS_Init(DWORD dwContext)
{
	RETAILMSG(1,(TEXT("[	AT88SC ++]\r\n")));
	Virtual_Alloc();
	RETAILMSG(1,(TEXT("[	AT88SC --]\r\n")));
	return TRUE;
}
BOOL WriteReg(at88* value)
{
	BOOL rc = FALSE;
	int index=0;
	ge param;
	/*transfer addr to userzone 0 byte to 32768 byte*/
	if((value->addr+value->size)>one_userzone_max*userzone_num)
		return FALSE;
	int user_zone_begin=value->addr/one_userzone_max;
	int user_zone_end=(value->addr+value->size)/one_userzone_max;
	memcpy(param.g,value->g,8);
	memcpy(param.pw,value->pw,3);	
	param.use_g=0;
	param.use_pw=0;
	param.page_size=one_page_max;
	for(param.zone_index=user_zone_begin;param.zone_index<=user_zone_end;param.zone_index++)
	{		
		param.addr=value->addr%one_userzone_max;
		if((param.addr+value->size)<=one_userzone_max)
			param.len=value->size;
		else
			param.len=one_userzone_max-param.addr;
			
		//memcpy(param.user_zone,value->data+index,param.len);
		param.user_zone=value->data+index;
		userzone_proc(param,0);
		value->size=value->size-param.len;		
		value->addr=value->addr+param.len;
		index=index+param.len;
	}
}
BOOL ReadReg(at88* value)
{
	BOOL rc = FALSE;
	int index=0;
	ge param;
	/*transfer addr to userzone 0 byte to 32768 byte*/
	if((value->addr+value->size)>one_userzone_max*userzone_num)
		return FALSE;
	int user_zone_begin=value->addr/one_userzone_max;
	int user_zone_end=(value->addr+value->size)/one_userzone_max;
	memcpy(param.g,value->g,8);
	memcpy(param.pw,value->pw,3);	
	param.use_g=0;
	param.use_pw=0;
	param.page_size=one_page_max;
	for(param.zone_index=user_zone_begin;param.zone_index<=user_zone_end;param.zone_index++)
	{		
		param.addr=value->addr%one_userzone_max;
		if((param.addr+value->size)<=one_userzone_max)
			param.len=value->size;
		else
			param.len=one_userzone_max-param.addr;
			
		//memcpy(param.user_zone,value->data+index,param.len);
		param.user_zone=value->data+index;
		userzone_proc(param,1);
		value->size=value->size-param.len;		
		value->addr=value->addr+param.len;
		index=index+param.len;
	}

}

//-----------------------------------------------------------------------------
BOOL ATS_IOControl(DWORD hOpenContext, 
		DWORD dwCode, 
		PBYTE pBufIn, 
		DWORD dwLenIn, 
		PBYTE pBufOut, 
		DWORD dwLenOut, 
		PDWORD pdwActualOut)
{	
	int i;
	switch(dwCode)
	{
		case IOCTL_AT88SC_WRITE:

			WriteReg((at88 *)pBufOut,dwLenOut);

			#if debug
			RETAILMSG(1,(TEXT("\r\nWrite =\r\n")));
			for(i=0;i<dwLenOut;i++)
				RETAILMSG(1,(TEXT("%x "),*(BYTE*)(pBufOut+i)));
			#endif
			break;
		case IOCTL_AT88SC_READ:
			if(pBufOut && dwLenOut>=sizeof(BYTE))
			{
				#if debug
				RETAILMSG(1,(TEXT("\r\nRead =\r\n")));
				#endif
				ReadReg((at88 *)pBufOut,dwLenOut);
				//RETAILMSG(1,(TEXT("Read %x="),*(BYTE*)pBufIn));
				#if debug
				for(i=0;i<dwLenOut;i++)
					RETAILMSG(1,(TEXT("%x "),*(BYTE*)(pBufOut+i)));
				#endif
				if(pdwActualOut) *(int*)pdwActualOut=sizeof(BYTE)*dwLenOut;
			}

			break;
		default:
			break;		
	}
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD ATS_Open(DWORD hDeviceContext, DWORD AccessCode, DWORD ShareMode)
{
	//RETAILMSG(1,(TEXT("USERMUL: ATS_Open\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL ATS_Close(DWORD hOpenContext)
{
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ATS_PowerDown(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: ATS_PowerDown\r\n")));

} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ATS_PowerUp(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: ATS_PowerUp\r\n")));

} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD ATS_Read(DWORD hOpenContext, LPVOID pBuffer, DWORD Count)
{
	//RETAILMSG(1,(TEXT("USERMUL: ATS_Read\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD ATS_Seek(DWORD hOpenContext, long Amount, DWORD Type)
{
	//RETAILMSG(1,(TEXT("USERMUL: ATS_Seek\r\n")));
	return 0;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD ATS_Write(DWORD hOpenContext, LPCVOID pSourceBytes, DWORD NumberOfBytes)
{
	//RETAILMSG(1,(TEXT("USERMUL: ATS_Write\r\n")));
	return 0;
}

