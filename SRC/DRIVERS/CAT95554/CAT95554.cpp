#include "CAT95554.h"
#define CAT9555_I2C_DEVICE_ADDR 0x48
//#define USE_IIC_BUS
#ifdef USE_IIC_BUS
HANDLE    m_hI2C;
#else
HANDLE g_gpioH;
DWORD GPIO_SCL = 133;//144
DWORD GPIO_SDA=134;//145
unsigned char port_0_status=0xff;// 0 for output , 1 for input
unsigned char port_1_status=0xff;// 0 for output , 1 for input
unsigned char write_port_0_status=0xff;
unsigned char write_port_1_status=0xff;


//int ack=-1;


#define SDA_H GPIOSetBit(g_gpioH,GPIO_SDA)
#define SDA_L GPIOClrBit(g_gpioH,GPIO_SDA)
#define SCL_H GPIOSetBit(g_gpioH,GPIO_SCL)
#define SCL_L GPIOClrBit(g_gpioH,GPIO_SCL)

#endif
void Dlyus(unsigned char a)
{
	volatile unsigned int i,j,k;
	k = 80*a;
	for(i=0;i<k;i++) j=0;

}

//#define _Nop for(i=0;i<100;i++) j=0
/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void Start_I2c()
{
  SDA_H;         /*发送起始条件的数据信号*/
  Dlyus(1);
  SCL_H; 
  Dlyus(5);        /*起始条件建立时间大于4.7us,延时*/   
  SDA_L;          /*发送起始信号*/
  Dlyus(5);        /* 起始条件锁定时间大于4μs*/
  //SCL_L;       /*钳住I2C总线，准备发送或接收数据 */ 
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void Stop_I2c()
{
	SCL_L;
	Dlyus(5);      /*发送结束条件的时钟信号*/
	SDA_L;      /*发送结束条件的数据信号*/	
	Dlyus(5);
  SCL_H;      /*结束条件建立时间大于4μs*/
  Dlyus(5);
  SDA_H;      /*发送I2C总线结束信号*/
  Dlyus(6);
}
/*******************************************************************
                 ACK应答错误处理             
函数原型: char Err_Check(void);
功能:     ACK读错误，发送总线停止信号 ， 检测SDA，为低发送9个SCLK复位
					从机
********************************************************************/
void Err_Pro(void)
{
	unsigned char i;
	

		//结束总线
		//-----------------------------------------------------------------
		Stop_I2c();             
	  //-----------------------------------------------------------------
	
	
		//复位从机
		//-----------------------------------------------------------------
		GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_INPUT);  
		Dlyus(5);
		if(GPIOGetBit(g_gpioH,GPIO_SDA)==0){//检测SDA是否为低电平，是发送9个SCLK,进行复位
		
					for(i=0;i<9;i++)  //发送9个CLK
					{
						Dlyus(6);
						SCL_L;        
						Dlyus(5);        
						SCL_H; 
						if(GPIOGetBit(g_gpioH,GPIO_SDA)==1)break;
					}
					
		}
		GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_OUTPUT);  	
		Dlyus(5);
		SDA_H;      /*拉高SDA*/
		Dlyus(5);
		//-----------------------------------------------------------------

}
/*******************************************************************
                 字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
           发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
char SendByte(unsigned char  Wb)
{
 unsigned char i,ack;

 for(i=0;i<8;i++)  /*要传送的数据长度为8位*/
    {
		 
		 SCL_L;
		 Dlyus(3);
     if(Wb&0x80)SDA_H;    /*判断发送位*/
     else  SDA_L;           
     Wb = Wb<<1;	
		 Dlyus(3);
     SCL_H;              /*置时钟线为高，通知被控器开始接收数据位*/
     Dlyus(5);             /*保证时钟高电平周期大于4μs*/
              
    }
		
		SCL_L;
		Dlyus(3);
    SDA_L;                /*8位发送完后释放数据线，准备接收应答位*/
    Dlyus(3);
    SCL_H; 
		Dlyus(1);
		GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_INPUT);  
		Dlyus(5);
	if(GPIOGetBit(g_gpioH,GPIO_SDA)==1) 
		ack=0;     
  else 
	  ack=1;        /*判断是否接收到应答信号*/
    		
  SCL_L; 
  Dlyus(3);
	GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_OUTPUT);  
	Dlyus(3);
	return ack;
	if(ack==1)
	RETAILMSG(1,(TEXT("Get Ack\r\n")));
}

