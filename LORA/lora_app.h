#ifndef _LORA_APP_H_
#define _LORA_APP_H_

#include "sys.h"
#include "lora_cfg.h"

#define LORA_AUX  PAin(4)    //LORAģ��״̬����
#define LORA_MD0  PAout(5)  //LORAģ���������

extern _LoRa_CFG LoRa_CFG;
extern u8 Lora_mode;

#define MY_ADDRH	0X00			//��¼�Լ��ĵ�ַ
#define MY_ADDRL	0X01
#define MY_ADDR 	((u16)(MY_ADDRH<<8))+MY_ADDRL
#define OBJ_ADDRH  	0xee     		//��¼Ŀ���ַ
#define OBJ_ADDRL  	0xee     		
#define OBJ_ADDR 	((u16)(OBJ_ADDRH<<8))+OBJ_ADDRL
#define OBJ_CHN 	LORA_CHN        //��¼Ŀ���ŵ�

u8 LoRa_Init(void);
void Aux_Int(u8 mode);
void LoRa_Set(void);
void LoRa_SendData(u8 addh,u8 addl,u8 chn,u8* Dire_Date,u8 Dire_DateLen);
void LoRa_ReceData(void);
void LoRa_Process(void);
void Lora_Test(void);
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* lora_check_cmd(u8 *str);

#endif
