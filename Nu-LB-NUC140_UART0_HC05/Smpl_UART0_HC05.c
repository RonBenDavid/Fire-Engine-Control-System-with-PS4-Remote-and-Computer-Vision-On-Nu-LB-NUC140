//
// Smpl_UART0_HC05 : bluetooth module receive 8 bytes and display on LCD
//
// HC05 Bluetooth module
// pin1 : KEY   N.C
// pin2 : VCC   to Vcc +5V
// pin3 : GND   to GND
// pin4 : TXD   to NUC140 UART0-RX (GPB0)
// pin5 : RXD   to NUC140 UART0-TX (GPB1)
// pin6 : STATE N.C.
//Ron Ben david
//ID 307906263
#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"
#include "NUC1xx-LB_002\LCD_Driver.h"
#include <string.h>
#include <stdlib.h>


// #define DATASIZE 8

// int32_t main()
// {
//     uint8_t  read_buf[16] = "                ";
//     char     TEXT[16]     = "                ";
//     STR_UART_T sParam;

//     UNLOCKREG();
//     DrvSYS_Open(48000000);
//     LOCKREG();
//    	
//     DrvGPIO_InitFunction(E_FUNC_UART0); // Set UART pins

//     /* UART Setting */
//     sParam.u32BaudRate 	= 9600;
//     sParam.u8cDataBits 	= DRVUART_DATABITS_8;
//     sParam.u8cStopBits 	= DRVUART_STOPBITS_1;
//     sParam.u8cParity 	= DRVUART_PARITY_NONE;
//     sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

// 	/* Set UART Configuration */
// if(DrvUART_Open(UART_PORT0,&sParam) != E_SUCCESS);

// Initial_panel();                 // initialize LCD panel
// clr_all_panel();                 // clear LCD panel					 	
// print_lcd(0, "Smpl_UART0_HC05"); // print title
//     		   
//  while(1) {
// // read 8 bytes from bluetooth
//  DrvUART_Read(UART_PORT0,read_buf,DATASIZE); 
//  sprintf(TEXT,"%s",read_buf); // display on LCD
//  print_lcd(1, TEXT);
//  }
// 	
// }



char TEXT[8];
char TEXT2[32];
char TEXT_LCD1[16];
char TEXT_LCD2[16];
volatile uint8_t comRbuf[9];
volatile uint8_t comRbytes = 0;
char Forward_STR[4];
char Backward_STR[4];
char Right_STR[4];
char Left_STR[4];
char Water_STR[4];
char Stop_Water_STR[4];
char LED_STR[4];
char Stop_LED_STR[4];
char Speed_PWM_STR[4];
char Servo_PWM_STR[4];
int Forward_FLAG;
int Backward_FLAG;
int Right_FLAG;
int Left_FLAG;
int Water_FLAG;
int Stop_Water_FLAG;
int LED_FLAG;
int Stop_LED_FLAG;
int flag_water;
int Speed_PWM_INT;
int Servo_PWM_INT;
#define PWM_CLOCK_FREQUENCY 12000000  // PWM clock frequency is 12 MHz
#define DUTY_CYCLE 0x8000  // Duty cycle, can be changed to control motor speed
#define PWM_FREQUENCY 50 // Set the frequency to 50 Hz
#define PWM_PERIOD (PWM_CLOCK_FREQUENCY / PWM_FREQUENCY)
#define MAX_DUTY_CYCLE (PWM_PERIOD / 256)

#define  PWM_CLKSRC_SEL   0        //0: 12M, 1:32K, 2:HCLK, 3:22M
#define  PWM_ClockSource  12000000 // 12M
#define  PWM_PreScaler    119      // clock is divided by (PreScaler + 1)
#define  PWM_ClockDivider 4        // 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1

