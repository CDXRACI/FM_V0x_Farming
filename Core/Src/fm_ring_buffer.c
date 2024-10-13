#include "fm_ring_buffer.h"

extern UART_HandleTypeDef huart2;

#define     FM_RBUFFER_UART2                ( &huart2  )
#define    FM_RBUFFER_TIMEOUT_DEF            500U     
uint16_t    FM_RBUFFER_TIMEOUT              = 0;


RBuffer_Def_t     RX_Buffer = { {0},  0, 0}; 
RBuffer_Def_t     TX_Buffer = { {0},  0, 0}; 
RBuffer_Def_t     *__RX_RBuffer;
RBuffer_Def_t     *__TX_RBuffer;

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

void    FM_RBuffer_Int( void ){

          __RX_RBuffer = &RX_Buffer;
          __TX_RBuffer = &TX_Buffer;

          __HAL_UART_ENABLE_IT(FM_RBUFFER_UART2, UART_IT_ERR);
          __HAL_UART_ENABLE_IT(FM_RBUFFER_UART2, UART_IT_RXNE);
}

uint8_t FM_RBuffer_Checking_Data_Is_Available( void ){
            return ( uint16_t )( FM_UART2_BUFFER_SIZE + ( __RX_RBuffer->head - __RX_RBuffer->tail ) ) %\
                                                                                    FM_UART2_BUFFER_SIZE;
}


void    FM_RBuffer_Store_Data( unsigned char chr, RBuffer_Def_t *RB_buffer ){
            uint8_t l_cnt = ( uint8_t )( RB_buffer->head + 1) % FM_UART2_BUFFER_SIZE;

            if( l_cnt != RB_buffer->tail ){

                    RB_buffer->fm_buffer[RB_buffer->head ] = chr;
                    RB_buffer->head = l_cnt;

            }
}
void Uart_printbase (long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *s = &buf[sizeof(buf) - 1];
  *s = '\0';
  // prevent crash if called with base == 1
  if (base < 2) base = 10;
  do {
    unsigned long m = n;
    n /= base;
    char c = m - base * n;
    *--s = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);
  while(*s) FM_RBuffer_Write_Data(*s++);
}


void    FM_RBuffer_UART_ISR (UART_HandleTypeDef *huart ) {
            uint32_t FM_ISR_FLAGS = READ_REG( huart->Instance->SR  );
            uint32_t FM_CR1_ITS   = READ_REG( huart->Instance->CR1 );

              /* if DR is not empty and the Rx Int is enabled */
         if (((FM_ISR_FLAGS & USART_SR_RXNE) != RESET) && ((FM_CR1_ITS & USART_CR1_RXNEIE) != RESET))  {

                huart->Instance->SR;        
                unsigned char chr = huart->Instance->DR;
                FM_RBuffer_Store_Data( chr,__RX_RBuffer );
                return;
         }
         if (((FM_ISR_FLAGS & USART_SR_TXE) != RESET) && ((FM_CR1_ITS & USART_CR1_TXEIE) != RESET)) {
                    if( TX_Buffer.head == TX_Buffer.tail ) {
    	      // Buffer empty, so disable interrupts
    	           __HAL_UART_DISABLE_IT( huart, UART_IT_TXE ); }
           else {
    	      // There is more data in the output buffer. Send the next byte
    	      unsigned char chr = TX_Buffer.fm_buffer[TX_Buffer.tail];
    	      TX_Buffer.tail = (TX_Buffer.tail + 1) % FM_UART2_BUFFER_SIZE;  
    	      huart->Instance->SR;
    	      huart->Instance->DR = chr; }
        return;    
    }  
}     

uint8_t FM_RBuffer_Receive_Data( void )
{
  // if the head isn't ahead of the tail, we don't have any characters
  if(__RX_RBuffer->head == __RX_RBuffer->tail) {
    return -1;
 } else {
    unsigned char chr = __RX_RBuffer->fm_buffer[__RX_RBuffer->tail];
    __RX_RBuffer->tail = (unsigned int)(__RX_RBuffer->tail + 1) % FM_UART2_BUFFER_SIZE;
    return chr;
  }
}

uint8_t FM_RBuffer_Peek( ) {
  if(__RX_RBuffer->head == __RX_RBuffer->tail) 
  {
        return -1;
  } else
   {
        return __RX_RBuffer->fm_buffer[__RX_RBuffer->tail];
   }
}


void    FM_RBuffer_Write_Data( uint8_t chr )
{
	if (chr>=0)
	{
		int i = (__TX_RBuffer->head + 1) % FM_UART2_BUFFER_SIZE;
		while (i == __TX_RBuffer->tail);

		__TX_RBuffer->fm_buffer[__TX_RBuffer->head] = (uint8_t)chr;
		__TX_RBuffer->head = i;

		__HAL_UART_ENABLE_IT(FM_RBUFFER_UART2, UART_IT_TXE); // Enable UART transmission interrupt
	}
}

