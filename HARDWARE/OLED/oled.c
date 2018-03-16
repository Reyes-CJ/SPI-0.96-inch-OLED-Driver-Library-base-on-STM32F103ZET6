#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "oledfont.h"
#include "graphic.h"

/*
	OLED_SCL - PE7
	OLED_SDIN - PE8
	OLED_RST - PE9
	OLED_DC - PE10
	OLED_CS - GND
*/

//Ĭ��6x8����,��������,x��0-20��y��0-7,����ʾ��21*8=168���ַ�
//Ĭ��8x16����,����������ʾ,x��0-15��y��0-3������ʾ��16*4=64���ַ�
//�ɸ�����Ҫ�޸�Ϊ����������ʾ��x:0-127,y:0-63
//������8*16���巴��,��˸�ַ����־���,2017��5��12��11:10:19����
#define OLED_SCL PEout(7)
#define OLED_SDA PEout(8)
#define OLED_RST PEout(9)
#define OLED_DC PEout(10)

#define OLED_DB GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10

void OLED_GPIO_Config(void)
{
    GPIO_InitTypeDef IO_Init;
	IO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	IO_Init.GPIO_Pin = OLED_DB;
	IO_Init.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOE, &IO_Init);
}

void OLED_WriteCmd(u8 cmd)
{
    u8 i = 8;
    OLED_DC = 0;
    OLED_SCL = 0;
    for(i = 0; i < 8; i++)
    {
        if(cmd & 0x80)
            OLED_SDA = 1;
        else
            OLED_SDA = 0;
        OLED_SCL = 1;
        OLED_SCL = 0;
        cmd <<= 1;;
    }
}

void OLED_DrawP(u8 x, u8 y, u8 dot) //���㣬����δ��ɣ�δ����
{
    OLED_SetCursor(x, y);
    OLED_DC = 1;
    OLED_SCL = 0;
    if(dot)
        OLED_SDA = 1;
    else
        OLED_SDA = 0;
    OLED_SCL = 1;
    OLED_SCL = 0;
}

void OLED_WriteData(u8 dat)		//дһ�ֽ�����
{
    u8 i = 8;
    OLED_DC = 1;
    OLED_SCL = 0;
    for(i = 0; i < 8; i++)
    {
        if(dat & 0x80)
            OLED_SDA = 1;
        else
            OLED_SDA = 0;
        OLED_SCL = 1;
        OLED_SCL = 0;
        dat <<= 1;
    }
}

void OLED_WriteData2(u8 dat)	//��������
{  
    u8 i = 8;
    OLED_DC = 1;
    OLED_SCL = 0;
    for(i = 0; i < 8; i++)
    {
        if(dat & 0x80)
            OLED_SDA = 0;
        else
            OLED_SDA = 1;
        OLED_SCL = 1;
        OLED_SCL = 0;
        dat <<= 1;
    }
}

void OLED_DisON(void)
{
    OLED_WriteCmd(0x8d);		//��ɱ�����
    OLED_WriteCmd(0x14);		//������ɱ�
    OLED_WriteCmd(0xaf);		//��ʾ��
}

void OLED_DisOFF(void)
{
    OLED_WriteCmd(0x8d);		//��ɱ�����
    OLED_WriteCmd(0x10);		//�رյ�ɱ�
    OLED_WriteCmd(0xae);		//��ʾ��
}