#define  SERVO_CYCTIME        2000 // 20ms
#define  SERVO_HITIME_MIN       50 // minimum Hi width = 0.5ms
#define  SERVO_HITIME_MAX      250 // maximum Hi width = 2.5ms
#define  HITIME_MIN 0
#define  HITIME_MAX 135
void InitPWM()
{
						 SYS->GPAMFP.PWM0_AD13=1;     // Enable PWM0 multi-function pin
             SYSCLK->CLKSEL1.PWM01_S = 0; // Select 12Mhz for PWM clock source		
             SYSCLK->APBCLK.PWM01_EN =1;  // Enable PWM0 & PWM1 clock	
	           PWMA->PPR.CP01=1;			      // Prescaler 0~255, Setting 0 to stop output clock
	           PWMA->CSR.CSR0=0;			      // PWM clock = clock source/(Prescaler + 1)/divider
	           PWMA->PCR.CH0MOD=1;			    // 0:One-shot mode, 1:Auto-load mode
 								                          // CNR and CMR will be auto-cleared after setting CH0MOD form 0 to 1.	
	           PWMA->CNR0=PWM_PERIOD;           // CNR : counting down   // PWM output high if CMRx+1 >= CNR
	           PWMA->CMR0=DUTY_CYCLE;		        // CMR : fix to compare  // PWM output low  if CMRx+1 <  CNR
	           PWMA->PCR.CH0INV=0;          // Inverter->0:off, 1:on
	           PWMA->PCR.CH0EN=1;			      // PWM function->0:Disable, 1:Enable
  	         PWMA->POE.PWM0=1;			      // Output to pin->0:Diasble, 1:Enable		

		
						 SYS->GPAMFP.PWM1_AD14=1;     // Enable PWM1 multi-function pin
             SYSCLK->CLKSEL1.PWM01_S = 0; // Select 12Mhz for PWM clock source		
             SYSCLK->APBCLK.PWM01_EN =1;  // Enable PWM0 & PWM1 clock	
	           PWMA->PPR.CP01=1;			      // Prescaler 0~255, Setting 0 to stop output clock
	           PWMA->CSR.CSR1=0;			      // PWM clock = clock source/(Prescaler + 1)/divider
	           PWMA->PCR.CH1MOD=1;			    // 0:One-shot mode, 1:Auto-load mode
								                          // CNR and CMR will be auto-cleared after setting CH1MOD form 0 to 1.	
	           PWMA->CNR1=PWM_PERIOD;           // CNR : counting down   // PWM output high if CMRx+1 >= CNR
	           PWMA->CMR1=DUTY_CYCLE;		        // CMR : fix to compare  // PWM output low  if CMRx+1 <  CNR		
	           PWMA->PCR.CH1INV=0;          // Inverter->0:off, 1:on
	           PWMA->PCR.CH1EN=1;			      // PWM function->0:Disable, 1:Enable
  	         PWMA->POE.PWM1=1;			      // Output to pin->0:Diasble, 1:Enable
						 
						 SYS->GPAMFP.PWM2_AD15=1;     // Enable PWM2 multi-function pin
             SYSCLK->CLKSEL1.PWM23_S = PWM_CLKSRC_SEL; // Select 12Mhz for PWM clock source		
             SYSCLK->APBCLK.PWM23_EN =1;  // Enable PWM2 & PWM3 clock	
	           PWMA->PPR.CP23=1;			      // Prescaler 0~255, Setting 0 to stop output clock
	           PWMA->CSR.CSR2=0;			      // PWM clock = clock source/(Prescaler + 1)/divider
	           PWMA->PCR.CH2MOD=1;			    // 0:One-shot mode, 1:Auto-load mode
								                          // CNR and CMR will be auto-cleared after setting CH2MOD form 0 to 1.	
	           PWMA->CNR2=0xFFFF;           // CNR : counting down   // PWM output high if CMRx+1 >= CNR
	           PWMA->CMR2=0xFFFF;		        // CMR : fix to compare  // PWM output low  if CMRx+1 <  CNR
	           PWMA->PCR.CH2INV=0;          // Inverter->0:off, 1:on
	           PWMA->PCR.CH2EN=1;			      // PWM function->0:Disable, 1:Enable
  	         PWMA->POE.PWM2=1;			      // Output to pin->0:Diasble, 1:Enable	
						 
				 					 						 
}

