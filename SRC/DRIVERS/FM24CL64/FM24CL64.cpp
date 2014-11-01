#include "FM24CL64.h"
#include "ceddkex.h"
#include "sdk_gpio.h"
#include "gpio_ioctls.h"
#ifndef USE_25115A
HANDLE    m_hI2C;
#define FM24CL64_I2C_DEVICE_ADDR 0x50
#else
#include "sdk_spi.h"
#include "omap_mcspi_regs.h"
UINT16 g_save_addr=0;
HANDLE  hSPI;
HANDLE  hGpio;
#endif
BOOL ReadReg(UINT8* data,UINT8 size);
BOOL WriteReg(UINT8* value,UINT8 size);

BOOL Virtual_Alloc()
{
	unsigned char send[3]={0x00,0x00,0x34};
	unsigned char recv[2]={0x05,0x00};
#ifndef USE_25115A
	m_hI2C = I2COpen(SOCGetI2CDeviceByBus(3)); //nmcca: using I2C3_MEMBASE make sense doesnt return a good value... so nothing is opened. Using hardcoded value 3 for now     
	I2CSetSlaveAddress(m_hI2C,  FM24CL64_I2C_DEVICE_ADDR); //nmcca: there is nothing to check to see if the handle is null...
	I2CSetSubAddressMode(m_hI2C, I2C_SUBADDRESS_MODE_0);  
#else
	DWORD  config;
	hGpio = GPIOOpen();
	GPIOSetMode(hGpio, 162, GPIO_DIR_OUTPUT );
	GPIOSetMode(hGpio, 158, GPIO_DIR_OUTPUT );
	GPIOSetBit(hGpio, 162);
	GPIOSetBit(hGpio, 158);
	hSPI = SPIOpen(L"SPI2:");
	if (hSPI == NULL) 
	{
		RETAILMSG(1, (TEXT("ERROR: PddInitializeHardware: Failed open SPI device driver\r\n")));
		return FALSE;
	}
	config =    MCSPI_PHA_ODD_EDGES |
		MCSPI_POL_ACTIVEHIGH |
		MCSPI_CHCONF_CLKD(11) |
		MCSPI_CSPOLARITY_ACTIVELOW |
		MCSPI_CHCONF_WL(8) |
		MCSPI_CHCONF_TRM_TXRX |
		MCSPI_CHCONF_DPE0;
	if (!SPIConfigure(hSPI, 0, config))
	{
		RETAILMSG(1, (TEXT("ERROR: PddInitializeHardware: Failed configure SPI device driver\r\n")));
		return FALSE;
	}
	//RETAILMSG(1,(TEXT("Use 25115A chip\r\n")));
	SPILockController(hSPI,INFINITE);
	/*SPIEnableChannel(hSPI);
	SPIWriteRead(hSPI, sizeof(UINT8)*2, recv,send);
	SPIDisableChannel(hSPI);
	RETAILMSG(1,(TEXT("get mode %x \r\n"),send[1]));
	send[1]=0x00;
	WriteReg(send,3);
	Sleep(10);
	g_save_addr=0x0000;
	recv[0]=0xff;
	ReadReg((BYTE *)recv,1);
	RETAILMSG(1,(TEXT("get data %x \r\n"),recv[0]));*/
#endif
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

BOOL FMC_Deinit(DWORD hDeviceContext)
{
	return TRUE;
} 
DWORD FMC_Init(DWORD dwContext)
{
	RETAILMSG(1,(TEXT("[	SPI FM24CL64++]\r\n")));
	Virtual_Alloc();
	RETAILMSG(1,(TEXT("[	SPI FM24CL64--]\r\n")));
	return TRUE;
}
BOOL WriteReg(UINT8* value,UINT8 size)
{
	BOOL rc = FALSE;
#ifndef USE_25115A
	if (m_hI2C)
	{
		DWORD len = I2CWrite(m_hI2C, 0, value, sizeof(UINT8)*size);
		if ( len != sizeof(UINT8)*size)
			ERRORMSG(ZONE_ERROR,(TEXT("Write FM24CL64 Failed!!\r\n")));
		else
			rc = TRUE;
	}
	
	return rc;
#else
	if(hSPI)
	{
		int i;
		unsigned char *out_buffer=(unsigned char *)malloc(size+1);
		unsigned char *in_buffer=(unsigned char *)malloc(size+1);
		out_buffer[0]=0x2;
		memcpy(&(out_buffer[1]),value,size);
		/*RETAILMSG(1,(TEXT("\r\nWrite OUT>\r\n")));
		for(i=0;i<size+1;i++)
			RETAILMSG(1,(TEXT("%x "),out_buffer[i]));*/
		SPIEnableChannel(hSPI);
		SPIWriteRead(hSPI, sizeof(UINT8)*(size+1), out_buffer,in_buffer);
		SPIDisableChannel(hSPI);
		free(in_buffer);
		free(out_buffer);
	}
	return TRUE;
#endif
	}
BOOL ReadReg(UINT8* data,UINT8 size)
{
#ifndef USE_25115A
	BOOL rc = FALSE;
	if (m_hI2C)
	{
		DWORD len = I2CRead(m_hI2C, 0,data, sizeof(UINT8)*size);
		if ( len != sizeof(UINT8)*size)
			ERRORMSG(ZONE_ERROR,(TEXT("Read FM24CL64 Failed!!\r\n")));
		else
			rc = TRUE;
	}
	return rc;
#else
	if(hSPI)
	{
		int i;
		unsigned char *out_buffer=(unsigned char *)malloc(size+3);
		unsigned char *in_buffer=(unsigned char *)malloc(size+3);
		memset(out_buffer,0,size+3);
		out_buffer[0]=0x3;
		out_buffer[1]=(g_save_addr)&0xff;
		out_buffer[2]=(g_save_addr>>8)&0xff;
		/*RETAILMSG(1,(TEXT("\r\nRead OUT>\r\n")));
		for(i=0;i<3;i++)
			RETAILMSG(1,(TEXT("%x "),out_buffer[i]));*/
		SPIEnableChannel(hSPI);
		SPIWriteRead(hSPI, sizeof(UINT8)*(size+3), out_buffer,in_buffer);
		SPIDisableChannel(hSPI);
		/*RETAILMSG(1,(TEXT("\r\nIN<\r\n")));
		for(i=0;i<size+3;i++)
			RETAILMSG(1,(TEXT("%x "),in_buffer[i]));*/
		memcpy(data,&(in_buffer[3]),size);
		free(out_buffer);
		free(in_buffer);
	}
	return TRUE;
#endif
	

}

//-----------------------------------------------------------------------------
BOOL FMC_IOControl(DWORD hOpenContext, 
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
		case IOCTL_FM24CL64_WRITE:

#ifndef USE_25115A
			WriteReg((BYTE *)pBufOut,dwLenOut);
#else
			if(dwLenOut==2)
			{
				g_save_addr=*(UINT16 *)pBufOut;
			}else
				WriteReg((BYTE *)pBufOut,dwLenOut);

#endif
			RETAILMSG(1,(TEXT("\r\nWrite =\r\n")));
			for(i=0;i<dwLenOut;i++)
				RETAILMSG(1,(TEXT("%x "),*(BYTE*)(pBufOut+i)));
			break;
		case IOCTL_FM24CL64_READ:
			if(pBufOut && dwLenOut>=sizeof(BYTE))
			{
				RETAILMSG(1,(TEXT("\r\nRead =\r\n")));
				ReadReg((BYTE*)pBufOut,dwLenOut);
				//RETAILMSG(1,(TEXT("Read %x="),*(BYTE*)pBufIn));
				for(i=0;i<dwLenOut;i++)
					RETAILMSG(1,(TEXT("%x "),*(BYTE*)(pBufOut+i)));
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
DWORD FMC_Open(DWORD hDeviceContext, DWORD AccessCode, DWORD ShareMode)
{
	//RETAILMSG(1,(TEXT("USERMUL: FMC_Open\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL FMC_Close(DWORD hOpenContext)
{
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void FMC_PowerDown(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: FMC_PowerDown\r\n")));

} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void FMC_PowerUp(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: FMC_PowerUp\r\n")));

} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD FMC_Read(DWORD hOpenContext, LPVOID pBuffer, DWORD Count)
{
	//RETAILMSG(1,(TEXT("USERMUL: FMC_Read\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD FMC_Seek(DWORD hOpenContext, long Amount, DWORD Type)
{
	//RETAILMSG(1,(TEXT("USERMUL: FMC_Seek\r\n")));
	return 0;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD FMC_Write(DWORD hOpenContext, LPCVOID pSourceBytes, DWORD NumberOfBytes)
{
	//RETAILMSG(1,(TEXT("USERMUL: FMC_Write\r\n")));
	return 0;
}

