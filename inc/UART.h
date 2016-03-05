/*
 * Header file for UART manager for EDU-CIAA
 */

#ifndef _UART_H_
#define _UART_H_

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

/* Initialize Task */
void Uart_Init(void);

/* Lets OS excecute UART task, gets frequency from calculator and sends it to UART */
void Uart_Exec (void);

/* Return read byte from latch */
unsigned char Uart_Get_Byte (void);

/* On UART interuption, save received byte */
void UART2_IRQHandler(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _UART_H_ */
