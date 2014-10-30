#include "CAT95552.h"
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
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����.  
********************************************************************/
void Start_I2c()
{
  SDA_H;         /*������ʼ�����������ź�*/
  Dlyus(1);
  SCL_H; 
  Dlyus(5);        /*��ʼ��������ʱ�����4.7us,��ʱ*/   
  SDA_L;          /*������ʼ�ź�*/
  Dlyus(5);        /* ��ʼ��������ʱ�����4��s*/
  //SCL_L;       /*ǯסI2C���ߣ�׼�����ͻ�������� */ 
}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:     ����I2C����,������I2C��������.  
********************************************************************/
void Stop_I2c()
{
	SCL_L;
	Dlyus(5);      /*���ͽ���������ʱ���ź�*/
	SDA_L;      /*���ͽ��������������ź�*/	
	Dlyus(5);
  SCL_H;      /*������������ʱ�����4��s*/
  Dlyus(5);
  SDA_H;      /*����I2C���߽����ź�*/
  Dlyus(6);
}
/*******************************************************************
                 ACKӦ�������             
����ԭ��: char Err_Check(void);
����:     ACK�����󣬷�������ֹͣ�ź� �� ���SDA��Ϊ�ͷ���9��SCLK��λ
					�ӻ�
********************************************************************/
void Err_Pro(void)
{
	unsigned char i;
	

		//��������
		//-----------------------------------------------------------------
		Stop_I2c();             
	  //-----------------------------------------------------------------
	
	
		//��λ�ӻ�
		//-----------------------------------------------------------------
		GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_INPUT);  
		Dlyus(5);
		if(GPIOGetBit(g_gpioH,GPIO_SDA)==0){//���SDA�Ƿ�Ϊ�͵�ƽ���Ƿ���9��SCLK,���и�λ
		
					for(i=0;i<9;i++)  //����9��CLK
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
		SDA_H;      /*����SDA*/
		Dlyus(5);
		//-----------------------------------------------------------------

}
/*******************************************************************
                 �ֽ����ݷ��ͺ���               
����ԭ��: void  SendByte(UCHAR c);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
           ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
char SendByte(unsigned char  Wb)
{
 unsigned char i,ack;

 for(i=0;i<8;i++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
    {
		 
		 SCL_L;
		 Dlyus(3);
     if(Wb&0x80)SDA_H;    /*�жϷ���λ*/
     else  SDA_L;           
     Wb = Wb<<1;	
		 Dlyus(3);
     SCL_H;              /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
     Dlyus(5);             /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/
              
    }
		
		SCL_L;
		Dlyus(3);
    SDA_L;                /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
    Dlyus(3);
    SCL_H; 
		Dlyus(1);
		GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_INPUT);  
		Dlyus(5);
	if(GPIOGetBit(g_gpioH,GPIO_SDA)==1) 
		ack=0;     
  else 
	  ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/
    		
  SCL_L; 
  Dlyus(3);
	GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_OUTPUT);  
	Dlyus(3);
	return ack;
	//if(ack==1)
	//	RETAILMSG(1,(TEXT("Get Ack\r\n")));
}

