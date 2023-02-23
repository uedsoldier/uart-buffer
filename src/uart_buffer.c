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

void uart_buffer_init(UARTBuffer *uartBuffer, void (*writeByte_callback)(uint8_t), uint8_t (*readByte_callback)(void))
{
    memset(uartBuffer->rxBuffer, 0, UART_RX_BUFFER_SIZE);
    uartBuffer->queueFront = -1;
    uartBuffer->queueEnd = -1;
    uartBuffer->writeByte = writeByte_callback;
    uartBuffer->readByte = readByte_callback;
}

void uart_puts(UARTBuffer *uartBuffer, const char *str)
{
    while (*str)
    {
        uartBuffer->writeByte(*str++);
    }
}

void uart_gets(UARTBuffer *uartBuffer, char *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        while(!uart_dataAvailable(uartBuffer)){};
        uart_readByteBuffer(uartBuffer, buffer);
        buffer++;
    }
}

void uart_writeLine(UARTBuffer *uartBuffer, const char *str)
{
    uart_puts(uartBuffer, str);
    uartBuffer->writeByte('\r');
    uartBuffer->writeByte('\n');
}

void uart_writeBuffer(UARTBuffer *uartBuffer, uint8_t *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        uartBuffer->writeByte(*buffer++);
    }
}

void uart_write(UARTBuffer *uartBuffer, void *data, uint16_t len)
{
    uint8_t *_data = (uint8_t *)data;
    while (len--)
    {
        uartBuffer->writeByte(*_data++);
    }
}

void uart_read(UARTBuffer *uartBuffer, void *data, uint16_t len)
{
    uint8_t *_data = (uint8_t *)data;
    while (len--)
    {
        while(uart_dataAvailable(uartBuffer)==0);
        uart_readByteBuffer(uartBuffer, _data++);
    }
}

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

uint16_t uart_dataAvailable(UARTBuffer *uartBuffer)
{
    uint16_t retVal;
    if(uartBuffer->queueFront==-1) 
        retVal = 0;
    else if(uartBuffer->queueFront<uartBuffer->queueEnd)
        retVal = (uartBuffer->queueEnd-uartBuffer->queueFront+1);
    else if(uartBuffer->queueFront>uartBuffer->queueEnd)
        retVal = (UART_RX_BUFFER_SIZE-uartBuffer->queueFront+uartBuffer->queueEnd+1);
    else
        retVal = 1;
    return retVal;
}

UART_rxQueue_Status uart_readByteBuffer(UARTBuffer *uartBuffer, uint8_t *byte)
{
    // Verify if queue is empty
    if (uartBuffer->queueFront == -1)
        return UART_RX_QUEUE_EMPTY;

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
    return UART_RX_QUEUE_STATUS_OK;
}

UART_rxQueue_Status uart_firstByteReceived(UARTBuffer *uartBuffer, uint8_t *data)
{
    if(!uart_dataAvailable(uartBuffer)){
        return UART_RX_QUEUE_EMPTY;
    }
    *data=uartBuffer->rxBuffer[uartBuffer->queueFront];
    return UART_RX_QUEUE_STATUS_OK;
}

UART_rxQueue_Status uart_lastByteReceived(UARTBuffer *uartBuffer, uint8_t *data)
{
    if(!uart_dataAvailable(uartBuffer)){
        return UART_RX_QUEUE_EMPTY;
    }
    *data=uartBuffer->rxBuffer[uartBuffer->queueEnd];
    return UART_RX_QUEUE_STATUS_OK;

}

void uart_readBuffer(UARTBuffer *uartBuffer, uint8_t *buffer, size_t len)
{
    for (size_t i = 0; i != len; i++)
    {
        uart_readByteBuffer(uartBuffer, buffer++);
    }
    
}

void uart_flushBuffer(UARTBuffer *uartBuffer)
{
    uartBuffer->queueEnd = -1;
    uartBuffer->queueFront = -1;
}

#if defined(UART_BUFFER_LOG) && (UART_BUFFER_LOG > 0)
void uart_printBuffer(UARTBuffer *uartBuffer){
    printf("UART buffer print:\n");
    for (size_t i = 0; i != UART_RX_BUFFER_SIZE; i++)
    {
        printf("Index: %u\tValue: 0x%02X\n",i,uartBuffer->rxBuffer[i]);
    }
    
}
#endif