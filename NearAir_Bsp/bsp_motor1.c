/**
  ******************************************************************************
  * @file    bsp_motor.c 
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
#include "bsp_motor.h"
#include "NearAir.h"
#include "stm32f2xx.h"

#define MOTOR_TIM_CCR           TIM2->CCR4

#define MOTOR_HALT_GEAR         0
#define MOTOR_SLEEP_GEAR        67
#define MOTOR_LOW_GEAR          70  
#define MOTOR_MIDIUM_GEAR       75
#define MOTOR_HIGH_GEAR         85
#define MOTOR_FLY_GEAR          90

static mico_timer_t _motor_timer;
static uint32_t motor_now_ccr;
static uint32_t motor_target_ccr;

static void MOTOR_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    MOTOR_CLK_INIT(MOTOR_CLK, ENABLE);	
		
    GPIO_InitStructure.GPIO_Pin = MOTOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //

    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
		
    GPIO_PinAFConfig(MOTOR_PORT, MOTOR_PIN_SOURCE, GPIO_AF_TIM2);
}

/*************************************************
Function:    void TIM_Config(void)  
Description: ��ʱ�����ú���       
Input:       ��
Output:      ��                            
*************************************************/
void MOTOR_TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    TIM_DeInit(TIM4);//��ʼ��TIM3�Ĵ���

    /*��Ƶ�����ڼ��㹫ʽ��
    Prescaler = (TIMxCLK / TIMx counter clock) - 1;
    Period = (TIMx counter clock / TIM3 output clock) - 1 ;TIMx counter clock = 10000
    TIMx counter clockΪ������Ҫ��TXM�Ķ�ʱ��ʱ�� 
    */
    TIM_TimeBaseStructure.TIM_Period = 100-1; //�������ֲ��֪��TIM3Ϊ16λ�Զ�װ��
    /*��system_stm32f2xx.c�����õ�APB1 Prescaler = 4 ,��֪
    *APB1ʱ��Ϊ120M/4*2 = 60 000 000,��Ϊ���APB1��Ƶ��Ϊ1����ʱʱ��x2 
    */
    TIM_TimeBaseStructure.TIM_Prescaler = 6-1;//8400-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /*��������Ƚϣ�����ռ�ձ�Ϊ20%��PWM����*/
    TIM_OCStructInit(&TIM_OCInitStructure);  //����ȱʡֵ�����к���ֻ��TIM1��TIM8��Ч
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM1Ϊ����ռ�ձ�ģʽ��PWM2Ϊ������ģʽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; //����CCR��ռ�ձ���ֵ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //HighΪռ�ձȸ߼��ԣ���ʱռ�ձ�Ϊ20%��Low��Ϊ�����ԣ�ռ�ձ�Ϊ80%
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
		
    TIM_ARRPreloadConfig(TIM4, ENABLE); //ARR�Զ�װ��Ĭ���Ǵ򿪵ģ����Բ�����

    TIM_Cmd(TIM4, ENABLE); //ʹ��TIM3��ʱ��
    TIM_CtrlPWMOutputs(TIM4,ENABLE);
    
}

void bsp_InitMotor(void)
{
    MOTOR_GPIO_Configuration();
//    MOTOR_TIM_Config();//TIM_Config is changed in bsp_led.c   TIM2_CH4!!!!
    MOTOR_TIM_CCR = MOTOR_HALT_GEAR;
}

/*����ı�����ת��*/
/*������Ŀ��ռ�ձȣ�ȡֵ0~100*/
void Motor_Gear(uint32_t target_ccr)
{
    if(target_ccr > 100)
      return;
    motor_target_ccr = target_ccr;
    motor_now_ccr = MOTOR_TIM_CCR;	
}

