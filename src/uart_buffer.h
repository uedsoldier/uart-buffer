/**
 * @file uart_buffer.h
 * @author Roberto Parra (uedsoldier1990@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UART_BUFFER_H
#define UART_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Dependencies
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#pragma endregion

/**
 * @brief 
 * 
 */
#define UART_RX_BUFFER_SIZE 128 					

/**
 * @brief 
 * 
 */
#define UART_BUFFER_LOG 0

typedef enum _UART_rxQueue_Status{
    UART_RX_QUEUE_STATUS_OK = 0,
    UART_RX_QUEUE_EMPTY,
    UART_STATUS_MAX
} UART_rxQueue_Status;

/**
 * @brief 
 * 
 */
typedef struct _UARTBuffer{
    uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
    volatile int16_t queueFront;		
    volatile int16_t queueEnd;	
    void (*writeByte)(uint8_t);
    uint8_t (*readByte)(void);
} UARTBuffer;

#pragma region Function prototypes

/**
 * @brief 
 * 
 * @param uartBuffer 
 * @param writeByte_callback 
 * @param readByte_callback 
 */
void uart_buffer_init(UARTBuffer *uartBuffer, void (*writeByte_callback)(uint8_t), uint8_t (readByte_callback)(void));

/**
 * @brief Rutina para mandar cadena de caracteres vía EUSART
 * 
 * @param uartBuffer 
 * @param str 
 */
void uart_puts(UARTBuffer *uartBuffer, const char *str);

/**
 * @brief Función para lectura de datos vía EUSART
 * 
 * @param uartBuffer 
 * @param buffer 
 * @param len 
 */
void uart_gets(UARTBuffer *uartBuffer, char *buffer, size_t len); 

/**
 * @brief Manda cadena de caracteres precedida de CR y NL
 * 
 * @param uartBuffer 
 * @param str 
 */
void uart_writeLine(UARTBuffer *uartBuffer, const char *str);

/**
 * @brief Rutina para mandar arreglo de bytes vía EUSART
 * 
 * @param uartBuffer 
 * @param buffer 
 * @param len 
 */
void uart_writeBuffer(UARTBuffer *uartBuffer, uint8_t *buffer, size_t len); 

/**
 * @brief 
 * 
 * @param uartBuffer 
 * @param data 
 * @param len 
 */
void uart_write(UARTBuffer *uartBuffer, void* data, uint16_t len);

/**
 * @brief 
 * 
 * @param uartBuffer 
 * @param data 
 * @param len 
 */
void uart_read(UARTBuffer *uartBuffer, void* data, uint16_t len);

/**
 * @brief Rutina a ejecutar en interrupción por recepción uart
 * 
 * @param uartBuffer 
 */
void uart_interruptHandler(UARTBuffer *uartBuffer);

/**
 * @brief Returns available bytes in indicated UART buffer
 * 
 * @param uartBuffer 
 * @return uint16_t 
 */
uint16_t uart_dataAvailable(UARTBuffer *uartBuffer); 

/**
 * @brief 
 * 
 * @param uartBuffer 
 * @param byte 
 * @return UART_rxQueue_Status 
 */
UART_rxQueue_Status uart_readByteBuffer(UARTBuffer *uartBuffer, uint8_t *byte);

/**
 * @brief  Devuelve el primer dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
 * 
 * @param uartBuffer 
 * @param data 
 * @return UART_rxQueue_Status 
 */
UART_rxQueue_Status uart_firstByteReceived(UARTBuffer *uartBuffer, uint8_t *data);

/**
 * @brief Devuelve el último dato del buffer FIFO sin modificar índices de principio y fin, a manera de consulta únicamente
 * 
 * @param uartBuffer 
 * @param data 
 * @return UART_rxQueue_Status 
 */
UART_rxQueue_Status uart_lastByteReceived(UARTBuffer *uartBuffer, uint8_t *data );

/**
 * @brief Lectura de 'len' elementos del buffer uart
 * 
 * @param uartBuffer 
 * @param buffer 
 * @param len 
 */
void uart_readBuffer(UARTBuffer *uartBuffer, uint8_t *buffer,size_t len);

/**
 * @brief Lee todos los datos del buffer, reiniciando sus indices
 * 
 * @param uartBuffer 
 */
void uart_flushBuffer(UARTBuffer *uartBuffer); 

#if defined(UART_BUFFER_LOG) && (UART_BUFFER_LOG > 0)
/**
 * @brief 
 * 
 * @param uartBuffer 
 */
void uart_printBuffer(UARTBuffer *uartBuffer);
#endif

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*uart_BUFFER_H*/