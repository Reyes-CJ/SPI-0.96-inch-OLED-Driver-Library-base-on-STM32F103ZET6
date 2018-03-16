#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "graphic.h"      //����ͼ�κ�����
#include "QR_Encode.h"
/*
    3��SPIģʽ��������
	OLED_SCL - PE7
	OLED_SDIN - PE8
	OLED_RST - PE9
	OLED_DC - PE10
	OLED_CS - GND
*/
int main(void)
{
    extern u8 BAR[];
    extern u8 STMMCU[];
    extern u8 RENESAS[];

    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    LED_Init();			     //LED�˿ڳ�ʼ��
    KEY_Init();
    OLED_GPIO_Config();
    OLED_Init();
//    OLED_WriteCmd(0xa7);    //������ʾ

    while(1)
    {
//        8x16������ʾ����
//        OLED_DisChar_F8X16(0,0,'A');
//        OLED_CharInverse_F8X16(0,1,'Z');
//        OLED_CharBlink_F8X16(1,0,'Z');
//        OLED_DisNumber_F8X16(0,2,99);
//        OLED_NumberInverse_F8X16(2,2,23);
//        OLED_NumberBlink_F8X16(2,0,34);
//        OLED_DisString_F8X16(0,3,"HELLO OLED!");
//        OLED_DisWhite_F8X16(2,1);

//        ͼƬ��ʾ����
//        OLED_DrawBMP(0,0,128,2,BAR);
//        OLED_DrawBMP(0,0,128,8,RENESAS);

//      6x8 �������
//        OLED_DisChar_F6X8(0,0,'Z');
//        OLED_DisNumber_F6X8(0,2,99);
//        OLED_DisString_F6X8(0,4,"HELLO STM32!");

//        �����������
//        Display_LOGO();     //ƻ��ͼ�꣬��������ʱ�����ֻ�״̬��
//        while(1);

//      ����ͼ�β���
//        OLED_DrawLine(0,0,120,50);           //��ֱ��
//        OLED_DrawRectangle(90,0,120,20);      //������
//        OLED_FillArea(0,40,30,80);         //����������
//        OLED_DrawCircle(60,35,28);        //��Բ
//        OLED_DrawTriangle(0,0,40,20,20,60);     //��������
//        OLED_DrawHLine(70,10,20);               //��ˮƽ��
//        OLED_DrawHLine(70,30,-20);
//        OLED_DrawVLine(70,20,20);           //����ֱ��
//        OLED_DrawVLine(80,40,-20);
//        OLED_DrawArc(60,35,28,3);         //��1/4Բ��
//        OLED_DrawRRectangle(50,10,120,60,10);       //��Բ�Ǿ���
//        OLED_DrawQRCode("HELLO STM32 OLED QRCODE");     //ֻ֧��֧����ɨ��
        OLED_DrawQRCode("https://github.com/whik");     //ֻ֧��֧����ɨ��
        OLED_RefreshGRAM();             //�����Դ棬�����²���ʾ
        LED0 = !LED0;
        delay_ms(400);
    }

}

