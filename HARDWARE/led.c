#include "led.h"
#include "delay.h"

bool flag_thing_done=true;

void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC,GPIO_Pin_13);

/***********************************************************************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);

    EXTI_InitStructure.EXTI_Line=EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //中断线打开
    EXTI_Init(&EXTI_InitStructure);//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;           //LORA_AUX
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //关闭外部中断通道（后面再打开）
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0))
    {
		// u16 i,j;
		// for(i=0;i<20;i++){
		// 	j=12000;
		// 	while(j--);
		// }
        delay_ms(50);   //Button anti-shake
        if( !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) ){
            // while(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1));
            //code here:
            flag_thing_done=true;
        }            
        EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE4上的中断标志位
    }
}



