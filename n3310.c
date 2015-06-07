/*
 * ���          :  n3310.c
 *
 * ��������     :  ��� ������� ��� ������������ LCD �� Nokia 3310, � ����� ��� ��������� ������.
 *                 ���������� �� ���� ��������� ���������� Sylvain Bissonnette � Fandi Gunawan:
 *                 http://www.microsyl.com/index.php/2010/03/24/nokia-lcd-library/
 *                 http://fandigunawan.wordpress.com/2008/06/18/lcd-nokia-3310-pcd8544-driver-in-winavravr-gcc/
 *                 �������� ������� ����� ���������� � ������ ������ ������� � ������ �� Aheir:
 *                 http://radiokot.ru/articles/29/
 *
 * �����        :  Xander Gresolio <xugres@gmail.com>
 * ���-�������� :  http://we.easyelectronics.ru/profile/XANDER/
 *
 * ��������     :  GPL v3.0
 *
 * ����������   :  WinAVR, GCC for AVR platform
 *
 * �������      :
 * ������ 1.0 (06.08.2011)
 * + ������ ������
 * + ��������� ��������� ��������� ������ LCD Nokia 3310
 * + ������ ������� ������������ � ��������� ���� ��������
 * + ������� �������� �������� ��������� ���������� (���������� Windows-1251)
 * + ��������� ������� ��������� ����������� LcdCircle
 * - ���������� ������ � �������� ������������ ��������� ��� ������ ������� ���������
 * - ���������� ������ � ������� LcdSingleBar (�������� ��������� �� y)
 */

#include <avr/io.h>
#include <string.h>
#include "n3310.h"

// ��������� ��������� ������� ��������

static void LcdSend(byte data, LcdCmdData cd);

static void Delay(void);

// ���������� ����������

// ��� � ��� 84*48 ��� ��� 504 �����
static byte LcdCache[LCD_CACHE_SIZE];

// ����� �� ��������� ���� �������, � ���� �� ����� ��� ����������,
// ����� �������� ��� ������� ���� ��� ��������� ���������. �����
// ����� ���������� ��� ����� ���� ����� ��������� � ��� �������.
static int LoWaterMark;   // ������ �������
static int HiWaterMark;   // ������� �������

// ��������� ��� ������ � LcdCache[]
static int LcdCacheIdx;

// ���� ��������� ����
static byte UpdateLcd;


/*
 * ���                   :  LcdInit
 * ��������              :  ���������� ������������� ����� � SPI ��, ����������� LCD
 * ��������(�)           :  ���
 * ������������ �������� :  ���
 */
void LcdInit(void) {
    // Pull-up �� ����� ������������ � reset �������
    LCD_PORT |= _BV (LCD_RST_PIN);

    // ������������� ������ ���� ����� �� �����
    LCD_DDR |= _BV(LCD_RST_PIN) | _BV(LCD_DC_PIN) | _BV(PB1) | _BV(PB2) | _BV(SPI_MOSI_PIN) | _BV(SPI_CLK_PIN);

    // ��������������� ��������
    Delay();

    // ������� reset
    LCD_PORT &= ~(_BV(LCD_RST_PIN));
    Delay();
    LCD_PORT |= _BV (LCD_RST_PIN);

    // ���������� SPI:
    // ��� ����������, ������� ��� ������, ����� �������, CPOL->0, CPHA->0, Clk/4
    //SPCR = 0x50;

    // ��������� LCD ���������� - ������� ������� �� SCE
    // LCD_PORT |= _BV( LCD_CE_PIN );

    // ���������� ������� �������
    LcdSend(0x21, LCD_CMD); // �������� ����������� ����� ������ (LCD Extended Commands)
    LcdSend(0xC8, LCD_CMD); // ��������� ������������� (LCD Vop)
    LcdSend(0x06, LCD_CMD); // ��������� �������������� ������������ (Temp coefficent)
    LcdSend(0x13, LCD_CMD); // ��������� ������� (LCD bias mode 1:48)
    LcdSend(0x20, LCD_CMD); // �������� ����������� ����� ������ � �������������� ��������� (LCD Standard Commands,Horizontal addressing mode)
    LcdSend(0x0C, LCD_CMD); // ���������� ����� (LCD in normal mode)

    // ��������� ������� �������
    LcdClear();
    LcdUpdate();
}


