//
// Sampl_GPIO_StepMotor_17HS5604 : GPIO to control StepMotor 
// Stepper Motor = 17HS5604, 12V, 0.4A, 1.8 degree
// Driver IC = ULN2003A
//
// driver IC control pins : 
// INA -> GPA3
// INB -> GPA2
// INC -> GPA1
// IND -> GPA0
//
#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"

// Definitions for Step Motor turning degree
#define d360 400

unsigned char CW[8] ={0x08,0x0a,0x02,0x06,0x04,0x05,0x01,0x09}; // Clockwise
unsigned char CCW[8]={0x09,0x01,0x05,0x04,0x06,0x02,0x0a,0x08}; // 

//Counter-Clockwise

void SpinMotor(uint8_t dir, uint16_t step)
{
  int i=0;
  while(step>0)
  {
    if (dir) GPIOA->DOUT=CW[i];
    else     GPIOA->DOUT=CCW[i];
    DrvSYS_Delay(12000); // Delay 5000us = 5ms
    i++;
    if (i>7) i = 0;
    step--;
  }
}

int main (void)
{
    int flag=0;
    UNLOCKREG();
    DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1); // Enable the 12MHz oscillator 
    DrvSYS_SelectHCLKSource(0); // HCLK clock source. 0: external 12MHz;
    LOCKREG();
    DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0);
    DrvGPIO_Open(E_GPB, 10, E_IO_INPUT);//change port accordingly
    DrvGPIO_Open(E_GPB, 14, E_IO_INPUT);
    DrvGPIO_Open(E_GPB, 12, E_IO_INPUT);
    DrvGPIO_ClrBit(E_GPB,10);
    DrvGPIO_ClrBit(E_GPB,14);
    DrvGPIO_ClrBit(E_GPB,12);
    flag=1;
    while(1)
    {
        if(DrvGPIO_GetBit(E_GPB,10)==1 && flag==1)
        {
        SpinMotor(0,24);
        flag=0;
        }
    else if(DrvGPIO_GetBit(E_GPB,12)==1 && flag==1)
    {
    SpinMotor(1,24);
        flag=2;
    }
    else if(DrvGPIO_GetBit(E_GPB,14)==1 && flag==0)
    {
    SpinMotor(1,24);
        flag=1;
    }
    else if(DrvGPIO_GetBit(E_GPB,12)==1 && flag==0)
    {
    SpinMotor(1,48);
        flag=2;
    }
    else if(DrvGPIO_GetBit(E_GPB,14)==1 && flag==2)
    {
    SpinMotor(0,24);
        flag=1;
    }
    else if(DrvGPIO_GetBit(E_GPB,10)==1 && flag==2)
    {
    SpinMotor(0,48);
        flag=0;
    }
}
}
