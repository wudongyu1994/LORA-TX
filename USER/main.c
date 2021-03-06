#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "lora_app.h"
#include "usart2.h"

/************************************************
逻辑摘要
    1.初始化lora模块
    2.lora模块设置（设置过程波特率必须为115200，设置完后波特率设为自定义波特率）
    3.每1s，给lora模块发送数据（通讯时波特率为自定义波特率）


************************************************/

int main(void)
{
	u8 i;
    u8 hour=1,min=2,locx=0,locy=2,crc=0;
    u8 data_rev[1024];
			//		      0,	1,	 2,	  3,	   4,		5,	 6,	 7,	  8,   9,  10
    u8 sendData[]=      {0x80,0x07,0x11,MY_ADDRH,MY_ADDRL,hour,min,locx,locy,crc,0x81};

    u8 len= sizeof(sendData)/sizeof(sendData[0]);

//	SystemInit();
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart1_init(115200);
    printf("uart1_init success!\n");
    LED_Init();
    TIM3_Int_Init(4999,7999);
    lora_atk_init();

    printf("start while(1)\n");
    while(1)
    {
        /*if(flag_thing_done){
            //获取两个RFID的坐标信息
            locx=getLocx();
            locy=getLocy();
            sendData[7]=locx;
            sendData[8]=locy;
            LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData);            //发送数据
        }*/

        if(USART2_RX_STA&0X8000){
            printf("data receive= ");
            for(i=0;i<(USART2_RX_STA&0x7fff);i++){
                printf("%x ",USART2_RX_BUF[i]);
                data_rev[i]=USART2_RX_BUF[i];
            }
            printf("\n");
            if(data_rev[0]==0x80 && data_rev[1]==0x01 && data_rev[2]==0x50 && data_rev[3]==0x51 && data_rev[4]==0x81){
                TIM_Cmd(TIM3,DISABLE);
            }
            USART2_RX_STA=0;
        }
        if(flag_thing_done){
            flag_thing_done=false;
            // sendData[8]++;
            for(i=1,crc=0; i<9; i++)
               crc+=sendData[i];
            sendData[9]=crc;
            LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData,len);
            TIM_SetCounter(TIM3,0); //clear counter
            TIM_Cmd(TIM3, ENABLE);  //start 5s countdown
        }

		/*sendData[9]++;
        LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData);            //发送数据
        delay_ms(1000);*/
        // delay_ms(500);
        // printf("500ms passed\n");
    }
}
