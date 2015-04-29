/**
  ******************************************************************************
  * @file    bsp_sensor_gp2y1010.c
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
#include "stm32f2xx.h"
#include "bsp_sensor_gp2y1010.h"

static void GPLED_GPIO_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
	
    GPLED_CLK_INIT(GPLED_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPLED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //
    GPIO_Init(GPLED_PORT, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPLED_PORT, GPLED_PIN_SOURCE, GPIO_AF_TIM8);
}

static void GPLED_TIM8_Init(void)        //��ʱ��PWM����
{
    TIM_OCInitTypeDef    TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);            //������ʱ��1��ʱ�ӡ�
    //SYSTEM_F = 120 000 000,APB2 Prescaler = 2,APB2_F = 120M/2*2 = 120M
    TIM_DeInit(TIM8);        //��λ��ʱ��4�Ĵ���ֵ��                                       
    TIM_TimeBaseStructure.TIM_Prescaler = 120-1;                   //12��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;           //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;       //�����Զ���װ������ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;       //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);                //����ָ���Ĳ�����ʼ��TIMx

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                                   //CH4PWM2ģʽ
    TIM_OCInitStructure.TIM_Pulse = 320;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //OC4 �͵�ƽ��Ч
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
    TIM_OC2Init(TIM8,&TIM_OCInitStructure);                                                      //����ָ���Ĳ�����ʼ������TIMx
    TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);  //CH4 Ԥװ��ʹ��
    TIM_ARRPreloadConfig(TIM8,ENABLE);
    TIM_Cmd(TIM8,ENABLE);                              //������ʱ��8
    
    TIM_CtrlPWMOutputs(TIM8,ENABLE);
}

void bsp_sensor_init_gp2y1010(void)
{
    GPLED_GPIO_Init();
    
    GPLED_TIM8_Init();
}