void PWM_Stop(uint8_t PWM_no)
{		
	switch(PWM_no) {
		case 0 : PWMA->POE.PWM0=0; break;
		case 1 : PWMA->POE.PWM1=0; break;
		case 2 : PWMA->POE.PWM2=0; break;
		case 3 : PWMA->POE.PWM3=0; break;
		case 4 : PWMB->POE.PWM0=0; break;
		case 5 : PWMB->POE.PWM1=0; break;
		case 6 : PWMB->POE.PWM2=0; break;
		case 7 : PWMB->POE.PWM3=0; break;				
		default: PWMA->POE.PWM0=1;
		         PWMA->POE.PWM1=1;
						 PWMA->POE.PWM2=1;
						 PWMA->POE.PWM3=1;
		         PWMB->POE.PWM0=1;
						 PWMB->POE.PWM1=1;
						 PWMB->POE.PWM2=1;
						 PWMB->POE.PWM3=1;
						 break;
		}
}

// cyctime & hitime unit is 10us
void PWM_Servo(uint8_t PWM_no, uint16_t Servo_HiTime)
{
    //PWM_FreqOut = PWM_Clock / (PWM_PreScaler + 1) / PWM_ClockDivider / (PWM_CNR + 1); 
	  // Duty Cycle = (CMR0+1) / (CNR0+1)

	  //PWM setting	 
		switch(PWM_no) {
			case 0 : PWMA->CSR.CSR0 = 0;             // divider factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMA->PPR.CP01 = PWM_PreScaler; // set PreScaler
			         PWMA->CNR0 = SERVO_CYCTIME -1;    // set CNR
	             PWMA->CMR0 = Servo_HiTime -1;     // set CMR
						   PWMA->POE.PWM0=1;
			         break;
			case 1 : PWMA->CSR.CSR1 = 0;             // divider factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMA->PPR.CP01 = PWM_PreScaler; // set PreScaler
			         PWMA->CNR1 = SERVO_CYCTIME -1;    // set CNR
	             PWMA->CMR1 = Servo_HiTime -1;     // set CMR
			         PWMA->POE.PWM1=1;
			         break;
			case 2 : PWMA->CSR.CSR2 = 0;             // diver factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMA->PPR.CP23 = PWM_PreScaler; // set PreScaler
			         PWMA->CNR2 = SERVO_CYCTIME -1;    // set CNR
	             PWMA->CMR2 = Servo_HiTime -1;     // set CMR
			         PWMA->POE.PWM2=1;
			         break;
			case 3 : PWMA->CSR.CSR3 = 0;             // diver factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMA->PPR.CP23 = PWM_PreScaler; // set PreScaler
			         PWMA->CNR3 = SERVO_CYCTIME -1;    // set CNR
	             PWMA->CMR3 = Servo_HiTime -1;     // set CMR
							 PWMA->POE.PWM3=1;
			         break;
			case 4 : PWMB->CSR.CSR0 = 0;             // diver factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMB->PPR.CP01 = PWM_PreScaler; // set PreScaler
			         PWMB->CNR0 = SERVO_CYCTIME -1;    // set CNR
	             PWMB->CMR0 = Servo_HiTime -1;     // set CMR
							 PWMB->POE.PWM0=1;
			         break;
			case 5 : PWMB->CSR.CSR1 = 0;             // diver factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMB->PPR.CP01 = PWM_PreScaler; // set PreScaler
			         PWMB->CNR1 = SERVO_CYCTIME -1;    // set CNR
	             PWMB->CMR1 = Servo_HiTime -1;     // set CMR
							 PWMB->POE.PWM1=1;
			         break;
			case 6 : PWMB->CSR.CSR2 = 0;             // diver factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMB->PPR.CP23 = PWM_PreScaler; // set PreScaler
			         PWMB->CNR2 = SERVO_CYCTIME -1;    // set CNR
	             PWMB->CMR2 = Servo_HiTime -1;     // set CMR
							 PWMB->POE.PWM2=1;
			         break;
			case 7 : PWMB->CSR.CSR3 = 0;             // diver factor = 0: 1/2, 1: 1/4, 2: 1/8, 3: 1/16, 4: 1
	             PWMB->PPR.CP23 = PWM_PreScaler; // set PreScaler
			         PWMB->CNR3 = SERVO_CYCTIME -1;    // set CNR
	             PWMB->CMR3 = Servo_HiTime -1;     // set CMR
							 PWMB->POE.PWM3=1;
			         break;			
			default: break;
	}
}