/*
 * ���                   :  LcdClear
 * ��������              :  ������� �������. ����� ���������� ��������� LcdUpdate
 * ��������(�)           :  ���
 * ������������ �������� :  ���
 */
void LcdClear(void) {
//    // ������� ���� �������
//    int i;
//    for ( i = 0; i < LCD_CACHE_SIZE; i++ )
//    {
//        LcdCache[i] = 0x00;
//    }

    // ����������� �� Jakub Lasinski (March 14 2009)
    memset(LcdCache, 0x00, LCD_CACHE_SIZE);

    // ����� ���������� ������ � ������������ ��������
    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;

    // ��������� ����� ��������� ����
    UpdateLcd = TRUE;
}


/*
 * ���                   :  LcdUpdate
 * ��������              :  �������� ��� � ��� �������
 * ��������(�)           :  ���
 * ������������ �������� :  ���
 */
void LcdUpdate(void) {
    int i;

    if (LoWaterMark < 0)
        LoWaterMark = 0;
    else if (LoWaterMark >= LCD_CACHE_SIZE)
        LoWaterMark = LCD_CACHE_SIZE - 1;

    if (HiWaterMark < 0)
        HiWaterMark = 0;
    else if (HiWaterMark >= LCD_CACHE_SIZE)
        HiWaterMark = LCD_CACHE_SIZE - 1;

#ifdef CHINA_LCD  // �������� ��� ���������� �� �� ������������� ������������

    byte x, y;

    // 102 x 64 - ������ �������������� ���������� ������ ���������� ��, ��� ���
    // ������ ������ ������������ �� ������� �� ������� ����� �� 3 �������.
    // ������� ������� �������� ���� - ������� � ������ ������ y+1, � �����
    // ������� ����� (����� ���� ���� �������, �������� � ������ ������)

    x = LoWaterMark % LCD_X_RES;      // ������������� ��������� ����� x
    LcdSend(0x80 | x, LCD_CMD);     // ������������ ������ ������� LoWaterMark

    y = LoWaterMark / LCD_X_RES + 1;  // ������������� ��������� ����� y+1
    LcdSend(0x40 | y, LCD_CMD);     // ������������ ������ ������� LoWaterMark

    for (i = LoWaterMark; i <= HiWaterMark; i++) {
        // �������� ������ � ����� �������
        LcdSend(LcdCache[i], LCD_DATA);

        x++;                 // ������ ������������ ���������� x, ����� ������� ������� �� ����� ������
        if (x >= LCD_X_RES)  // ���� ����� �� ������, �� ��������� �� ��������� ������ (x=0; y++)
        {
            // ����� ������, ����� ����� ��������� ������ ����� �������������� ������,
            // �������� ���� ��������� ��������� �����, ����� ��� �������� :)
            x = 0;
            LcdSend(0x80, LCD_CMD);
            y++;
            LcdSend(0x40 | y, LCD_CMD);
        }
    }

    LcdSend(0x21, LCD_CMD);    // �������� ����������� ����� ������
    LcdSend(0x45, LCD_CMD);    // �������� �������� �� 5 �������� ����� (������������� ������� �������, �������� � ����������)
    LcdSend(0x20, LCD_CMD);    // �������� ����������� ����� ������ � �������������� ���������

#else  // �������� ��� ������������� �������

    // ������������� ��������� ����� � ������������ � LoWaterMark
    LcdSend(0x80 | (LoWaterMark % LCD_X_RES), LCD_CMD);
    LcdSend(0x40 | (LoWaterMark / LCD_X_RES), LCD_CMD);

    // ��������� ����������� ����� ������ �������
    for (i = LoWaterMark; i <= HiWaterMark; i++) {
        // ��� ������������� ������� �� ����� ������� �� ������� � ������,
        // ����� ������ ��������������� �������� ������
        LcdSend(LcdCache[i], LCD_DATA);
    }

#endif

    // ����� ���������� ������ � �������
    LoWaterMark = LCD_CACHE_SIZE - 1;
    HiWaterMark = 0;

    // ����� ����� ��������� ����
    UpdateLcd = FALSE;
}


/*
 * ���                   :  LcdSend
 * ��������              :  ���������� ������ � ���������� �������
 * ��������(�)           :  data -> ������ ��� ��������
 *                          cd   -> ������� ��� ������ (������ enum � n3310.h)
 * ������������ �������� :  ���
 */
