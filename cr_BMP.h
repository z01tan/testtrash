/* 
 * File:   cr_BMP.h
 * Author: andr-u
 *
 * Created on 7 Май 2013 г., 16:07
 */

#ifndef CR_BMP_H
#define	CR_BMP_H
//---------------------------------------------------------------------------




typedef unsigned long	DWORD;
typedef unsigned short  WORD;
typedef long 		LONG;
typedef unsigned char   BYTE;

#pragma pack (push,1)
typedef struct tgBITMAPFILEHEADER
{
  short   bfType;        // смещение 0 байт от начала файла
  DWORD   bfSize;        // смещение 2 байта от начала файла, длина 4 байта
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;     // смещение 10 байт от начала файла, длина 4 байта
} BMAPFILEHEADER, *PBMAPFILEHEADER;

typedef struct tgBITMAPINFOHEADER{
  DWORD  biSize;
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
  DWORD  biCompression;
  DWORD  biSizeImage;
  LONG   biXPelsPerMeter;
  LONG   biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;
} BMAPINFOHEADER, *PBMAPINFOHEADER;
#pragma pack (pop)
typedef struct tgRGBQUAD {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} RGB_QUAD;

//extern char* PixelBMP;
//extern int WidthBMP;
//extern int HeightBMP;
    int MakeBMPFile(char* nameF,int W, int H);


//---------------------------------------------------------------------------

#endif	/* CR_BMP_H */

