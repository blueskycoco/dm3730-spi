// All rights reserved ADENEO EMBEDDED 2010
// Copyright (c) 2007, 2008 BSQUARE Corporation. All rights reserved.

/*
================================================================================
*             Texas Instruments OMAP(TM) Platform Software
* (c) Copyright Texas Instruments, Incorporated. All Rights Reserved.
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied.
*
================================================================================
*/
//
//  File:  menu.c
//
#include <eboot.h>

//------------------------------------------------------------------------------
//
//  Define:  dimof
//
#ifdef dimof
#undef dimof
#endif
#define dimof(x)                (sizeof(x)/sizeof(x[0]))

extern void *ch7033;  //main.c


static unsigned long long div_64_rem(unsigned long long dividend,unsigned int divisor,unsigned int *remainder)
{
	*remainder = dividend % divisor;
	return dividend /divisor;
}
static unsigned long long div_64(unsigned long long dividend,unsigned int divisor)
{
	unsigned int remainder;
	return div_64_rem(dividend,divisor,&remainder);
}

/*
//1280x720 vga timmingstatic 
DWORD CH7033_RegTable[][2] = 
{	
       { 0x03, 0x04 },	
	{ 0x52, 0x01 },	
	{ 0x52, 0x03 },	
	{ 0x03, 0x00 },	
	{ 0x07, 0xD9 },	
	{ 0x08, 0xF1 },	
	{ 0x09, 0x03 },	
	{ 0x0B, 0x2C },	
	{ 0x0C, 0x00 },	
	{ 0x0D, 0x3F },	
	{ 0x0F, 0x12 },	
	{ 0x10, 0x35 },	
	{ 0x11, 0x1B },	
	{ 0x12, 0x00 },	
	{ 0x13, 0x26 },	
	{ 0x15, 0x03 },	
	{ 0x16, 0x06 },	
	{ 0x19, 0xC9 },	
	{ 0x1A, 0x19 },	
	{ 0x1B, 0x40 },	
	{ 0x1C, 0x69 },	
	{ 0x1D, 0x78 },	
	{ 0x1E, 0x80 },	
	{ 0x1F, 0x35 },	
	{ 0x20, 0x00 },	
	{ 0x21, 0x98 },	
	{ 0x25, 0x24 },	
	{ 0x26, 0x00 },	
	{ 0x27, 0x2A },	
	{ 0x2E, 0x3D },	
	{ 0x55, 0x30 },	
	{ 0x56, 0x70 },	
	{ 0x58, 0x01 },	
	{ 0x59, 0x03 },	
	{ 0x5A, 0x04 },	
	{ 0x5E, 0x54 },	
	{ 0x64, 0x30 },	
	{ 0x68, 0x46 },	
	{ 0x6A, 0x45 },	
	{ 0x6B, 0x90 },	
	{ 0x6D, 0x7C },	
	{ 0x6E, 0xAE },	
	{ 0x74, 0x30 },	
	{ 0x03, 0x01 },	
	{ 0x08, 0x05 },	
	{ 0x12, 0xD6 },	
	{ 0x13, 0x28 },	
	{ 0x15, 0x00 },	
	{ 0x23, 0xE3 },	
	{ 0x28, 0x54 },	
	{ 0x29, 0x60 },	
	{ 0x6B, 0x11 },	
	{ 0x03, 0x03 },	
	{ 0x03, 0x04 },	
	{ 0x10, 0x01 },	
	{ 0x11, 0xA5 },	
	{ 0x12, 0xE0 },	
	{ 0x13, 0x02 },	
	{ 0x14, 0x88 },	
	{ 0x15, 0x70 },	
	{ 0x54, 0xC4 },	
	{ 0x55, 0x5B },	
	{ 0x56, 0x4D },	
	{ 0x61, 0x60 },	
	{ 0x7F, 0xFE },
};
*/

