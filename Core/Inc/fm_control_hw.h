/****************************************************************************************************
 * @file :        fm_control_hw.h
 * --------------------------------------------------------------------------------------------------
 * @brief:        Declare parameters, functions that are be used for the devices.
 *                Purpose, Turn on/off the deveices.
 * @version  :    0.0.1
 * --------------------------------------------------------------------------------------------------
 * @author   :    CDP_Darkness
 * @copyright:    DP.Greens © all rights reserved          
 * @date     :    2024-10-13
 * *************************************************************************************************/

#ifndef __FM_CONTROL_HW_H
#define __FM_CONTROL_HW_H


#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------*** Inclusion ***--------------------------------------------*/

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdbool.h> 

/*-------------------------------------*** Definition ***-------------------------------------------*/

#define             FM_V0x_CONTROL_HW_OFF                   ( 0 )
#define             FM_V0x_CONTROL_HW_ON                    ( 1 )

/*-------------------------------------*** Structures ***-------------------------------------------*/

typedef struct {

        uint8_t FM_V0x_CTR_Light;
        uint8_t FM_V0x_CTR_Pump ;
        uint8_t FM_V0x_CTR_STR  ;

} FM_V0x_Control_Machines_t;

/*-------------------------------------*** Enumeration ***------------------------------------------*/

typedef enum {

        FM_V0x_CONTROL_STATE_OFF              =   FM_V0x_CONTROL_HW_OFF,
        FM_V0x_CONTROL_STATE_ON                                        ,
        FM_V0x_CONTROL_STATE_PUMP_IS_OFF                               ,
        FM_V0x_CONTROL_STATE_PUMP_IS_ON                                ,
        FM_V0x_CONTROL_STATE_LIGHT_IS_OFF                              ,
        FM_V0x_CONTROL_STATE_LIGHT_IS_ON                               ,
        FM_V0x_CONTROL_STATE_STR_IS_OFF                                ,
        FM_V0x_CONTROL_STATE_STR_IS_ON  

} FM_V0x_Control_States_t;


/*-------------------------------------*** Functions ***--------------------------------------------*/



/**
 * 
 * @func     FM_V0x_Control_Hw_Init( )
 * 
 * @brief:   FM_V0x_Control_Hw_Init is be used to initialization for the devices.
 * 
 * @pram:    None
 * @return:  - True if success.
 *           - False otherwise.   
 * @author:  CDP.Darkness
 * @version: 0.0.1
 * @date:    13/10/2024
 */

bool FM_V0x_Control_Hw_Init( );

/**
 * 
 * @func : FM_V0x_Control_Start( )
 * 
 * @brief:   FM_V0x_Control_Start To start the devices
 * 
 * @pram:    None
 * @return:  None
 * @author:  CDP.Darkness
 * @version: 0.0.1
 * @date:    13/10/2024
 */

bool FM_V0x_Control_Start( );

/**
 * 
 * @func : FM_V0x_Control_Stop( )
 * 
 * @brief:   FM_V0x_Control_Stop to stop the devices.
 * 
 * @pram:    None
 * @return:  None
 * @author:  CDP.Darkness
 * @version: 0.0.1
 * @date:    13/10/2024
 */

bool FM_V0x_Control_Stop( );

/**
 * 
 * @func : FM_V0x_Control_Pending( )
 * 
 * @brief:   FM_V0x_Control_Pending pausing the devices.
 * 
 * @pram:    None
 * @return:  None
 * @author:  CDP.Darkness
 * @version: 0.0.1
 * @date:    13/10/2024
 */

bool FM_V0x_Control_Pending( );

#ifdef __cpluspus
}
#endif

#endif /* _FM_CONTROL_HW_*/
