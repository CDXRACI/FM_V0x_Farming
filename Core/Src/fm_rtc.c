/* __FM_RTC_C */

#include "fm_rtc.h"

FM_V0x_Parameters_RTC_t              FM_V0x_RTC_Pars  ;
FM_V0x_Parameters_RTC_t              FM_V0x_Get_RTC   ;
FM_V0x_Alarm_RTC_t                   FM_V0x_Alarm_RTC ;
FM_V0x_RTC_States_t                  FM_V0x_RTC_States;
RTC_TimeTypeDef                                  pTime;
RTC_HandleTypeDef                                 hrtc;
RTC_TimeTypeDef                                  sTime;
RTC_DateTypeDef                           DateToUpdate;


extern void Error_Handler(void);

uint8_t FM_V0x_Rtc_Init( void ){
        FM_V0x_RTC_States = FM_V0x_RTC_STATE_FLAG_IS_OFF;
        printf("--> \"FM_RTC_V0x is Initializing... \" \r\n");
        printf("--> Time and Data were initialized is: 7h-21min-00s \r\n");
    
        FM_V0x_RTC_Pars.Set_Hour   = 0X10;
        FM_V0x_RTC_Pars.Set_Min    = 0x57;
        FM_V0x_RTC_Pars.Set_Second =    0;
        FM_V0x_RTC_Pars.Set_Day    = 0x12;
        FM_V0x_RTC_Pars.Set_Mon    = 0x10;
        FM_V0x_RTC_Pars.Set_Year   = 0x24;
    
        FM_V0x_RTC_States = FM_V0x_RTC_STATE_FLAG_IS_ON;
        printf("RTC STATE: RTC Was intialized. Mode is: %d\r\n", FM_V0x_RTC_States);
        FM_V0x_RTC_Set_Time(FM_V0x_RTC_Pars );
        FM_V0x_RTC_Set_Date(FM_V0x_RTC_Pars );
        return FM_V0x_RTC_States;
}

uint8_t FM_V0x_RTC_Set_Time (FM_V0x_Parameters_RTC_t  RTC_SetTime ){

        if( FM_V0x_RTC_States == FM_V0x_RTC_STATE_FLAG_IS_ON ){
            hrtc.Instance = RTC;   
            if (( RTC_SetTime.Set_Hour >= FM_V0x_CHECK_TIME_IS_24HOUR ) && ( RTC_SetTime.Set_Hour < FM_V0x_CHECK_TIME_IS_0HOUR  )){
                printf("-->BUG: Hour in RTC's Setting is not seted \r\n");
                return false;
            }
        //    if( HAL_RTCEx_BKUPRead( &hrtc, RTC_BKP_DR1 ) != 0x32F2 ){
            hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
            hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
            if (HAL_RTC_Init( &hrtc ) != HAL_OK ){
                Error_Handler( ); }
         if( HAL_RTCEx_BKUPRead( &hrtc, RTC_BKP_DR1 ) != 0x32F2 ){
            sTime.Hours   =  RTC_SetTime.Set_Hour;
            sTime.Minutes =  RTC_SetTime.Set_Min;
            sTime.Seconds =  RTC_SetTime.Set_Second; 

            if (HAL_RTC_SetTime( &hrtc, &sTime, RTC_FORMAT_BCD ) != HAL_OK ){
                Error_Handler( );}
      
          HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2 );}
            FM_V0x_RTC_States  = FM_V0x_RTC_STATE_FLAG_IS_2;
            printf("\r\nRTC STATE: RTC was set time. Mode is: %d\r\n", FM_V0x_RTC_States);
        }
        if (( sTime.Hours >= FM_V0x_CHECK_TIME_IS_24HOUR ) && ( sTime.Hours < FM_V0x_CHECK_TIME_IS_0HOUR )){
                printf( "-->BUG: Hour in RTC isn't set!\r\n" );
                return false;
        } else {
                return FM_V0x_RTC_States;
        }
}

