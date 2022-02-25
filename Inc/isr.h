/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    isr.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ISR_H
#define __ISR_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Exported functions prototypes ---------------------------------------------*/

void NMI_Handler(void) __attribute__ ((interrupt ("IRQ")));
void HardFault_Handler(void) __attribute__ ((interrupt ("IRQ")));
void MemManage_Handler(void) __attribute__ ((interrupt ("IRQ")));
void BusFault_Handler(void) __attribute__ ((interrupt ("IRQ")));
void UsageFault_Handler(void) __attribute__ ((interrupt ("IRQ")));
void SVC_Handler(void) __attribute__ ((interrupt ("IRQ")));
void DebugMon_Handler(void) __attribute__ ((interrupt ("IRQ")));
void PendSV_Handler(void) __attribute__ ((interrupt ("IRQ")));
void SysTick_Handler(void) __attribute__ ((interrupt ("IRQ")));
void OTG_FS_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));

#ifdef __cplusplus
}
#endif

#endif /* __ISR_H */
