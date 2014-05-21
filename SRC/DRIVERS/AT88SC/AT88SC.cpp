#include "AT88SC.h"
#include "ceddkex.h"
#include "s1.h"
#define debug 0
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

BOOL ReadReg(at88* data);
BOOL WriteReg(at88* value);

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
			RETAILMSG(1,(TEXT("at88sc0104c found\r\n")));
		}
		else if(memcmp(chip_info,chip_at88sc0204c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=64;
			userzone_num=4;
			RETAILMSG(1,(TEXT("at88sc0204c found\r\n")));
		}		
		else if(memcmp(chip_info,chip_at88sc0404c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=128;
			userzone_num=4;
			RETAILMSG(1,(TEXT("at88sc0404c found\r\n")));
		}		
		else if(memcmp(chip_info,chip_at88sc0808c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=128;
			userzone_num=8;
			RETAILMSG(1,(TEXT("at88sc0808c found\r\n")));
		}		
		else if(memcmp(chip_info,chip_at88sc1616c,10)==0)
		{
			one_page_max=16;	
			one_userzone_max=128;
			userzone_num=16;
			RETAILMSG(1,(TEXT("at88sc1616c found\r\n")));
		}		
		else if(memcmp(chip_info,chip_at88sc3216c,10)==0)
		{
			one_page_max=64;	
			one_userzone_max=256;
			userzone_num=16;
			RETAILMSG(1,(TEXT("at88sc3216c found\r\n")));
		}		
		else if(memcmp(chip_info,chip_at88sc6416c,10)==0)
		{
			one_page_max=64;	
			one_userzone_max=512;
			userzone_num=16;
			RETAILMSG(1,(TEXT("at88sc6416c found\r\n")));
		}		
		else if(memcmp(chip_info,chip_at88sc12816c,10)==0)
		{
			one_page_max=128;	
			one_userzone_max=1024;
			userzone_num=16;
			RETAILMSG(1,(TEXT("at88sc12816c found\r\n")));
		}		
		else if(memcmp(chip_info,chip_at88sc25616c,10)==0)
		{
			one_page_max=128;	
			one_userzone_max=2048;
			userzone_num=16;
			RETAILMSG(1,(TEXT("at88sc25616c found\r\n")));
		}		
		else
		{
			RETAILMSG(1,(TEXT("UnKnown at88sc chip\r\n")));
			int i;
			for(i=0;i<24;i++)
			{
				if(i%8==0 && i!=0)
					RETAILMSG(1,(TEXT("\n")));
				RETAILMSG(1,(TEXT("%4X "),chip_info[i]));		
			}
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
	int i;
	RETAILMSG(1,(TEXT("[	AT88SC ++]\r\n")));
	Virtual_Alloc();
	at88 at88;	
	at88.data=(unsigned char *)malloc(128);	
	memset(at88.data,123,128);	
	for(i=0;i<128;i++)		
		at88.data[i]=i;	
	for(i=0;i<3;i++)	
		{		
		at88.pw[i]=i;	
		}	
	for(i=0;i<8;i++)	
		{		
		at88.g[i]=i;	
		}		
	at88.addr=0;	
	at88.size=128;	
	if(ReadReg(&at88))
	{	
	RETAILMSG(1,(TEXT("\r\nRead user zone data again:\r\n")));
	for(i=0;i<at88.size;i++)	
		{		
		if(i%8==0 && i!=0)			
			RETAILMSG(1,(TEXT("\r\n")));		
		RETAILMSG(1,(TEXT("%4X "),at88.data[i])); 		
		}
	}
	else
	RETAILMSG(1,(TEXT("read at88sc user zone failed\r\n")));
	free(at88.data);
	RETAILMSG(1,(TEXT("[	AT88SC --]\r\n")));
	return TRUE;
}


BOOL WriteReg(at88* value)
{
	BOOL rc = FALSE;
	int index=0,local_size,local_addr;
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
	local_size=value->size;
	local_addr=value->addr;
	for(param.zone_index=user_zone_begin;param.zone_index<user_zone_end;param.zone_index++)
	{		
		param.addr=local_addr%one_userzone_max;
		if((param.addr+local_size)<=one_userzone_max)
			param.len=local_size;
		else
			param.len=one_userzone_max-param.addr;
			
		//memcpy(param.user_zone,value->data+index,param.len);
		param.user_zone=value->data+index;
		if(!userzone_proc(&param,0))
			return FALSE;
		local_size=local_size-param.len;		
		local_addr=local_addr+param.len;
		index=index+param.len;
	}
}

BOOL ReadReg(at88* value)
{
	int index=0,local_size,local_addr;
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
	local_size=value->size;
	local_addr=value->addr;
	for(param.zone_index=user_zone_begin;param.zone_index<user_zone_end;param.zone_index++)
	{		
		param.addr=local_addr%one_userzone_max;
		if((param.addr+local_size)<=one_userzone_max)
			param.len=local_size;
		else
			param.len=one_userzone_max-param.addr;
			
		//memcpy(param.user_zone,value->data+index,param.len);
		param.user_zone=value->data+index;
		/*rt_kprintf("user_zone %x\r\ng %x %x %x %x %x %x %x %x\r\npw %x %x %x\r\npage_size %x\r\nzone_index %x\r\n",
			param.user_zone,param.g[0],param.g[1],param.g[2],param.g[3],param.g[4],param.g[5],param.g[6],param.g[7],
			param.pw[0],param.pw[1],param.pw[2],param.page_size,param.zone_index);*/
		if(!userzone_proc(&param,1))
			return FALSE;
		local_size=local_size-param.len;		
		local_addr=local_addr+param.len;
		index=index+param.len;
	}
return TRUE;
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

			WriteReg((at88 *)pBufOut);

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
				ReadReg((at88 *)pBufOut);
				//RETAILMSG(1,(TEXT("Read %x="),*(BYTE*)pBufIn));
				#if debug
				for(i=0;i<dwLenOut;i++)
					RETAILMSG(1,(TEXT("%x "),*(BYTE*)(pBufOut+i)));
				#endif
				if(pdwActualOut) *(int*)pdwActualOut=sizeof(BYTE)*dwLenOut;
			}

			break;
			
			case IOCTL_AT88SC_READ_ID:
				if(pBufOut && dwLenOut>=sizeof(BYTE))
				{
		#if debug
					RETAILMSG(1,(TEXT("\r\nRead ID=\r\n")));
		#endif
					for(i=0;i<8;i++)
					*(BYTE *)pBufOut[i]=chip_info[16+i];
					dwLenOut=8;
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

