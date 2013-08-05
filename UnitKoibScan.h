﻿// Заголовочный  файл  для файла  реализации  функций  сканирования  бюллетеня
//
//                     О.Чудаков      15.01.2013
//******************************************************************************
#ifndef UnitKoibScanH
#define UnitKoibScanH
#include <string.h>
#include "HeaderMain.h"
//-----------------переменные по характеритсикам страницы в  точках------------
extern   	int PrKoef;
extern       int NstrFrControl; //  счетчик строк при прореживании контроля границ
extern   	int PageW; // ширина  страницы  в  точках
extern   	int PageH; // высота  страницы  в  точках
extern   	int LineWx;			//  толщина линии в  точках по горизонтали
extern   	int LineWy;			//  толщина линии в  точках по вертикали
extern   	int LMarSize; // размер полей  бюллетеня в  точках по горизонтали
extern   	int TMarSize; // размер полей  бюллетеня в  точках  по вертикали
extern   	int dxStampArea; // ширина  зоны  печати в  точках
extern   	int dyStampArea; // высота  зоны  печати в  точках

extern      int dxStamp; // ширина  зоны  печати в  точках
extern      int dyStamp; // высота  зоны  печати в  точках
extern      float dyNumZone;// высота  зоны номера  в точках
extern      int nstrStampStartL; // строка начала поиска штампа слева
extern      int nstrStampEndL; // строка окончания поиска штампа слева
extern      int nstrStampStartR; // строка начала поиска штампа  справа
extern      int nstrStampEndR; // строка окончания поиска штампа справа
extern		int PrFirstL; // признак нахождения первой точки слева при поиске штампа, если
					// бюллетиень идет обратным ходом
extern		int PrFirstR; // признак нахождения первой точки справа при поиске штампа, если
					// бюллетиень идет обратным ходом
extern 		int PrXMin,PrXMax; // признаки нахождения минимума и максимума X
extern      int XoShR1,YoShR1,PrAlfaShR,XoMax,YoMax,nbXoMax;
extern		int dynstr; // интервал, на котором рассчитыватся угол наклона штампа
extern     	int colStampL;  // текущая колонка начала поиска штампа слева
extern     	int colStampR;  // текущая колонка начала поиска штампа справа
extern     	int colStampLo; // начальная колонка начала поиска штампа слева
extern     	int colStampRo; // начальная колонка начала поиска штампа справа
extern      int dxStFindL;  // количество точек для  поиска левого угла штампа
extern      int dxStFindR;  // количество точек для  поиска правого угла штампа
extern   	int dyStampNum; // высота  зоны  размещения номера участка
extern		int masXoR[6],masXoL[6]; // массивы хранения первых пяти значений правой и левой
							// границ штампа для исключения ошибок
extern	 	int PrShL,PrShR; 	// признаки того, что координаты левой и правой  части штампа найдены
extern		int PrShFound; 	 	// признак того, что координаты штампа найдены
extern  	int nbShL,nbShR; 	// счетчики  пропуска линии штампа
extern  	int nbyShL,nbyShR;	// счетчики пропуска линий в штампе
extern		int PrNaklShL,PrNaklShR; // признак наклона  штампа "1" - влево, "0" - прямо или вправо
extern		int NumUIKRec; // номер участковой избирательной  комиссии распознанный,
								// заданный в исходных данных
extern      char NumUIKStr[5]; // строковое  значение номера избирательной  комиссии
extern  	int dyMF; // высота поля сверху + зоны типа выборов
extern   	int dyHeader; // высота заголовка  в точках

 //------------------массивы для  определения типа голосования

extern      int PrVoteType[NumVoteType+1]; // массив  признаков  типа  голосования
					// в начале  листа
	   //  ячейка [0] - хранит направление  движения бюллетеня ( BulHead)
extern      int PrVoteTypeTop[NumVoteType+1]; // массив  признаков  типа  голосования
							//  в  конце листа
extern      int nbVoteT[NumVoteType+1]; // счетчик строк с  белым цветом при поиске
					// ороиентации бюллетеня и  типа выборов
extern      int nstrVoteType[NumVoteType+1]; // номер  строки, с  которой начинается
		// поиск признака  для каждого типа выборов ( в  случае наклона листа)
extern      int ncVoteType[NumVoteType+1]; // столбец с  которого анализируется наличие

extern      int dxVoteType; // ширина зоны признака типа  выборов    в точках
extern      int dyVoteType; // ширина зоны по вертикали для определения типа выборов в точках
	  // белого цвета в поле  типа  выборов (признака начала (конца) бюллетеня
// массив с координатами  квадратов и их  размерами	для каждого типа выборов
// для  прямого и обратного  прохождения
extern		int PrVoteTypeStart[NumVoteType+1]; // признаки начала поиска типа выборов
			// длz  каждого места
extern		int NumSQMax[NumVoteType+1];  // максимальное  количество квадратов
					//для каждого типа голосования  (из исходных данных)
//extern		int NumSQMaxVopr[NumVoteType+1][NumVoprMax][2];  // данные по квадратам для
			// для каждого вопроса
					//для каждого типа голосования  (из исходных данных)
extern	int**** MasSQ;  // координаты квадратов для прямого (задаются из исходных
					// данных) прохода бюллетеня
extern  int **NumBlP,**NumPMax; // количество черных и белых точек в  области квадрата
extern 	int** SearchSQ; // массив для  поиска начала квадрата, а затем для продсчета
					// точек  внутри квадрата

extern  int **RezRec; // результаты  распознавания квадратов ( наличия отметки)

//------ переменные для поиска верхней  левой или  правой  точки четырехугольника
extern  int ncL;    // номер  начальной колонки (столбца) для поиска левой границы
extern  int ncTL;   // номер  начальной колонки (столбца) для поиска верхней
				// границы слева
