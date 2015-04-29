/**
  ******************************************************************************
  * @file    bsp_buzzer.h
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
#ifndef __BUZZER_H
#define __BUZZER_H

#define FREQ_NO  0
#define FREQ_2K3 2300
#define FREQ_2K6 2600
#define FREQ_2K9 2900
#define FREQ_2K  2000
#include "bsp_beep.h" 

typedef  uint32_t  FREQ_Type;

typedef enum
{
  MONO = 0,     //����
  POLY_ON = 1,  //��������
  POLY_OFF = 2  //�ػ�����
}Tone_Type;  //��������������
 
typedef struct
{
  FREQ_Type Freq;    //Ƶ��
  u8 OSCTime; //�񵴳���ʱ��,��С��λΪ10ms
  u8 PWRTime; //�������ʱ��,��С��λΪ10ms
} TONE_Def; //�����ṹ��
 
extern void BuzzerStart(Tone_Type ToneType);
extern void BuzzerCtrl(void* arg);

#endif