uint8_t FM_V0x_RTC_Set_Date (FM_V0x_Parameters_RTC_t  RTC_SetDate  ){

        if( FM_V0x_RTC_States == FM_V0x_RTC_STATE_FLAG_IS_2 ){

            hrtc.Instance = RTC;     

            if (( RTC_SetDate.Set_Day   > 31 )  && ( RTC_SetDate.Set_Day < 0 )){
                printf("-->BUG: Day in RTC  is not seted \r\n");  
                return false; }
            hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
            hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
          
            if (HAL_RTC_Init( &hrtc ) != HAL_OK ){
                Error_Handler( ); }

            DateToUpdate.Month   = RTC_SetDate.Set_Mon ;
            DateToUpdate.Date    = RTC_SetDate.Set_Day ;
            DateToUpdate.Year    = RTC_SetDate.Set_Year;

            if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK){
                Error_Handler(); }
          
            FM_V0x_RTC_States  = FM_V0x_RTC_STATE_FLAG_IS_3;
            printf("\r\nRTC STATE: RTC Was set date. Mode is: %d\r\n",FM_V0x_RTC_States );         
           
        }  
        if (( DateToUpdate.Date >= FM_V0x_CHECK_TIME_IS_24HOUR ) && ( DateToUpdate.Date < FM_V0x_CHECK_TIME_IS_0HOUR )) {
                printf( "-->BUG: Hour in RTC isn't set!\r\n" );
                return false;
        } else { return  FM_V0x_RTC_States; }       
}


uint8_t FM_V0x_RTC_Get_Time ( FM_V0x_Parameters_RTC_t  *RTC_GetTime ){
        
        if( FM_V0x_RTC_States == FM_V0x_RTC_STATE_FLAG_IS_3 ){

            HAL_RTC_GetTime(&hrtc, &pTime, RTC_FORMAT_BIN);
            RTC_GetTime->FM_V0x_Get_TimeDate.Get_Hour   = pTime.Hours;
            RTC_GetTime->FM_V0x_Get_TimeDate.Get_Min    = pTime.Minutes;
            RTC_GetTime->FM_V0x_Get_TimeDate.Get_Second = pTime.Seconds;
           // FM_V0x_RTC_WakeUp(FM_V0x_Get_RTC); 
            FM_V0x_RTC_States  = FM_V0x_RTC_STATE_FLAG_IS_4;
            printf("RTC_LOG: ==================***RTC***=====================\r\n");
            printf("RTC STATE: RTC gets time. Mode is: %d. ",FM_V0x_RTC_States );
            printf("Time is: %d %d %d\r\n",    RTC_GetTime->FM_V0x_Get_TimeDate.Get_Hour  ,  RTC_GetTime->FM_V0x_Get_TimeDate.Get_Min  ,  RTC_GetTime->FM_V0x_Get_TimeDate.Get_Second );
        }
        if ((   RTC_GetTime->FM_V0x_Get_TimeDate.Get_Hour     >= FM_V0x_CHECK_TIME_IS_24HOUR )   && (      RTC_GetTime->FM_V0x_Get_TimeDate.Get_Hour     < FM_V0x_CHECK_TIME_IS_0HOUR )) {
                printf( "-->BUG: Hour in RTC isn't set!\r\n" );
                return false;
        } else { return   FM_V0x_RTC_States  ;}
}

uint8_t FM_V0x_RTC_Get_Date (FM_V0x_Parameters_RTC_t  *RTC_GetDate  ){
        if( FM_V0x_RTC_States == FM_V0x_RTC_STATE_FLAG_IS_4 ){

            HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN );
            RTC_GetDate->FM_V0x_Get_TimeDate.Get_Day    = DateToUpdate.Date;
            RTC_GetDate->FM_V0x_Get_TimeDate.Get_Mon    = DateToUpdate.Month;
            RTC_GetDate->FM_V0x_Get_TimeDate.Get_Year   = DateToUpdate.Year;
            FM_V0x_RTC_States  = FM_V0x_RTC_STATE_FLAG_IS_3;
            printf("RTC STATE: RTC gets time. Mode is: %d. ",FM_V0x_RTC_States );
            printf("Date is: %d %d %d\r\n",    RTC_GetDate->FM_V0x_Get_TimeDate.Get_Day  ,  RTC_GetDate->FM_V0x_Get_TimeDate.Get_Mon  ,  RTC_GetDate->FM_V0x_Get_TimeDate.Get_Year );
            printf("RTC_LOG: ==================***RTC***=====================\r\n");
           // FM_V0x_RTC_WakeUp(FM_V0x_Get_RTC);  
        }    
        if ((   RTC_GetDate->FM_V0x_Get_TimeDate.Get_Day     >= FM_V0x_CHECK_TIME_IS_24HOUR )   && (      RTC_GetDate->FM_V0x_Get_TimeDate.Get_Day     < FM_V0x_CHECK_TIME_IS_0HOUR )) {
                printf( "-->BUG: Hour in RTC isn't set!\r\n" );
                return false;
            } else {
                return   RTC_GetDate->FM_V0x_Get_TimeDate.Get_Day  ;
        }

}

