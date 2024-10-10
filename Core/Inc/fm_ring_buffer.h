#ifndef FM_RING_BUFFER_H
#define FM_RING_BUFFER_H

#ifdef __cplusplus
extern "c" {
#endif

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>  
#include <string.h>

#define FM_UART2_BUFFER_SIZE                    ( 128U )

typedef enum {
        RBuffer_Logic_State_Is_LOW  = 0,
        RBuffer_Logic_State_Is_HIGH = 1

}RBuffer_Logic_State_t;

typedef struct {
    unsigned char fm_buffer[ FM_UART2_BUFFER_SIZE ];
    volatile unsigned int head;
    volatile unsigned int tail;

}RBuffer_Def_t;

void    FM_RBuffer_Int( void );

uint8_t FM_RBuffer_Receive_Data( void );

void    FM_RBuffer_Write_Data( uint8_t chr );

void    FM_RBuffer_Send_Data_String( const char *str );

uint8_t FM_RBuffer_Checking_Data_Is_Available( void );

uint8_t FM_RBuffer_Peek( );

void    FM_RBuffer_Get_Data_From_Buffer(char *startString, char *endString, char *buffertocopyfrom, char *buffertocopyinto);

uint8_t    FM_RBuffer_Waiting( char *str );
void Uart_printbase (long n, uint8_t base);
void    FM_RBuffer_UART_ISR (UART_HandleTypeDef *huart);
void    FM_RBuffer_Erase_Buffer ( void );
void    FM_RBuffer_Store_Data( unsigned char chr,RBuffer_Def_t *RB_buffer );

int Copy_upto (char *string, char *buffertocopyinto);
void Uart_sendstring (const char *s);
int Get_after (char *string, uint8_t numberofchars, char *buffertosave);


void FM_RBuffer_Copy_An_Array(char *arr1, char *arr2 );
#ifdef __cplusplus
}
#endif
#endif /* FM_RING_BUFFER_H_ */