void OLED_SetCursor(u8 x, u8 y)
{
    OLED_WriteCmd(0xb0 + (y >> 3));
//    OLED_WriteCmd(0xb0 + y);
    OLED_WriteCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Clear(void)		//ȫ���Ǻ�ɫ��.û�е���
{
    u8 i, n;
    for(i = 0; i < 8; i++)
    {
        OLED_WriteCmd (0xb0 + i); //����ҳ��ַ��0~7��
        OLED_WriteCmd (0x00);     //������ʾλ�á��е͵�ַ
        OLED_WriteCmd (0x10);     //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)	//ÿ��128����
            OLED_WriteData(0);		//Ϩ�����ص�
    }
}
//���öԱȶ�
void OLED_SetContrast(u8 con)
{
    OLED_WriteCmd(0x81);	 //�Աȶ�����
    OLED_WriteCmd(con); 	 //1~255;Ĭ��0X7F (��������,Խ��Խ��)
}



void OLED_Init(void)
{
    OLED_RST = 1;
    delay_ms(100);
    OLED_RST = 0;
    delay_ms(100);
    OLED_RST = 1;
    /*�ٷ����룬��ʾ����*/
	/*
//	  OLED_WriteCmd(0xAE); //--turn off oled panel
//    OLED_WriteCmd(0x00); //---set low column address
//    OLED_WriteCmd(0x10); //---set high column address
//    OLED_WriteCmd(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//    OLED_WriteCmd(0x81); //���öԱȶ�
//    OLED_WriteCmd(0xCF); // Set SEG Output Current Brightness
//    OLED_WriteCmd(0xA1); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
//    OLED_WriteCmd(0xC8); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
//    OLED_WriteCmd(0xA6); //--set normal display
//    OLED_WriteCmd(0xA8); //--set multiplex ratio(1 to 64)
//    OLED_WriteCmd(0x3f); //--1/64 duty
//    OLED_WriteCmd(0xD3); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//    OLED_WriteCmd(0x00); //-not offset
//    OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
//    OLED_WriteCmd(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
//    OLED_WriteCmd(0xD9); //--set pre-charge period
//    OLED_WriteCmd(0xF1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//    OLED_WriteCmd(0xDA); //--set com pins hardware configuration
//    OLED_WriteCmd(0x12);
//    OLED_WriteCmd(0xDB); //--set vcomh
//    OLED_WriteCmd(0x40); //Set VCOM Deselect Level
//    OLED_WriteCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
//    OLED_WriteCmd(0x02); //
//    OLED_WriteCmd(0x8D); //--set Charge Pump enable/disable
//    OLED_WriteCmd(0x14); //--set(0x10) disable
//    OLED_WriteCmd(0xA4); // Disable Entire Display On (0xa4/0xa5)
//    OLED_WriteCmd(0xA6); // Disable Inverse Display On (0xa6/a7)
//    OLED_WriteCmd(0xAF); //--turn on oled panel
//    OLED_WriteCmd(0xAF); //display ON


*/
    /*ԭ����Ƶ����,��ʾ����*/
    OLED_WriteCmd(0xAE); //�ر���ʾ
    OLED_WriteCmd(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
    OLED_WriteCmd(0x80);  //[3:0],��Ƶ����;[7:4],��Ƶ��
    OLED_WriteCmd(0xA8); //��������·��
    OLED_WriteCmd(0X3F); //Ĭ��0X3F(1/64)
    OLED_WriteCmd(0xD3); //������ʾƫ��
    OLED_WriteCmd(0X00); //Ĭ��Ϊ0
    OLED_WriteCmd(0x40); //������ʾ��ʼ�� [5:0],����.
    OLED_WriteCmd(0x8D); //��ɱ�����
    OLED_WriteCmd(0x14); //bit2������/�ر�
    OLED_WriteCmd(0x20); //�����ڴ��ַģʽ
    OLED_WriteCmd(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    OLED_WriteCmd(0xA1); //0xa0���ҷ��� 0xa1����
    OLED_WriteCmd(0xC8); //0xc0���·��� 0xc8����
    OLED_WriteCmd(0xDA); //����COMӲ����������
    OLED_WriteCmd(0x12); //[5:4]����
    OLED_WriteCmd(0x81); //�Աȶ�����
    OLED_WriteCmd(0x7f); //1~255;Ĭ��0X7F--------(��������,Խ��Խ��)
    OLED_WriteCmd(0xD9); //����Ԥ�������
    OLED_WriteCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WriteCmd(0xDB); //����VCOMH ��ѹ����
    OLED_WriteCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WriteCmd(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    OLED_WriteCmd(0xA6); //������ʾ��ʽ;bit0:1-0xa7������ʾ;0xa6������ʾ
    OLED_WriteCmd(0xAF); //������ʾ

    OLED_Clear();
    OLED_SetCursor(0, 0);
}


void Display_LOGO(void)
{
    u8 n;
    OLED_DrawBMP(0, 0, 128, 8, IPHONE);
	
    OLED_DisString_F8X16(0, 3, "Starting...");
    for( n = 5; n > 0; n--)
    {
        OLED_DisChar_F8X16(12, 3, n + 0x30);
        delay_ms(800);
    }
    OLED_Clear();
    delay_ms(800);
    OLED_DrawBMP(0, 0, 128, 2, BAR);
    delay_ms(1000);
}

//��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMPx[])
{
    u16 j = 0;
    u8 x, y;

    if(y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for(y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for(x = x0; x < x1; x++)
        {
            OLED_WriteData(BMPx[j++]);
        }
    }
}
void OLED_Set_Pos(u8 x, u8 y)	 //������ʾ��������ʾ����
{
    OLED_WriteCmd(0xb0 + y);
    OLED_WriteCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WriteCmd((x & 0x0f) | 0x01);
}
void OLED_DisChinese(u8 x, u8 y, u8 Num)
{
    u8 i;
    OLED_Set_Pos(x, y);
    for(i = 0; i < 16; i++)
        OLED_WriteData(CHN[2 * Num][i]);		//���ֱ���ĵ�һ�У�
    OLED_Set_Pos(x, y + 1);
    for(i = 0; i < 16; i++)
        OLED_WriteData(CHN[2 * Num + 1][i]);	//���ֱ���ĵڶ���
}


#if 1		//8x16����,����������ʾ,x��0-15��y��0-3

//��ʾ�ַ�,8*16����,x��0-15��y��0-3
void OLED_DisChar_F8X16(u8 x, u8 y, u8 ch)
{
    u8 c, i;
    c = ch - ' ';
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)					//��ʾ�ϰ벿��8*8
        OLED_WriteData(F8X16[c][i]);		//ÿһ�е�ǰ8��
    OLED_SetCursor(x * 8, y * 16 + 8);			//��ʾ�°벿��8*8
    for(i = 0; i < 8; i++)
        OLED_WriteData(F8X16[c][i + 8]);	//ÿһ�еĺ�8��
}
//��ʾ8*16�ַ�����,x��0-15��y��0-3
void OLED_CharInverse_F8X16(u8 x, u8 y, u8 ch)
{
    u8 c, i;
    c = ch - ' ';
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(F8X16[c][i]);
    OLED_SetCursor(x * 8, y * 16 + 8);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(F8X16[c][i + 8]);
}
//��ʾ8x16�ַ���˸
void OLED_CharBlink_F8X16(u8 x, u8 y, u8 ch)
{
    OLED_CharInverse_F8X16(x, y, ch);
    delay_ms(400);
    OLED_DisChar_F8X16(x, y, ch);
    delay_ms(400);
}
//��ʾ��λ���֣�8X16���壬x��0-14��y��0-3
void OLED_DisNumber_F8X16(u8 x, u8 y, u8 num)
{
    //�����ַ�������ʾ
    OLED_DisChar_F8X16(x, y, num / 10 + 0x30);
    OLED_DisChar_F8X16(x + 1, y, num % 10 + 0x30);
}
//��ʾ��λ���ַ���,8*16����,x��0-14��y��0-3
void OLED_NumberInverse_F8X16(u8 x, u8 y, u8 num)
{
    OLED_CharInverse_F8X16(x, y, num / 10 + 0x30);
    OLED_CharInverse_F8X16(x + 1, y, num % 10 + 0x30);
}
//��ʾ��λ������˸:�����ͷ��Խ���
void OLED_NumberBlink_F8X16(u8 x, u8 y, u8 num)
{
    OLED_NumberInverse_F8X16(x, y, num);
    delay_ms(100);
    OLED_DisNumber_F8X16(x, y, num);
    delay_ms(100);
}
//��ʾ�ַ���,8X16���壬x��0-15��y��0-3
void OLED_DisString_F8X16(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F8X16(x, y, *str++);
        x++;
    }
}
//ָ��λ����ʾ8*16�ڿ��ַ�
void OLED_DisBlack_F8X16(u8 x, u8 y)		//ָ��λ����ʾ�ַ��ڿ�
{
    u8 i;
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)
        OLED_WriteData(0);			//Ϩ�����ص�
    OLED_SetCursor(x * 8, y * 16 + 8);
    for(i = 0; i < 8; i++)
        OLED_WriteData(0);
}
//ָ��λ����ʾ8*16�׿��ַ�
void OLED_DisWhite_F8X16(u8 x, u8 y)		//ָ��λ����ʾ�ַ��׿�
{
    u8 i;
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(0);			//�������ص�
    OLED_SetCursor(x * 8, y * 16 + 8);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(0);
}
#endif


#if 0		//8x16����,����������ʾ,x��0-127��y��0-63

/*��ʾ�ַ�,8X16����,x��0-127��y��0-63*/
void OLED_DisChar_F8X16(u8 x, u8 y, u8 ch)
{
    u8 c, i;
    c = ch - ' ';		//��ַƫ��
    OLED_SetCursor(x, y);
    for(i = 0; i < 8; i++)		//��ʾ�ϰ벿��8*8
        OLED_WriteData(F8X16[c][i]);			//ÿһ�е�ǰ8��
    OLED_SetCursor(x, y + 8);		//��ʾ�°벿��8*8
    for(i = 0; i < 8; i++)
        OLED_WriteData(F8X16[c][i + 8]);		//ÿһ�еĺ�8��
}
/*��ʾ�ַ���,8X16���壬x��0-15��y��0-3*/
void OLED_DisString_F8X16(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F8X16(x, y, *str++);
        x += 8;
    }
}
/*��ʾ��λ���֣�8X16���壬x��0-14��y��0-3*/
void OLED_DisNumber_F8X16(u8 x, u8 y, u8 num)
{
    //�����ַ�������ʾ
    OLED_DisChar_F8X16(x, y, num / 10 + 0x30);
    OLED_DisChar_F8X16(x + 8, y, num % 10 + 0x30);
}

#endif





#if 1		//6x8����,��������,x��0-20��y��0-7

/*��ʾ�ַ���6X8���壬x��0-20��y��0-7*/
void OLED_DisChar_F6X8(u8 x, u8 y, u8 ch)
{
    u8 c = 0, i = 0;
    c = ch - ' ';
    OLED_SetCursor(x * 6, y * 8);
    for(i = 0; i < 6; i++)
        OLED_WriteData(F6X8[c][i]);
}
/*��ʾ�ַ�����6X8����,���Ȳ��ܳ���21���ַ�*/
void OLED_DisString_F6X8(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F6X8(x, y, *str++);
        x++;
    }
}
/*��ʾ��λ����,6X8����,x��0-19��y:0-7*/
void OLED_DisNumber_F6X8(u8 x, u8 y, u8 num)
{
    //�����ַ�������ʾ
    OLED_DisChar_F6X8(x, y, num / 10 + 0x30);
    OLED_DisChar_F6X8(x + 1, y, num % 10 + 0x30);
}

#endif

#if 0		//6x8����,����������ʾ,x��0-127��y��0-63

/*��ʾ�ַ���6X8���壬x��0-127��y��0-36*/
void OLED_DisChar_F6X8(u8 x, u8 y, u8 ch)
{
    u8 c = 0, i = 0;
    c = ch - ' ';
    OLED_SetCursor(x, y);
    for(i = 0; i < 6; i++)
        OLED_WriteData(F6X8[c][i]);
}
/*��ʾ�ַ�����6X8����,���Ȳ��ܳ���21���ַ�*/
void OLED_DisString_F6X8(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F6X8(x, y, *str++);
        x += 6;
    }
}
/*��ʾ��λ����,6X8����,x��0-19��y:0-7*/
void OLED_DisNumber_F6X8(u8 x, u8 y, u8 num)
{
    //�����ַ�������ʾ
    OLED_DisChar_F6X8(x, y, num / 10 + 0x30);
    OLED_DisChar_F6X8(x + 6, y, num % 10 + 0x30);
}

#endif

