#include "graphic.h"
#include "oled.h"
#include "QR_Encode.h"      //��ά����ʾ��

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];

//��ͼ����,���½�Ϊԭ������,����x,����y

void OLED_RefreshGRAM(void)		//ˢ��������Ļ12864,�ɸ�����Ҫ����
{
    u8 i, n;
    for(i = 0; i < 8; i++)		//ҳ 0-7
    {
        OLED_WriteCmd(0xb0 + i); //����ҳ��ַ��0~7��
        OLED_WriteCmd(0x00);     //������ʾλ�á��е͵�ַ
        OLED_WriteCmd(0x10);     //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)
            OLED_WriteData(OLED_GRAM[n][i]);
    }
}
//����
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x, u8 y)
{
    u8 pos, bx, temp = 0;
    if(x > 127 || y > 63)return; //������Χ��.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if(1)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}
//���㣬��ά��
void OLED_DrawPoint_QRcode(u8 x, u8 y, u8 dot)
{
    u8 pos, bx, temp = 0;
    if(x > 127 || y > 63)return; //������Χ��.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if(dot)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2,x1<=127 ,0<=y1<=63
//dot:0,���;1,���
void OLED_FillArea(u8 x1, u8 y1, u8 x2, u8 y2)
{
    u8 x, y;
    for(x = x1; x <= x2; x++)
    {
        for(y = y1; y <= y2; y++)
            OLED_DrawPoint(x, y);
    }
}
//��ֱ��
//�������,�յ�����,ע�����½�Ϊԭ��
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if(delta_x > 0)
        incx = 1; 			//���õ�������
    else if(delta_x == 0)
        incx = 0; //��ֱ��
    else			//С��0
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)
        incy = 1;
    else if(delta_y == 0)
        incy = 0; //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if( delta_x > delta_y)
        distance = delta_x; //ѡȡ��������������
    else
        distance = delta_y;

    for(t = 0; t <= distance + 1; t++ ) //�������
    {
        OLED_DrawPoint(uRow, uCol); //����
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

//������
//(x1,y1)���½�����,(x2,y2)���Ͻ�����
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    //������ֱ��
    OLED_DrawLine(x1, y1, x2, y1);
    OLED_DrawLine(x1, y1, x1, y2);
    OLED_DrawLine(x1, y2, x2, y2);
    OLED_DrawLine(x2, y1, x2, y2);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
//ʹ��Bresenham�㷨��Բ
void OLED_DrawCircle(u16 x0, u16 y0, u8 r)
{
    OLED_DrawArc(x0, y0, r, 1);
    OLED_DrawArc(x0, y0, r, 2);
    OLED_DrawArc(x0, y0, r, 3);
    OLED_DrawArc(x0, y0, r, 4);
   
    /*
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);         //�ж��¸���λ�õı�־
    while(a <= b)
    {
        OLED_DrawPoint(x0 + a, y0 - b);       //5
        OLED_DrawPoint(x0 + b, y0 - a);       //0
        OLED_DrawPoint(x0 + b, y0 + a);       //4
        OLED_DrawPoint(x0 + a, y0 + b);       //6
        OLED_DrawPoint(x0 - a, y0 + b);
        OLED_DrawPoint(x0 - b, y0 + a);
        OLED_DrawPoint(x0 - a, y0 - b);
        OLED_DrawPoint(x0 - b, y0 - a);
        a++;
        //ʹ��Bresenham�㷨��Բ
        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
    */
}


//�������Σ����������
void OLED_DrawTriangle(u8 x0,u8 y0,u8 x1,u8 y1, u8 x2,u8 y2)
{
    OLED_DrawLine(x0, y0, x1, y1);
    OLED_DrawLine(x1, y1, x2, y2);
    OLED_DrawLine(x2, y2, x0, y0);    
}

//����ֱ���߶Σ����ͳ���,����Ϊ�������»�
void OLED_DrawVLine(u8 x0, u8 y0,int length)
{
    OLED_DrawLine(x0,y0,x0,y0+length);   
}
//��ˮƽ���߶�
void OLED_DrawHLine(u8 x0, u8 y0,int length)
{
    OLED_DrawLine(x0,y0,x0+length,y0);   
}
//��1/4Բ����Բ�ģ��뾶��λ�ã���������1234
void OLED_DrawArc(u16 x0, u16 y0, u8 r, u8 part)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);         //�ж��¸���λ�õı�־
    while(a <= b)
    {
        switch(part)
        {
            case 1:
                OLED_DrawPoint(x0 + b, y0 + a);   
                OLED_DrawPoint(x0 + a, y0 + b);      
                break;
            case 2:
                OLED_DrawPoint(x0 - a, y0 + b);
                OLED_DrawPoint(x0 - b, y0 + a);
                break;
            case 3:
                OLED_DrawPoint(x0 - a, y0 - b);
                OLED_DrawPoint(x0 - b, y0 - a);
                break;
            case 4:
                OLED_DrawPoint(x0 + a, y0 - b);    
                OLED_DrawPoint(x0 + b, y0 - a);  
                break;
            default:
                break;
        }
        a++;
        //ʹ��Bresenham�㷨��Բ
        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

//��Բ�Ǿ��Σ������Բ�ǰ뾶
void OLED_DrawRRectangle(u8 x0, u8 y0, u8 x1, u8 y1, u8 R)
{
    u8 L,W;
    L = x1 - x0 - 2 * R;
    W = y1 - y0 - 2 * R;
    OLED_DrawHLine(x0 + R, y0, L);
    OLED_DrawHLine(x0 + R, y1, L);
    OLED_DrawVLine(x0, y0 + R, W);
    OLED_DrawVLine(x1, y0 + R, W);
    
    OLED_DrawArc(x1 - R, y1 - R, R, 1);
    OLED_DrawArc(x0 + R, y1 - R, R, 2);
    OLED_DrawArc(x0 + R, y0 + R, R, 3);
    OLED_DrawArc(x1 - R, y0 + R, R, 4);
}
//��ʾ��ά�룬��վ���Ȳ���̫������ά��ֻ֧��֧����ɨ��
void OLED_DrawQRCode(char *website)
{
//  MAX_MODULESIZE = 33;
//  m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE];
    u8 i,j;
    EncodeData(website);
    for(i = 0; i < MAX_MODULESIZE; i++)
    {
        for(j = 0; j < MAX_MODULESIZE; j++)
        {
            if(m_byModuleData[i][j] == 1)
            {
            //2���Ŵ�1����Ŵ�Ϊ4����
                OLED_DrawPoint(2*i, 2*j);
                OLED_DrawPoint(2*i, 2*j+1);        
                OLED_DrawPoint(2*i+1, 2*j);  
                OLED_DrawPoint(2*i+1, 2*j+1);
//            OLED_DrawPoint_QRcode(2*i, 2*j, m_byModuleData[i][j]);
//            OLED_DrawPoint_QRcode(2*i, 2*j+1, m_byModuleData[i][j]);        
//            OLED_DrawPoint_QRcode(2*i+1, 2*j, m_byModuleData[i][j]);  
//            OLED_DrawPoint_QRcode(2*i+1, 2*j+1, m_byModuleData[i][j]);
            }   
        }
    }    
}
    