uint8_t FM_V0x_RTC_WakeUp   ( FM_V0x_Parameters_RTC_t RTC_WakeUp){
         FM_V0x_RTC_WakeUp_States_t RTC_WakeUp_States = FM_V0x_FLAG_IS_0_DEF;
         if( FM_V0x_RTC_States == FM_V0x_RTC_STATE_FLAG_IS_5 ){

            if((RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour > FM_V0x_SET_23_HOUR_DEF           )||\
            ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour > FM_V0x_SET_0_HOUR_DEF           )){
                    return false;
            }
            
            if (( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour > FM_V0x_SET_18_HOUR_DEF         )||\
                ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour < FM_V0x_SET_6_HOUR_DEF          )){
                        HAL_GPIO_WritePin( GPIOA, FM_LED_STATUS_Pin, GPIO_PIN_SET          );                            
                        HAL_GPIO_WritePin( GPIOA, FM_PUMP_DRV_Pin, GPIO_PIN_SET            );
                        HAL_GPIO_WritePin( GPIOA, FM_LIGHT_DRV_Pin, GPIO_PIN_RESET         );
                        RTC_WakeUp_States = FM_V0x_FLAG_IS_0_DEF;
            }

        else if ((( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour < FM_V0x_SET_18_HOUR_DEF    ))||\
                    (( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour > FM_V0x_SET_6_HOUR_DEF     ))){
                        HAL_GPIO_WritePin( GPIOA, FM_LED_STATUS_Pin,    GPIO_PIN_RESET     );                            
                        HAL_GPIO_WritePin( GPIOA, FM_PUMP_DRV_Pin,      GPIO_PIN_SET       );
                        HAL_GPIO_WritePin( GPIOA, FM_LIGHT_DRV_Pin,     GPIO_PIN_SET       );
                        RTC_WakeUp_States = FM_V0x_FLAG_IS_1_DEF;
            }
        
            if ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour == FM_V0x_SET_6_HOUR_DEF          ){
                if (( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Min == FM_V0x_SET_0_MIN_DEF     )&&\
                    ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Second >= FM_V0x_SET_SECOND_DEF )){
                        HAL_GPIO_WritePin( GPIOA, FM_LED_STATUS_Pin,   GPIO_PIN_SET        );
                        HAL_GPIO_WritePin( GPIOA, FM_LIGHT_DRV_Pin,    GPIO_PIN_SET        );
                        HAL_GPIO_WritePin( GPIOA, FM_PUMP_DRV_Pin,     GPIO_PIN_RESET      );
                        RTC_WakeUp_States = FM_V0x_FLAG_IS_2_DEF;
                }
                else if ((( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Min >= FM_V0x_SET_2_MIN_DEF      )&&\
                            ( RTC_WakeUp.Set_Min <= FM_V0x_SET_59_MIN_DEF                         ))&&\
                            ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Second >= FM_V0x_SET_SECOND_DEF  )){
                        HAL_GPIO_WritePin( GPIOA, FM_LED_STATUS_Pin,   GPIO_PIN_RESET             );
                        HAL_GPIO_WritePin( GPIOA, FM_PUMP_DRV_Pin,     GPIO_PIN_SET               );
                        RTC_WakeUp_States = FM_V0x_FLAG_IS_3_DEF;
                }
            }

            if( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Hour == FM_V0x_SET_16_HOUR_DEF                 ){
                if (( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Min == FM_V0x_SET_0_MIN_DEF                               )&&\
                    ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Second >= FM_V0x_SET_SECOND_DEF        )){
                        HAL_GPIO_WritePin( GPIOA, FM_LED_STATUS_Pin, GPIO_PIN_SET                 );
                        HAL_GPIO_WritePin( GPIOA, FM_PUMP_DRV_Pin,    GPIO_PIN_RESET              );
                        RTC_WakeUp_States = FM_V0x_FLAG_IS_4_DEF;
                }
                else if ((( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Min >= FM_V0x_SET_2_MIN_DEF      )&&\
                            ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Min <= FM_V0x_SET_59_MIN_DEF     ))&&\
                            ( RTC_WakeUp.FM_V0x_Get_TimeDate.Get_Second >= FM_V0x_SET_SECOND_DEF  )){
                        HAL_GPIO_WritePin( GPIOA, FM_LED_STATUS_Pin,   GPIO_PIN_RESET             );
                        HAL_GPIO_WritePin( GPIOA, FM_PUMP_DRV_Pin,   GPIO_PIN_SET                 );
                        RTC_WakeUp_States = FM_V0x_FLAG_IS_5_DEF;
                }
            }
    }    
        FM_V0x_RTC_States  = FM_V0x_RTC_STATE_FLAG_IS_5;
        printf("RTC STATE: RTC gets time. Mode is: %d\r\n",FM_V0x_RTC_States );
        return  RTC_WakeUp_States;
}