void change_motor_positon(Motor_Position motor_positon)
{
//    mico_reload_timer(&_motor_timer);
//    mico_start_timer(&_motor_timer);
    switch(motor_positon)
    {
        case MOTOR_HALT:
        {
            Motor_Gear(MOTOR_HALT_GEAR);
        }
        break;
        case MOTOR_SLEEP:
	{
            Motor_Gear(MOTOR_SLEEP_GEAR);
        }
        break;
        case MOTOR_LOW:
        {
            Motor_Gear(MOTOR_LOW_GEAR);
        }
        break;
        case MOTOR_MIDIUM:
        {
            Motor_Gear(MOTOR_MIDIUM_GEAR);
        }
        break;
        case MOTOR_HIGH:
        {
            Motor_Gear(MOTOR_HIGH_GEAR);
        }
        break;
        case MOTOR_FLY:
        {
            Motor_Gear(MOTOR_FLY_GEAR);
        }
        break;
        default:
        break;
    }
}

void _motor_tick(void *inContext)
{
    (void)(inContext);
    if(motor_now_ccr < motor_target_ccr)
    {
        MOTOR_TIM_CCR = ++motor_now_ccr;
    }
    if(motor_now_ccr > motor_target_ccr)
    {
        MOTOR_TIM_CCR = --motor_now_ccr;
    }
    if(motor_now_ccr == motor_target_ccr)
    {
//        mico_stop_timer(&_motor_timer);
    }
}

void motor_thread(void *inContext)
{
    nearair_Context_t *airContext = inContext;
    char mode_temp;
    char position_temp;
    mode_temp = airContext->deviceStatus.mode;
    position_temp = airContext->deviceStatus.position;
    bsp_InitMotor();
    
    mico_init_timer(&_motor_timer,50,_motor_tick,airContext);
    mico_start_timer(&_motor_timer);
    
    while(1)
    {
        if(airContext->setStatus.motor_enable)
        {
            Motor_Gear(airContext->setStatus.motor);
            continue;
        }
        if(mode_temp != airContext->deviceStatus.mode)
        {
            mode_temp = airContext->deviceStatus.mode;
            switch( mode_temp )
            {
            case MODE_CLOSE:
              change_motor_positon( MOTOR_HALT );
              break;
            case MODE_INIT:
              change_motor_positon( MOTOR_FLY );
              break;
            case MODE_SMART:
              {
                switch( airContext->airStatus.airclass )
                {
                  case AQ_GOOD:
                    change_motor_positon( MOTOR_LOW );
                    break;
                  case AQ_SOSO:
                    change_motor_positon( MOTOR_MIDIUM );
                    break;
                  case AQ_BAD:
                    change_motor_positon( MOTOR_HIGH );
                    break;
                  default:
                    break;            
                }
              }
              break;
            case MODE_SLEEP:
              change_motor_positon( MOTOR_SLEEP );
              break;
            case MODE_FLY:
              change_motor_positon( MOTOR_FLY );
              break;
            case MODE_MANUAL:
              //change_motor_positon( );
              break;
            default:
              break;
            }
        }
        if(MODE_SMART == airContext->deviceStatus.mode)
        {
                switch( airContext->airStatus.airclass )
                {
                  case AQ_GOOD:
                    change_motor_positon( MOTOR_LOW );
                    break;
                  case AQ_SOSO:
                    change_motor_positon( MOTOR_MIDIUM );
                    break;
                  case AQ_BAD:
                    change_motor_positon( MOTOR_HIGH );
                    break;
                  default:
                    break;            
                }
                mico_thread_sleep(2);
        }
        if((MODE_MANUAL == airContext->deviceStatus.mode) && (position_temp != \
          airContext->deviceStatus.position))
        {
            position_temp = airContext->deviceStatus.position;
            switch(airContext->deviceStatus.position)
            {
            case MOTOR_LOW:
              change_motor_positon( MOTOR_LOW );
              break;
            case MOTOR_MIDIUM:
              change_motor_positon( MOTOR_MIDIUM );
              break;
            case MOTOR_HIGH:
              change_motor_positon( MOTOR_HIGH );
              break;
            case MOTOR_FLY:
              change_motor_positon( MOTOR_FLY );
              break;
            default:
              break;
            }
        }   
    }
}
