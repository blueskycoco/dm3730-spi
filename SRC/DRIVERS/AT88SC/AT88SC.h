#include <windows.h>
//#include <ceddk.h>
#include <nkintr.h>
#include <pm.h>
#include "omap.h"
#include "bsp_cfg.h"
#include "soc_cfg.h"
#include "sdk_i2c.h"
#include <ceddkex.h>

//------------------------------------------------------------------------------
#define AT88SC_DEVICE_COOKIE       0x11223377
#define IOCTL_AT88SC_WRITE       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0322, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_AT88SC_READ       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0323, METHOD_BUFFERED, FILE_ANY_ACCESS)
    
#define IOCTL_AT88SC_READ_ID       \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0324, METHOD_BUFFERED, FILE_ANY_ACCESS)
typedef struct {
	UINT8 *data;
	UINT8 g[8];
	UINT8 pw[3];
	UINT16 addr;
	UINT16 size;
}at88,*pat88;
	
