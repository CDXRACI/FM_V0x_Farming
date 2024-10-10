/****************************************************************************************************
 * @file :        fm_rtc.h
 * --------------------------------------------------------------------------------------------------
 * @brief:        Declare parameters, functions that will be used in RTC.
 *                Purpose, Turn on/off lights and pumps when it's time
 * @version  :    0.0.1
 * --------------------------------------------------------------------------------------------------
 * @author   :    CDP_Darkness
 * @copyright:    DP.Greens © all rights reserved          
 * @date     :    2024-10-05
 * *************************************************************************************************/




#ifndef __FM_RTC_H
#define __FM_RTC_H

#ifdef __cplusplsus
extern "C" {
#endif   

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rtc.h"
#include <stdio.h>
#include <stdbool.h> 

#define                 FM_V0x_SET_SECOND_DEF                (0  )
#define                 FM_V0x_SET_0_MIN_DEF                 (0  )
#define                 FM_V0x_SET_2_MIN_DEF                 (2  )
#define                 FM_V0x_SET_10_MIN_DEF                (10 )
#define                 FM_V0x_SET_20_MIN_DEF                (20 )
#define                 FM_V0x_SET_30_MIN_DEF                (30 )
#define                 FM_V0x_SET_35_MIN_DEF                (35 )
#define                 FM_V0x_SET_40_MIN_DEF                (40 )
#define                 FM_V0x_SET_50_MIN_DEF                (50 )
#define                 FM_V0x_SET_55_MIN_DEF                (55 )
#define                 FM_V0x_SET_59_MIN_DEF                (59 )
#define                 FM_V0x_SET_0_HOUR_DEF                (0  )
#define                 FM_V0x_SET_5_HOUR_DEF                (5  )
#define                 FM_V0x_SET_6_HOUR_DEF                (6  )
#define                 FM_V0x_SET_16_HOUR_DEF               (16 )
#define                 FM_V0x_SET_18_HOUR_DEF               (18 )
#define                 FM_V0x_SET_23_HOUR_DEF               (23 )

#define                 FM_V0x_CHECK_TIME_IS_0HOUR           (0  )      
#define                 FM_V0x_CHECK_TIME_IS_24HOUR          (24 )


#define FM_LED_STATUS_Pin                                     GPIO_PIN_8
#define FM_LIGHT_DRV_Pin                                      GPIO_PIN_4
#define FM_PUMP_DRV_Pin                                       GPIO_PIN_0
#define FM_STR_DRV_Pin                                        GPIO_PIN_1

typedef struct  {
        uint8_t Set_Second;
        uint8_t Set_Min;
        uint8_t Set_Hour;
        uint8_t Set_Day;
        uint8_t Set_Week;
        uint8_t Set_Mon;
        uint8_t Set_Year;
         struct {
                uint8_t Get_Second;
                uint8_t Get_Min;
                uint8_t Get_Hour;
                uint8_t Get_Day;
                uint8_t Get_Week;
                uint8_t Get_Mon;
                uint8_t Get_Year;                    
        }FM_V0x_Get_TimeDate;
}FM_V0x_Parameters_RTC_t;

typedef struct {

        uint8_t Set_Second;
        uint8_t Set_Min;
        uint8_t Set_Hour;

}FM_V0x_Alarm_RTC_t;

typedef enum {
        FM_V0x_FLAG_IS_0_DEF = 0, //
        FM_V0x_FLAG_IS_1_DEF,
        FM_V0x_FLAG_IS_2_DEF,
        FM_V0x_FLAG_IS_3_DEF,
        FM_V0x_FLAG_IS_4_DEF,
        FM_V0x_FLAG_IS_5_DEF
}FM_V0x_RTC_WakeUp_States_t;

typedef enum {
        FM_V0x_RTC_STATE_FLAG_IS_OFF = 0,
        FM_V0x_RTC_STATE_FLAG_IS_ON  = 1,
        FM_V0x_RTC_STATE_FLAG_IS_2   = 2,  // set parameters for RTC
        FM_V0x_RTC_STATE_FLAG_IS_3   = 3,  // set parameters for RTC
        FM_V0x_RTC_STATE_FLAG_IS_4   = 4,  // get data from RTC
        FM_V0x_RTC_STATE_FLAG_IS_5   = 5,  // get on/off time for Light, Pump.
        FM_V0x_RTC_STATE_FLAG_IS_6   = 6  // clear RTC's buffer.
}FM_V0x_RTC_States_t;




uint8_t FM_V0x_Rtc_Init( void );
uint8_t FM_V0x_Clear_Backup_Registers( void );
uint8_t FM_V0x_RTC_Set_Time (FM_V0x_Parameters_RTC_t  RTC_SetTime  );
uint8_t FM_V0x_RTC_Set_Date (FM_V0x_Parameters_RTC_t  RTC_SetDate  );
uint8_t FM_V0x_RTC_Get_Time (FM_V0x_Parameters_RTC_t  *RTC_GetTime );
uint8_t FM_V0x_RTC_Get_Date (FM_V0x_Parameters_RTC_t  *RTC_GetDate );
uint8_t FM_V0x_RTC_WakeUp   (FM_V0x_Parameters_RTC_t  RTC_WakeUp   );
uint8_t FM_V0x_RTC_Set_Alarm(FM_V0x_Alarm_RTC_t       RTC_Alarm    );



#ifdef __cplusplus
}
#endif

#endif /* __FM_RTC_H */