void InitGPIO()
{
	DrvGPIO_Open(E_GPA,0,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,1,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,2,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,3,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,4,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,5,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,6,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,7,E_IO_OUTPUT);
	DrvGPIO_ClrBit(E_GPA,0);
	DrvGPIO_ClrBit(E_GPA,1);
	DrvGPIO_ClrBit(E_GPA,2);
	DrvGPIO_ClrBit(E_GPA,3);
	DrvGPIO_ClrBit(E_GPA,4);
	DrvGPIO_SetBit(E_GPA,5);
	DrvGPIO_SetBit(E_GPA,6);
	DrvGPIO_SetBit(E_GPA,7);
}

void Forward()
{
	DrvGPIO_SetBit(E_GPA,0); 
	DrvGPIO_ClrBit(E_GPA,1);
	DrvGPIO_SetBit(E_GPA,2);
	DrvGPIO_ClrBit(E_GPA,3);
}

void Backward()
{
	DrvGPIO_ClrBit(E_GPA,0); 
	DrvGPIO_SetBit(E_GPA,1);
	DrvGPIO_ClrBit(E_GPA,2);
	DrvGPIO_SetBit(E_GPA,3);
}

void Right()
{
	DrvGPIO_SetBit(E_GPA,0); 
	DrvGPIO_ClrBit(E_GPA,1);
	DrvGPIO_ClrBit(E_GPA,2);
	DrvGPIO_SetBit(E_GPA,3);
}

void Left()
{
	DrvGPIO_ClrBit(E_GPA,0); 
	DrvGPIO_SetBit(E_GPA,1);
	DrvGPIO_SetBit(E_GPA,2);
	DrvGPIO_ClrBit(E_GPA,3);
}

void Stop()
{
	DrvGPIO_ClrBit(E_GPA,0); 
	DrvGPIO_ClrBit(E_GPA,1);
	DrvGPIO_ClrBit(E_GPA,2);
	DrvGPIO_ClrBit(E_GPA,3);
}
void Water()
{
	DrvGPIO_SetBit(E_GPA,4); 
}
void Stop_Water()
{
	DrvGPIO_ClrBit(E_GPA,4); 
}
  
