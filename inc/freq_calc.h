/*
 * Header file for frequency calculator for EDU-CIAA
 */

#ifndef _FREQ_CALC_H_
#define _FREQ_CALC_H_

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

/* Initialize Task. Task Period must be in seconds */
void Freq_Calc_Init (unsigned char Task_Period);

/* Lets OS execute frequency calculator task */
void Freq_Calc_Exec (void);

/* Return Frequency value when other task ask for it */
unsigned char Get_Frequency (void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _FREQ_CALC_H_ */