extern  int dxT;    // количество  точек строки для  оценки перехода на новый цвет
				// при поиска  верхней границы
extern  int dxL;    // количество  точек строки для  оценки перехода на новый цвет
				// при поиска  левой границы
extern  int nbTL;   // счетчик строк с  новым цветом при поиске верха  слева
extern  int nbLL;   // счетчик  точек, изменивших цвет при поиске границ слева
extern  int PgXoL;  // координаты левой найденной  точки  по оси  ОХ (вертикали)
extern  int PgYoL;  // координаты левой найденной  точки  по оси  ОY (вертикали)
extern  int ncR;    // номер  начальной колонки (столбца) для поиска правой границы
extern  int ncTR;   // номер  начальной колонки (столбца) для поиска верхней
				// границы справа
extern  int nbTR;   // счетчик строк с  новым цветом при поиске верха справа
extern  int nbRR;   // счетчик  точек, изменивших цвет при поиске границ справа
extern  int PgXoR;  // координаты правой найденной  точки  по оси  ОХ (вертикали)
extern  int PgYoR;  // координаты правой найденной  точки  по оси  ОY (вертикали)
extern  int PrLeft;  // признак того, что  левая  граница определена
extern  int PrTopL; //  признак того,  что верх  слева  найден
extern  int PrTopR; //  признак того,  что верх  справа  найден
extern  int PrRight;// признак того, что  правая  граница определена
extern  int nstrL;  //  номер  строки, с  которой  начало  поиска
				// следующего элемента слева
extern  int nstrR;  //  номер  строки, с  которой  начало  поиска
				// следующего элемента справа
extern  int nstrBegVoteType; // номер строки, с  которой  начинается просмотр строк
				// для выявления типа выборов

extern  int nstrHeaderEnd; //  строка, на  которой  заканчивается проверка заголовка
					// бюллетеня

extern int nstrLsEnd; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня
extern int nstrRsEnd; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня

extern int nstrLsStart; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня
extern int nstrRsStart; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня


extern  int colTestPageL; // колонка, по которой  проверяется окончание  бюллетеня   слева
extern  int colTestPageR; // колонка, по которой  проверяется окончание  бюллетеня   справа
extern  int dxTestPage;  // длина  строки для  проверки окончания бюллетеня
extern	int nstrPageEnd; //  строка, на которой заканчивается страница
extern int PrFrLEnd,PrFrREnd,PrFrTLEnd,PrFrTREnd;
extern int PrPageEnd;

extern  int Step;   //  шаг, определяющий направление поиска: "1" - слева направо;
					// "-1" справа налево
extern  int nstrFrYmax; // максимальнsq номер строки до начала рамки
// -------------------переменные параметров  рамки бюллетеня,  ---------------
//--------------------определенные во время сканирования  -------------------

extern  int FrXoL;
extern  int FrYoL; // координаты левого угла рамки бюллетеня
extern  int FrXoR;
extern  int FrYoR; // координаты правого угла рамки бюллетеня
extern  int EndYoL,EndYoR,EndXoR,EndXoL,PrEndXoL; // координаты начала рамки внизу листа
extern  int BulHead;// признак направления бюллетеня:
				// "0" не определено;
				// "1" - в прямом направлении
				// "-1"  - в обратном направлении
// переменные для  контроля рамки бюллетеня
extern int LsCol,Lsdx,nbLs,LsXo,LsPr,LsNs;
extern int RsCol,Rsdx,nbRs,RsXo,RsPr,RsNs;

//---------------- переменные для управления сканированием  --------------
extern  int PrPage;  // признак того, что начало листа определено
extern  int PrFrame; // признак определения  рамки  листа
				 // "0" рамка листа  не найдена,
				 // "1" - начало рамки  найдено,
				 // "2" - найден  конец  рамки бюллетеня

//----------------- промежуточные переменные ------------------------------
extern  float dd, gam, sigma,u1,u2,xx,yy,aa,bb,AlfaShR;

extern  float SinAlfaPage,CosAlfaPage,TanAlfaPage;
extern	int i, j,prizn,ii,nVoteType,XoSh1,YoSh1,PrAlfaSh,XoMin,YoMin,nbXoMin;


extern  int dxSQ,dySQ,NumPLines,NumWtP,SPL,SPM,NumPV,PrAlfa,PrPageSize;

double erf(double z); // интеграл вероятности
double erf1(double z);// обратная функция лапласа

int FindPageStart(int* PrPage); // поиск начала страницы
int FindFrame(int* PrFrame);  // поиск начала рамки
int FindFrame1(int* PrSt);
int FrameControl(void); // контрольналичия  границ  рамки
int FindShAreaStart(void); //  поиск начала зоны штампа
int FindVoteTypeTop(int *PrPg); // определение типа  голосования и направления листа
int FindVoteTypeBot(int *PrPg); //  поиск типа выборов в  конце листа
int TestPageEnd(int* PrPg); //  поик нижней  рамки  бюллетеня
int FindNumYchTop(int* PrSt); // определение рамки штампа в прямом направлении
int FindNumYchBot(int* PrSt);// определение рамки штампа в обратном направлении
void FindVoteMarks(void); // определение наличия  отметок  голосования  в  квадратах
//int ScanBulString(void);//  функция управления  сканированием бюллетеня при поступлении  очередной  строки
void GashMasVote(void); // гашение динамических массивов
//  для  работы  с  сервисным меню
int MyWaitYesNoEnter(int pr); // определение нажатой  кнопки
int FormInfS(char Infs[]); // формирвоание  информационного  сообщения  на  табло
int GetKodDost(int kod); // получение  введеного  кода  доступа



#endif