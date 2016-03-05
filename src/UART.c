/*
 * UART port manager task for EDU-CIAA
 */

/*
 * Convertir esto en un DRIVER de UART en próxima versión
 * y llamarlo desde freq_calq OJO
 */

/*
 * Initials    Name
 * ---------------------------
 * SDJ			Sergio De Jesús
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160303   v0.0.1   First version
 * 20160304	  v0.1.0   Test version fix frequency streaming issue
 */

/*==================[inclusions]=============================================*/
#include "UART.h"       /* <= own header */

#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif

#include "stdint.h"
#include "string.h"
#include "freq_calc.h"

/*==================[macros and definitions]=================================*/

#define INPUT     0
#define OUTPUT    1

#define ON        1
#define OFF       0

#define TICKRATE_HZ (1000) /* 1000 ticks per second --> 1ms Tick */

#define UART_USB      LPC_USART2  /* UART2 (USB-UART) */
#define BAUD_RATE     115200
#define UART2_TXD_P   7
#define UART2_TXD_P_  1
#define UART2_RXD_P   7
#define UART2_RXD_P_  2

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

uint8_t UARTByte;

/*==================[external data definition]===============================*/


/*==================[internal functions definition]==========================*/

/* Initialize UART port */
void uartConfig(unsigned int baudRate){

   Chip_UART_Init(UART_USB);
   Chip_UART_SetBaud(UART_USB, baudRate);  /* Set Baud rate */
   Chip_UART_SetupFIFOS(UART_USB, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0); /* Modify FCR (FIFO Control Register)*/
   Chip_UART_TXEnable(UART_USB); /* Enable UART Transmission */
   Chip_SCU_PinMux(UART2_TXD_P, UART2_TXD_P_ , MD_PDN, FUNC6);              /* P7_1,FUNC6: UART2_TXD */
   Chip_SCU_PinMux(UART2_RXD_P, UART2_RXD_P_ , MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2,FUNC6: UART2_RXD */

   /* Enable UART Rx Interrupt */
   Chip_UART_IntEnable(UART_USB, UART_IER_RBRINT ); /* Receiver Buffer Register Interrupt */
   /* Enable UART line status interrupt */
   Chip_UART_IntEnable(UART_USB, UART_IER_RLSINT ); /* LPC43xx User manual page 1118 */
   NVIC_SetPriority(USART2_IRQn, 6);
   /* Enable Interrupt for UART channel */
   NVIC_EnableIRQ(USART2_IRQn);

}

/* Write a byte on UART */
void uartWriteByte(unsigned char byte){
   while ((Chip_UART_ReadLineStatus(UART_USB) & UART_LSR_THRE) == 0); /* Wait for space in FIFO */
   Chip_UART_SendByte(UART_USB, byte);
}

/* Write a string on UART */
void uartWrite(unsigned char * str){
   while(*str != 0){
	  uartWriteByte(*str);
	  *str++;
   }
}

/* Return a Received Byte from UART */
unsigned char uartReadByte(void){

	unsigned char receivedByte = 0;

   if (Chip_UART_ReadLineStatus(UART_USB) & UART_LSR_RDR) {
      receivedByte = Chip_UART_ReadByte(UART_USB);
   }
   return receivedByte;
}



/*==================[external functions definition]==========================*/


/* Initialize Task */
void Uart_Init(void) {

	/* Initialize Uart 2 - USB, use /dev/ttyUSB1 @ 115200, 8, N, 1 */
	   uartConfig(BAUD_RATE);

	/* Initialize received byte latch */
	   UARTByte = 0;

	/* Sends welcome and instruction message */
	   uartWrite( (unsigned char*) "Por favor introduzca g para devolver la frecuencia medida:\n" );

}

/* Lets OS excecute UART task, gets frequency from calculator and sends it to UART */
void Uart_Exec (void){
	volatile char frequency;
	frequency = Get_Frequency();
	uartWrite( (unsigned char*) "La frecuencia medida es de " );
	volatile char aux = frequency / 10;
	uartWriteByte( aux + 48 );
	aux = aux * 10;
	frequency = frequency - aux;
	uartWriteByte( frequency + 48 );
	uartWrite( (unsigned char*) " Hz.\n" );
}

/* Return read byte from latch */
unsigned char Uart_Get_Byte (void){
	return (UARTByte);
}

/* On UART interuption, save received byte */
void UART2_IRQHandler(void){
   UARTByte = uartReadByte() ;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
