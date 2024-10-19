#include "fm_multiADC.h"

extern void Error_Handler(void);

FM_V0x_multiADC_States_t                 FM_V0x_MultiADC_States;
FM_V0x_MultADC_Buffer_t                  __FM_V0x_MultiADC_Buffer = { 0,0,0 };
FM_V0x_MultADC_Buffer_t                  *FM_V0x_MultiADC_Buffer;
ADC_HandleTypeDef                                          hadc1;
FM_V0x_MultiADC_Handle_t                                   hadcx; 
FM_V0x_multiADC_Calib_Data_t             FM_V0x_MultiADC_Calbid_Data;

ADC_ChannelConfTypeDef sConfig =    {0};

void FM_V0x_MultiADC_Init ( ){
    
            FM_V0x_MultiADC_States = FM_V0x_MULTI_STATES_TURNS_ON ;

            FM_V0x_MultiADC_Handle_t hadcx = {
                .FM_V0x_MultiADC_Chooses_ADC = FM_V0x_MULTI_STATES_CHANNEL_6,
                .hadc_handle = &hadc1 };
            
            printf("LOG_ADC: ADC is initializing. Mode is: %d\r\n", FM_V0x_MultiADC_States);

            FM_V0x_MultiADC_Buffer = &__FM_V0x_MultiADC_Buffer;
            FM_V0x_MultiADC_States = FM_V0x_MULTI_STATES_SETTINGS;
            printf("LOG_ADC: Settings parameters for ADC. Mode is: %d\r\n", FM_V0x_MULTI_STATES_SETTINGS);
            
            hadcx.hadc_handle->Instance = ADC1;
            hadcx.hadc_handle->Init.ScanConvMode = ADC_SCAN_DISABLE;
            hadcx.hadc_handle->Init.ContinuousConvMode = ENABLE;
            hadcx.hadc_handle->Init.DiscontinuousConvMode = DISABLE;
            hadcx.hadc_handle->Init.ExternalTrigConv = ADC_SOFTWARE_START;
            hadcx.hadc_handle->Init.DataAlign = ADC_DATAALIGN_RIGHT;
            hadcx.hadc_handle->Init.NbrOfConversion = 1;

            
            if (HAL_ADC_Init(hadcx.hadc_handle) != HAL_OK)
            {
                Error_Handler();
            }
            FM_V0x_MultiADC_Calbid_Data = FM_V0x_MULTIADC_CALIB_DATA_IS_DARK;
            FM_V0x_MultiADC_States = FM_V0x_MULTI_STATES_CHANNEL_6;
            switch (FM_V0x_MultiADC_States)
            {
            case FM_V0x_MULTI_STATES_CHANNEL_6:
                    hadcx.FM_V0x_MultiADC_Chooses_ADC = FM_V0x_MULTI_STATES_CHANNEL_6;
                    sConfig.Channel = ADC_CHANNEL_6;
                    sConfig.Rank = ADC_REGULAR_RANK_1;
                    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;    
                    if (HAL_ADC_ConfigChannel( hadcx.hadc_handle, &sConfig) != HAL_OK ){
                        Error_Handler();
                    }
                    printf("LOG_ADC: Settings for ADC1 channel %d\r\n", FM_V0x_MULTI_STATES_CHANNEL_6);
                    FM_V0x_MultiADC_Starts(hadcx);
                    FM_V0x_MultiADC_Reads_ADC_Signal(hadcx);
                    FM_V0x_MultiADC_States = FM_V0x_MULTI_STATES_CHANNEL_7;
            case FM_V0x_MULTI_STATES_CHANNEL_7:
                    hadcx.FM_V0x_MultiADC_Chooses_ADC = FM_V0x_MULTI_STATES_CHANNEL_7;
                    sConfig.Channel = ADC_CHANNEL_7;
                    sConfig.Rank = ADC_REGULAR_RANK_1;
                    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;    
                                
                    if (HAL_ADC_ConfigChannel(hadcx.hadc_handle, &sConfig) != HAL_OK ){
                        Error_Handler();
                    }
                    printf("LOG_ADC: Settings for ADC1 channel %d\r\n", FM_V0x_MULTI_STATES_CHANNEL_7);
                    FM_V0x_MultiADC_Starts(hadcx);
                    FM_V0x_MultiADC_Reads_ADC_Signal(hadcx);
                    FM_V0x_MultiADC_States = FM_V0x_MULTI_STATES_CHANNEL_8;
            case  FM_V0x_MULTI_STATES_CHANNEL_8:
                    hadcx.FM_V0x_MultiADC_Chooses_ADC = FM_V0x_MULTI_STATES_CHANNEL_8;
                    sConfig.Channel = ADC_CHANNEL_8;
                    sConfig.Rank = ADC_REGULAR_RANK_1;
                    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;   
                                
                    if (HAL_ADC_ConfigChannel(hadcx.hadc_handle, &sConfig) != HAL_OK ){
                        Error_Handler();
                    } 

                    printf("LOG_ADC: Settings for ADC1 channel %d\r\n", FM_V0x_MULTI_STATES_CHANNEL_8);
                    FM_V0x_MultiADC_Starts(hadcx);
                    FM_V0x_MultiADC_Reads_ADC_Signal(hadcx);
                    break;
            default:
                break;
            }
}