static void LcdSend(byte data, LcdCmdData cd) {
    // �������� ���������� ������� (������ ������� ��������)
    //  LCD_PORT &= ~( _BV( LCD_CE_PIN ) );

    /*  if (cd == LCD_DATA) {
          LCD_PORT |= _BV(LCD_DC_PIN);
      }
      else {
          LCD_PORT &= ~(_BV(LCD_DC_PIN));
      }

      // �������� ������ � ���������� �������
      SPDR = data;

      // ���� ��������� ��������
      while ((SPSR & 0x80) != 0x80);*/

    unsigned char i;

    if (cd == LCD_DATA)
        LCD_PORT |= _BV(LCD_DC_PIN);
    else
        LCD_PORT &= ~_BV(LCD_DC_PIN);

    for (i = 0; i < 8; i++) {
        if ((data >> (7 - i)) & 0x01)
            LCD_PORT |= _BV(SPI_MOSI_PIN);
        else
            LCD_PORT &= ~_BV(SPI_MOSI_PIN);

        LCD_PORT |= _BV(SPI_CLK_PIN);
        LCD_PORT &= ~_BV(SPI_CLK_PIN);

    }
    LCD_PORT |= _BV(SPI_MOSI_PIN);
    LCD_PORT |= _BV(LCD_DC_PIN);


}
/*
static void LcdSend(byte data, LcdCmdData cd)   // Sends data to display controller
{
    unsigned char i;

    if(cd == LCD_DATA)                // ������� ��� ������
        LCD_PORT |= _BV(LCD_DC_PIN);               // ������
    else
        LCD_PORT &= ~(_BV(LCD_DC_PIN));                // �������

    for(i=0;i<8;i++)
    {
        if((data>>(7-i)) & 0x01)      // ������ �� ��� DATA
            LCD_PORT |= _BV(SPI_MOSI_PIN);
        else
            LCD_PORT &= ~(_BV(SPI_MOSI_PIN));

        LCD_PORT |= _BV(SPI_CLK_PIN);
        LCD_PORT &= ~(_BV(SPI_CLK_PIN));
    }

    LCD_PORT |= _BV(SPI_MOSI_PIN);
    LCD_PORT |= _BV(LCD_DC_PIN);
}
*/

/*
 * ���                   :  LcdContrast
 * ��������              :  ������������� ������������� �������
 * ��������(�)           :  �������� -> �������� �� 0x00 � 0x7F
 * ������������ �������� :  ���
 */
void LcdContrast(byte contrast) {
    LcdSend(0x21, LCD_CMD);              // ����������� ����� ������
    LcdSend(0x80 | contrast, LCD_CMD);   // ��������� ������ �������������
    LcdSend(0x20, LCD_CMD);              // ����������� ����� ������, �������������� ���������
}


/*
 * ���                   :  Delay
 * ��������              :  ��������������� �������� ��� ��������� ������������� LCD
 * ��������(�)           :  ���
 * ������������ �������� :  ���
 */
static void Delay(void) {
    int i;

    for (i = -32000; i < 32000; i++);
}


/*
 * ���                   :  LcdGotoXYFont
 * ��������              :  ������������� ������ � ������� x,y ������������ ������������ ������� ������
 * ��������(�)           :  x,y -> ���������� ����� ������� �������. ��������: 0,0 .. 13,5
 * ������������ �������� :  ������ ������������ �������� � n3310.h
 */
byte LcdGotoXYFont(byte x, byte y) {
    // �������� ������
    if (x > 13 || y > 5) return OUT_OF_BORDER;

    //  ���������� ���������. ��������� ��� ����� � �������� 504 ����
    LcdCacheIdx = x * 6 + y * 84;
    return OK;
}


/*
 * ���                   :  LcdChr
 * ��������              :  ������� ������ � ������� ������� �������, ����� �������������� ��������� �������
 * ��������(�)           :  size -> ������ ������. ������ enum � n3310.h
 *                          ch   -> ������ ��� ������
 * ������������ �������� :  ������ ������������ �������� � n3310lcd.h
 */
