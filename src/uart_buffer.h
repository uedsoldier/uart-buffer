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
 * @brief Set this macro to a non-zero value to use multiple user-defined UART buffers
 */
#define UART_MULTIPLE_BUFFERS 0
    
/**
 * @brief Maximum RX buffer size in bytes
 */
#define UART_RX_BUFFER_SIZE 128				

/**
 * @brief Set this macro to a non-zero value to activate logging functionality.
 */
#define UART_BUFFER_LOG 0
    
static const char* UART_BUFFER_TAG = "UART-buffer";

typedef enum _UART_rxQueue_Status{
    UART_RX_QUEUE_STATUS_OK = 0,
    UART_RX_QUEUE_EMPTY,
    UART_STATUS_MAX
} UART_rxQueue_Status;

/**
 * @brief Data structure definition for UART FIFO buffer
 */
typedef struct _UARTBuffer{
    uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
    volatile int16_t queueFront;		
    volatile int16_t queueEnd;	
    void (*writeByte)(uint8_t);
    uint8_t (*readByte)(void);
} UARTBuffer;


#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)

#else
/**
 * @brief Single UART buffer
 */
static UARTBuffer uartBuffer;
#endif

#pragma region Function prototypes

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief UART buffer initialization (Multiple UART buffers)
 * @param uartBuffer Reference to UART buffer 
 * @param writeByte_callback Reference to writeByte callback function
 * @param readByte_callback Reference to readByte callback function
 */
void uart_buffer_init(UARTBuffer *uartBuffer, void (*writeByte_callback)(uint8_t), uint8_t (*readByte_callback)(void));
#else
/**
 * @brief UART buffer initialization (Single UART buffer)
 * @param writeByte_callback Reference to writeByte callback function
 * @param readByte_callback Reference to readByte callback function
 */
void uart_buffer_init(void (*writeByte_callback)(uint8_t), uint8_t (*readByte_callback)(void));
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Sends a string of characters through UART
 * 
 * @param uartBuffer Reference to UART buffer 
 * @param str String of characters to be send
 */
void uart_puts(UARTBuffer *uartBuffer, const char *str);
#else
/**
 * @brief Sends a string of characters through UART
 * @param str String of characters to be send
 */
void uart_puts(const char *str);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Gets a string of characters through UART
 * @param uartBuffer Reference to UART buffer
 * @param buffer Reference to char array that will store the received characters
 * @param len Max byte quantity to read
 * @return 
 */
char *uart_gets(UARTBuffer *uartBuffer, char *buffer, size_t len);
#else
/**
 * @brief Gets a string of characters through UART
 * @param buffer Reference to char array that will store the received characters
 * @param len Max byte quantity to read
 * @return 
 */
char *uart_gets(char *buffer, size_t len);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Sends a string of characters through UART appending CR & LF
 * 
 * @param uartBuffer Reference to UART buffer 
 * @param str String of characters to be send
 */
void uart_writeLine(UARTBuffer *uartBuffer, const char *str);
#else
/**
 * @brief Sends a string of characters through UART appending CR & LF
 * 
 * @param str String of characters to be send
 */
void uart_writeLine(const char *str);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Sends a byte buffer through UART
 * 
 * @param uartBuffer Reference to UART buffer 
 * @param buffer Byte buffer to be send
 * @param len Byte quantity to send
 */
void uart_writeBuffer(UARTBuffer *uartBuffer, uint8_t *buffer, size_t len); 
#else
/**
 * @brief Sends a byte buffer through UART
 * @param buffer Byte buffer to be send
 * @param len Byte quantity to send
 */
void uart_writeBuffer( uint8_t *buffer, size_t len); 
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Sends data (any type) through UART. Internally, data providen will be casted to an array of bytes
 * 
 * @param uartBuffer Reference to UART buffer 
 * @param data Reference to data that is going to be sended
 * @param len Byte quantity to be send
 */
void uart_write(UARTBuffer *uartBuffer, void* data, size_t len);
#else
/**
 * @brief Sends data (any type) through UART. Internally, data providen will be casted to an array of bytes
 * @param data Reference to data that is going to be sended
 * @param len Byte quantity to be send
 */
