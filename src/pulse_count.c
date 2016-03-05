/*
 * Button based pulse counter task for EDU-CIAA
 * The execution period of this task will define
 * sampling period for frequencymeter application.
 * Recommended range of period ( 1ms to 20ms )
 */

/*
 * Initials     Name
 * ---------------------------
 * SDJ          Sergio De Jesús
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160303   v0.0.1   First version
 */

/*==================[inclusions]=============================================*/

#include "pulse_count.h"   /* own header  */

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

#define TU  40          /* Time for "UP" state in milliseconds */
#define TD  40         /* Time for "DOWN" state in milliseconds */

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

typedef enum {UP,FALLING,DOWN,RISING} STATUS;
static STATUS But_Stat;
static int Down_Count;
static char Up_Act;
static char Down_Act;
static int Time_Count;
static int Time_Up;
static int Time_Down;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* Falling edge action: increases down counter */
void But_Down (void){
	Down_Count++;
}

/* Rising edge action: no action defined */
void But_Up (void){
	/*
	 * write rising edge action here
	 * then uncomment function call
	 */
}

/*
 * Increases verification time counter if state machine is on
 * a verification state
 */
void Time_Update (void){
	if (((But_Stat == RISING) | (But_Stat == FALLING))){
		Time_Count ++;
	}
}

/*Update Status*/
void Stat_Update (void){
	switch (But_Stat){
	   case UP:{
		  if (Up_Act == 0){
			  //But_Up();    /* Uncomment for "rising edge" action */
			  Up_Act = 1;
		  }
		  /*Change Status*/
		  if (!(Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, TEC1_GPIO, TEC1_PIN ))){
			  But_Stat = FALLING;  /* Enter on a "Down verification" phase */
		  }
	   }
	   break;
	   case FALLING:{
		      /*Change Status*/
	   		  if ((Time_Count == Time_Down) & (!(Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, TEC1_GPIO, TEC1_PIN )))){
	   			  But_Stat = DOWN;      /* "Down verification" accepted */
	   			  Time_Count = 0;    /* Reset verification time counter */
	   			  Up_Act = 0;		   /* Reset "rising edge" flag */
	   		  }
	   		  else if ((Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, TEC1_GPIO, TEC1_PIN ))){
	   			  But_Stat = UP;       /* "Down verification" rejected */
	   			  Time_Count = 0;  /* Reset verification time counter */
	   		  }
	   	   }
	   break;
	   case DOWN:{
			  /*Update Outputs*/
		          if (Down_Act == 0){
		        	  But_Down();        /* Execute falling edge action */
		        	  Down_Act = 1;
				  }
			  /*Change Status*/
			  if ((Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, TEC1_GPIO, TEC1_PIN ))){
				  But_Stat = RISING; /* Enter on a "UP verification" phase */
			  }
	   	   }
	   break;
	   case RISING:{
		   	  /*Change Status*/
	   		  if ((Time_Count == Time_Up) & ((Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, TEC1_GPIO, TEC1_PIN )))){
	   			  But_Stat = UP;      /* "Up verification" accepted */
                  Time_Count = 0; /* Reset verification time counter */
                  Down_Act = 0;       /* Reset "falling edge" flag */
	   		  }
	   		  else if (!(Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, TEC1_GPIO, TEC1_PIN ))){
	   			  But_Stat = DOWN; 	 /* "Up verification" rejected */
	   			  Time_Count = 0;  /* Reset verification time counter */
	   		  }
	   	   }
	   break;
	   default:{
			  while (1);
  	   }
	   break;
	}
}



/*==================[external functions definition]==========================*/

/* Initialize Task. Task Period must be in milliseconds */
void Pulse_Count_Init (unsigned char Task_Period){
	But_Stat = UP;			    /* Set initial button status on "UP" */
	Down_Count = 0;             /* Initialize "DOWN" status counter */
	Up_Act = 0;			         /* Initialize "rising edge" flag */
	Down_Act = 0;			    /* Initialize "falling edge" flag */
	Time_Count = 0;           /* Initialize verification time counter */
	Time_Up = TU / Task_Period; /* Set "UP" verification time in function of sampling period */
	Time_Down = TD / Task_Period; /* Set "Down" verification time in function of sampling period */

}

/* Lets OS execute pulse counter task */
void Pulse_Count_Exec (void){
	Time_Update();
	Stat_Update();
}

/* Reset Down counter when other task ask for it */
void Reset_Down_Counter (void){
	Down_Count = 0;
}

/* Return Down counter value when other task ask for it */
int Get_Down_counter (void){
	return (Down_Count);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