/*******************************************************************
                 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能:        用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/    
unsigned char   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  int i,j;
  retc=0; 
  //GPIOSetBit(g_gpioH,GPIO_SDA);                   /*置数据线为输入方式*/
  GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_INPUT);  
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        Dlyus(2);
        SCL_L;                   /*置时钟线为低，准备接收数据位*/ 
        Dlyus(6);                /*时钟低电平周期大于4.7μs*/
        SCL_H;                   /*置时钟线为高使数据线上数据有效*/
        Dlyus(2);
				
        retc=retc<<1;
        if(GPIOGetBit(g_gpioH,GPIO_SDA)==1)
				{
				retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
				//RETAILMSG(1,(TEXT("Get 1\r\n")));
      	}
        Dlyus(3);
      }
  SCL_L;       
  Dlyus(6);
  GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_OUTPUT);  
  return(retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(int a)
{
  int i,j;
  if(a==0)SDA_L;                   /*在此发出应答或非应答信号 */
  else SDA_H;       
  Dlyus(3);    
  SCL_H;       
  Dlyus(5);                    /*时钟低电平周期大于4μs*/
  SCL_L;                         /*清时钟线，钳住I2C总线以便继续接收*/
  Dlyus(6);   
}
int ISendByte(unsigned char sla,unsigned char c)
{
   unsigned char ack;
   Start_I2c();              //启动总线
	
   ack = SendByte(sla);            //发送器件地址
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   //read_proc=TRUE;
   //RETAILMSG(1,(TEXT("ISendByte 1\r\n")));
   ack = SendByte(c);              //发送数据
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   //read_proc=FALSE;
   //   RETAILMSG(1,(TEXT("ISendByte 2\r\n")));

   Stop_I2c();               //结束总线
   return(1);
}
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c,ack;

   Start_I2c();          //启动总线
  // read_proc=TRUE;
   ack = SendByte(sla+1);      //发送器件地址
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
  // read_proc=FALSE;
  //  RETAILMSG(1,(TEXT("IRcvByte 1\r\n")));

   c=RcvByte();          //读取数据0

   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
//  RETAILMSG(1,(TEXT("Read==> %x\r\n"),c));
   return(c);
}
int PortConfig(unsigned char sla,unsigned char c,  unsigned char Val)
{
	unsigned char ack;
		
   Start_I2c();              //启动总
	
   ack = SendByte(sla);            //发送器件地址
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(c);              //发送控制字节
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(Val);            //发送DAC的数值  

   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   Stop_I2c();               //结束总线
   return(1);
}
int PortWrite(unsigned char sla,unsigned char c,  unsigned char Val)
{
	unsigned char ack;
		
   Start_I2c();              //启动总
	
   ack = SendByte(sla);            //发送器件地址
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(c);              //发送控制字节
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(Val);            //发送DAC的数值  

   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   Stop_I2c();               //结束总线
   return(1);
}
unsigned char PortRead(unsigned char sla,unsigned char c)
{
	unsigned char ack;
		
   Start_I2c();              //启动总
	
   ack = SendByte(sla);            //发送器件地址
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(c);              //发送控制字节
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   //ack = SendByte(Val);            //发送DAC的数值  

   //if(ack==0){
	//	 Err_Pro();
	//	 return(0);
	// }
   //Stop_I2c();               //结束总线
   return IRcvByte(sla);
}

BOOL Virtual_Alloc()
{
#ifdef USE_IIC_BUS
	m_hI2C = I2COpen(SOCGetI2CDeviceByBus(1)); //nmcca: using I2C3_MEMBASE make sense doesnt return a good value... so nothing is opened. Using hardcoded value 3 for now     
	I2CSetSlaveAddress(m_hI2C,  CAT9555_I2C_DEVICE_ADDR); //nmcca: there is nothing to check to see if the handle is null...
	I2CSetSubAddressMode(m_hI2C, I2C_SUBADDRESS_MODE_8);    
#else
	g_gpioH = GPIOOpen();
	GPIOSetMode(g_gpioH,GPIO_SCL,GPIO_DIR_OUTPUT);
	GPIOSetBit(g_gpioH,GPIO_SCL);
	GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_OUTPUT);    	 
	GPIOSetBit(g_gpioH,GPIO_SDA);
	//while(1);
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

