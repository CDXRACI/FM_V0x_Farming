/*
  @dev : Phuc.cd
  @file: fm_init_project.h
  -------------------------
  @function: fm_fc_init()
  @return: None
  @param: Initialize program for FM V0xx 
  -------------------------
  @date: 4/2/2023
  @ver : 0.3.2
  -------------------------
*/

#ifndef __FM_INIT_PROJECT_H
#define __FM_INIT_PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>  
#include <stdbool.h>

void fm_fc_init();
#ifdef __cplusplus
}
#endif
#endif