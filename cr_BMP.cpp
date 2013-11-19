//---------------------------------------------------------------------------

//#pragma hdrstop
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cr_BMP.h"
#include "CreateImage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


// Пусть у нас будет картинка размером 30 x 50 пикселей
int WidthBMP; //= 30;//1728;
int HeightBMP; //= 50;//4919;

    char FBMP[]        = "/www/pages/Scan.bmp";


int MakeBMPFile(char* nameF,int W, int H)
{
	FILE* hFile;

	int i, j;
	DWORD RW;
	DWORD a;
	// Объявим нужные структуры
	BMAPFILEHEADER bfh;
	BMAPINFOHEADER bih;
	RGB_QUAD Palette [256];		// Палитра
        WidthBMP = W;
        HeightBMP = H;
	// Заполним их
	memset (&bfh, 0, sizeof(bfh));
	bfh.bfType = 0x4D42;		// Обозначим, что это bmp 'BM'
	// Палитра занимает 1Kb
	bfh.bfOffBits  = sizeof(bfh) + sizeof(bih) + 1024;
	// Посчитаем размер конечного файла
	bfh.bfSize = bfh.bfOffBits +
				 1 * WidthBMP * HeightBMP +      // 1 - это sizeof(color)
				 HeightBMP * ((3*WidthBMP) % 4);

	memset (&bih, 0, sizeof(bih));
	bih.biSize = sizeof(bih);	  // Так положено
	bih.biBitCount = 8;		  // 8 бит на пиксель
	bih.biCompression = 0;//BI_RGB;	  // Без сжатия
	bih.biHeight = HeightBMP;
	bih.biWidth = WidthBMP;
	bih.biPlanes = 1;                 // Должно быть 1
	bih.biXPelsPerMeter = 2834;       // 72 DPI
	bih.biYPelsPerMeter = 2834;
	bih.biClrUsed	=	256;      // Количество цветов
	// А остальные поля остаются 0
	if ((hFile = fopen(nameF, "wb")) == NULL) return -1;

	// Запишем заголовки
	fwrite (&bfh, 1,14,  hFile);
	fwrite (&bih, 1,40,  hFile);

	// Создадим и запишем палитру
	memset (&Palette[0], 0, sizeof (RGB_QUAD));
	for (i = 1; i < 256; i++)
	{
		Palette[i].rgbBlue = Palette[i-1].rgbBlue + 1;//20;
		Palette[i].rgbGreen = Palette[i-1].rgbGreen + 1;//30;
		Palette[i].rgbRed = Palette[i-1].rgbRed + 1;//10;
	}
	fwrite ( &Palette, 4 , 256, hFile);

	// Сохраняем BITMAP:
	for (i = HeightBMP-1; i >= 0 ; i--)
	{
		for (j = 0; j < WidthBMP; j++)
		{
			fwrite ( PixCol[i]+j, 1, 1, hFile);
		}
		// Выровняем строку по границе слова
		fwrite ( &Palette, (3 * WidthBMP) % 4, 1, hFile);
	}

	fclose(hFile);     // FERTIG!!!
        return 0;
}
