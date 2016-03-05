/*
 * Simple Cooperative Operative System designed for
 * tasks administration for an EDU-CIAA based Frequency Meter
 * with UART communication.
 */


/*
 * Initials     Name
 * ---------------------------
 * SDJ          Sergio De Jesús
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160303   v0.0.1   First version
 * 20160304	  v0.1.0   Test version includes heartbeat and unconditional execution for UART
 */

/*==================[inclusions]=============================================*/

#include "SCOS.h"   		/* own header  */
#include "pulse_count.h"  /* Button based pulse counter header */
#include "freq_calc.h"	  /* Frequency calculator header */
#include "UART.h"		  /* UART Manager header */

#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif


/*==================[macros and definitions]=================================*/

/* Define GPIO according to EDU-CIAA Configuration */
#define TEC1_GPIO 0
#define TEC1_PIN  4

#define TEC2_GPIO 0
#define TEC2_PIN  8

#define TEC3_GPIO 0
#define TEC3_PIN  9

#define TEC4_GPIO 1
#define TEC4_PIN  9

#define LEDB_GPIO 5
#define LEDB_PIN  2

#define LEDR_GPIO 5
#define LEDR_PIN  0

#define LEDG_GPIO 5
#define LEDG_PIN  1

#define LED1_GPIO 0
#define LED1_PIN  14

#define LED2_GPIO 1
#define LED2_PIN  11

#define LED3_GPIO 1
#define LED3_PIN  12

/* OS Definitions */

#define TICKRATE_HZ 1000  /* Define tickrate in Hz */
#define SAMP_PERIOD 1	  /* Define Sampling period in ticks */
#define SAMP_WINDOW 2000  /* Define Sampling window in ticks */
#define UART_ACT_PERIOD 2000 /* Define UART task execution period in ticks */
#define HEART_BEAT_TIME 500 /* Define HB period in ms */

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/* Task initialization variables */
static unsigned char Sec_Window ;
static unsigned char mSec_Period ;
/* Task schedule counters */
static int Sampling;
static int Calculation;
static int Sending;
static int Heartbeating;
/* Task schedule FLAGS */
static unsigned char PULSE_COUNT_FLAG;
static unsigned char FREQ_CALC_FLAG;
static unsigned char UART_ACT_FLAG;
static unsigned char HEART_BEAT_FLAG;
/* UART sending FLAG */
static unsigned char SEND;
/* Clock FLAG */
static unsigned char TICK;
static unsigned char led_stat = 0;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* Heartbeat */
	void heartbeat (void){
		if ( led_stat == 0 ) led_stat = 1;
		else led_stat = 0;
		Chip_GPIO_SetPinState( LPC_GPIO_PORT, LEDR_GPIO, LEDR_PIN, led_stat);
	}

/* Set Button configurations */
static void boardButtonsInit(void) {

   /* Config EDU-CIAA-NXP Button Pins as GPIOs */
   Chip_SCU_PinMux(1,0,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[4], TEC1 */
   Chip_SCU_PinMux(1,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[8], TEC2 */
   Chip_SCU_PinMux(1,2,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[9], TEC3 */
   Chip_SCU_PinMux(1,6,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO1[9], TEC4 */

   /* Config EDU-CIAA-NXP Button Pins as Inputs */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<4)|(1<<8)|(1<<9),0);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<9),0);

}

/* Set LED configurations */
static void boardLedsInit(void) {

   /* Config EDU-CIAA-NXP Led Pins as GPIOs */
   Chip_SCU_PinMux(2,0,MD_PUP,FUNC4);  /* GPIO5[0],  LEDR */
   Chip_SCU_PinMux(2,1,MD_PUP,FUNC4);  /* GPIO5[1],  LEDG */
   Chip_SCU_PinMux(2,2,MD_PUP,FUNC4);  /* GPIO5[2],  LEDB */
   Chip_SCU_PinMux(2,10,MD_PUP,FUNC0); /* GPIO0[14], LED1 */
   Chip_SCU_PinMux(2,11,MD_PUP,FUNC0); /* GPIO1[11], LED2 */
   Chip_SCU_PinMux(2,12,MD_PUP,FUNC0); /* GPIO1[12], LED3 */

   /* Config EDU-CIAA-NXP Led Pins as Outputs */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<14),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<11)|(1<<12),1);

   /* Init EDU-CIAA-NXP Led Pins OFF */
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0,(1<<14));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1,(1<<11)|(1<<12));

}

