/**
  ******************************************************************************
  * @file    bsp_sensor.h
  * @author  Adam Huang
  * @version V1.0.0
  * @date    05-Nov-2014
  * @brief   .
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, NEAR Inc. SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 Near Inc.</center></h2>
  ******************************************************************************
  */

#ifndef __BSP_SENSOR_H
#define __BSP_SENSOR_H
#include "bsp_sensor_mhtrd06.h"
#include "bsp_sensor_tgs2600.h"
//#include "bsp_sensor_gp2y1010.h"
#include "bsp_sensor_gp2y1050.h"
#include "bsp_adc.h"

extern void bsp_InitSensor(void);

//extern void bsp_UinitSensor(void);

//extern void bsp_sensor_tick(void);

//extern void bsp_sensor_irq(void);
extern void sensor_thread(void *inContext);

#endif