uint8_t  FM_RBuffer_Waiting( char *str ){
	uint16_t l_waitting = 0;
	uint16_t l_len = strlen ( str );

again:
	FM_RBUFFER_TIMEOUT = FM_RBUFFER_TIMEOUT_DEF;
	while ((!FM_RBuffer_Checking_Data_Is_Available()) && FM_RBUFFER_TIMEOUT );  // let's wait for the data to show up
	if (FM_RBUFFER_TIMEOUT == 0) return 0;    
	while ( FM_RBuffer_Peek() != str[ l_waitting ] )  // peek in the rx_buffer to see if we get the string
	{
		if (__RX_RBuffer->tail != __RX_RBuffer->head)
		{
			__RX_RBuffer->tail = (unsigned int)(__RX_RBuffer->tail + 1) % FM_UART2_BUFFER_SIZE;  // increment the tail
		}
        		else
		{
			return 0;
		}
	}
	while (FM_RBuffer_Peek() == str [ l_waitting ]) // if we got the first letter of the string
	{
		// now we will peek for the other letters too
		l_waitting++;
		__RX_RBuffer->tail = (unsigned int)(__RX_RBuffer->tail + 1) % FM_UART2_BUFFER_SIZE;  // increment the tail
		FM_RBUFFER_TIMEOUT = FM_RBUFFER_TIMEOUT_DEF;
		while ((!FM_RBuffer_Checking_Data_Is_Available())&&FM_RBUFFER_TIMEOUT);
		if (FM_RBUFFER_TIMEOUT == 0) return 0;
	}

	if (l_waitting != l_len)
	{
		l_waitting = 0;
		goto again;
	}

	if (l_waitting == l_len) {
            return __RX_RBuffer->tail ;     
    } else {
        return 0;}
}

void    FM_RBuffer_Erase_Buffer (void ){
            memset(__RX_RBuffer->fm_buffer,'\0', FM_UART2_BUFFER_SIZE);
	        __RX_RBuffer->head = 0;
	        __RX_RBuffer->tail = 0;

}


void    FM_RBuffer_Get_Data_From_Buffer(char *startString, char *endString, char *buffertocopyfrom, char *buffertocopyinto)
{
	int startStringLength = strlen (startString);
	int endStringLength   = strlen (endString);
	int so_far = 0;
	int indx = 0;
	int startposition = 0;
	int endposition = 0;

repeat1:
	while (startString[so_far] != buffertocopyfrom[indx]) indx++;
	if (startString[so_far] == buffertocopyfrom[indx])
	{
		while (startString[so_far] == buffertocopyfrom[indx])
		{
			so_far++;
			indx++;
		}
	}

	if (so_far == startStringLength) startposition = indx;
	else
	{
		so_far =0;
		goto repeat1;
	}

	so_far = 0;

repeat2:
	while (endString[so_far] != buffertocopyfrom[indx]) indx++;
	if (endString[so_far] == buffertocopyfrom[indx])
	{
		while (endString[so_far] == buffertocopyfrom[indx])
		{
			so_far++;
			indx++;
		}
	}

	if (so_far == endStringLength) endposition = indx-endStringLength;
	else
	{
		so_far =0;
		goto repeat2;
	}

	so_far = 0;
	indx=0;

	for (int i=startposition; i<endposition; i++)
	{
		buffertocopyinto[indx] = buffertocopyfrom[i];
		indx++;
	}
}

int Copy_upto (char *string, char *buffertocopyinto)
{
	int so_far =0;
	int len = strlen (string);
	int indx = 0;

again:
	while (FM_RBuffer_Peek() != string[so_far])
		{
			buffertocopyinto[indx] = __RX_RBuffer->fm_buffer[__RX_RBuffer->tail];
			__RX_RBuffer->tail = (unsigned int)(__RX_RBuffer->tail + 1) % FM_UART2_BUFFER_SIZE;
			indx++;
			while (!FM_RBuffer_Checking_Data_Is_Available());

		}
	while (FM_RBuffer_Peek() == string [so_far])
	{
		so_far++;
		buffertocopyinto[indx++] = FM_RBuffer_Receive_Data();
		if (so_far == len) return 1;
		FM_RBUFFER_TIMEOUT = FM_RBUFFER_TIMEOUT_DEF;
		while ((!FM_RBuffer_Checking_Data_Is_Available())&&FM_RBUFFER_TIMEOUT);
		if (FM_RBUFFER_TIMEOUT == 0) return 0;
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len) return 1;
	else return 0;
}

void Uart_sendstring (const char *s)
{
	while(*s) FM_RBuffer_Write_Data(*s++);
}

int Get_after (char *string, uint8_t numberofchars, char *buffertosave)
{
	for (int indx=0; indx<numberofchars; indx++)
	{
		FM_RBUFFER_TIMEOUT = FM_RBUFFER_TIMEOUT_DEF;
		while ((!FM_RBuffer_Checking_Data_Is_Available())&&FM_RBUFFER_TIMEOUT);  // wait until some data is available
		if (FM_RBUFFER_TIMEOUT == 0) return 0;  // if data isn't available within time, then return 0
		buffertosave[indx] = FM_RBuffer_Receive_Data();  // save the data into the buffer... increments the tail
        // strcpy(buffertosave, __RX_RBuffer->fm_buffer);   
	}
	return 1;
}

void FM_RBuffer_Copy_An_Array(char *arr1, char *arr2 ){

 int n = __RX_RBuffer->fm_buffer[__RX_RBuffer->tail];
strcpy(arr1, __RX_RBuffer->fm_buffer);
    printf("Copied string: %s\n",arr1);
}