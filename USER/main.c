#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "lora_app.h"
#include "usart2.h"

/************************************************
�߼�ժҪ
    1.��ʼ��loraģ��
    2.loraģ�����ã����ù��̲����ʱ���Ϊ115200���������������Ϊ�Զ��岨���ʣ�
    3.ÿ1s����loraģ�鷢�����ݣ�ͨѶʱ������Ϊ�Զ��岨���ʣ�


************************************************/

int main(void)
{
	u8 i;
    u8 hour=1,min=2,locx=5,locy=6,crc=0;
    u8 data_rev[1024];
			//		      0,	1,	 2,	  3,	   4,		5,	 6,	 7,	  8,   9,  10
    u8 sendData[]=      {0x80,0x07,0x11,MY_ADDRH,MY_ADDRL,hour,min,locx,locy,crc,0x81};
    u8 test[]={1,2,3,4,5};
    // u8 sendData_old[]=  {0x80,0x07,0x11,MY_ADDRH,MY_ADDRL,hour,min,locx,locy,crc,0x81};

    u8 len= sizeof(sendData)/sizeof(sendData[0]);

//	SystemInit();
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart1_init(115200);
    printf("uart1_init success!\n");
    LED_Init();
    TIM3_Int_Init(4999,7999);

    while(LoRa_Init())  //��ʼ��ATK-LORA-01ģ��,����ʼ��ʧ����300ms�����ԣ�ֱ���ɹ�
    {
        printf("LoRa undetected...\n");
        delay_ms(300);
    }
    printf("LoRa detected!\n");
    
    LoRa_Set();     //LoRa����(�������������ô��ڲ�����Ϊ115200)
    LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,test,5);
    printf("start while(1)\n");
printf("add= %x %x %x %x %x\n",MY_ADDRH,MY_ADDRL,OBJ_ADDRH,OBJ_ADDRL,len);
    while(1)
    {
        /*if(flag_thing_done){
            //��ȡ����RFID��������Ϣ
            locx=getLocx();
            locy=getLocy();
            sendData[7]=locx;
            sendData[8]=locy;
            LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData);            //��������
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
            sendData[8]++;
            for(i=1,crc=0; i<9; i++)
               crc+=sendData[i];
            sendData[9]=crc;
            LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData,len);
            TIM_SetCounter(TIM3,0); //clear counter
            TIM_Cmd(TIM3, ENABLE);  //start 5s countdown
        }

		/*sendData[9]++;
        LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData);            //��������
        delay_ms(1000);*/
        // delay_ms(500);
        // printf("500ms passed\n");
    }
}
