/*
 *	brife: this is at88sc driver code,gpio code 
 */
#include "s1.h"
#include "sdk_gpio.h"
#include "gpio_ioctls.h"
HANDLE  hGpio;

 /*config scl,sda as gpio mode and dir out*/
 #define DELAY 10
 #define GPIO_SCL 156
 #define GPIO_SDA 157
 void i2c_init(void)
{
	 hGpio = GPIOOpen();
	 GPIOSetMode(hGpio, GPIO_SCL, GPIO_DIR_OUTPUT );
	 GPIOSetMode(hGpio, GPIO_SDA, GPIO_DIR_OUTPUT );
	 GPIOSetBit(hGpio, GPIO_SCL);
	 GPIOSetBit(hGpio, GPIO_SDA);
}
/*scl set hi or low*/
void i2c_scl_set(unsigned char level)
{
    volatile unsigned long delay = 0;
    if(level)
        GPIOSetBit(hGpio, GPIO_SCL);
    else
        GPIOClrBit(hGpio, GPIO_SCL);
    for (delay = 0; delay < DELAY; )
    {
        delay++;
    }
}
/*sda set hi or low*/
void i2c_sda_set(unsigned char level)
{
    volatile unsigned long delay = 0;
    if(level)
        GPIOSetBit(hGpio, GPIO_SDA);
    else
        GPIOClrBit(hGpio, GPIO_SDA);
    for (delay = 0; delay < DELAY; )
    {
        delay++;
    }
}
/*return sda level */
unsigned char i2c_sda_get(void)
{
		return (unsigned char)GPIOGetBit(hGpio,GPIO_SDA);
}
/*config sda dir input*/
void i2c_sda_input(void)
{
    volatile unsigned long delay = 0;
	GPIOSetMode(hGpio, GPIO_SDA, GPIO_DIR_INPUT );
    for (delay = 0; delay < DELAY; )
    {
        delay++;
    }
}
/*config sda dir output*/
void i2c_sda_output(void)
{
	GPIOSetMode(hGpio, GPIO_SDA, GPIO_DIR_OUTPUT );
}
/*sleep function*/
void sleep_ms(unsigned long n)
{
	volatile unsigned long i,j;
	for(i=0;i<n;i++)
	for(j=0;j<DELAY;j++)
	;
}
