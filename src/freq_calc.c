/*
 * Frequency calculator task for EDU-CIAA
 * The execution period of this task will define
 * sampling window for frequencymeter application.
 * Recommended range of period ( 1s to 5s )
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

#include "freq_calc.h"		     /* own header */
#include "pulse_count.h"   /* pulse counter include  */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/


static int Pulse_Counter;
static unsigned char Samp_Window;
static unsigned char Freq;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* Calculate frequency value */
void calculator (void){
	Freq = Pulse_Counter / Samp_Window;
}

/*==================[external functions definition]==========================*/

/* Initialize Task. Task Period must be in seconds */
void Freq_Calc_Init (unsigned char Task_Period){
	Pulse_Counter = 0;		/* initialize Pulse Counter Value */
	Freq = 0;       	     /* Initialize frequency value */
	Samp_Window = Task_Period; /* Set sampling window on execution period for this task [s] */
}

/* Lets OS execute frequency calculator task */
void Freq_Calc_Exec (void){
	Pulse_Counter = Get_Down_counter(); /* Receive Pulse Counter value from Button based pulse counter task */
	Reset_Down_Counter();				/* Reset Pulse Counter for a new sampling window */
	calculator ();						/* Calculate frequency value [Hz] */
}

/* Return Frequency value when other task ask for it */
unsigned char Get_Frequency (void){
	return (Freq);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
