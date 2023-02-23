#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "../src/uart_buffer.h"

UARTBuffer buffer1, buffer2;
uint8_t tx_buffer[4] = {0x30, 0x31, 0x32, 0x33};
uint8_t rx_buffer[4];


uint8_t buffer_to_read[16];

void write_cb1(uint8_t data)
{
    printf("Writing %c (0x%02X) with UART 1\n", data, data);
}

uint8_t read_cb1()
{
    static uint8_t byte = 0x00;
    printf("Reading something (0x%02X)from UART 1\n",byte++);
    return byte-1;
}

void write_cb2(uint8_t data)
{
    printf("Writing %c (0x%02X) with UART 2\n", data, data);
}

uint8_t read_cb2()
{
    static uint8_t byte = 0x00;
    printf("Reading something (0x%02X)from UART 2\n",byte++);
    return byte-1;
}

int main(int argc, char const *argv[])
{
    uart_buffer_init(&buffer1, write_cb1, read_cb1);
    uart_buffer_init(&buffer2, write_cb2, read_cb2);
    printf("Puts buffer 1\n");        
    uart_puts(&buffer1, "Hola perros");
    printf("\n");
    printf("Puts buffer 2\n");    
    uart_puts(&buffer2, "Hola culeros");
    printf("\n");
    printf("Emulating UART interrupt handler\n");
    for (size_t i = 0; i != sizeof(rx_buffer); i++)
    {
        uart_interruptHandler(&buffer1);
        Sleep(50);
    }
    printf("Gets\n");
    uart_gets(&buffer1, rx_buffer, sizeof(rx_buffer));
    for (size_t i = 0; i != sizeof(rx_buffer); i++)
    {
        printf("rxBuffer[%u]=0x%02X\n",i,rx_buffer[i]);
    }
    
    printf("\n");
    printf("writeLine\n");
    uart_writeLine(&buffer1, "Linea");
    printf("\n");
    printf("writeBuffer\n");
    uart_writeBuffer(&buffer2, tx_buffer, sizeof(tx_buffer));
    printf("\n");
    printf("write\n");
    uint16_t num = 0x1234;
    uart_write(&buffer1, &num, sizeof(uint16_t));
    printf("\n");
    uart_flushBuffer(&buffer1);
    printf("Emulating UART interrupt handler\n");
    for (size_t i = 0; i != sizeof(uint16_t); i++)
    {
        uart_interruptHandler(&buffer2);
        Sleep(50);
    }
    printf("read\n");
    uint16_t two_bytes;
    uart_read(&buffer2,&two_bytes,sizeof(two_bytes));
    printf(" -> Data: 0x%04X\n",two_bytes);

    printf("Emulating UART interrupt handler\n");
    for (size_t i = 0; i != sizeof(float); i++)
    {
        uart_interruptHandler(&buffer1);
        Sleep(50);
    }
    printf("read float\n");
    float real_num;
    uart_read(&buffer1,&real_num,sizeof(real_num));
    printf(" -> Float: %0.9e\n",real_num);
    uart_flushBuffer(&buffer1);
    printf("Read buffer\n");
    printf("Emulating UART interrupt handler\n");
    for (size_t i = 0; i != sizeof(buffer_to_read); i++)
    {
        uart_interruptHandler(&buffer1);
        Sleep(50);
    }
    uart_readBuffer(&buffer1,buffer_to_read,sizeof(buffer_to_read));
    for (size_t i = 0; i != sizeof(buffer_to_read); i++)
    {
        printf("bufferToRead[%u]=0x%02X\n",i,buffer_to_read[i]);
    }
    return 0;
}
