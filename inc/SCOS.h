/*
 * Header file for Frequency meter OS for EDU-CIAA
 */

#ifndef _SCOS_H_
#define _SCOS_H_

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

/* Initialize OS configurations and variables */
void initOS(void);

/* Schedule tasks according counters */
void scheduleTasks(void);

/* Dispatch tasks according to scheduler instructions */
void dispatchTasks(void);

/* Manage SysTick Handler for OS tick */
void SysTick_Handler(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _SCOS_H_ */