void uart_write(void* data, size_t len);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Receives data (any type) through UART. Internally, data providen will be casted to an array of bytes
 * 
 * @param uartBuffer Reference to UART buffer 
 * @param data Reference to data that is going to be received
 * @param len Byte quantity to be received
 */
void uart_read(UARTBuffer *uartBuffer, void* data, size_t len);
#else
void uart_read(void* data, size_t len);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Serial reception interrupt handler
 * 
 * @param uartBuffer Reference to UART buffer 
 */
void uart_interruptHandler(UARTBuffer *uartBuffer);
#else
/**
 * @brief Serial reception interrupt handler
 */
void uart_interruptHandler(void);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Returns available bytes in indicated UART buffer
 * @param uartBuffer Reference to UART buffer 
 * @return size_t Available bytes in UART buffer
 */
size_t uart_dataAvailable(UARTBuffer *uartBuffer); 
#else
/**
 * @brief Returns available bytes in indicated UART buffer
 * @return size_t Available bytes in UART buffer
 */
size_t uart_dataAvailable(void); 
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Reads the first byte from the UART buffer and "removes" it from the FIFO
 * @param uartBuffer Reference to UART buffer 
 * @param byte Reference to store read byte
 */
void uart_readByteBuffer(UARTBuffer *uartBuffer, uint8_t *byte);
#else
/**
 * @brief Reads the first byte from the UART buffer and "removes" it from the FIFO
 * @param byte Reference to store read byte
 */
void uart_readByteBuffer(uint8_t *byte);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Reads the first byte from the UART buffer as a query only (doesn't modify UART buffer indexes)
 * @param uartBuffer Reference to UART buffer 
 * @param data Reference to store read byte
 * @return UART_rxQueue_Status 
 */
UART_rxQueue_Status uart_firstByteReceived(UARTBuffer *uartBuffer, uint8_t *byte);
#else
/**Reads the first byte from the UART buffer as a query only (doesn't modify UART buffer indexes) 
 * @brief 
 * @param byte Reference to store read byte
 * @return UART_rxQueue_Status 
 */
UART_rxQueue_Status uart_firstByteReceived(uint8_t *byte);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Reads the last byte from the UART buffer as a query only (doesn't modify UART buffer indexes) 
 * @param uartBuffer Reference to UART buffer 
 * @param byte Reference to store read byte 
 * @return UART_rxQueue_Status 
 */
UART_rxQueue_Status uart_lastByteReceived(UARTBuffer *uartBuffer, uint8_t *byte );
#else
/**
 * @brief Reads the last byte from the UART buffer as a query only (doesn't modify UART buffer indexes) 
 * @param byte Reference to store read byte
 * @return UART_rxQueue_Status 
 */
UART_rxQueue_Status uart_lastByteReceived(uint8_t *byte );
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Reads 'len' bytes from UART buffer
 * @param uartBuffer Reference to UART buffer 
 * @param buffer Reference to buffer that will store data read
 * @param len Byte quantity to read
 */
void uart_readBuffer(UARTBuffer *uartBuffer, uint8_t *buffer,size_t len);
#else
/**
 * @brief Reads 'len' bytes from UART buffer
 * @param buffer Reference to buffer that will store data read
 * @param len Byte quantity to read
 */
void uart_readBuffer(uint8_t *buffer,size_t len);
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Flush UART buffer, resetting indexes
 * @param uartBuffer Reference to UART buffer 
 */
void uart_flushBuffer(UARTBuffer *uartBuffer); 
#else
/**
 * @brief Flush UART buffer, resetting indexes
 */
void uart_flushBuffer(void);
#endif


#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Flush UART buffer, resetting indexes and discarding all data stored
 * @param uartBuffer Reference to UART buffer 
 */
void uart_hardFlushBuffer(UARTBuffer *uartBuffer); 
#else
/**
 * @brief Flush UART buffer, resetting indexes and discarding all data stored
 */
void uart_hardFlushBuffer(void);
#endif

#if defined(UART_BUFFER_LOG) && (UART_BUFFER_LOG > 0)
#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
/**
 * @brief Dumps UART buffer content through std output
 * @param uartBuffer Reference to UART buffer 
 */
void uart_printBuffer(UARTBuffer *uartBuffer);
#else
/**
 * @brief Dumps UART buffer content through std output
 */
void uart_printBuffer(void);
#endif
#endif

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*uart_BUFFER_H*/