static DWORD CH7033_RegTable[][2] = {
{ 0x03, 0x04 },
	{ 0x52, 0x01 },
	{ 0x52, 0x03 },
	{ 0x03, 0x00 },
	{ 0x07, 0xD9 },
	{ 0x08, 0xF1 },
	{ 0x09, 0x03 },
	{ 0x0B, 0x2C },
	{ 0x0C, 0x00 },
	{ 0x0D, 0x3F },
	{ 0x0F, 0x12 },
	{ 0x10, 0x35 },
	{ 0x11, 0x1B },
	{ 0x12, 0x00 },
	{ 0x13, 0x26 },
	{ 0x15, 0x03 },
	{ 0x16, 0x06 },
	{ 0x19, 0xC9 },
	{ 0x1A, 0x19 },
	{ 0x1B, 0x40 },
	{ 0x1C, 0x61 },
	{ 0x1D, 0xA8 },
	{ 0x1E, 0x80 },
	{ 0x1F, 0x2C },
	{ 0x20, 0x00 },
	{ 0x21, 0x40 },
	{ 0x25, 0x1B },
	{ 0x26, 0x00 },
	{ 0x27, 0x26 },
	{ 0x2E, 0x3D },
	{ 0x55, 0x12 },
	{ 0x56, 0x35 },
	{ 0x58, 0x03 },
	{ 0x59, 0x06 },
	{ 0x5A, 0x03 },
	{ 0x5B, 0xE6 },
	{ 0x5C, 0x66 },
	{ 0x5D, 0x66 },
	{ 0x5E, 0x4E },
	{ 0x64, 0x20 },
	{ 0x68, 0x46 },
	{ 0x6B, 0x60 },
	{ 0x6E, 0xA8 },
	{ 0x03, 0x01 },
	{ 0x08, 0x05 },
	{ 0x12, 0xD4 },
	{ 0x15, 0x00 },
	{ 0x23, 0xE3 },
	{ 0x28, 0x4E },
	{ 0x29, 0x20 },
	{ 0x6B, 0x11 },
	{ 0x03, 0x03 },
	{ 0x03, 0x04 },
	{ 0x11, 0xFD },
	{ 0x12, 0xE8 },
	{ 0x13, 0x02 },
	{ 0x14, 0x88 },
	{ 0x15, 0x70 },
	{ 0x54, 0xC4 },
	{ 0x55, 0x5B },
	{ 0x56, 0x4D },
	{ 0x61, 0x60 },
	{ 0x7F, 0xFE },
};
#define REGTABLE_VGA_LEN	((sizeof(CH7033_RegTable))/(2*sizeof(DWORD)))
DWORD i2c_write_data[2] = {0x00,0x04};
DWORD InitializeCH7033()
{
	DWORD i=0;
	DWORD val_t=0;
	DWORD value=0;
	DWORD hinc_reg=0,hinca_reg=0,hincb_reg=0;
	DWORD vinc_reg=0,vinca_reg=0,vincb_reg=0;
	DWORD hdinc_reg=0,hdinca_reg=0,hdincb_reg=0;
	for(i = 0;i < REGTABLE_VGA_LEN;i++)
	{
		I2CWrite(ch7033, CH7033_RegTable[i][0], &CH7033_RegTable[i][1], 1);
	}

	// calculate online parameters:
	I2CWrite(ch7033,0x03,&i2c_write_data[0],1);
	I2CRead(ch7033, 0x25,&i, 1);
	//RETAILMSG(1,(L"the 0x25 value is %x\r\n",i));
	I2CWrite(ch7033,0x03,&i2c_write_data[1],1);
	I2CRead(ch7033, 0x03,&value, 1);
	//RETAILMSG(1,(L"the 0x03 value is %x\r\n",value));
	//HINCA
	I2CRead(ch7033, 0x2A,&val_t, 1);
	//RETAILMSG(1,(L"the 0x2A value is %x\r\n",val_t));
	I2CRead(ch7033,0x2B,&value,1);
	//RETAILMSG(1,(L"the 0x2B value is %x\r\n",value));
	value &= 0x07;
	hinca_reg = (val_t << 3) | (value);
	//RETAILMSG(1,(L"the hinca_reg is %x\r\n",hinca_reg));
	//HINCB
	
	I2CRead(ch7033, 0x2C,&val_t, 1);
	I2CRead(ch7033,0x2D,&value,1);
	//RETAILMSG(1,(L"the 0x2C value is %x\r\n",val_t));
	//RETAILMSG(1,(L"the 0x2D value is %x\r\n",value));
	value &= 0x07;
	hincb_reg = (val_t << 3) | (value);
	//RETAILMSG(1,(L"the hincb_reg is %x\r\n",hincb_reg));
	//VINCA
	val_t =0;
	value =0;
	I2CRead(ch7033, 0x2E,&val_t, 1);
	I2CRead(ch7033,0x2F,&value,1);
	//RETAILMSG(1,(L"the 0x2E value is %x\r\n",val_t));
	//RETAILMSG(1,(L"the 0x2F value is %x\r\n",value));
	value &= 0x07;
	vinca_reg = (val_t << 3) | (value);
	//RETAILMSG(1,(L"the vinca_reg is %x\r\n",vinca_reg));
	//VINCB
	val_t =0;
	value =0;
	I2CRead(ch7033, 0x30,&val_t, 1);
	I2CRead(ch7033,0x31,&value,1);
	//RETAILMSG(1,(L"the 0x2E value is %x\r\n",val_t));
	//RETAILMSG(1,(L"the 0x2F value is %x\r\n",value));
	value &= 0x07;
	vincb_reg = (val_t << 3) | (value);
	//RETAILMSG(1,(L"the vincb_reg is %x\r\n",vincb_reg));
	//HDINCA
	val_t =0;
	value =0;
	I2CRead(ch7033, 0x32,&val_t, 1);
	I2CRead(ch7033,0x33,&value,1);
	//RETAILMSG(1,(L"the 0x2E value is %x\r\n",val_t));
	//RETAILMSG(1,(L"the 0x2F value is %x\r\n",value));
	value &= 0x07;
	hdinca_reg = (val_t << 3) | (value);
	//	RETAILMSG(1,(L"the hdinca_reg is %x\r\n",hdinca_reg));
	//HDINCB
	val_t =0;
	value =0;
	I2CRead(ch7033, 0x34,&val_t, 1);
	I2CRead(ch7033,0x35,&value,1);
//	RETAILMSG(1,(L"the 0x2E value is %x\r\n",val_t));
	//RETAILMSG(1,(L"the 0x2F value is %x\r\n",value));
	value &= 0x07;
	hdincb_reg = (val_t << 3) | (value);
//	RETAILMSG(1,(L"the hdincb_reg is %x\r\n",hdincb_reg));
	// no calculate hdinc if down sample disaled
	if(i & (1 << 6))
	{
	if(hdincb_reg == 0)  
		{
		RETAILMSG(1,(L"ERROR: hdincb_reg =0\r\n"));
		return 0;
	}
	hdinc_reg = (DWORD)div_64(((unsigned long long)hdinca_reg) * (1<<20),hdincb_reg);
//	RETAILMSG(1,(L"the hdinc_reg is %x\r\n",hdinc_reg));	
	
	value = (hdinc_reg >>16) & 0xff;
	I2CWrite(ch7033, 0x3C, &value, 1);
	value = (hdinc_reg >>8) & 0xff;
	I2CWrite(ch7033, 0x3D, &value, 1);
	value = (hdinc_reg >>0) & 0xff;
	I2CWrite(ch7033, 0x3C, &value, 1);
	}
	if(hincb_reg ==0 || vincb_reg == 0)
	{
		RETAILMSG(1,(L"ERROR: hincb_reg =0 or vincb_reg =0\r\n"));
		return 0;
	}
	if(hinca_reg > hincb_reg)
	{
		RETAILMSG(1,(L"ERROR: hinca > hincb =0\r\n"));
		return 0;
	}
	hinc_reg = (DWORD)div_64((unsigned long long )hinca_reg * (1<<20),hincb_reg);
      vinc_reg = (DWORD)div_64((unsigned long long)vinca_reg *(1<<20),vincb_reg);
 // RETAILMSG(1,(L"the hinc_reg is %x\r\n",hinc_reg));
	  RETAILMSG(1,(L"the vinc_reg is %x\r\n",vinc_reg));
	value = ((hinc_reg >>16) & 0xff);
	I2CWrite(ch7033, 0x36, &value, 1);

	I2CRead(ch7033, 0x36, &value, 1);
	//RETAILMSG(1,(L"the 0x36 is %x\r\n",value));
	value = ((hinc_reg >>8) & 0xff);
	I2CWrite(ch7033, 0x37, &value, 1);

	I2CRead(ch7033, 0x37, &value, 1);
	//RETAILMSG(1,(L"the 0x37 is %x\r\n",value));
	value = ((hinc_reg >> 0) & 0xff);
	I2CWrite(ch7033, 0x38, &value, 1);
	I2CRead(ch7033, 0x38, &value, 1);
	//RETAILMSG(1,(L"the 0x38 is %x\r\n",value));
	value = ((vinc_reg >>16) & 0xff);
	I2CWrite(ch7033, 0x39, &value, 1);
	I2CRead(ch7033, 0x39, &value, 1);
	//RETAILMSG(1,(L"the 0x39 is %x\r\n",value));
	value = ((vinc_reg >> 8) & 0xff);
	I2CWrite(ch7033, 0x3A, &value, 1);
	I2CRead(ch7033, 0x3A, &value, 1);
	//RETAILMSG(1,(L"the 0x3A is %x\r\n",value));
	value = ((vinc_reg >> 0) & 0xff);
	I2CWrite(ch7033, 0x3B, &value, 1);
	I2CRead(ch7033, 0x3B, &value, 1);
	//RETAILMSG(1,(L"the 0x3B is %x\r\n",value));
	// start to runing 
	//RETAILMSG(1,(L"RUNING:\r\n"));
	value = 0x00;
	I2CWrite(ch7033, 0x03, &value, 1);
	I2CRead(ch7033, 0x0A, &val_t, 1);
	value = (val_t | 0x80);
	I2CWrite(ch7033, 0x0A, &value, 1);
	value = (val_t & 0x7f);
	I2CWrite(ch7033, 0x0A, &value, 1);
	I2CRead(ch7033, 0x0A, &val_t, 1);
	value = (val_t & 0xEF);
	I2CWrite(ch7033, 0x0A, &value, 1);
	value = (val_t | 0x10);
	I2CWrite(ch7033, 0x0A, &value, 1);
	value = (val_t & 0xEF);
	I2CWrite(ch7033, 0x0A, &value, 1);
	I2CRead(ch7033, 0x0A, &val_t, 1);
	//RETAILMSG(1,(L" %d\r\n",val_t));
	return 1;
	
}

