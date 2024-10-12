#ifndef __FM_MULTIADC_H
#define __FM_MULTIADC_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"
#include "stm32f1xx_hal_def.h"
#include <stdio.h>
#include <stdbool.h> 
#include <assert.h>

#define         FM_V0X_MULTIADC_IS_0             ( 0 )
#define         FM_V0X_MUTLIADC_IS_1             ( 1 )
#define         FM_V0X_MUTLIADC_CHANNEL_6        ( 6 )
#define         FM_V0X_MUTLIADC_CHANNEL_7        ( 7 )
#define         FM_V0X_MUTLIADC_CHANNEL_8        ( 8 )
#define         FM_V0X_MUTLIADC_CHANNEL_9        ( 9 )

typedef enum {  
    
        FM_V0x_MULTI_STATES_TURNS_OFF             =   FM_V0X_MULTIADC_IS_0,
        FM_V0x_MULTI_STATES_TURNS_ON              =   FM_V0X_MUTLIADC_IS_1,  
        FM_V0x_MULTI_STATES_SETTINGS                                      ,
        FM_V0x_MULTI_STATES_IS_STARTING                                   , 
        FM_V0x_MULTI_STATES_IS_READING                                    , 
        FM_V0x_MULTI_STATES_CHANNEL_6             =   FM_V0X_MUTLIADC_CHANNEL_6,
        FM_V0x_MULTI_STATES_CHANNEL_7             =   FM_V0X_MUTLIADC_CHANNEL_7,
        FM_V0x_MULTI_STATES_CHANNEL_8             =   FM_V0X_MUTLIADC_CHANNEL_8,
        FM_V0x_MULTI_STATES_CHANNEL_9             =   FM_V0X_MUTLIADC_CHANNEL_9,
        FM_V0x_MULTI_STATES_READS_LIGHT_SENSOR                            ,
        FM_V0x_MULTI_STATES_READS_RAIN_SENSOR                             ,
        FM_V0x_MULTI_STATES_READS_SOLID_SENSOR                            ,
        FM_V0x_MULTI_STATES_IS_WRITING_BUFFER                             ,
        FM_V0x_MULTI_STATES_IS_PAUSING                                    ,
        FM_V0x_MULTI_STATES_IS_CLEANING_BUFFER                            ,                      

}   FM_V0x_multiADC_States_t;


typedef struct {
    
    ADC_HandleTypeDef   *hadc_handle;
    uint16_t            FM_V0x_MultiADC_Chooses_ADC;

}   FM_V0x_MultiADC_Handle_t;

typedef struct {
  
    uint32_t  FM_V0x_Light_Sensor_Buffer;
    uint32_t  FM_V0x_Rain_Sensor_Buffer ;
    uint32_t  FM_V0x_Solid_Sensor_Buffer;

}   FM_V0x_MultADC_Buffer_t;

void FM_V0x_MultiADC_Init   ( );
void FM_V0x_MultiADC_Reads_ADC_Signal (FM_V0x_MultiADC_Handle_t hadc );
void FM_V0x_MultiADC_Starts ( FM_V0x_MultiADC_Handle_t hadc );
bool FM_V0x_MultiADC_Stop   ( FM_V0x_MultiADC_Handle_t *hadc );
bool FM_V0x_MultiADC_Is_Pausing ( FM_V0x_MultiADC_Handle_t *hadc );

#ifdef __cplusplus
}
#endif

#endif /* __FM_MULTIADC_H */