BOOL TAC_Deinit(DWORD hDeviceContext)
{
          RETAILMSG(1,(TEXT("[	CAT95554  aaaaaaaaaaaaaaaaaaaaaa]\r\n")));
	return TRUE;
} 
DWORD TAC_Init(DWORD dwContext)//----------------------------------------
{
	RETAILMSG(1,(TEXT("[	CAT95554++]\r\n")));
	Virtual_Alloc();
	PortConfig(0x42,CONFIG_PORT_0,0xff);
	PortConfig(0x42,CONFIG_PORT_1,0xff);
	RETAILMSG(1,(TEXT("[	CAT95554--]\r\n")));
	return TRUE;
}
#if 0
BOOL WriteReg(UINT8 control,UINT8* value,UINT8 size)
{
    BOOL rc = FALSE;
	#ifdef USE_IIC_BUS
    if (m_hI2C)
    {
        DWORD len = I2CWrite(m_hI2C, control, value, sizeof(UINT8)*size);
        if ( len != sizeof(UINT8)*size)
            ERRORMSG(ZONE_ERROR,(TEXT("Write CAT9555 Failed!!\r\n")));
            else
               rc = TRUE;
        }
	#else
	DACconversion(0x90,control, *value);
  
	#endif
    return rc;
}
BOOL ReadReg(UINT8 control,UINT8* data,UINT8 size)
{

    BOOL rc = FALSE;
	#ifdef USE_IIC_BUS
    if (m_hI2C)
    {
        DWORD len = I2CRead(m_hI2C, control,data, sizeof(UINT8)*size);
        if ( len != sizeof(UINT8)*size)
            ERRORMSG(ZONE_ERROR,(TEXT("Read CAT9555 Failed!!\r\n")));
            else
               rc = TRUE;
        }
	#else
	ISendByte(0x90,control);
	//Sleep(100);
	*data= IRcvByte(0x90);
	#endif
    return rc;

}
#endif
//-----------------------------------------------------------------------------
BOOL TAC_IOControl(DWORD hOpenContext, 
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
	case IOCTL_CAT95554_WRITE://------------------------
		//RETAILMSG(1,(TEXT("[	CAT95553-- IOCTL_CAT95554_WRITE  ]\r\n")));
		//GPIOSetBit(g_gpioH,GPIO_DIR);
		//IOWRITE(*(BYTE*)pBufIn,(*(BYTE*)(pBufOut+1)<<8)|*(BYTE*)(pBufOut));	
		//WriteReg(*(UINT8 *)pBufIn,(BYTE *)pBufOut,dwLenOut);
		//RETAILMSG(1,(TEXT("Driver Write =\r\n")));
		//for(i=0;i<dwLenOut;i++)
		//RETAILMSG(1,(TEXT("Write Port %x Level%x %x %x %x %x\r\n"),*(BYTE *)pBufIn,*(BYTE*)(pBufOut),port_0_status,port_1_status,write_port_0_status,write_port_1_status));
		if(*(UINT8 *)pBufIn<=7 && *(UINT8 *)pBufIn>=0)
		{
			if(port_0_status&(1<<*(UINT8 *)pBufIn))
			{
				port_0_status &= ~(1<<*(UINT8 *)pBufIn);
				PortConfig(0x42,CONFIG_PORT_0,port_0_status);//--------------------------------------------
				//RETAILMSG(1,(TEXT("Config Port %x Output\r\n"),*(BYTE *)pBufIn));
			}
			if(*(BYTE *)pBufOut==0x1)
			{//output 1
				if(!(write_port_0_status&(1<<*(UINT8 *)pBufIn)))
				{
					write_port_0_status |= (1<<*(UINT8 *)pBufIn);
					if(PortWrite(0x42,OUTPUT_PORT_0,write_port_0_status))
					{
					 RETAILMSG(1,(TEXT("[	CAT95554-- OK ]\r\n")));
					 }//---------------------------------------
					else{RETAILMSG(1,(TEXT("[	CAT95554-- FALSE]\r\n")));}
					//RETAILMSG(1,(TEXT("Write Port %x Level H\r\n"),*(BYTE *)pBufIn));
				}
			}
			else
			{
				if(write_port_0_status&(1<<*(UINT8 *)pBufIn))
				{
					write_port_0_status &= ~(1<<*(UINT8 *)pBufIn);
					PortWrite(0x42,OUTPUT_PORT_0,write_port_0_status);//-------------------------------------
					//RETAILMSG(1,(TEXT("Write Port %x Level L\r\n"),*(BYTE *)pBufIn));
				}
			}
		}
		else
		{
			if(port_1_status&(1<<(*(UINT8 *)pBufIn-8)))
			{
				port_1_status &= ~(1<<(*(UINT8 *)pBufIn-8));
				PortConfig(0x42,CONFIG_PORT_1,port_1_status);//----------------------------
				//RETAILMSG(1,(TEXT("Config Port %x Output\r\n"),*(BYTE *)pBufIn));
			}
			if(*(BYTE *)pBufOut==0x1){//output 1
				if(!(write_port_1_status&(1<<(*(UINT8 *)pBufIn-8))))
				{
					write_port_1_status |= 1<<(*(UINT8 *)pBufIn-8);
					PortWrite(0x42,OUTPUT_PORT_1,write_port_1_status);//-----------------------------------
					//RETAILMSG(1,(TEXT("Write Port %x Level H\r\n"),*(BYTE *)pBufIn));
				}
			}else
			{
				if(write_port_1_status&(1<<(*(UINT8 *)pBufIn-8)))
				{
					write_port_1_status &= ~(1<<(*(UINT8 *)pBufIn-8));
					PortWrite(0x42,OUTPUT_PORT_1,write_port_1_status);
					//RETAILMSG(1,(TEXT("Write Port %x Level L\r\n"),*(BYTE *)pBufIn));
				}
			}

		}
		break;
	case IOCTL_CAT95554_READ://---------------------------
			//RETAILMSG(1,(TEXT("[	CAT95553-- IOCTL_CAT95554_READ  ]\r\n")));
		if(pBufOut && dwLenOut>=sizeof(BYTE))
		{
			//USHORT outValue;
			//GPIOClrBit(g_gpioH,GPIO_DIR);
			//*(USHORT*)pBufOut = IOREAD(*(BYTE*)pBufIn);
			//RETAILMSG(0,(TEXT("Read =\r\n")));
			//ReadReg(*(UINT8 *)pBufIn,(BYTE*)pBufOut,dwLenOut);
			//RETAILMSG(1,(TEXT("Read %x="),*(BYTE*)pBufIn));
			//for(i=0;i<dwLenOut;i++)
			//RETAILMSG(0,(TEXT("%x\r\n"),*(BYTE*)(pBufOut+i)));
			
			if(*(UINT8 *)pBufIn<=7 && *(UINT8 *)pBufIn>=0)
			{
				if(!(port_0_status&(1<<*(UINT8 *)pBufIn)))
				{
					port_0_status |= (1<<*(UINT8 *)pBufIn);
					PortConfig(0x42,CONFIG_PORT_0,port_0_status);//-----------------------------------
					//RETAILMSG(1,(TEXT("Config Port %x Input\r\n"),*(BYTE *)pBufIn));
				}
				if(!(PortRead(0x42,INPUT_PORT_0)&(1<<*(UINT8 *)pBufIn)))//-------------------------------
					*(BYTE *)pBufOut=0x0;
				else
					*(BYTE *)pBufOut=0x1;				
			}
			else
			{
				if(!(port_1_status&(1<<(*(UINT8 *)pBufIn-8))))
				{
					port_1_status |= (1<<(*(UINT8 *)pBufIn-8));
					PortConfig(0x42,CONFIG_PORT_1,port_1_status);//------------------------
					//RETAILMSG(1,(TEXT("Config Port %x Input\r\n"),*(BYTE *)pBufIn));
				}
				if(!(PortRead(0x42,INPUT_PORT_1)&(1<<(*(UINT8 *)pBufIn-8))))//-----------------------
					*(BYTE *)pBufOut=0x0;
				else
					*(BYTE *)pBufOut=0x1;
			}
			//RETAILMSG(1,(TEXT("Read Port %x Level%x %x %x %x %x\r\n"),*(BYTE *)pBufIn,*(BYTE*)(pBufOut),port_0_status,port_1_status,write_port_0_status,write_port_1_status));
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
DWORD TAC_Open(DWORD hDeviceContext, DWORD AccessCode, DWORD ShareMode)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Open\r\n")));
	//RETAILMSG(1,(TEXT("[	CAT95554-- OPEN  ]\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL TAC_Close(DWORD hOpenContext)
{
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TAC_PowerDown(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_PowerDown\r\n")));
	
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TAC_PowerUp(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_PowerUp\r\n")));

} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD TAC_Read(DWORD hOpenContext, LPVOID pBuffer, DWORD Count)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Read\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD TAC_Seek(DWORD hOpenContext, long Amount, DWORD Type)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Seek\r\n")));
	return 0;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD TAC_Write(DWORD hOpenContext, LPCVOID pSourceBytes, DWORD NumberOfBytes)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Write\r\n")));
	return 0;
}