byte LcdChr(LcdFontSize size, byte ch, byte margin) {
    byte i, c;
    byte b1, b2;
    int tmpIdx;

    if (LcdCacheIdx < LoWaterMark) {
        // ��������� ������ �������
        LoWaterMark = LcdCacheIdx;
    }

    if ((ch >= 0x20) && (ch <= 0x7F)) {
        // �������� � ������� ��� �������� ASCII[0x20-0x7F]
        ch -= 32;
    }
    else if (ch >= 0xC0) {
        // �������� � ������� ��� �������� CP1251[0xC0-0xFF]
        ch += 97;
    }
    else {
        // ��������� ���������� (�� ������ ��� � ������� ��� �������� ������)
        ch = 95;
    }

    if (size == FONT_1X) {
        for (i = 0; i < 5; i++) {
            // �������� ��� ������� �� ������� � ���
            LcdCache[LcdCacheIdx++] = pgm_read_byte(&(FontLookup[ch][i])) << 1;
        }
    }
    else if (size == FONT_2X) {
        tmpIdx = LcdCacheIdx - 84;

        if (tmpIdx < LoWaterMark) {
            LoWaterMark = tmpIdx;
        }

        if (tmpIdx < 0) return OUT_OF_BORDER;

        for (i = 0; i < 5; i++) {
            // �������� ��� ������� �� ������� � ��������� ����������
            c = pgm_read_byte(&(FontLookup[ch][i])) << 1;
            // ����������� ��������
            // ������ �����
            b1 = (c & 0x01) * 3;
            b1 |= (c & 0x02) * 6;
            b1 |= (c & 0x04) * 12;
            b1 |= (c & 0x08) * 24;

            c >>= 4;
            // ������ �����
            b2 = (c & 0x01) * 3;
            b2 |= (c & 0x02) * 6;
            b2 |= (c & 0x04) * 12;
            b2 |= (c & 0x08) * 24;

            // �������� ��� ����� � ���
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx + 82] = b2;
            LcdCache[tmpIdx + 83] = b2;
        }

        // ��������� x ���������� �������
        LcdCacheIdx = (LcdCacheIdx + margin) % LCD_CACHE_SIZE;
    }

    if (LcdCacheIdx > HiWaterMark) {
        // ��������� ������� �������
        HiWaterMark = LcdCacheIdx;
    }

    // �������������� ������ ����� ���������
    LcdCache[LcdCacheIdx] = 0x00;
    // ���� �������� ������� ��������� LCD_CACHE_SIZE - 1, ��������� � ������
    if (LcdCacheIdx == (LCD_CACHE_SIZE - 1)) {
        LcdCacheIdx = 0;
        return OK_WITH_WRAP;
    }
    // ����� ������ �������������� ���������
    LcdCacheIdx++;
    return OK;
}


/*
 * ���                   :  LcdStr
 * ��������              :  ��� ������� ������������� ��� ������ ������ ������� �������� � RAM
 * ��������(�)           :  size      -> ������ ������. ������ enum � n3310.h
 *                          dataArray -> ������ ���������� ������ ������� ����� ����������
 * ������������ �������� :  ������ ������������ �������� � n3310lcd.h
 */
