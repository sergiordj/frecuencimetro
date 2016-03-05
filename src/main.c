/*
 * Main function for an EDU-CIAA based Frequency Meter
 * with UART communication.
 */

/*
 * Probar la implementación de un Wait For interrupt en la
 * próxima versión OJO
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
 * 201600303   v0.0.1   First version
 */


/*==================[inclusions]=============================================*/

#include "main.h"       /* own header */
#include "SCOS.h"   	/* OS header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


int main(void)
{
   /* -------- SETUP -------- */


   /* Initialize OS Schedule by Initialize SysTick Interrupt */
   initOS();


   /* -------- LOOP --------- */
   while(1) {

	   /* Schedule task according to TICKS */
	   scheduleTasks();
	   /* Dispatch OS Tasks in foreground */
	   dispatchTasks();

   }

   return 0;
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
