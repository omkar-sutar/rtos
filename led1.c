//Program to count number of pulses
#include "config.h"
#include "stdlib.h"
#include <stdio.h>

#define	TaskStkLengh	64				//Define the Task0 stack length 
 
OS_STK	TaskStk0 [TaskStkLengh];		//Define the Task stack 
OS_STK	TaskStk1 [TaskStkLengh];


void 	Task0(void *pdata);		
void 	Task1(void *pdata);		
			  

char buffer[16];
unsigned int count=0;
unsigned char err;
int flag=0;


OS_EVENT* pSem;

// Protoytpe of interrupt initialization function 
void irq_init(void);
// Protoytpe of interrupt service routine 1 
void isr_int1(void) __irq;

int main (void)

{   
	LED_init();
	
	
	TargetInit();	
	OSInit ();
	lcd_init();
	irq_init();

	
	pSem=OSSemCreate(0);
	OSTaskCreate (Task0,(void *)0, &TaskStk0[TaskStkLengh - 1], 6);	
	OSTaskCreate (Task1,(void *)0, &TaskStk1[TaskStkLengh - 1], 7);
	
	OSStart();
	return 0;															
}
void Task0	(void *pdata)
{
	pdata = pdata;						        /* Dummy data */
	LCD_SendData("Count: 0");
	while(1)
	{		 
		OSSemPend(pSem,0,&err);
		count++;
		lcd_command(0x01);
		lcd_command(0x80);
		sprintf(buffer,"Count: %d",count);
		LCD_SendData(buffer);
		flag=0;
	}
}

void Task1	(void *pdata)
{
	pdata = pdata;						        /* Dummy data */
	
	while(1)
	{		 
		LED_on(0);
		OSTimeDly(5);
		LED_off(0);
		OSTimeDly(5);
	}
}

void isr_int1(void) __irq
{		
	if (flag==0)
	{
		flag=1;
		OSSemPost(pSem);
	}
		VICIntEnable = 0x00000000;
		// clear the flag
		EXTINT = 0x00000002;
		VICVectAddr = 0x0;
}

void irq_init(void)
{
 	// select the EXINT1
	PINSEL0|=(3<<6);
	VICIntSelect = 0x00000000;
	
	VICVectAddr10 = (unsigned)isr_int1;
	VICVectCntl10 = 0x0000002F;	
	// set the mode to edge triggered 
	EXTMODE = 0x00000006;
	// enable interrupts
	VICIntEnable = 0x00018000;
}