//------------------------------------------------------------------------------

static VOID SetMacAddress(OAL_BLMENU_ITEM *pMenu);
static VOID ShowSettings(OAL_BLMENU_ITEM *pMenu);
static VOID ShowNetworkSettings(OAL_BLMENU_ITEM *pMenu);
static VOID SetKitlMode(OAL_BLMENU_ITEM *pMenu);
static VOID SetKitlType(OAL_BLMENU_ITEM *pMenu);
static VOID SetDeviceID(OAL_BLMENU_ITEM *pMenu);
static VOID SaveSettings(OAL_BLMENU_ITEM *pMenu);
static VOID SetRetailMsgMode(OAL_BLMENU_ITEM *pMenu);
static VOID SetDisplayResolution(OAL_BLMENU_ITEM *pMenu);


#if BUILDING_EBOOT_SD
static VOID ShowSDCardSettings(OAL_BLMENU_ITEM *pMenu);
static VOID EnterSDCardFilename(OAL_BLMENU_ITEM *pMenu);
#endif

//------------------------------------------------------------------------------

// Functions
extern BOOL LAN911XFindController(void* pAddr, DWORD * pdwID);
extern BOOL LAN911XSetMacAddress(void* pAddr, UINT16 mac[3]);
extern BOOL LAN911XGetMacAddress(void* pAddr, UINT16 mac[3]);