byte LcdStr(LcdFontSize size, char *dataArray, byte margin) {
    byte tmpIdx = 0;
    byte response;
    while (dataArray[tmpIdx] != '\0') {
        // ������� ������
        response = LcdChr(size, dataArray[tmpIdx], margin);
        // �� ����� ����������� ���� ���������� OUT_OF_BORDER,
        // ������ ����� ���������� ������ �� ������ �������
        if (response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
        // ����������� ���������
        tmpIdx++;
    }
    return OK;
}


/*
 * ���                   :  LcdPixel
 * ��������              :  ���������� ������� �� ���������� ����������� (x,y)
 * ��������(�)           :  x,y  -> ���������� ���������� �������
 *                          mode -> Off, On ��� Xor. ������ enum � n3310.h
 * ������������ �������� :  ������ ������������ �������� � n3310lcd.h
 */
byte LcdPixel(byte x, byte y) {
    int index;
    byte offset;
    byte data;

    // ������ �� ������ �� �������
    if (x >= LCD_X_RES || y >= LCD_Y_RES) return OUT_OF_BORDER;

    // �������� ������� � ��������
    index = ((y / 8) * 84) + x;
    offset = y - ((y / 8) * 8);

    data = LcdCache[index];

    // ��������� �����

    data |= (0x01 << offset);

    // ������������� ��������� �������� � ���
    LcdCache[index] = data;

    if (index < LoWaterMark) {
        // ��������� ������ �������
        LoWaterMark = index;
    }

    if (index > HiWaterMark) {
        // ��������� ������� �������
        HiWaterMark = index;
    }
    return OK;
}


/*
 * ���                   :  LcdLine
 * ��������              :  ������ ����� ����� ����� ������� �� ������� (�������� ����������)
 * ��������(�)           :  x1, y1  -> ���������� ���������� ������ �����
 *                          x2, y2  -> ���������� ���������� ����� �����
 *                          mode    -> Off, On ��� Xor. ������ enum � n3310.h
 * ������������ �������� :  ������ ������������ �������� � n3310lcd.h
 */
byte LcdLine(byte x1, byte y1, byte x2, byte y2) {
    int dx, dy, stepx, stepy, fraction;
    byte response;

    // dy   y2 - y1
    // -- = -------
    // dx   x2 - x1

    dy = y2 - y1;
    dx = x2 - x1;

    // dy �������������
    if (dy < 0) {
        dy = -dy;
        stepy = -1;
    }
    else {
        stepy = 1;
    }

    // dx �������������
    if (dx < 0) {
        dx = -dx;
        stepx = -1;
    }
    else {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

    // ������ ��������� �����
    response = LcdPixel(x1, y1);
    if (response)
        return response;

    // ������ ��������� ����� �� �����
    if (dx > dy) {
        fraction = dy - (dx >> 1);
        while (x1 != x2) {
            if (fraction >= 0) {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            response = LcdPixel(x1, y1);
            if (response)
                return response;

        }
    }
    else {
        fraction = dx - (dy >> 1);
        while (y1 != y2) {
            if (fraction >= 0) {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;

            response = LcdPixel(x1, y1);
            if (response)
                return response;
        }
    }

    // ��������� ����� ��������� ����
    UpdateLcd = TRUE;
    return OK;
}


/*
 * ���                   :  LcdRect
 * ��������              :  ������ ������������� �������������
 * ��������(�)           :  x1    -> ���������� ���������� x ������ �������� ����
 *                          y1    -> ���������� ���������� y ������ �������� ����
 *                          x2    -> ���������� ���������� x ������� ������� ����
 *                          y2    -> ���������� ���������� y ������� ������� ����
 *                          mode  -> Off, On ��� Xor. ������ enum � n3310.h
 * ������������ �������� :  ������ ������������ �������� � n3310lcd.h
 */
byte LcdRect(byte x1, byte y1, byte x2, byte y2) {
    byte tmpIdx;

    // �������� ������
    if ((x1 >= LCD_X_RES) || (x2 >= LCD_X_RES) || (y1 >= LCD_Y_RES) || (y2 >= LCD_Y_RES))
        return OUT_OF_BORDER;

    if ((x2 > x1) && (y2 > y1)) {
        // ������ �������������� �����
        for (tmpIdx = x1; tmpIdx <= x2; tmpIdx++) {
            LcdPixel(tmpIdx, y1);
            LcdPixel(tmpIdx, y2);
        }

        // ������ ������������ �����
        for (tmpIdx = y1; tmpIdx <= y2; tmpIdx++) {
            LcdPixel(x1, tmpIdx);
            LcdPixel(x2, tmpIdx);
        }

        // ��������� ����� ��������� ����
        UpdateLcd = TRUE;
    }
    return OK;
}


byte LCDIcon(const byte *pic, byte x, byte y, byte arrayRows, byte arrayColumns, byte progMem) {
    byte iterCol;
    byte iterRow;
    byte iterByte;
    byte picX = x;
    byte picY = y;
    byte symbolChar;

    for (iterCol = 0; iterCol < arrayColumns; iterCol++) {
        for (iterRow = 0; iterRow < arrayRows; iterRow++) {
            if (progMem) {
                symbolChar = (byte) pgm_read_byte(&pic[iterCol + iterRow * arrayColumns]);
            } else {
                symbolChar = pic[iterCol + iterRow * arrayColumns];
            }
            for (iterByte = 0; iterByte < 8; iterByte++) {
                if ((symbolChar >> iterByte) & 1) {
                    LcdPixel(picX, picY);
                }
                picY++;
            }
        }
        picY = y;
        picX++;
    }

    UpdateLcd = TRUE;
    return OK;
}