/*******************************************************************
                 �ֽ����ݽ��պ���               
����ԭ��: UCHAR  RcvByte();
����:        �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
          ���������Ӧ����Ӧ��ӻ���  
********************************************************************/    
unsigned char   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  int i,j;
  retc=0; 
  //GPIOSetBit(g_gpioH,GPIO_SDA);                   /*��������Ϊ���뷽ʽ*/
  GPIOSetMode(g_gpioH,GPIO_SDA,GPIO_DIR_INPUT);  
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        Dlyus(2);
        SCL_L;                   /*��ʱ����Ϊ�ͣ�׼����������λ*/ 
        Dlyus(6);                /*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
        SCL_H;                   /*��ʱ����Ϊ��ʹ��������������Ч*/
        Dlyus(2);
				
        retc=retc<<1;
        if(GPIOGetBit(g_gpioH,GPIO_SDA)==1)
				{
				retc=retc+1;  /*������λ,���յ�����λ����retc�� */
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
                     Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
********************************************************************/
void Ack_I2c(int a)
{
  int i,j;
  if(a==0)SDA_L;                   /*�ڴ˷���Ӧ����Ӧ���ź� */
  else SDA_H;       
  Dlyus(3);    
  SCL_H;       
  Dlyus(5);                    /*ʱ�ӵ͵�ƽ���ڴ���4��s*/
  SCL_L;                         /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
  Dlyus(6);   
}
int ISendByte(unsigned char sla,unsigned char c)
{
   unsigned char ack;
   Start_I2c();              //��������
	
   ack = SendByte(sla);            //����������ַ
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   //read_proc=TRUE;
   //RETAILMSG(1,(TEXT("ISendByte 1\r\n")));
   ack = SendByte(c);              //��������
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   //read_proc=FALSE;
   //   RETAILMSG(1,(TEXT("ISendByte 2\r\n")));

   Stop_I2c();               //��������
   return(1);
}
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c,ack;

   Start_I2c();          //��������
  // read_proc=TRUE;
   ack = SendByte(sla+1);      //����������ַ
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
  // read_proc=FALSE;
  //  RETAILMSG(1,(TEXT("IRcvByte 1\r\n")));

   c=RcvByte();          //��ȡ����0

   Ack_I2c(1);           //���ͷǾʹ�λ
   Stop_I2c();           //��������
//  RETAILMSG(1,(TEXT("Read==> %x\r\n"),c));
   return(c);
}
int PortConfig(unsigned char sla,unsigned char c,  unsigned char Val)
{
	unsigned char ack;
		
   Start_I2c();              //������
	
   ack = SendByte(sla);            //����������ַ
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(c);              //���Ϳ����ֽ�
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(Val);            //����DAC����ֵ  

   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   Stop_I2c();               //��������
   return(1);
}
int PortWrite(unsigned char sla,unsigned char c,  unsigned char Val)
{
	unsigned char ack;
		
   Start_I2c();              //������
	
   ack = SendByte(sla);            //����������ַ
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(c);              //���Ϳ����ֽ�
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(Val);            //����DAC����ֵ  

   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   Stop_I2c();               //��������
   return(1);
}
unsigned char PortRead(unsigned char sla,unsigned char c)
{
	unsigned char ack;
		
   Start_I2c();              //������
	
   ack = SendByte(sla);            //����������ַ
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   ack = SendByte(c);              //���Ϳ����ֽ�
   if(ack==0){
		 Err_Pro();
		 return(0);
	 }
   //ack = SendByte(Val);            //����DAC����ֵ  

   //if(ack==0){
	//	 Err_Pro();
	//	 return(0);
	// }
   //Stop_I2c();               //��������
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

BOOL CTA_Deinit(DWORD hDeviceContext)
{
	return TRUE;
} 
DWORD CTA_Init(DWORD dwContext)
{
	RETAILMSG(1,(TEXT("[	CAT95552++]\r\n")));
	Virtual_Alloc();
	PortConfig(0x48,CONFIG_PORT_0,0xff);
	PortConfig(0x48,CONFIG_PORT_1,0xff);
	RETAILMSG(1,(TEXT("[	CAT95552--]\r\n")));
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
BOOL CTA_IOControl(DWORD hOpenContext, 
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
	case IOCTL_CAT95552_WRITE:
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
				PortConfig(0x48,CONFIG_PORT_0,port_0_status);
				//RETAILMSG(1,(TEXT("Config Port %x Output\r\n"),*(BYTE *)pBufIn));
			}
			if(*(BYTE *)pBufOut==0x1)
			{//output 1
				if(!(write_port_0_status&(1<<*(UINT8 *)pBufIn)))
				{
					write_port_0_status |= (1<<*(UINT8 *)pBufIn);
					PortWrite(0x48,OUTPUT_PORT_0,write_port_0_status);
					//RETAILMSG(1,(TEXT("Write Port %x Level H\r\n"),*(BYTE *)pBufIn));
				}
			}else
			{
				if(write_port_0_status&(1<<*(UINT8 *)pBufIn))
				{
					write_port_0_status &= ~(1<<*(UINT8 *)pBufIn);
					PortWrite(0x48,OUTPUT_PORT_0,write_port_0_status);
					//RETAILMSG(1,(TEXT("Write Port %x Level L\r\n"),*(BYTE *)pBufIn));
				}
			}
		}
		else
		{
			if(port_1_status&(1<<(*(UINT8 *)pBufIn-8)))
			{
				port_1_status &= ~(1<<(*(UINT8 *)pBufIn-8));
				PortConfig(0x48,CONFIG_PORT_1,port_1_status);
				//RETAILMSG(1,(TEXT("Config Port %x Output\r\n"),*(BYTE *)pBufIn));
			}
			if(*(BYTE *)pBufOut==0x1){//output 1
				if(!(write_port_1_status&(1<<(*(UINT8 *)pBufIn-8))))
				{
					write_port_1_status |= 1<<(*(UINT8 *)pBufIn-8);
					PortWrite(0x48,OUTPUT_PORT_1,write_port_1_status);
					//RETAILMSG(1,(TEXT("Write Port %x Level H\r\n"),*(BYTE *)pBufIn));
				}
			}else
			{
				if(write_port_1_status&(1<<(*(UINT8 *)pBufIn-8)))
				{
					write_port_1_status &= ~(1<<(*(UINT8 *)pBufIn-8));
					PortWrite(0x48,OUTPUT_PORT_1,write_port_1_status);
					//RETAILMSG(1,(TEXT("Write Port %x Level L\r\n"),*(BYTE *)pBufIn));
				}
			}

		}
		break;
	case IOCTL_CAT95552_READ:
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
					PortConfig(0x48,CONFIG_PORT_0,port_0_status);
					//RETAILMSG(1,(TEXT("Config Port %x Input\r\n"),*(BYTE *)pBufIn));
				}
				if(!(PortRead(0x48,INPUT_PORT_0)&(1<<*(UINT8 *)pBufIn)))
					*(BYTE *)pBufOut=0x0;
				else
					*(BYTE *)pBufOut=0x1;				
			}
			else
			{
				if(!(port_1_status&(1<<(*(UINT8 *)pBufIn-8))))
				{
					port_1_status |= (1<<(*(UINT8 *)pBufIn-8));
					PortConfig(0x48,CONFIG_PORT_1,port_1_status);
					//RETAILMSG(1,(TEXT("Config Port %x Input\r\n"),*(BYTE *)pBufIn));
				}
				if(!(PortRead(0x48,INPUT_PORT_1)&(1<<(*(UINT8 *)pBufIn-8))))
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
DWORD CTA_Open(DWORD hDeviceContext, DWORD AccessCode, DWORD ShareMode)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Open\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL CTA_Close(DWORD hOpenContext)
{
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CTA_PowerDown(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_PowerDown\r\n")));
	
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CTA_PowerUp(DWORD hDeviceContext)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_PowerUp\r\n")));

} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD CTA_Read(DWORD hOpenContext, LPVOID pBuffer, DWORD Count)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Read\r\n")));
	return TRUE;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD CTA_Seek(DWORD hOpenContext, long Amount, DWORD Type)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Seek\r\n")));
	return 0;
} 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DWORD CTA_Write(DWORD hOpenContext, LPCVOID pSourceBytes, DWORD NumberOfBytes)
{
	//RETAILMSG(1,(TEXT("USERMUL: CTA_Write\r\n")));
	return 0;
}

