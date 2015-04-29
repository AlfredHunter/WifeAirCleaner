/**
  ******************************************************************************
  * @file    bsp_sensor_mhtrd06.c
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
#include "bsp_sensor_mhtrd06.h"

#include "PlatformUART.h"

#define mhtrd_log(M, ...) custom_log("MHTRD", M, ##__VA_ARGS__)
#define mhtrd_log_trace() custom_log_trace("MHTRD")

uint16_t  U8FLAG;
uint8_t  U8temp;
uint8_t  temperature_value,humidity_value;

static uint8_t READ_MHTRD_DATA(void)    {if (GPIO_ReadInputDataBit(MHTRD_PORT, MHTRD_PIN) == Bit_SET) return 1; return 0;}
void MHTRD_TIM_Config(void);

void bsp_sensor_init_mhtrd06(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
		
    RCC_AHB1PeriphClockCmd(MHTRD_CLK,ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = MHTRD_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_OD;//����������裬����Ϊ��©������������ͨ��
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //
    GPIO_Init(MHTRD_PORT, &GPIO_InitStructure);
    
    MHTRD_TIM_Config();
}
void MHTRD_TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    TIM_DeInit(TIM7);//��ʼ��TIM7�Ĵ���

    /*��Ƶ�����ڼ��㹫ʽ��
    Prescaler = (TIMxCLK / TIMx counter clock) - 1;
    Period = (TIMx counter clock / TIM7 output clock) - 1 ;TIMx counter clock = 10000
    TIMx counter clockΪ������Ҫ��TXM�Ķ�ʱ��ʱ�� 
    */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //�������ֲ��֪��TIM7Ϊ16λ�Զ�װ��
    /*��system_stm32f2xx.c�����õ�APB1 Prescaler = 4 ,��֪
    *APB1ʱ��Ϊ120M/4*2 = 60 000 000,��Ϊ���APB1��Ƶ��Ϊ1����ʱʱ��x2 
    */
    TIM_TimeBaseStructure.TIM_Prescaler = 6-1;//8400-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    
 //   TIM_Cmd(TIM7, ENABLE); //ʹ��TIM7��ʱ��

}

void delay_us(uint16_t us)
{
    TIM_Cmd(TIM7, ENABLE); 
    while(TIM7->CNT < 10*us);
    TIM_Cmd(TIM7, DISABLE);
    TIM7->CNT = 0;
}
static uint8_t  gather_th_U8data(void)
{
    uint8_t i;
    uint8_t  U8comdata;
    for(i=0;i<8;i++)
    {
        U8FLAG = 2;	
        while(!READ_MHTRD_DATA() && U8FLAG++){};
        delay_us(700);
        U8temp=0;
        if(READ_MHTRD_DATA())
            U8temp = 1;
        U8FLAG = 2;
        while(READ_MHTRD_DATA() && U8FLAG++);
        //��ʱ������forѭ��		  
        if(U8FLAG == 1)
            break;

        //�ж�����λ��0����1	 
				 
        // ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
        U8comdata <<= 1;
        U8comdata |= U8temp;        //0
    }//rof
    return U8comdata;
}
/*--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H_temp------
//----�¶ȵ�8λ== U8T_data_L_temp------
//----ʪ�ȸ�8λ== U8H_data_H_temp-----
//----ʪ�ȵ�8λ== U8H_data_L_temp-----
//----У�� 8λ == U8checkdata_temp-----
//--------------------------------*/

void monitor_temperature_humidity(void)
{
    uint8_t  U8T_data_H_temp,U8T_data_L_temp,U8H_data_temp,U8checkdata_temp;   
    uint8_t  humidity_temp,temperature_temp;
    //��������18ms 
    MHTRD_DATA_IO(0);
//    msleep(4);
    delay_us(5000);
    MHTRD_DATA_IO(1);
    //������������������ ������ʱ20us
//    msleep(3);
    delay_us(500);
    while(READ_MHTRD_DATA()) {;}
    delay_us(50);
    //������Ϊ���� �жϴӻ���Ӧ�ź� 
//		DHT11_DATA_IO(1);
    //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
    if(!READ_MHTRD_DATA())		 //T !	  
    {
        U8FLAG=2;
        //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
        while(!READ_MHTRD_DATA() && U8FLAG++){};
        U8FLAG=2;
        //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
        while(READ_MHTRD_DATA() && U8FLAG++);
        //���ݽ���״̬		 
				
        U8H_data_temp = gather_th_U8data();
					
        U8T_data_H_temp = gather_th_U8data();

        U8T_data_L_temp = gather_th_U8data();

        U8checkdata_temp = gather_th_U8data();

        //MHTRD_DATA_IO(1);
        //����У�� 

        U8temp = (U8T_data_H_temp + U8T_data_L_temp + U8H_data_temp);
        if(U8temp == U8checkdata_temp)
        {
            humidity_temp = (U8H_data_temp>>4)*10+(U8H_data_temp&0x0f);
//              U8H_data_L = U8H_data_L_temp;//С����������
            temperature_temp = (U8T_data_H_temp&0x0f)*10+((U8T_data_L_temp&0xf0)>>4);
//              U8T_data_L = U8T_data_L_temp;//С����������
            if( humidity_temp  < 100 )
            {
              humidity_value = humidity_temp;
              temperature_value = temperature_temp;
            }
        }
    }
}

void _get_temperature_humidity(void *inContext)
{
    nearair_Context_t *airContext = inContext;

    monitor_temperature_humidity();
    airContext->sensorStatus.humidity = humidity_value;
    airContext->sensorStatus.temperature = temperature_value;
}