extern OAL_BLMENU_ITEM g_menuFlash[];

//------------------------------------------------------------------------------

static OAL_BLMENU_ITEM g_menuNetwork[] = {
    {
        L'1', L"Show Current Settings", ShowNetworkSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Enable/disable KITL", OALBLMenuEnable,
        L"KITL", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_ENABLED
    }, {
        L'3', L"KITL interrupt/poll mode", SetKitlMode,
        NULL, NULL, NULL
    }, {
        L'4', L"KITL Active/Passive mode", SetKitlType,
        NULL, NULL, NULL
    }, {
        L'5', L"Enable/disable DHCP", OALBLMenuEnable,
        L"DHCP", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_DHCP
    }, {
        L'6', L"Set IP address", OALBLMenuSetIpAddress,
        L"Device", &g_bootCfg.ipAddress, NULL
    }, {
        L'7', L"Set IP mask", OALBLMenuSetIpMask,
        L"Device", &g_bootCfg.ipMask, NULL
    }, {
        L'8', L"Set default router", OALBLMenuSetIpAddress,
        L"Default router", &g_bootCfg.ipRoute, NULL
    }, {
        L'9', L"Enable/disable VMINI", OALBLMenuEnable,
        L"VMINI", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_VMINI
    }, {
        L'a', L"Set MAC address", SetMacAddress,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};

#if BUILDING_EBOOT_SD
static OAL_BLMENU_ITEM g_menuSDCard[] = {
    {
        L'1', L"Show Current Settings", ShowSDCardSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Enter Filename", EnterSDCardFilename,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#endif

#if BUILDING_EBOOT_SD
static OAL_BLMENU_ITEM g_menuMain[] = {
    {
        L'1', L"Show Current Settings", ShowSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Select Boot Device", OALBLMenuSelectDevice,
        L"Boot", &g_bootCfg.bootDevLoc, g_bootDevices
    }, {
        L'3', L"Select KITL (Debug) Device", OALBLMenuSelectDevice,
        L"Debug", &g_bootCfg.kitlDevLoc, g_kitlDevices
    }, {
        L'4', L"Network Settings", OALBLMenuShow,
        L"Network Settings", &g_menuNetwork, NULL
    }, {
        L'5', L"SDCard Settings", OALBLMenuShow,
        L"SDCard Settings", &g_menuSDCard, NULL
    }, {
        L'6', L"Set Device ID", SetDeviceID,
        NULL, NULL, NULL
    }, {
        L'7', L"Save Settings", SaveSettings,
        NULL, NULL, NULL
    }, {
        L'8', L"Flash Management", OALBLMenuShow,
        L"Flash Management", &g_menuFlash, NULL
    },  {
        L'9', L"Enable/Disable OAL Retail Messages", SetRetailMsgMode,
        NULL, NULL, NULL
    },  {
        L'a', L"Select Display Resolution", SetDisplayResolution,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#else
static OAL_BLMENU_ITEM g_menuMain[] = {
    {
        L'1', L"Show Current Settings", ShowSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Select Boot Device", OALBLMenuSelectDevice,
        L"Boot", &g_bootCfg.bootDevLoc, g_bootDevices
    }, {
        L'3', L"Select KITL (Debug) Device", OALBLMenuSelectDevice,
        L"Debug", &g_bootCfg.kitlDevLoc, g_kitlDevices
    }, {
        L'4', L"Network Settings", OALBLMenuShow,
        L"Network Settings", &g_menuNetwork, NULL
    }, {
        L'5', L"Flash Management", OALBLMenuShow,
        L"Flash Management", &g_menuFlash, NULL
    }, {
        L'6', L"Set Device ID", SetDeviceID,
        NULL, NULL, NULL
    }, {
        L'7', L"Save Settings", SaveSettings,
        NULL, NULL, NULL
    }, {
        L'8', L"Enable/Disable OAL Retail Messages", SetRetailMsgMode,
        NULL, NULL, NULL
    }, {
        L'9', L"Select Display Resolution", SetDisplayResolution,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#endif

static OAL_BLMENU_ITEM g_menuRoot = {
    0, NULL, OALBLMenuShow,
    L"Main Menu", g_menuMain, NULL
};

typedef struct OMAP_LCD_DVI_RES_MENU {
    DWORD       lcdDviRes;
    LPCWSTR     resName;
} OMAP_LCD_DVI_RES_MENU;

OMAP_LCD_DVI_RES_MENU dispResMenu[] = {
    {OMAP_LCD_DEFAULT,          L"LCD  480x272 60Hz"},
    {OMAP_DVI_640W_480H,        L"DVI  640x480 60Hz"},
    {OMAP_DVI_640W_480H_72HZ,   L"DVI  640x480 72Hz"},
    {OMAP_DVI_800W_480H,        L"LCD  800x480 60Hz"},
    {OMAP_DVI_800W_600H,        L"LVDS 800x600 60Hz"},
    {OMAP_DVI_800W_600H_56HZ,   L"LVDS 1024x600 56Hz"},
    {OMAP_DVI_1024W_768H,       L"VGA 1024x768 60Hz"},
    {OMAP_DVI_1280W_720H,       L"DVI 1280x720 60Hz"},
    {OMAP_RES_INVALID,          L"Invalid Entry"},
};



//------------------------------------------------------------------------------

VOID BLMenu(BOOL bForced)
{
    UINT32 time, delay = 5;
    WCHAR key = 0;
    BSP_ARGS *pArgs = OALCAtoUA(IMAGE_SHARE_ARGS_CA);
    
    // First let user break to menu
    while (!bForced && (delay > 0 && key != L' ')) {
       OALLog(L"Hit space to enter configuration menu %d...\r\n", delay);
       time = OALGetTickCount();
       while ((OALGetTickCount() - time) < 1000) {
          if ((key = OALBLMenuReadKey(FALSE)) == L' ') break;
       }
       delay--;
    }
    
    if ((bForced == TRUE) || (key == L' ')) 
    {
        OALBLMenuShow(&g_menuRoot);
        // Invalidate arguments to force them to be reinitialized
        // with new config data generated by the boot menu
        pArgs->header.signature = 0;
    }        
}

//------------------------------------------------------------------------------

VOID SetMacAddress(OAL_BLMENU_ITEM *pMenu)
{
    DWORD dummy;
#pragma warning(push)
#pragma warning(disable:4204 4221)
    UINT16 mac[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    // We're using OALBLMenuSetMacAddress outside of a menu, but it still requires 
    // a menu item input structure
    OAL_BLMENU_ITEM DummyMacMenuItem = 
    {
        0, NULL, NULL,
        L"EVM ", mac, NULL
    };
#pragma warning(pop)

    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n Set Ethernet (LAN9xxx) MAC Address:\r\n");   
	
	
    memset(mac,0xFF,sizeof(mac));
    LAN911XGetMacAddress(OALPAtoUA((DWORD) BSP_LAN9115_REGS_PA),mac);
    if (((mac[0]== 0xFFFF) && (mac[1] == 0xFFFF) && (mac[2] == 0xFFFF)) || ((mac[0]== 0x0) && (mac[1] == 0x0) && (mac[2] == 0x0)))
    {
        RETAILMSG(1,(TEXT("mac address not found in EEPROM. Using Mac address stored with the boot settings\r\n")));
        memcpy(mac,g_bootCfg.mac,sizeof(mac));       
    }

    OALBLMenuSetMacAddress(&DummyMacMenuItem);
    
    // Make sure we don't come out of OALBLMenuSetMacAddress with the original 0xFFFF FFFF FFFF or with all zeros
    if (((mac[0]== 0xFFFF) && (mac[1] == 0xFFFF) && (mac[2] == 0xFFFF)) || ((mac[0]== 0x0) && (mac[1] == 0x0) && (mac[2] == 0x0)))
    {
        OALLog(L"\r\n   WARNING: MAC address not programmed!\r\n");
        return;
    }
        
   memcpy(g_bootCfg.mac,mac,sizeof(mac));


    if (!LAN911XFindController(OALPAtoUA((DWORD) BSP_LAN9115_REGS_PA),&dummy))
    {     
        return;
    }
   LAN911XSetMacAddress(OALPAtoUA((DWORD) BSP_LAN9115_REGS_PA),mac);  
}

//------------------------------------------------------------------------------

VOID ShowSettings(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n Main:\r\n");
    OALLog(
        L"  Boot device:   %s\r\n",
        OALKitlDeviceName(&g_bootCfg.bootDevLoc, g_bootDevices)
    );
    OALLog(
        L"  Debug device:  %s\r\n",
        (g_bootCfg.kitlDevLoc.LogicalLoc) ? OALKitlDeviceName(&g_bootCfg.kitlDevLoc, g_kitlDevices) : L"not specified"
    );
    OALLog(
        L"  Retail Msgs:   %s\r\n",
        (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) ? L"enabled" : L"disabled"
    );
    OALLog(
        L"  Device ID:     %d\r\n", g_bootCfg.deviceID
    );
    OALLog(
        L"  Display Res:   %s\r\n", dispResMenu[g_bootCfg.displayRes].resName
    );
    OALLog(
        L"  Flashing NK.bin:   %s\r\n", 
        (g_bootCfg.flashNKFlags & ENABLE_FLASH_NK) ? L"enabled" : L"disabled"
    );
	
#if BUILDING_EBOOT_SD
    ShowSDCardSettings(pMenu);
#endif
    ShowNetworkSettings(pMenu);
}

//------------------------------------------------------------------------------

VOID ShowNetworkSettings(OAL_BLMENU_ITEM *pMenu)
{
    BOOL fValidExtMacAddr;
    UINT16 mac[3];
    UNREFERENCED_PARAMETER(pMenu);

    memset(mac,0xFF,sizeof(mac));
    LAN911XGetMacAddress(OALPAtoUA((DWORD) BSP_LAN9115_REGS_PA),mac);
    if (((mac[0]== 0xFFFF) && (mac[1] == 0xFFFF) && (mac[2] == 0xFFFF)) || ((mac[0]== 0x0) && (mac[1] == 0x0) && (mac[2] == 0x0)))
    {
        fValidExtMacAddr = FALSE;        
    }
    else
    {
        fValidExtMacAddr = TRUE;        
    }


    OALLog(L"\r\n Network:\r\n");
    OALLog(
        L"  KITL state:    %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_ENABLED) ? L"enabled" : L"disabled"
    );

    OALLog(
        L"  KITL type:     %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) ? L"passive" : L"active"
    );

    OALLog(
        L"  KITL mode:     %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) ? L"polled" : L"interrupt"
    );
    OALLog(
        L"  DHCP:          %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_DHCP) ? L"enabled" : L"disabled"
    );
    OALLog(L"  IP address:    %s\r\n", OALKitlIPtoString(g_bootCfg.ipAddress));
    OALLog(L"  IP mask:       %s\r\n", OALKitlIPtoString(g_bootCfg.ipMask));
    OALLog(L"  IP router:     %s\r\n", OALKitlIPtoString(g_bootCfg.ipRoute));
    OALLog(L"  Eth MAC Addr:  %s (%s)\r\n", 
        OALKitlMACtoString(fValidExtMacAddr ? mac : g_bootCfg.mac)
        ,fValidExtMacAddr ? L"EEPROM" : L"Boot settings");
    OALLog(
        L"  VMINI:         %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_VMINI) ? L"enabled" : L"disabled"
    );
    OALLog(L"  Note: USBFN RNDIS MAC Addr cannot be changed.\r\n");
    
    OALBLMenuReadKey(TRUE);
}

//------------------------------------------------------------------------------

VOID SetKitlMode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) != 0) {
        OALLog(L" Set KITL to interrupt mode [y/-]: ");
    } else {
        OALLog(L" Set KITL to polled mode [y/-]: ");
    }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') {
        if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) != 0) {
            g_bootCfg.kitlFlags &= ~OAL_KITL_FLAGS_POLL;
            OALLog(L" KITL set to interrupt mode\r\n");
        } else {
            g_bootCfg.kitlFlags |= OAL_KITL_FLAGS_POLL;
            OALLog(L" KITL set to polled mode\r\n");
        }            
    }
}

//------------------------------------------------------------------------------

VOID
SetKitlType(
    OAL_BLMENU_ITEM *pMenu
    )
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) != 0) 
        {
        OALLog(L" Set KITL to active mode [y/-]: ");
        } 
   else 
        {
        OALLog(L" Set KITL to passive mode [y/-]: ");
        }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') 
        {
        if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) != 0) 
            {
            g_bootCfg.kitlFlags &= ~OAL_KITL_FLAGS_PASSIVE;
            OALLog(L" KITL set to active mode\r\n");
            }
        else 
            {
            g_bootCfg.kitlFlags |= OAL_KITL_FLAGS_PASSIVE;
            OALLog(L" KITL set to passive mode\r\n");
            }            
        }
}