void FM_V0x_MultiADC_Starts ( FM_V0x_MultiADC_Handle_t hadc ){

             if (HAL_ADC_Start(hadc.hadc_handle) != HAL_OK ){
                // Start Error
                Error_Handler();
            }
            FM_V0x_MultiADC_States = FM_V0x_MULTI_STATES_IS_STARTING;
            printf("LOG_ADC: Staring ADC function reads signal from sensor. Mode is %d\r\n",FM_V0x_MULTI_STATES_IS_STARTING);

}

void FM_V0x_MultiADC_Reads_ADC_Signal (FM_V0x_MultiADC_Handle_t hadc ){

            if (HAL_ADC_PollForConversion(hadc.hadc_handle, HAL_MAX_DELAY) == HAL_OK){
                    switch (hadc.FM_V0x_MultiADC_Chooses_ADC)
                    {
                        case FM_V0x_MULTI_STATES_CHANNEL_6:
                             FM_V0x_MultiADC_Buffer->FM_V0x_Light_Sensor_Buffer  = HAL_ADC_GetValue(hadc.hadc_handle);
                             FM_V0X_MultiADC_Calib_Data(hadc,FM_V0x_MultiADC_Buffer);
                                                        // printf("LOG_ADC: Reading ADC from channel %d. Value is : %ld. Mode is %d\r\n", hadc.FM_V0x_MultiADC_Chooses_ADC, (FM_V0x_MultiADC_Buffer->FM_V0x_Light_Sensor_Buffer), FM_V0x_MULTI_STATES_IS_READING);
                             break;
                        case FM_V0x_MULTI_STATES_CHANNEL_7:
                              FM_V0x_MultiADC_Buffer->FM_V0x_Rain_Sensor_Buffer  = HAL_ADC_GetValue(hadc.hadc_handle);
                            //    printf("LOG_ADC: Reading ADC from channel %d. Value is : %ld. Mode is %d\r\n", hadc.FM_V0x_MultiADC_Chooses_ADC, (FM_V0x_MultiADC_Buffer->FM_V0x_Rain_Sensor_Buffer), FM_V0x_MULTI_STATES_IS_READING);
                               FM_V0X_MultiADC_Calib_Data(hadc,FM_V0x_MultiADC_Buffer);
                              break;
                        case FM_V0x_MULTI_STATES_CHANNEL_8:
                              FM_V0x_MultiADC_Buffer->FM_V0x_Solid_Sensor_Buffer = HAL_ADC_GetValue(hadc.hadc_handle);
                            //   printf("LOG_ADC: Reading ADC from channel %d. Value is : %ld. Mode is %d\r\n", hadc.FM_V0x_MultiADC_Chooses_ADC, (FM_V0x_MultiADC_Buffer->FM_V0x_Solid_Sensor_Buffer), FM_V0x_MULTI_STATES_IS_READING);
                             FM_V0X_MultiADC_Calib_Data(hadc,FM_V0x_MultiADC_Buffer);
                            break;
                        default:
                            break;
                    }
            //   printf("LOG_ADC: Reading ADC from channel %d. Value is : %ld. Mode is %d\r\n", hadc.FM_V0x_MultiADC_Chooses_ADC, (FM_V0x_MultiADC_Buffer->FM_V0x_Light_Sensor_Buffer), FM_V0x_MULTI_STATES_IS_READING);
            }
}

bool FM_V0X_MultiADC_Calib_Data(FM_V0x_MultiADC_Handle_t hadc, FM_V0x_MultADC_Buffer_t *buffer ){
        switch (hadc.FM_V0x_MultiADC_Chooses_ADC)
        {
        case FM_V0x_MULTI_STATES_CHANNEL_6:
                    
                    if( buffer->FM_V0x_Light_Sensor_Buffer == FM_V0x_MULTIADC_CALIB_DATA_IS_DARK ){
                        
                    }
                    else if( buffer->FM_V0x_Light_Sensor_Buffer == FM_V0x_MULTIADC_CALIB_DATA_FOR_IDR_IS_LIGHT ){
                        
                    }
                    else if( buffer->FM_V0x_Light_Sensor_Buffer == FM_V0x_MULTIADC_CALIB_DATA_FOR_IDR_IS_BRIGHT ){
                        
                    }
                    printf("LOG_ADC: Reading ADC from channel %d. Value is : %ld. Mode is %d\r\n",  hadc.FM_V0x_MultiADC_Chooses_ADC, \
                                                    buffer->FM_V0x_Light_Sensor_Buffer, FM_V0x_MULTI_STATES_IS_READING);
                    break;
                
        case FM_V0x_MULTI_STATES_CHANNEL_7:
            
                    printf("LOG_ADC: Reading ADC from channel %d. Value is : %ld. Mode is %d\r\n",  hadc.FM_V0x_MultiADC_Chooses_ADC, \
                                                    buffer->FM_V0x_Rain_Sensor_Buffer, FM_V0x_MULTI_STATES_IS_READING);
                    break;
                
        case FM_V0x_MULTI_STATES_CHANNEL_8:
               
                    printf("LOG_ADC: Reading ADC from channel %d. Value is : %ld. Mode is %d\r\n",  hadc.FM_V0x_MultiADC_Chooses_ADC, \
                                                    buffer->FM_V0x_Solid_Sensor_Buffer, FM_V0x_MULTI_STATES_IS_READING);
                    break;
                
        default:
            break;
        }  
    return true;
}
