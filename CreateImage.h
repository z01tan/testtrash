/*
 * File:   CreateImage.h
 * Author: andr-u
 *
 * Created on 22 Апрель 2013 г., 14:58
 */

#ifndef CREATEIMAGE_H
#define	CREATEIMAGE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define SCAN_MAXL   4912
#define SCAN_LINEW  1728

#define BremsDelay  100;//160;//es war 170
#define StartDelay  45;//50;//70;  //110 было

#define LINE_LENGTH 1728
#define PCKT_AGGR 16
#define PCKT_SIZE 220
#define BLOCK_SIZE (PCKT_SIZE+4)
#define PIXELS_PER_BLOCK 216
#define PIXEL_DATA_OFFSET 8

#define KEYBUF  21
#define KEYYES  96 //74
#define KEYNO   78
#define KEYHELP 74 //96
#define KEYMENU 55
#define KEYARROW 14 //98
#define KEYBSPCE 98 //14
#define KEYPUNKT 83
#define KEY0     82
#define KEY1     79
#define KEY2     80
#define KEY3     81
#define KEY4     75
#define KEY5     76
#define KEY6     77
#define KEY7     71
#define KEY8     72
#define KEY9     73


#define IMAGE_DEVICE "/dev/ts0"

typedef char  LineS[SCAN_LINEW];

extern  char KEYB[KEYBUF];      // кольцевой буфер клавиатуры
extern  int  Key_B;             // указатель на начальную позицию буфера
extern  int  Key_E;             // указатель на конечную позицию

extern  int  Starting;    // задержка передачи инфы в буфер после затемнения 3-го оптрона
extern  int  Braking;     // задержка выключения двигла после освобождения 3-го оптрона

extern  char*   PixelBMP;       // массив скана бюллетеня
extern  char**  PixCol;         // массив указателей строк массива скана
extern  pthread_mutex_t  mutx;
extern  pthread_t       PID_Image;
extern  sem_t          semBut;
extern  sem_t          semOpt;
extern  sem_t          semRC;
extern  sem_t          semSD;
extern  pthread_t       PID_But;
extern int Scptr_s,Scptr_r;//указатели на сканированные и обработанные строки
extern int max_gelesen;    //кол-во записанных в массив строк скана
extern int ScanInProg;     //идет сканирование и третий оптрон сработал!!!
extern int image_fd;       //дескриптор файла скана

extern bool ScanReady;  // Готовность к обработке скана
extern bool MotorRqst;
extern bool CalibrRqst; // требование установить калибровку сканера
extern int MAXLINES;    //600*8;
extern bool KeyStrPocess; // идет ввод строки с клавиатуры

 int Create_Threads();
 void Close_Threads();
 void fill_Init_param();
 void configure_line(int line_select, int speed, int led_r, int led_g, int scroll_1, int scroll_2,
        int led_div, int mode);
 void print_line(char* line);

#endif	/* CREATEIMAGE_H */