/* Set SysTick configuration */
static void coreInit(void) {

	/* Read clock settings and update SystemCoreClock variable */
	   SystemCoreClockUpdate();

	   /* Fix SysTick on 1ms*/
	   SysTick_Config( SystemCoreClock / TICKRATE_HZ );

}

/* Set up and initialize board hardware */
void boardInit(void) {

   /* Config Core */
   coreInit();

   /* Initializes GPIO */
   Chip_GPIO_Init(LPC_GPIO_PORT);

   /* Config EDU-CIAA-NXP Button Pins */
   boardButtonsInit();

   /* Config EDU-CIAA-NXP Led Pins */
   boardLedsInit();

}

/* Initialize Tasks */
void initTasks(void){

	/* Initialize Button based pulse counter task */
	Pulse_Count_Init( Sec_Window );
	/* Initialize Frequency Calculator task */
	Freq_Calc_Init( mSec_Period );
	/* Initialize UART task */
	Uart_Init();
}

/*==================[external functions definition]==========================*/

/* Initialize OS configurations and variables */
void initOS(void){

	/* Board initialization */
	boardInit();
	/* Set sampling window in seconds for task initialization */
	Sec_Window = SAMP_WINDOW / TICKRATE_HZ ;
	/* Set sampling period in milliseconds for task initialization */
	mSec_Period = ( SAMP_PERIOD * 1000 ) / TICKRATE_HZ ;
	/* Initialize schedule counters */
	Sampling = SAMP_PERIOD;
	Calculation = SAMP_WINDOW;
	Sending = UART_ACT_PERIOD;
	Heartbeating = HEART_BEAT_TIME;
	/* Initialize flags */
	PULSE_COUNT_FLAG = 0;
	FREQ_CALC_FLAG = 0;
	UART_ACT_FLAG = 0;
	HEART_BEAT_FLAG = 0;
	SEND = 0;
	TICK = 0;
	/* Initialize tasks */
	initTasks();

}

/* Schedule tasks according counters */
void scheduleTasks(void){

	/* Actualize counters */
	if (TICK > 0){
		Sampling--;
		Calculation--;
		Sending--;
		Heartbeating--;
		TICK = 0;
	}

   /* Actualize flags */
   if (Sampling == 0) {
	   PULSE_COUNT_FLAG = 1;
	   Sampling = SAMP_PERIOD;
   }
   if (Calculation == 0) {
	   FREQ_CALC_FLAG = 1;
	   Calculation = SAMP_WINDOW;
   }
   if (Sending == 0) {
	   UART_ACT_FLAG = 1;
	   Sending = UART_ACT_PERIOD;
   }
   if (Heartbeating == 0) {
	   HEART_BEAT_FLAG = 1;
	   Heartbeating = HEART_BEAT_TIME;
   }

}

/* Dispatch tasks according to scheduler instructions */
void dispatchTasks(void){

	/* Dispatch Pulse counter every sampling period */
   if( PULSE_COUNT_FLAG ) {
		Pulse_Count_Exec();
		PULSE_COUNT_FLAG = 0;
	}

   /* Dispatch Frequency calculator every sampling window */
   if( FREQ_CALC_FLAG ) {
	   Freq_Calc_Exec();
	   FREQ_CALC_FLAG = 0;
   }

   /* Actualize UART every actualization time */
   if( UART_ACT_FLAG ) {
	   Uart_Exec();
	   UART_ACT_FLAG = 0;
   }

   /* Heartbeat */
   if( HEART_BEAT_FLAG ) {
	   heartbeat();
	   HEART_BEAT_FLAG = 0;
   }
}

/* Manage SysTick Handler for OS tick */
void SysTick_Handler(void) {
   TICK++;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
