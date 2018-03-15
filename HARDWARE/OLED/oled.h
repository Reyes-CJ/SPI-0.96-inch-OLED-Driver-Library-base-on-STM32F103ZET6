#ifndef __OLED_H__
#define __OLED_H__
#include "stm32f10x.h"
/*
	OLED_SCL - PE7
	OLED_SDIN - PE8
	OLED_RST - PE9
	OLED_DC - PE10
	OLED_CS - GND
*/
void OLED_GPIO_Config(void);
void OLED_WriteCmd(u8 cmd);
void OLED_WriteData(u8 dat);	//дһ�ֽ�����
void OLED_WriteData2(u8 dat);	//��������
void OLED_DisON(void);
void OLED_DisOFF(void);
void OLED_SetCursor(u8 x, u8 y);
void OLED_Clear(void);		//ȫ���Ǻ�ɫ��.û�е���
void OLED_SetContrast(u8 con);
void Display_LOGO(void);
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMPx[]);
void OLED_Set_Pos(u8 x, u8 y);	 //������ʾ��������ʾ����
void OLED_DisChinese(u8 x, u8 y, u8 Num);
void OLED_Init(void);


void OLED_DisChar_F8X16(u8 x, u8 y, u8 ch);
void OLED_CharInverse_F8X16(u8 x, u8 y, u8 ch);
void OLED_CharBlink_F8X16(u8 x, u8 y, u8 ch);
void OLED_DisNumber_F8X16(u8 x, u8 y, u8 num);
void OLED_NumberInverse_F8X16(u8 x, u8 y, u8 num);
void OLED_NumberBlink_F8X16(u8 x, u8 y, u8 num);
void OLED_DisString_F8X16(u8 x, u8 y, u8 *str);
void OLED_DisBlack_F8X16(u8 x, u8 y);		//ָ��λ����ʾ�ַ��ڿ�
void OLED_DisWhite_F8X16(u8 x, u8 y);		//ָ��λ����ʾ�ַ��׿�


void OLED_DisChar_F6X8(u8 x, u8 y, u8 ch);
void OLED_DisString_F6X8(u8 x, u8 y, u8 *str);
void OLED_DisNumber_F6X8(u8 x, u8 y, u8 num);




#endif