//------------------------------------------------------------------------------

VOID 
SetDeviceID(
    OAL_BLMENU_ITEM *pMenu
    )
{
    WCHAR  szInputLine[16];
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(
        L" Current Device ID:  %d\r\n", g_bootCfg.deviceID
    );

    OALLog(L"\r\n New Device ID: ");

    if (OALBLMenuReadLine(szInputLine, dimof(szInputLine)) == 0) 
        {
        goto cleanUp;
        }

    // Get device ID
    g_bootCfg.deviceID = OALStringToUINT32(szInputLine);

cleanUp:
    return;
}

//------------------------------------------------------------------------------

VOID SetRetailMsgMode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;

    UNREFERENCED_PARAMETER(pMenu);

    if (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) {
        OALLog(L" Disable OAL Retail Messages [y/-]: ");
    } else {
        OALLog(L" Enable OAL Retail Messages [y/-]: ");
    }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') {
        if (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) 
		{
            g_bootCfg.oalFlags &= ~BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE;
            OALLog(L" OAL Retail Messages disabled\r\n");
        }
		else 
		{
            g_bootCfg.oalFlags |= BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE;
            OALLog(L" OAL Retail Messages enabled\r\n");
        }    
    }
}

//------------------------------------------------------------------------------

VOID SetDisplayResolution(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UINT32 i;

    UNREFERENCED_PARAMETER(pMenu);
    
    OALBLMenuHeader(L"Select Display Resolution");

    for (i=0; i<OMAP_RES_INVALID; i++)
    {
        OALLog(L" [%d] %s\r\n", i+1 , dispResMenu[i].resName);
    }
    OALLog(L" [0] Exit and Continue\r\n");

    OALLog(L"\r\n Selection (actual %s): ", dispResMenu[g_bootCfg.displayRes].resName);
    // Get key
    do {
        key = OALBLMenuReadKey(TRUE);  
    } while (key < L'0' || key > L'0' + i);    
    OALLog(L"%c\r\n", key);
    
    // If user select exit don't change device
    if (key == L'0') return;

    g_bootCfg.displayRes = (key - L'0' - 1);

	if(g_bootCfg.displayRes == 6)
	{
	}
	
}


