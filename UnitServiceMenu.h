//*****************************************************************************
// Файл реализации  функций  сервисного  меню КОИБ
//
//                     О.Чудаков      23.05.2013
//******************************************************************************
//******************************************************************************
#ifndef UnitServiceMenuH
#define UnitServiceMenuH
//---------------------------------------------------------------------------

#include <stdint.h>
#include "HeaderMain.h"
//  переменные  для  сервисного  меню
extern  int  PrExKeyB;
extern  int  PrMenuBtn;
extern  int  KodDost;
extern  int  KodOp;
extern  int  BtnPushed;
extern  int  PrUsb1,PrUsb2,PrUsb3,PrSDCard,PrPrinter;
extern  int  MenuItems0[NumMenuItems+1]; //
extern  int ItemMenu0,ItemMenu1,MenuUr;// состояние уровнней меню
extern  char InfS1[124];
extern  char InfS2[124];
extern  char InfS[124];  // информационные сообщения
extern  int  CodesOp[3]; //  сохраненные коды  доступа операторов
//extern  float MasSenValue[NumSensors][3];//массив значений порогов датчиков для листов
extern  int PrSenYes; // признак получения  данных от  датчиков
extern  int BtnYesPushed; // сохранение признака нажатия кн. ДА для ожидания поступления данных от датчиков
extern  unsigned  MasBlUr[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
extern  unsigned  MasWtUr[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
extern  float MasBlUrSig[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
extern  float MasWtUrSig[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
extern  int MasNumPoints[NumStrScan]; //  массив  количества  точек для подсчитанных для  осреднения
extern double MasKoefPzs[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного

extern  int PrPageCal,PgXoLCal,PgYoLCal,PgXoRCal,PgYoRCal;



#endif

