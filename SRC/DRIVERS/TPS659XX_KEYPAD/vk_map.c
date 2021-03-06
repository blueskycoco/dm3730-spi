// All rights reserved ADENEO EMBEDDED 2010
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
//  File: vk_map.c
//
//  This file containts definition for keyboard row/column to virtual key
//  mapping.
//
#include "bsp.h"
#include "tps659xx_keypad.h"
#include <winuserm.h>


#ifndef dimof
#define dimof(x)            (sizeof(x)/sizeof(x[0]))
#endif

#if KEYPAD_ROWS != 8 || KEYPAD_COLUMNS != 8
    #error g_keypadVK dimensions do not match KEYPAD_ROWS and/or KEYPAD_COLUMNS
#endif

//------------------------------------------------------------------------------
const UCHAR g_keypadVK[KEYPAD_ROWS * KEYPAD_COLUMNS] = {
//  Column 0    Column 1        Column 2    Column 3    Column 4    Column 5    Column 6    Column 7
    /* S18 */   /* S6 */        /* S8 */    /* S3* */   /* N/A */   /* N/A */   /* N/A */   /* N/A */
    VK_TACTION, VK_THOME,       0,          0,          0,          0,          0,          0,          // Row 0

    /* S15 */   /* S17 */       /* S5 */    /* S10 */   /* N/A */   /* N/A */
    VK_TDOWN,   VK_TRIGHT,      VK_TAB,     VK_TSOFT2,  0,          0,          0,          0,          // Row 1

    /* S12 */   /* S14 */       /* S16 */   /* S7 */    /* N/A */   /* N/A */
    VK_SPACE,   0,              VK_TBACK,   VK_TSOFT1,  0,          0,          0,          0,          // Row 2

    /* S9 */    /* S11 */       /* S13 */   /* S4 */    /* N/A */   /* N/A */
    0,          VK_TLEFT,       VK_TUP,     VK_ESCAPE,  0,          0,          0,          0,          // Row 3

    /* N/A */   /* N/A */       /* N/A */   /* N/A */   /* N/A */   /* N/A */
    0,          0,              0,          0,          0,          0,          0,          0,          // Row 4

    /* N/A */   /* N/A */       /* N/A */   /* N/A */   /* N/A */   /* N/A */
    0,          0,              0,          0,          0,          0,          0,          0,          // Row 5

    /* N/A */   /* N/A */       /* N/A */   /* N/A */   /* N/A */   /* N/A */
    0,          0,              0,          0,          0,          0,          0,          0,          // Row 6

    /* N/A */   /* N/A */       /* N/A */   /* N/A */   /* N/A */   /* N/A */
    0,          0,              0,          0,          0,          0,          0,          0,          // Row 7
 };

const UCHAR g_powerVK[1] = {
//  Column 0
    VK_OFF // Row 0
 };

//------------------------------------------------------------------------------
const KEYPAD_REMAP g_keypadRemap = { 0, NULL };
const KEYPAD_REMAP g_powerRemap = { 0, 0 };

//------------------------------------------------------------------------------

static const KEYPAD_REPEAT_ITEM repeatItems[] = {
    { VK_TUP,     400, 200, FALSE, NULL },
    { VK_TRIGHT,  400, 200, FALSE, NULL },
    { VK_TLEFT,   400, 200, FALSE, NULL },
    { VK_TDOWN,   400, 200, FALSE, NULL }
};

const KEYPAD_REPEAT g_keypadRepeat = { dimof(repeatItems), repeatItems };
const KEYPAD_REPEAT g_powerRepeat = { 0, 0 };

//------------------------------------------------------------------------------

const KEYPAD_EXTRASEQ g_keypadExtraSeq = { 0, NULL };

//------------------------------------------------------------------------------
