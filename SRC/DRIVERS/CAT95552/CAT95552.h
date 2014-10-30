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
#define CAT9555_DEVICE_COOKIE       0x11223366
#define IOCTL_CAT95552_WRITE       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0322, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CAT95552_READ       \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0323, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define INPUT_PORT_0 	0x0
#define INPUT_PORT_1 	0x1
#define OUTPUT_PORT_0 	0x2
#define OUTPUT_PORT_1 	0x3
#define CONVERT_PORT_0 	0x4
#define CONVERT_PORT_1 	0x5
#define CONFIG_PORT_0 	0x6
#define CONFIG_PORT_1 	0x7