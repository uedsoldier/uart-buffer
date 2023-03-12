/**
 * @file uart_buffer.c
 * @author Roberto Parra (uedsoldier1990@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "uart_buffer.h"

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_buffer_init(UARTBuffer *uartBuffer, void (*writeByte_callback)(uint8_t), uint8_t (*readByte_callback)(void))
{
    memset(uartBuffer->rxBuffer, 0, UART_RX_BUFFER_SIZE);
    uartBuffer->queueFront = -1;
    uartBuffer->queueEnd = -1;
    uartBuffer->writeByte = writeByte_callback;
    uartBuffer->readByte = readByte_callback;
}
#else
void uart_buffer_init(void (*writeByte_callback)(uint8_t), uint8_t (*readByte_callback)(void)){
    memset(uartBuffer.rxBuffer, 0, UART_RX_BUFFER_SIZE);
    uartBuffer.queueFront = -1;
    uartBuffer.queueEnd = -1;
    uartBuffer.writeByte = writeByte_callback;
    uartBuffer.readByte = readByte_callback;
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_puts(UARTBuffer *uartBuffer, const char *str)
{
    while (*str)
    {
        uartBuffer->writeByte(*str++);
    }
}
#else
void uart_puts(const char *str)
{
    while (*str)
    {
        uartBuffer.writeByte(*str++);
    }
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_gets(UARTBuffer *uartBuffer, char *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        while(!uart_dataAvailable(uartBuffer)){};
        uart_readByteBuffer(uartBuffer, buffer);
        buffer++;
    }
}
#else
void uart_gets(char *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        while(uart_dataAvailable() == 0){};
        uart_readByteBuffer(buffer);
        buffer++;
    }
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_writeLine(UARTBuffer *uartBuffer, const char *str)
{
    uart_puts(uartBuffer, str);
    uartBuffer->writeByte('\r');
    uartBuffer->writeByte('\n');
}
#else
void uart_writeLine(const char *str)
{
    uart_puts(str);
    uartBuffer.writeByte('\r');
    uartBuffer.writeByte('\n');
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_writeBuffer(UARTBuffer *uartBuffer, uint8_t *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        uartBuffer->writeByte(*buffer++);
    }
}
#else
void uart_writeBuffer(uint8_t *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        uartBuffer.writeByte(*buffer++);
    }
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_write(UARTBuffer *uartBuffer, void *data, size_t len)
{
    uint8_t *_data = (uint8_t *)data;
    while (len--)
    {
        uartBuffer->writeByte(*_data++);
    }
}
#else
void uart_write(void *data, size_t len)
{
    uint8_t *_data = (uint8_t *)data;
    while (len--)
    {
        uartBuffer.writeByte(*_data++);
    }
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_read(UARTBuffer *uartBuffer, void *data, size_t len)
{
    uint8_t *_data = (uint8_t *)data;
    while (len--)
    {
        while(uart_dataAvailable(uartBuffer)==0);
        uart_readByteBuffer(uartBuffer, _data++);
    }
}
#else
void uart_read(void *data, size_t len)
{
    uint8_t *_data = (uint8_t *)data;
    while (len--)
    {
        while(uart_dataAvailable()==0);
        uart_readByteBuffer( _data++);
    }
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_interruptHandler(UARTBuffer *uartBuffer)
{
    uint8_t data = uartBuffer->readByte();
    if (((uartBuffer->queueEnd == UART_RX_BUFFER_SIZE - 1) && uartBuffer->queueFront == 0) || ((uartBuffer->queueEnd + 1) == uartBuffer->queueFront))
    {
        uartBuffer->queueFront++;
        if (uartBuffer->queueFront == UART_RX_BUFFER_SIZE)
            uartBuffer->queueFront = 0;
    }

    if (uartBuffer->queueEnd == UART_RX_BUFFER_SIZE - 1)
        uartBuffer->queueEnd = 0;
    else
        uartBuffer->queueEnd++;
    uartBuffer->rxBuffer[uartBuffer->queueEnd] = data;
    
    if (uartBuffer->queueFront == -1)
        uartBuffer->queueFront = 0;
}
#else
void uart_interruptHandler()
{
    uint8_t data = uartBuffer.readByte();
    if (((uartBuffer.queueEnd == UART_RX_BUFFER_SIZE - 1) && uartBuffer.queueFront == 0) || ((uartBuffer.queueEnd + 1) == uartBuffer.queueFront))
    {
        uartBuffer.queueFront++;
        if (uartBuffer.queueFront == UART_RX_BUFFER_SIZE)
            uartBuffer.queueFront = 0;
    }

    if (uartBuffer.queueEnd == UART_RX_BUFFER_SIZE - 1)
        uartBuffer.queueEnd = 0;
    else
        uartBuffer.queueEnd++;
    uartBuffer.rxBuffer[uartBuffer.queueEnd] = data;
    
    if (uartBuffer.queueFront == -1)
        uartBuffer.queueFront = 0;
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
size_t uart_dataAvailable(UARTBuffer *uartBuffer)
{
    if(uartBuffer->queueFront==-1) 
        return 0;
    else if(uartBuffer->queueFront<uartBuffer->queueEnd)
        return (uartBuffer->queueEnd-uartBuffer->queueFront+1);
    else if(uartBuffer->queueFront>uartBuffer->queueEnd)
        return(UART_RX_BUFFER_SIZE-uartBuffer->queueFront+uartBuffer->queueEnd+1);
    else
        return 1;
}
#else
size_t uart_dataAvailable()
{
    if(uartBuffer.queueFront==-1) 
        return 0;
    else if(uartBuffer.queueFront<uartBuffer.queueEnd)
        return (size_t)(uartBuffer.queueEnd-uartBuffer.queueFront+1);
    else if(uartBuffer.queueFront>uartBuffer.queueEnd)
        return (size_t)(UART_RX_BUFFER_SIZE-uartBuffer.queueFront+uartBuffer.queueEnd+1);
    else
        return 1;
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_readByteBuffer(UARTBuffer *uartBuffer, uint8_t *byte)
{
    // Verify if queue is empty
    if (uartBuffer->queueFront == -1)
        return;

    *byte = uartBuffer->rxBuffer[uartBuffer->queueFront];

    if (uartBuffer->queueFront == uartBuffer->queueEnd)
    {
        // If it's only one byte, queue is now empty
        uartBuffer->queueFront = -1;
        uartBuffer->queueEnd = -1;
    }
    else
    {
        uartBuffer->queueFront++;
        if (uartBuffer->queueFront == UART_RX_BUFFER_SIZE)
            uartBuffer->queueFront = 0;
    }
}
#else
void uart_readByteBuffer(uint8_t *byte)
{
    // Verify if queue is empty
    if (uartBuffer.queueFront == -1)
        return;

    *byte = uartBuffer.rxBuffer[uartBuffer.queueFront];

    if (uartBuffer.queueFront == uartBuffer.queueEnd)
    {
        // If it's only one byte, queue is now empty
        uartBuffer.queueFront = -1;
        uartBuffer.queueEnd = -1;
    }
    else
    {
        uartBuffer.queueFront++;
        if (uartBuffer.queueFront == UART_RX_BUFFER_SIZE)
            uartBuffer.queueFront = 0;
    }
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
UART_rxQueue_Status uart_firstByteReceived(UARTBuffer *uartBuffer, uint8_t *byte)
{
    if(!uart_dataAvailable(uartBuffer)){
        return UART_RX_QUEUE_EMPTY;
    }
    *byte=uartBuffer->rxBuffer[uartBuffer->queueFront];
    return UART_RX_QUEUE_STATUS_OK;
}
#else
UART_rxQueue_Status uart_firstByteReceived(uint8_t *byte)
{
    if(uart_dataAvailable() == 0){
        return UART_RX_QUEUE_EMPTY;
    }
    *byte=uartBuffer.rxBuffer[uartBuffer.queueFront];
    return UART_RX_QUEUE_STATUS_OK;
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
UART_rxQueue_Status uart_lastByteReceived(UARTBuffer *uartBuffer, uint8_t *byte)
{
    if(!uart_dataAvailable(uartBuffer)){
        return UART_RX_QUEUE_EMPTY;
    }
    *byte=uartBuffer->rxBuffer[uartBuffer->queueEnd];
    return UART_RX_QUEUE_STATUS_OK;
}
#else
UART_rxQueue_Status uart_lastByteReceived(uint8_t *byte)
{
    if(uart_dataAvailable() == 0){
        return UART_RX_QUEUE_EMPTY;
    }
    *byte=uartBuffer.rxBuffer[uartBuffer.queueEnd];
    return UART_RX_QUEUE_STATUS_OK;
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_readBuffer(UARTBuffer *uartBuffer, uint8_t *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        while(uart_dataAvailable(uartBuffer)==0);
        uart_readByteBuffer(uartBuffer, buffer++);
    } 
}
#else
void uart_readBuffer( uint8_t *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        while(uart_dataAvailable()==0);
        uart_readByteBuffer( buffer++);
    } 
}
#endif

#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_flushBuffer(UARTBuffer *uartBuffer)
{
    uartBuffer->queueEnd = -1;
    uartBuffer->queueFront = -1;
}
#else
void uart_flushBuffer()
{
    uartBuffer.queueEnd = -1;
    uartBuffer.queueFront = -1;
}
#endif

#if defined(UART_BUFFER_LOG) && (UART_BUFFER_LOG > 0)
#if defined(UART_MULTIPLE_BUFFERS) && (UART_MULTIPLE_BUFFERS > 0)
void uart_printBuffer(UARTBuffer *uartBuffer){
    printf("UART buffer print:\n");
    for (size_t i = 0; i != UART_RX_BUFFER_SIZE; i++)
    {
        printf("Index: %u\tValue: 0x%02X\n",i,uartBuffer->rxBuffer[i]);
    } 
}
#else
void uart_printBuffer(){
    printf("UART buffer print:\n");
    for (size_t i = 0; i != UART_RX_BUFFER_SIZE; i++)
    {
        printf("Index: %u\tValue: 0x%02X\n",i,uartBuffer->rxBuffer[i]);
    } 
}
#endif
#endif