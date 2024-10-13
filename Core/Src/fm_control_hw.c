#include "fm_control_hw.h"

FM_V0x_Control_States_t         FM_V0x_Control_State;

bool FM_V0x_Control_Hw_Init( ){
        FM_V0x_Control_State = FM_V0x_CONTROL_STATE_ON;

        return true        

}