//------------------------------------------------------------------------------

VOID SaveSettings(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L" Do you want save current settings [-/y]? ");

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    // Depending on result
    if (key != L'y' && key != L'Y') goto cleanUp;

    if (BLWriteBootCfg(&g_bootCfg)) {
        OALLog(L" Current settings has been saved\r\n");
    } else {        
        OALLog(L"ERROR: Settings save failed!\r\n");
    }

cleanUp:
    return;
}

//------------------------------------------------------------------------------

UINT32 OALStringToUINT32(LPCWSTR psz)
{
    UINT32 i = 0;

    // Replace the dots with NULL terminators
    while (psz != NULL && *psz != L'\0') 
        {
        if (*psz < L'0' || *psz > L'9') 
            {
            break;
            }
        i = i * 10 + (*psz - L'0');
        psz++;
        }

    return i;
}

#if BUILDING_EBOOT_SD

//------------------------------------------------------------------------------

VOID ShowSDCardSettings(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n SDCard:\r\n");
    OALLog(
        L"  Filename:      \"%s\"\r\n", g_bootCfg.filename
    );
//  OALBLMenuReadKey(TRUE);
}

//------------------------------------------------------------------------------
#define dimof(x)                (sizeof(x)/sizeof(x[0]))

VOID EnterSDCardFilename(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n Type new filename (8.3 format) :");
    OALBLMenuReadLine(g_bootCfg.filename, dimof(g_bootCfg.filename));
}

//------------------------------------------------------------------------------

#endif
