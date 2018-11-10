#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "lora_app.h"
#include "usart3.h"

/************************************************
�߼�ժҪ
    1.��ʼ��loraģ��
    2.loraģ�����ã����ù��̲����ʱ���Ϊ115200���������������Ϊ�Զ��岨���ʣ�
    3.ÿ1s����loraģ�鷢�����ݣ�ͨѶʱ������Ϊ�Զ��岨���ʣ�


************************************************/

int main(void)
{
    u8 hour=1,min=2,locx=5,locy=6,crc=0;
			//		0,	1,	 2,	  3,	   4,		5,	 6,	 7,	  8,   9,  10
    u8 sendData[]={0x80,0x07,0x11,MY_ADDRH,MY_ADDRL,hour,min,locx,locy,crc,0x81};
u8 len= sizeof(sendData)/sizeof(sendData[0]);

//	SystemInit();
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart1_init(115200);
    printf("uart1_init success!\n");
    LED_Init();

    while(LoRa_Init())  //��ʼ��ATK-LORA-01ģ��,����ʼ��ʧ����300ms�����ԣ�ֱ���ɹ�
    {
        printf("LoRa undetected...\n");
        delay_ms(300);
    }
    printf("LoRa detected!\n");
    
    LoRa_Set();     //LoRa����(�������������ô��ڲ�����Ϊ115200)

    printf("start while(1)\n");
printf("add= %d %d %d %d %d\n",MY_ADDRH,MY_ADDRL,OBJ_ADDRH,OBJ_ADDRL,len);
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
		sendData[9]++;
        LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData);            //��������
        delay_ms(1000);
    }
}
