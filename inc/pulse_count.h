/*
 * Header file for Button based pulse counter for EDU-CIAA
 */

#ifndef _PULSE_COUNT_H_
#define _PULSE_COUNT_H_

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

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/* Initialize Task. Task Period must be in milliseconds */
void Pulse_Count_Init (unsigned char Task_Period);

/* Lets OS execute pulse counter task */
void Pulse_Count_Exec (void);

/* Reset Down counter when other task ask for it */
void Reset_Down_Counter (void);

/* Return Down counter value when other task ask for it */
int Get_Down_counter (void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _PULSE_COUNT_H_ */