void LED()
{
	DrvGPIO_ClrBit(E_GPA,5);
	DrvGPIO_SetBit(E_GPA,6);
	DrvGPIO_SetBit(E_GPA,7);
	DrvSYS_Delay(1000000);
	DrvGPIO_ClrBit(E_GPA,5);
	DrvGPIO_ClrBit(E_GPA,6);
	DrvGPIO_ClrBit(E_GPA,7);
	DrvSYS_Delay(2000000);
}
void Stop_LED()
{
	DrvGPIO_ClrBit(E_GPA,5);
	DrvGPIO_ClrBit(E_GPA,6);
	DrvGPIO_ClrBit(E_GPA,7); 
}
void UART_INT_HANDLE(void)
{

   strcpy(Forward_STR, "U");
   strcpy(Backward_STR, "D");
	 strcpy(Right_STR, "R");
	 strcpy(Left_STR, "L");
	 strcpy(Water_STR, "W");
	 strcpy(Stop_Water_STR, "R");
	 strcpy(LED_STR, "J");
	 strcpy(Stop_LED_STR, "K");

	Forward_FLAG=1;
	Backward_FLAG=1;
	Right_FLAG=1;
	Left_FLAG=1;
	Water_FLAG=1;
	Stop_Water_FLAG=1;
	LED_FLAG=1;
	Stop_LED_FLAG=1;
	while(UART0->ISR.RDA_IF==1) 
	{
		comRbuf[comRbytes]=UART0->DATA;
		comRbytes++;		
		if (comRbytes==7) {	
			sprintf(TEXT,"%s",comRbuf);
			
			Forward_FLAG = strncmp(TEXT, Forward_STR, 1);
			Backward_FLAG = strncmp(TEXT, Backward_STR, 1);
			Right_FLAG = strncmp(TEXT, Right_STR, 1);
			Left_FLAG = strncmp(TEXT, Left_STR, 1);
			Water_FLAG = strncmp(TEXT, Water_STR, 1);
			Stop_Water_FLAG = strncmp(TEXT, Stop_Water_STR, 1);
			LED_FLAG = strncmp(TEXT, LED_STR, 1);
			Stop_LED_FLAG = strncmp(TEXT, Stop_LED_STR, 1);
			strncpy(Speed_PWM_STR,TEXT+1,3);
			strncpy(Servo_PWM_STR,TEXT+4,3);

			
			// set GPIO pins to output Low

			clr_all_panel();
			if(!Forward_FLAG){
			Forward();
			sprintf(TEXT2,"Forward");
			DrvUART_Write(UART_PORT0,(uint8_t *) TEXT2,strlen(TEXT2));	// write to SRF04
			}
			else if(!Backward_FLAG){
			Backward();
			sprintf(TEXT2,"Backward");
			DrvUART_Write(UART_PORT0,TEXT2,strlen(TEXT2));	// write to SRF04
			}
			else if(!Right_FLAG){
			Right();
			sprintf(TEXT2,"Right");
			DrvUART_Write(UART_PORT0,TEXT2,strlen(TEXT2));	// write to SRF04
			}
			else if(!Left_FLAG){
				Left();
				sprintf(TEXT2,"Left");
				DrvUART_Write(UART_PORT0,TEXT2,strlen(TEXT2));	// write to SRF04
			}

			if(!Water_FLAG){
				Water_FLAG=1;
				Water();
				sprintf(TEXT2,"Water on");
				DrvUART_Write(UART_PORT0,TEXT2,strlen(TEXT2));	// write to SRF04
				}
			if(!Stop_Water_FLAG){
				Stop_Water_FLAG=1;
				Stop_Water();
				sprintf(TEXT2,"Water off");
				DrvUART_Write(UART_PORT0,TEXT2,strlen(TEXT2));	// write to SRF04
				}
			if(!LED_FLAG){
				if(!Stop_LED_FLAG){
					LED_FLAG=1;
					break;
				}
						LED();
						sprintf(TEXT2,"LED on");
						DrvUART_Write(UART_PORT0,TEXT2,strlen(TEXT2));	// write to SRF04
					LED_FLAG=0;
				}
			
			Speed_PWM_INT= atoi(Speed_PWM_STR);
			Servo_PWM_INT= atoi(Servo_PWM_STR);
			PWMA->CMR0=MAX_DUTY_CYCLE *Speed_PWM_INT;// CMR0 is 32 bit which equale to ADDR[7].RSLT which shift left and can only be read
			PWMA->CMR1=MAX_DUTY_CYCLE *Speed_PWM_INT;// CMR1 is 32 bit which equale to ADDR[7].RSLT which shift left and can only be read
			PWM_Servo(2, Servo_PWM_INT);
			comRbytes=0;
		}
	}
}

int32_t main()
{
	STR_UART_T sParam;
	uint16_t h_hitime, v_hitime;

	UNLOCKREG();
  DrvSYS_Open(48000000);
	LOCKREG();
	InitGPIO();
	DrvGPIO_InitFunction(E_FUNC_UART0);	// Set UART pins
	flag_water=0;
	
	SYSCLK->PWRCON.XTL12M_EN = 1;
	SYSCLK->CLKSEL0.HCLK_S = 0;
	InitPWM();
	
	/* UART Setting */
    sParam.u32BaudRate 		  = 9600;
    sParam.u8cDataBits 		  = DRVUART_DATABITS_8;
    sParam.u8cStopBits 		  = DRVUART_STOPBITS_1;
    sParam.u8cParity 		    = DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
 	if(DrvUART_Open(UART_PORT0,&sParam) != E_SUCCESS);
	DrvUART_EnableInt(UART_PORT0, DRVUART_RDAINT, UART_INT_HANDLE);
	
	Initial_panel();                 // initialize LCD panel
	clr_all_panel();                 // clear LCD panel			
	h_hitime=HITIME_MIN;
	while(1)
	{
	}
	//DrvUART_Close(UART_PORT0);
}


