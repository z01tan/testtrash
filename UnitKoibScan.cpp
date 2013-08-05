//*****************************************************************************
// Файл реализации  функций  сканирования  бюллетеня
//
//                     О.Чудаков      15.01.2013
//******************************************************************************

#include <math.h>
#include <stdio.h>
#include <exception>
#include <iostream>
#include "UnitKoibScan.h"
#include "CreateImage.h"
#include "HeaderMain.h"
#include "InitAltera.h"
#include "HardProc.h"

using std::cout;
using std::endl;

//-----------------данные  по  странице из исходных данных---------------------
   int PrKoef;
   int NstrFrControl; //  счетчик строк при прореживании контроля границ
   float dyLine1M;
   int dyLine1;

   char NUMBER[32]; //  буфер, где  формируется   число с  клавиатуры


   int PrUprOpt; // признак состояния  оптронов
   int PrNoBtnWait; // признак пропуска ожидания  кнопки
   int Sost_Optrons[4];

   int	NumV;   // угол наклона  листа
   int MaxMarks;   // угол наклона  листа
  // параметры КОИБ
   int Lang;  // язык интерфейса
   int VolumeUr; // уровень громкости
   int VolumeUrMax; // максималный уровень громкости
   char SourceDir1[128]; //
   char SaveDir[128]; //
   int PrPrint; //
   int PrSlave;
   int wngtime;
   int PrSaveLog;
   char Ip_master[16];
   char Ip_slave[16];
   char iface[16];
   char udp_port[16];
   int timeout;

   // пареметры  управления
   int PrCheckBr; // признак необходимости проверки границ рамки
   int PrCheckSh; // признак необходимости проверки  штампа
   int PrCheckEx;// признак необходимости маркировки недействительных бюллетеней

   int MaxLightR; // максимальный уровень подсветки при калибровке
   int MaxLightG; // максимальный уровень подсветки при калибровке
   int MaxLightB; // максимальный уровень подсветки при калибровке
   float KoefY,KoefX; // коэффициенты  сжатия или растяжения листа
   int PrKoefY,NumDotKoefY;
   int PrIdOk; // признак того, что исходные данные для сканирования переписаны в рабочин ячейки
   int PagePar[NumVoteType+1][NumParPage]; // массив параметров страницы
   int PrVoteTypeId[NumVoteType+1]; // признаки типа бюллетеней в  выборах на основе исходных данных



   int NumDotX; // количество  точек на  мм  по горизонтали листа
   int NumDotY; // количество  точек на  мм  по вертикали   листа

   float Max_Bull_Height; // максимальная длина бюллетеня
   float Max_Bull_Width; // максимальная длина бюллетеня
   float Dx_Max; // допустимое отклонение ширины листа
   float Dy_Max; //  допустимое отклонение высоты  листа
   float MaxAngle;// максимальныц угол наклона  бланка
   float Dig_Height; //  высота  цифр в  бюллетене
   float Dig_Width; //  высота  цифр в  бюллетене
   float Dig_Interv; //  расстояние между  цифрами штампа
   float Max_ShAngle; // максимальный угол наклона штампа

   float PageWM; // ширина  страницы в  мм
   float PageHM; // высота  страницы в  мм
   float dxVoteTypeM; // ширина зоны типа выборов по горизонтали в  милиметрах
   float dyVoteTypeM;    // ширина зоны типа выборов по вертикали в  милиметрах
   float LineWM;    // толщина  линий зоны типа выборов в  милиметрах
   float LMarSizeM; // размер полей  бюллетеня в  миллиметрах по горизонтали
   float TMarSizeM; // размер полей  бюллетеня в  миллиметрах  по вертикали
   float dxStampAreaM; // ширина  зоны  печати в  мм
   float dyStampAreaM; // высота  зоны  печати в  мм
   float dxStampM; // ширина  штампа (печати) в  мм
   float dyStampM; // высота  штампа (печати) в  мм
   float dyNumZoneM;// высота  зоны номера  в мм
//-----------------переменные по характеритсикам страницы в  точках------------
	int PageW; // ширина  страницы  в  точках
	int PageH; // высота  страницы  в  точках
	int LineWx;			//  толщина линии в  точках по горизонтали
	int LineWy;			//  толщина линии в  точках по вертикали
	int LMarSize; // размер полей  бюллетеня в  точках по горизонтали
	int TMarSize; // размер полей  бюллетеня в  точках  по вертикали
	int dxStampArea; // ширина  зоны  печати в  точках
	int dyStampArea; // высота  зоны  печати в  точках
	int nsShArStart,nbShAr,ShArXo,ShArYo,ShArPr,colShAr;
	// характеристики и переменные для поиска штампа
	int dxStamp; // ширина  печати в  точках
	int dyStamp; // высота  печати в  точках
	float dyNumZone;// высота  зоны номера  в точках
	int nstrStampStartL; // строка начала поиска штампа слева
	int nstrStampEndL; // строка окончания поиска штампа слева
	int nstrStampStartR; // строка начала поиска штампа  справа
	int nstrStampEndR; // строка окончания поиска штампа справа
	int PrFirstL,PrFirstLy; // признак нахождения первой точки слева при поиске штампа, если
					// бюллетиень идет обратным ходом
	int PrFirstR,PrFirstRy; // признак нахождения первой точки справа при поиске штампа, если
					// бюллетиень идет обратным ходом
	int PrXMin,PrXMax; // признаки нахождения минимума и максимума X
			// при поиске штамп, если бюллетиень идет обратным ходом
	int XoShR1,YoShR1,PrAlfaShR,XoMax,YoMax,nbXoMax;
	int dynstr; // интервал, на котором рассчитыватся угол наклона штампа
	int colStampL; // текущая колонка начала поиска штампа слева
	int colStampR; // текущая колонка начала поиска штампа справа
	int colStampLo; // начальная колонка начала поиска штампа слева
	int colStampRo; // начальная колонка начала поиска штампа справа
	int dxStFindL; // количество точек для  поиска левого угла штампа
	int dxStFindR; // количество точек для  поиска правого угла штампа
	int dyStampNum; // высота  зоны  размещения номера участка
	int masXoR[6],masXoL[6]; // массивы хранения первых пяти значений правой и левой
						// границ штампа для исключения ошибок
	int XoShL,XoShR,YoShL,YoShR; // координаты левого и правого нижних углов штампа
	int PrShL,PrShR; // признаки того, что координаты левой и правой  части штампа найдены
	int PrShFound; // признак того, что координаты штампа найдены
	int nbShL,nbShR; // счетчики  количества подряд идущих искомых точек
	int nbyShL,nbyShR; // счетчики пропуска линий в штампе
	int PrNaklShL,PrNaklShR; // признак наклона  штампа "1" - влево, "0" - прямо или вправо
	int NumUIKRec=0;// номер участковой избирательной  комиссии распознанный,
					// заданный в исходных данных
	char NumUIKStr[5]; // строковое  значение номера избирательной  комиссии
	int UIKNum[MaxNumUIK];      //номера штампов, допустимых при выборах
	int dyMF; // высота поля сверху + зоны типа выборов
	int dyHeader; // высота заголовка  в точках
 // ---------------массивы для  определения типа голосования
	int PrVoteType[NumVoteType+1]; // массив  признаков  типа  голосования
	int PrVoteTypeTop[NumVoteType+1]; // массив  признаков  типа  голосования
			//  в  конце листа
			//  ячейка [0] - хранит направление  движения бюллетеня ( BulHead)
	int nbVoteT[NumVoteType+1]; // счетчик строк с  белым цветом при поиске
			// ороиентации бюллетеня и  типа выборов
	int nstrVoteType[NumVoteType+1]; // номер  строки, с  которой начинается
// поиск признака  для каждого типа выборов ( в  случае наклона листа)
    int ncVoteType[NumVoteType+1]; // столбец с  которого анализируется наличие
	int dxVoteType; // ширина зоны признака типа  выборов    в точках
	int dyVoteType; // ширина зоны по вертикали для определения типа выборов в точках
	  // белого цвета в поле  типа  выборов (признака начала (конца) бюллетеня
	int PrVoteTypeStart[NumVoteType+1]; // признаки начала поиска типа выборов дл  каждого места
// массив с координатами  квадратов и их  размерами	для каждого типа выборов
// для  прямого и обратного  прохождения
	int NumSQMax[NumVoteType+1];  // максимальное  количество квадратов
					//для каждого типа голосования  (из исходных данных)
	//int NumSQMaxVopr[NumVoteType+1][NumVoprMax+1][2];  // данные по квадратам для
			// для каждого вопроса
	int**** MasSQ;// координаты квадратов для прямого (задаются из исходных
					// данных) и обратного (вычисляются) прохода бюллетеня
	int** SearchSQ; // массив для  поиска начала квадрата, а затем для продсчета
					// точек  внутри квадрата
	int **NumBlP,**NumPMax; // количество черных и белых точек в  области квадрата
	int **RezRec; // результаты  распознавания квадратов ( наличия отметки)
	// структура выходных даннх сканирвоания  листа
	TScanOut MasScan; //TScanOut
	// ------------------переменные  для  определения  цвета  точки (черный-белый)
	int UrWhite0; // эталон уровня белого
	int UrBlack0; // эталон уровня черного

  float p00,   // вероятность того,  что  точка  имеет  черный  цвет
 		p11,    // вероятность того,  что  точка  имеет  белый  цвет
		alfa,   // вероятность ошибки  первого  рода ( пропуска  черного)
		beta,// вероятность ошибки  второго рода (принрятия белого за  черный)
		g0,     // гипотеза "Черный" "1" - цвет  черный
		g1,     // гипотеза "Белый"  "1" - цвет  белый
		surw,   // Оценка СКО мат. ожидания  уровня  белого (среднее значение)
		surbl,  // Оценка СКО мат. ожидания  уровня  черного (среднее значение)
		urw, urw1,UrWtSh,   // Оценка мат. ожидания  уровня  белого (среднее значение)
		urbl,urbl1,UrBlSh;   // Оценка мат. ожидания  уровня  черного (среднее значение)
	double	AlfaPage;   // угол наклона  листа
	int dyALfa,dxALfa;   // разница координат Y  слева и справа листа

//------ переменные для поиска верхней  левой или  правой  точки четырехугольника
  int ncL;  // номер  начальной колонки (столбца) для поиска левой границы
  int ncTL; // номер  начальной колонки (столбца) для поиска верхней
			// границы слева
  int dxT;  // количество  точек строки для  оценки перехода на новый цвет
			// при поиска  верхней границы
  int dxL;  // количество  точек строки для  оценки перехода на новый цвет
			// при поиска  левой границы
  int nbTL; // счетчик строк с  новым цветом при поиске верха  слева
  int nbLL; // счетчик  точек, изменивших цвет при поиске границ слева
  int PgXoL;// координаты левой найденной  точки  по оси  ОХ (вертикали)
  int PgYoL;// координаты левой найденной  точки  по оси  ОY (вертикали)
  int ncR;  // номер  начальной колонки (столбца) для поиска правой границы
  int ncTR; // номер  начальной колонки (столбца) для поиска верхней
			// границы справа
  int nbTR; // счетчик строк с  новым цветом при поиске верха справа
  int nbRR; // счетчик  точек, изменивших цвет при поиске границ справа
  int PgXoR;  // координаты правой найденной  точки  по оси  ОХ (вертикали)
  int PgYoR;  // координаты правой найденной  точки  по оси  ОY (вертикали)
  int PrLeft;  // признак того, что  левая  граница определена
  int PrTopL; //  признак того,  что верх  слева  найден
  int PrTopR; //  признак того,  что верх  справа  найден
  int PrRight;// признак того, что  правая  граница определена
  int nstr;   // номер  текущей  строки
  int nstrL;  //  номер  строки, с  которой  начало  поиска
			// следующего элемента слева
  int nstrR;  //  номер  строки, с  которой  начало  поиска
			// следующего элемента справа
  int nstrBegVoteType; // номер строки, ск  которой  начинается просмотр строк
			// для выявления типа выборов
  int nstrHeaderEnd; //  строка, на  которой  заканчивается проверка заголовка
				// бюллетеня
 int nstrLsEnd; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня
 int nstrRsEnd; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня

 int nstrLsStart; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня
 int nstrRsStart; 	//  строка, на которой заканчивается проверка левой рамки бюллетеня

 int colTestPageL; 	// колонка, по которой  проверяется окончание  бюллетеня   слева
 int colTestPageR; 	// колонка, по которой  проверяется окончание  бюллетеня   справа
 int dxTestPage;  	// длина  строки для  проверки окончания бюллетеня
 int nstrPageEnd; 		//  строка, на которой заканчивается страница
 int PrPageEnd;

 int Step;   //  шаг, определяющий направление поиска: "1" - слева направо;
				// "-1" справа налево

  int nstrFrYmax; // максимальнsq номер строки до начала рамки
// -------------------переменные параметров  рамки бюллетеня,  ---------------
//--------------------определенные во время сканирования  -------------------

 int FrXoL;
 int FrYoL; // координаты левого угла рамки бюллетеня
 int FrXoR;
 int FrYoR; // координаты правого угла рамки бюллетеня
 int EndYoL,EndYoR,EndXoR,EndXoL,PrEndXoL; // координаты начала рамки внизу листа
 int BulHead=0;// признак направления бюллетеня:
				// "0" в прямом направлении;
				// "1" - в обратном направлении
 // переменные для  контроля рамки бюллетеня
 int LsCol,Lsdx,nbLs,LsXo,LsPr,LsNs;
 int RsCol,Rsdx,nbRs,RsXo,RsPr,RsNs;

//----------------------переменные  для проведения  дополнительной  калибровки
//--------------------- в  начале  листа (после  определения начала листа)
 int nr1;// номер строки,  с которой  начинается  уточнение  уровней черного (белого)
 int nr2;// номер строки, на которой заканчивается  уточнение  уровней черного (белого)
 int nc1;// номер столбца, с которого  начинается  уточнение  уровней черного (белого)
 int nc2;// номер столбца,  на котором заканчивается  уточнение  уровней черного (белого)
 int nn; //  для  калибровки
//---------------- переменные для управления сканированием  --------------
 int PrPage;  // признак того, что начало листа определено
 int PrFrame; // признак определения  рамки  листа
				 // "0" рамка листа  не найдена,
				 // "1" - начало рамки  найдено,
//----------------- промежуточные переменные ------------------------------
	unsigned urw_ch; // значение интенсивности точки
	float dd, gam, sigma,u1,u2,xx,yy,aa,bb,AlfaSh,AlfaShR;
	float SinAlfaPage,CosAlfaPage,TanAlfaPage;
	int i,j,k,prizn,ii,nVoteType,XoSh1,YoSh1,PrAlfaSh,XoMin,YoMin,nbXoMin,XiL,XiR;
	int zz,dyy,DeltaX,DeltaY,dxAl,dyAl;
	int dxSQ,dySQ,NumPLines,NumWtP,SPL,SPM,NumPV,PrAlfa,PrPageSize=0,summa=0;
	int dxFindSQ; // количество точек  в  строке  для  поиска начала квадрата
	int nstrUrWhite; //  строка, на  которой  калибруется  белый цвет
	char Ch;
	int XoLSh,PrXoLSh, XoRSh,PrXoRSh; // координаты рамки штампа по Х


int ReadSetUpFiles(void)
{ // считывание  файлов при инициализации системы
  int i,/*sum0=0,sum1=0,sum2=0,*/ pr=0;
  std::string sst;
  //  считывание  параметров КОИБ
  pr=SetUpKoibRead();
  if(pr==1)   {  sst=Out.Menu1[40];   setSecline(&sst[0],1);   };
  // считывания параметров управления
  if(pr==0) pr=SetUpUprRead();
    if(pr==1)   {  sst=Out.Menu1[41];   setSecline(&sst[0],1);   };
    // считывание параметров  бюллетеней
  if(pr==0) pr=SetUpBullRead();
    if(pr==1)   {  sst=Out.Menu1[42];   setSecline(&sst[0],1);   };
  // считывания параметров  распознавания
  if(pr==0) pr=SetUpRecRead();
    if(pr==1)   {  sst=Out.Menu1[43];   setSecline(&sst[0],1);   };
  // считывания параметров  сенсоров
  if(pr==0) pr=ReadKoeffSens();
    if(pr==1)   {  sst=Out.Menu1[44];   setSecline(&sst[0],1);   };
    // установка  уровня громкости динамиков
    setVoiceVolume(VolumeUr);
    setVoiceVolume(3);
  // вычисление  пороговых значений по листам
  //MaxLightR=160; MaxLightG=160;  MaxLightB=160;
  altera_backlight(MaxLightR,MaxLightG,MaxLightB);

 NO_PAPER  =MasSenValue[0][0];
 ONE_PAPER=MasSenValue[0][2];
   for(i=1;i<NumSensors;i++)  if(NO_PAPER > MasSenValue[i][0]) NO_PAPER=MasSenValue[i][0];
            NO_PAPER=NO_PAPER-20;
    for(i=1;i<NumSensors;i++)  if(ONE_PAPER  < MasSenValue[i][2]) ONE_PAPER=MasSenValue[i][2];
            ONE_PAPER=ONE_PAPER+20;

	 //  формирование параметров бюллетеня  в  рабочие переменные

  PageWM=Max_Bull_Width;// ширина  страницы в  мм  (при загрузке ( максимальная)
  PageHM=Max_Bull_Height;// высота  страницы в при загрузке (максимальная) мм
  PageW=int(PageWM*float(NumDotX)); // ширина  страницы  в  точках
  PageH=int(PageHM*float(NumDotY)); // высота  страницы  в  точках

  LMarSize= int(LMarSizeM*float(NumDotX));
  TMarSize= int(TMarSizeM*float(NumDotY));// размер полей  бюллетеня в  точках  по вертикали
  LineWx=int(LineWM*float(NumDotX));	//  толщина линии в  точках по горизонтали
  LineWy=int(LineWM*float(NumDotY));		//  толщина линии в  точках по вертикали
  LMarSize=int(LMarSizeM*float(NumDotX)); // размер полей  бюллетеня в  точках по горизонтали
  TMarSize=int(TMarSizeM*float(NumDotY)); // размер полей  бюллетеня в  точках  по вертикали
  dxStamp=int(dxStampM*float(NumDotX)); // ширина  зоны  печати в  точках
  dyStamp=int(dyStampM*float(NumDotY)); // высота  зоны  печати в  точках
  dyNumZone=int(dyNumZoneM*float(NumDotY)); // высота  зоны  печати в  точках
  dyStampNum=int(dyNumZoneM*NumDotY); // высота  зоны  размещения номера участка
  dxVoteType=int(dxVoteTypeM*float(NumDotX)); // ширина зоны типа выборов
  dyVoteType=int(dyVoteTypeM*float(NumDotY)); // высота зоны типа выборов
  dyMF=TMarSize+2*LineWy+dyVoteType; // высота зоны от начала листа до начала  зоны  заголовка
  nstrStampStartL=LineWy+dyStamp-dyStampNum; // строка начала поиска штампа
  colStampR=PageW-(LMarSize+LineWx)-2;  // колонка начала поиска штампа справа
  surw=5;   // Оценка СКО мат. ожидания  уровня  белого (среднее значение)
  surbl=5;  // Оценка СКО мат. ожидания  уровня  черного (среднее значение)

    //MAXLINES=PageH+5*TMarSize;
    MAXLINES=int(Max_Bull_Height*float(NumDotY))+5*TMarSize;

    //printf(" MAXLINES  =   %d\n",MAXLINES);

  return pr;
} //ReadSetUpFiles

unsigned GetXYValue(int xi,int yi) // получение  значения  уровня  сигнала в  точке
{ //xi  номер строки,  yi - номер столбца
   urw_ch=PixCol[xi][yi];
   return urw_ch;
}

void FormIdScan(void)
{ // формирование данных для сканирования из структур исходных данных
  // формируются после  провекрки и утверждения исходных данных и сохраняются
  // до  выключения КОИБ после окончания  голосования
	int dxR,dyR,dySh;
   // погасить переменные данные по типам голосования
   for(i=0;i <= NumVoteType;i++)
   {  NumSQMax[i]=0;
	  for(j=0;j < NumParPage ;j++)PagePar[i][j]=0;
   }
   // допустимые номера штампов для данного  участка
   // из исходных данных
   for(i=0;i < MaxNumUIK;i++)UIKNum[i]=0;

   UIKNum[0]=Votes.Nuik;         //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   // далее временные номера избирательных участков
   // переменные  параметры бюллетеня
   dySh=TMarSizeM+dyVoteTypeM+2*LineWM; // расстояние от начала листа до начала зоны штампа
    for(i=1;i <= Votes.Nblank;i++)
   if(Votes.Blank[i].ID!="0")
   { // номер бланка существует
		nVoteType=Votes.Blank[i].marker;
		PagePar[nVoteType][1]=Votes.Blank[i].Height; //297;высота бюллетеня в мм
		PagePar[nVoteType][2]=40;  //  ширина  зоны  печати в  мм
		PagePar[nVoteType][3]=int(float(Votes.Blank[i].ShAreaHeight)/10)-dySh; //  высота  зоны  печати в  мм
		PagePar[nVoteType][4]=Votes.Blank[i].Width;
        PagePar[nVoteType][5]=Votes.Blank[i].LH[1].y/10;
		PrVoteTypeId[nVoteType]=1;
		for(j=1;j <= Votes.Blank[i].NVopr;j++)
		if(Votes.Blank[i].Vopr[j].ID!="0")
		{  // подсчет количества квадратов в бюллетене всего (для всех вопросов)
		   NumSQMax[nVoteType]=NumSQMax[nVoteType]+Votes.Blank[i].Vopr[j].NCandidates;
		}
	}
	// выделение динамической памяти
	try
	{  // массив квадратов отметок голосования
	   MasSQ =new int***[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) 	   for(j=0;j < NumSQMax[i]+1;j++)
	   MasSQ[i]=new int**[NumSQMax[i]+1];
	   for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
	   MasSQ[i][j]=new int*[2];
	   for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
	   { MasSQ[i][j][0]=new int[6]; MasSQ[i][j][1]=new int[6];	   }
	   // массивы для подсчета  числа точек при анализе  квадратов
	   NumBlP=new int *[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) NumBlP[i]=new int [NumSQMax[i]+1];
	   // массив подсчета максимального  количества точек
	   NumPMax=new int *[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) NumPMax[i]=new int [NumSQMax[i]+1];
	   //  массив результатов  поиска отметок в бюллетене
	   RezRec=new int *[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) RezRec[i]=new int [NumSQMax[i]+1];
	}
	catch (std::bad_alloc)
   {  cout << "Нет  памяти. Массив zamXY ..."; }
   //  гашение  массива MasSQ
   for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
   for(k=0;k < 5;k++) { MasSQ[i][j][0][k]=0; MasSQ[i][j][1][k]=0;	   }
   //  переписывание данных из исходных в  массив  MasSQ
   // для прямого и обратного  хода бюллетеня
   for(i=1;i <= Votes.Nblank;i++)
   if(Votes.Blank[i].ID!="0")
   { // номер бланка существует
		nVoteType=Votes.Blank[i].marker;  // тип бюллетеня ( по маркеру на бюллетене)
		PageHM=PagePar[nVoteType][1];    // высота  страницы в  мм
		PageH=int(PageHM*float(NumDotY));//+0.5); // высота  страницы  в  точках
		PageWM=PagePar[nVoteType][4]; // ширина  страницы в  мм  (при загрузке ( максимальная)
		PageW=int(PageWM*float(NumDotX)); // ширина  страницы  в  точках
		PrVoteTypeId[nVoteType]=1;	dxR=PageW-2*LMarSize+LineWx;   dyR=PageH-2*TMarSize+LineWy;
		// определение максимального количества строк по типам бюллетеней
		if(MAXLINES < 1.2*PageH)MAXLINES = 1.2*PageH;
		ii=0;
		for(j=1;j <= Votes.Blank[i].NVopr;j++)
		if(Votes.Blank[i].Vopr[j].ID!="0")

		{ // NVopr - количество секций в бюллетене ( количество вопросов)
		  for(k=1; k <= Votes.Blank[i].Vopr[j].NCandidates;k++)
		  if((Votes.Blank[i].Vopr[j].Candidates[k].N!=0)&&(Votes.Blank[i].Vopr[j].Candidates[k].disabled==false))
		  {   // если не  снят с голосования. Правильно ли отсутствует идентификатор кандидата - ?
			  // параметры квадрата
			  ii=ii+1;
			  MasSQ[nVoteType][ii][0][0]=1; // есть кандидат ( не  снят)
			  dxSQ= int((float(Votes.Blank[i].Vopr[j].Candidates[k].dx)*NumDotX/10)/2);
			  dySQ =int((float(Votes.Blank[i].Vopr[j].Candidates[k].dy)*NumDotY/10)/2);
			  MasSQ[nVoteType][ii][0][3]=2*dxSQ; MasSQ[nVoteType][ii][0][4]=2*dySQ;
			  // координаты точек начала поиска квадрата относительно  рамки бюллетеня
			  MasSQ[nVoteType][ii][0][1]=int(float(Votes.Blank[i].Vopr[j].Candidates[k].SQx*NumDotY)/10)-LMarSize+int(0.5*float(LineWx))+dxSQ;
			  MasSQ[nVoteType][ii][0][2]=int(float(Votes.Blank[i].Vopr[j].Candidates[k].SQy*NumDotY)/10)-TMarSize-int(1*float(dySQ))+int(0.5*float(LineWy));
			  // сохранение  координат угла квадрата для дальнейшего использования  в  расчетах коэффициента
			  // достаточно  только  координаты  по ОУ, по ОХ коэффициент можно  считать
			  // по расстоянию между вертикальными границами рамки (функция контроля рамки)
			  MasSQ[nVoteType][ii][0][5]=int(float(Votes.Blank[i].Vopr[j].Candidates[k].SQy*NumDotY)/10)-TMarSize+int(0.5*float(LineWy));
			  nn=j;
		  }
		}
		// формирование параметров квадратов для обратного  хода бюллетеня
		if(NumSQMax[i] > 0)
		{	for(j=1;j < NumSQMax[nVoteType]+1;j++)
			{ MasSQ[nVoteType][j][1][1]=dxR-MasSQ[nVoteType][j][0][1];
			  MasSQ[nVoteType][j][1][2]=dyR-MasSQ[nVoteType][j][0][2]- int(4*float(dySQ));
			  MasSQ[nVoteType][j][1][3]=MasSQ[nVoteType][j][0][3];
			  MasSQ[nVoteType][j][1][4]=MasSQ[nVoteType][j][0][4];
			  if(j==18)
			  nn=j;
			  // для  расчета  коэффициента  сжатия по вертикали
			  MasSQ[nVoteType][j][1][5]=dyR -MasSQ[nVoteType][j][0][5]-2*float(dySQ);
			  // для прямого хода расстояние от правой границы рамки
			  MasSQ[nVoteType][j][0][1]=dxR-MasSQ[nVoteType][j][0][1];
			}
		}
	}
	// выделить память  под  массив  для  сканирвоания


	PrIdOk=1; // признак того, что исходные данные для сканирования переписаны в рабочин ячейки
} //FormIdScan


int FormRezScanBull(void)
{ // определение результатов сканирования бюллетеня
   // 999 int NumV, MaxMarks;
   int pr1=0;

  for(i=1;i <= Votes.Nblank;i++)
   if(Votes.Blank[i].ID!="0")
   {  if(nVoteType==Votes.Blank[i].marker)
	  { // распознанный бюллетень относится к бланку
		// общие данные распознанного бюллетеня
		pr1=111;
		NumV=Votes.Blank[i].NVopr;
		MasScan.MasRecOut[0]=pr1; //  признак окончания сканирования бюллетеня
		MasScan.MasRecOut[1]=i;   // номер типа бланка распознанного бюллетеня
		MasScan.MasRecOut[2]=NumUIKRec; // номер штампа на бюллетене
		MasScan.MasRecOut[3]=NumV;
		ii=0;  MaxMarks=0;
		for(j=1;j <= NumV;j++)
		if(Votes.Blank[i].Vopr[j].ID!="0")
		{ //  j - номер вопроса в  бюллетене
		  MaxMarks=Votes.Blank[i].Vopr[j].maxMarks;//NumSQMaxVopr[nVoteType][j][0];
		  MasScan.MasVoprOut[j][0]=MaxMarks;
		  summa=0;
		  MasScan.MasCandOut[j+1][0]=Votes.Blank[i].Vopr[j].NCandidates;//NumSQMaxVopr[nVoteType][j][1]; // количество кандидатов в вопросе
		  for(k=1; k <= Votes.Blank[i].Vopr[j].NCandidates;k++)
		  if((Votes.Blank[i].Vopr[j].Candidates[k].N!=0)&&(Votes.Blank[i].Vopr[j].Candidates[k].disabled==false))
		  { // формирование выходных данных по кандидатам
			ii=ii+1;
			// подсчет количества отметок по вопросу
			if(RezRec[nVoteType][ii]==1)summa=summa+1;
			MasScan.MasVoprOut[j][2]=summa;
			// проверка и перезапись
			MasScan.MasCandOut[j][k]=RezRec[nVoteType][ii];
		  }
		  // определение недействительных бюллетеней, проверяем по всем вопросам (выборам)
		  // данного бюллетеня. Если послен просмотра всех выборов в нулевой ячейке останется "0"
		  // то бюллетень действительный
		  if(summa==0) MasScan.MasVoprOut[j][1]=-1;//недействительный, нет отметок
		  else
		  {  if(summa > MaxMarks) MasScan.MasVoprOut[j][1]=-2;  //недействительный, много отметок
			 else MasScan.MasVoprOut[j][1]= i;  //   бюллетень действительный
		  }
		}
	  }
   }
   return pr1;
} //FormRezScanBull

void GashBeforeScan(void)
{
  KoefY=0.985; KoefX=1.0;   PrKoefY=0;
  NstrFrControl=NumDotY+1;
  PrKoef=0;
  NumDotKoefY=0;  // счетчик  числа  расчетов коэффициента сжатия

  for(j=0;j < 4 ;j++) MasScan.MasRecOut[j]=0;
  for(j=0;j <= NumVoprMax;j++)
  { MasScan.MasVoprOut[j][0]=0; MasScan.MasVoprOut[j][1]=0;
	for(k=0; k <= NumCandMax; k++)  MasScan.MasCandOut[j][k]=0;
  }
  // гашение массивов определения типа бюллетеня
  for(i=0;i < NumVoteType+1; i++)
  { PrVoteTypeTop[i]=0; nstrVoteType[i]=0;
	PrVoteType[i]=0; nbVoteT[i]=0; ncVoteType[i]=0;
	PrVoteTypeStart[i]=0;
  }
  zz=int(dxVoteType/2);
  for(i=0;i < NumVoteType+1; i++) ncVoteType[i]= LMarSize+zz+dxVoteType*i;
  for(i=0;i < NumVoteType+1; i++) nstrVoteType[i]=int(float(TMarSize)/2);
  dyy= int(dyVoteType/2+0.5);
  nstrBegVoteType=0;
  ncL=0;// номер столбца для  поиска левой границы листа
  ncR=NumStrScan-1;   // начальная точка строки для поиска правой границы
  ncTL=130; // номер столбца для поиска верха листа слева
  ncTR=NumStrScan-ncTL; // номер столбца для поиска правой верхней части листа
  dxT=10; dxL=2*LMarSize; PrLeft=0; PrTopL=0;PrTopR=0;PrRight=0;
  nbTL=0;nbLL=0;nbRR=0; nbTR=0;  BulHead=0; AlfaPage=0; PrPage=0; PrFrame=0;
  PrFirstL=0; PrFirstR=0; PrXMin=0; PrXMax=0; XoShR1=0; YoShR1=0; PrAlfaShR=0;
  XoMax=0; YoMax=0; nbXoMax=0; PrPageEnd=0;
  PrAlfa=0;
  MAXLINES=int(Max_Bull_Height*float(NumDotY))+5*TMarSize;
} //GashBeforeScan

 double erf(double z)
{ // Интеграл  вероятностей
  const float pi = 3.14159;
  int pr=0; double x;
  if (z> 20.0) z=20; else if (z < -20)z=-20;
  if (z > 0.0) pr=1;  if (z < 0.0) pr=-1;
  z=-2.0*z*z/pi; z=1-exp(z); if(z >= 0)x=sqrt(z);
  if (pr<0) x=-x;
  return x;
} // erf

 double erf1(double z)
{ // Обратная функция лапласа
  const float pi = 3.14159;
  double x;
  x=1-z*z;
  if(x > 0.0001)
  { x=pi/2*(-log(x));
	if(x > 0 ) x=sqrt(x); else x=0;
  } else x=0;

  return x;
}// erf1


int RecBlack(float alfa,float beta,float urw,float surw,float urbl,float surbl,
			 float urs,float* p00,float* p11)
{ // определение того,  что пиксед имеет черный цвет
  float sigma,gam,u1,u2,aa,bb,dd;//,p00,p11,;
  int g0,g1;
  dd=1-sqrt(alfa);   gam=erf1(dd);
  sigma=sqrt(surw*surw+surbl*surbl);
  gam=erf1(dd)*sigma;   gam=0;
  aa=(1-beta)/alfa;  bb=beta/(1-alfa);  dd=urbl;
  u1=1;  u2=(urs-gam-urbl)/sigma/1.4;
  *p00=0.5*(1-erf(u2));  		// вероятность того,  что это черная точка
  u1=(urs+gam-urw)/sigma/1.4;   // u2=(urs-gam-urw)/sigma/1.4;
  u2=(gam+dd)/sigma/1.4;
  *p11=0.5*(1+erf(u1));  		// вероятность того,  что это белая точка
  // принятие  решения о том, что точка  черная
  g0=0; g1=0;
  if  (*p00 >= *p11*bb)  { g0=1;  }else
  if (*p00 >= *p11*aa)g1=1;
  if (g0==0 && g1 == 0) { if (*p00 >= *p11*(aa+bb)/2)g0=1;else g1=1;   }
  return g0;
}//RecBlack

//---------------------------------------------------------------------------
void BLev(int nn,unsigned urw_ch,float* ur,float* sur)
{ // функция определения  среднего  уровня сигнала и СКО
  //  путем последовательного накопления
	int urw1;  float dd;
	urw1=urw_ch; dd=*ur;
	if(nn>1)
	{ *ur=((*ur)*(nn-1)+urw1)/nn;
	  *sur=sqrt((*sur)*(*sur)+((dd-(*ur))*(dd-(*ur)))/nn);
	} else { *ur=urw1;  *sur=1;}
}// функция определения  среднего  уровня сигнала и СКО

void EvChangeColCol(int BlWt,int nstr,int step,int ncT,int dxT,int* nbT,int* yo,int* prT)
  // BlWt - признак направления смены цвета:
  //         "0"  - с  черного  на  белый
  //         "1"  - с  белого на  черный
  // nstr - текущий номер строки
  // step - значение  шага  при определении  среднего значения или при поиске
  //       левой (1) или правой (-1)  границы листа
  // ncT - номер колонки (столбца) по которому  определяется наличие поля страницы
  // dxT - количество  точек в  строке, по которым определяется
  //      среднее  значение сигнала  в  данной  строке
  // nbT - количество точек подряд, определнных как белые
  // yo - номер строки, с  которой  начинается  бюллетень (верхняя граница)
  // prT - признак того,  что левая  граница  бюллетеня найдена

  //       левой (1) или правой (-1)  границы листа
{ // определение  начала  верхней (нижней) части страницы
  int i; float dd;
  if (*prT==0) // поиск верха листа, если он  еще  не  найден
  {  *yo=0;
	 dd=0;
	 if(step <0)dxT=-dxT;
	 if(step>0)
	 {  // поиск слева направо
		for(i=ncT;i < ncT+dxT;i=i+step)
		{ // определение  среднего значения интенсивности сигнала по стробу строки (dxT)
//---------------Обращение  к  массиву точек-----------------------------------
		  urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
		  dd=dd+urw_ch;
		 }
		 dd=dd/dxT; // среднее значение  уровня  сигнала
		 // обращение  к функции  принятия решения о наличии  черного  в  точке
		 g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
		 if(BlWt==0) { if(g0==0) *nbT=*nbT+1; else *nbT=*nbT-1; };
		 if(BlWt==1) { if(g0==1) *nbT=*nbT+1; else *nbT=*nbT-1; }
		 if(*nbT <0) *nbT=0;
		 if(*nbT >2)
		 { // уже третья подряд строка  является  белой, тогда принимается
			// решение о  том,  что начало листа  найдено
			*yo=nstr- *nbT;  *prT=1;
		}
	 }
	 if(step < 0)
	 {  // поиск слева направо
		for(i=ncT;i > ncT+dxT;i=i+step)
		{ // определение  среднего значения интенсивности сигнала по стробу строки (dxT)
//---------------Обращение  к  массиву точек-----------------------------------

		  urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
		  dd=dd+urw_ch;
		 }
		 dd=dd/dxT; // среднее значение  уровня  сигнала
		 // обращение  к функции  принятия решения о наличии  черного  в  точке
		 g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);

		 if(BlWt==0) {if(g0==0) *nbT=*nbT+1; else *nbT=*nbT-1;}
		 if(BlWt==1) {if(g0==1) *nbT=*nbT+1; else *nbT=*nbT-1; }
		if(*nbT <0) *nbT=0;
		 if(*nbT >2)
		 { // уже третья подряд строка  является  белой, тогда принимается
			// решение о  том,  что начало листа  найдено
			*yo=nstr- *nbT;  *prT=1;
		}
	 }
  }// конец поиска верха  листа
} // PageTop

void EvChangeColRow(int BlWt,int nstr,int step,int ncL,int dxL,int* nbL,int* xo,int* prL)
  // BlWt - признак направления смены цвета:
  //         "0"  - с  черного  на  белый
  //         "1"  - с  белого на  черный
  // nstr - текущий номер строки
  // step - значение  шага  при определении  среднего значения или при поиске
  //       левой (1) или правой (-1)  границы листа
  // ncL - номер колонки (столбца) от которой начинается  поиск левой границы листа
  // dxL - количество  точек в  строке, которые определяют строб для поиска левой
  //      или правой  границы листа
  // nbL - количество точек подряд, определнных как белые
  // xo - номер столбца, с  которого  начинается  левая граница бюллетеня
  // prL - признак того,  что левая  граница  бюллетеня найдена
  // step - значение  шага  при определении  среднего значения или при поиске
  //       левой (1) или правой (-1)  границы листа
{ // определение  начала  верхней (нижней) части страницы
  int i,ii; float dd;
  if (*prL==0) // поиск левой  границы  листа, если она еще не  найдена
  {  *xo=0;
	 dd=0;
	 *nbL=0;
	 if((step) > (int(0)))ii=dxL; else ii=-dxL;
	 if(step>0)
	 {  // поиск слева направо
		for(i=ncL;i < ncL+ii;i=i+step)
	   {//---------------Обращение  к  массиву точек---------

			urw_ch=GetXYValue(nstr,i);

		//---------------------------------------------------
			dd=urw_ch;
			// обращение  к функции  принятия решения о наличии  черного  в  точке
			g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
			if(BlWt==0) { if(g0==0) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(BlWt==1) { if(g0==1) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(*nbL <0) *nbL=0;
			if(*nbL >2)
			{ // уже третья подряд точка строки  является  белой (черной), тогда принимается
			  // решение о  том,  что левая сторона найдена
			  *xo=i-(*nbL);   *prL=1; goto fin1;
			}
		};
	 }
	 if(step < 0)
	 {  //  поиск справа налево
		for(i=ncL;i > ncL+ii;i=i+step)
		{
//---------------Обращение  к  массиву точек-----------------------------------

			urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
			dd=urw_ch;
			// обращение  к функции  принятия решения о наличии  черного  в  точке
			g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);

			if(BlWt==0) { if(g0==0) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(BlWt==1) { if(g0==1) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(*nbL <0) *nbL=0;
			if(*nbL >2)
			{ // уже третья подряд точка строки  является  белой (черной), тогда принимается
			  // решение о  том,  что левая сторона найдена
			*xo=i+(*nbL);  *prL=1; goto fin1;
			}
		}
	 }
	 fin1:;
  }// конец поиска левого края листа
} // PageLeft

void EvChangeColRowSh(int BlWt,int nstr,int step,int ncL,int dxL,int* nbL,int* xo,int* prL)
  // BlWt - признак направления смены цвета:
  //         "0"  - с  черного  на  белый
  //         "1"  - с  белого на  черный
  // nstr - текущий номер строки
  // step - значение  шага  при определении  среднего значения или при поиске
  //       левой (1) или правой (-1)  границы листа
  // ncL - номер колонки (столбца) от которой начинается  поиск левой границы листа
  // dxL - количество  точек в  строке, которые определяют строб для поиска левой
  //      или правой  границы листа
  // nbL - количество точек подряд, определнных как белые
  // xo - номер столбца, с  которого  начинается  левая граница бюллетеня
  // prL - признак того,  что левая  граница  бюллетеня найдена
  // step - значение  шага  при определении  среднего значения или при поиске
  //       левой (1) или правой (-1)  границы листа
{ // определение  начала  верхней (нижней) части страницы
  int i,ii; float dd;
  if (*prL==0) // поиск левой  границы  листа, если она еще не  найдена
  {  *xo=0;
	 dd=0;
	 *nbL=0;
	 //ur2bl=urbl; ur2w=urw;
	 if((step) > (int(0)))ii=dxL; else ii=-dxL;
	 if(step>0)
	 {  // поиск слева направо
		//ur1w=0; ur1bl=0; nbl=0; nw=0;
		for(i=ncL;i < ncL+ii;i=i+step)
	   {//---------------Обращение  к  массиву точек---------

			urw_ch=GetXYValue(nstr,i);

		//---------------------------------------------------
			dd=urw_ch;
			// обращение  к функции  принятия решения о наличии  черного  в  точке
			g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);

			if(BlWt==0) { if(g0==0) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(BlWt==1) { if(g0==1) *nbL=*nbL+1; else *nbL=*nbL-1;}
		  /*	if (i>ncL)
			{ if(g0==1)
				{ ur1bl=ur1bl+dd; nbl=nbl+1; urbl=ur1bl/nbl;
				} else
				{ ur1w=ur1w+dd; nw=nw+1; urw=ur1w/nw;

                }
			}
		  */
			if(*nbL <0) *nbL=0;
			if(*nbL > 2)
			{ // уже третья подряд точка строки  является  белой (черной), тогда принимается
			  // решение о  том,  что левая сторона найдена
			  *xo=i-(*nbL);   *prL=1; goto fin1;
			}
		};
	 }
	 if(step < 0)
	 {  //  поиск справа налево
		for(i=ncL;i > ncL+ii;i=i+step)
		{
//---------------Обращение  к  массиву точек-----------------------------------

			urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
			dd=urw_ch;
			// обращение  к функции  принятия решения о наличии  черного  в  точке
			g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
					   //	if(dd > 85) g0=0;
			if(BlWt==0) { if(g0==0) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(BlWt==1) { if(g0==1) *nbL=*nbL+1; else *nbL=*nbL-1;}

			if(*nbL <0) *nbL=0;
			if(*nbL >2)
			{ // уже третья подряд точка строки  является  белой (черной), тогда принимается
			  // решение о  том,  что левая сторона найдена
			*xo=i+(*nbL);  *prL=1; goto fin1;
			}
		}
	 }
	 fin1:;
	 //urbl=ur2bl; urw=ur2w;
  }// конец поиска левого края листа
} // ColRowSh

int FindPageStart(int* PrPage)
{  //  функция  поиска начала  листа бюллетеня
	 int PrErr=0;
	 if(nstr > 10*TMarSize)
	 PrErr=1;
	 if(PrTopL==0)
	 {  //определение верхней границы слева
		//Step=int(1);
		EvChangeColCol(0,nstr,1,ncTL,dxT,&nbTL,&PgYoL,&PrTopL);
		if (PrTopL==1)
		{   // nr1 - номер  строки  для уочнения  уровня  черного
			// nr2 - номер  строки  для  уточнения  уровня белого
			nstrL=PgYoL+5;//+20;
			nr1=0;  nr2=nstrL;
			if(PgYoL > 10) nr1=5; else nr1=0; nr2=nstrL;
			if(PgYoL > 10)
			// уточнение оценки уровня  черного
			{ 	nn=1;  urbl=0; nr1=5; nc1=100; nc2=500;
				for (j = nr1; j <= nr1+3; j++)
				{  for (i = nc1; i <= nc2; i++)
				  {
//-------------------здесь нужно обратиться  к  массиву  точек строки-------

					urw_ch=GetXYValue(nr1,i);

   //---------------------------------------------------
					 // определение  среднего уровня черного
					 BLev(nn,urw_ch,&urbl,&surbl);
					 // переход к  следующей  точке
					 nn=nn+1;
				  }
				};
				if(surbl <5) surbl=5;
			};
			goto fin1;
		}
	 }
	 fin1:;
	 if((nstr > nstrL)&&(PrTopL==1)&&(PrLeft==0))
	 {  // определение левой границы
		 EvChangeColRow(0,nstr,1,ncL,dxL,&nbLL,&PgXoL,&PrLeft);
		   if(PrLeft==1)  // уточнение уровней по  40 точкам строк
		   {  nc1=PgXoL+20; nc2=PgXoL+60; nr2=nstr;
			  //  оценка  уровня  белого
			  nn=1;  urw=0;
			  {  for (i = nc1; i <= nc2; i++)
				 {  // определение  уровня  интенсивности пикселя
//-----------------------здесь нужно обратиться  к  массиву  точек строки-------

					urw_ch=GetXYValue(nstr,i);

					//---------------------------------------------------
					// определение  среднего уровня черного
					BLev(nn,urw_ch,&urw,&surw);
					//	 переход к  следующей  точке
					nn=nn+1;
				 }
			  };
				if(surw <15) surw=15;
		   }
	 }
	 // определение координат верхней правой  части  листа
	 if(PrTopR==0)
	 {  //определение верхней границы справа
		EvChangeColCol(0,nstr,1,ncTR,dxT,&nbTR,&PgYoR,&PrTopR);
		if (PrTopR==1)
		{ nstrR=PgYoR+5;  goto fin11;	}
	 }
	 fin11:;
	 if ((nstr > nstrR)&&(PrTopR==1)&&(PrRight==0))
	 {  // определение правой границы
		   EvChangeColRow(0,nstr,-1,ncR,dxL,&nbRR,&PgXoR,&PrRight);
	 }
	  // формирование признака  окончания  поиска   начала  листа PrPage=1
	  if((PrTopR==1)&&(PrRight==1)&&(PrTopL==1)&&(PrLeft==1)) *PrPage=1;
	 if(*PrPage==1)
	 { // начало листа определено, далее  идет  привязка  к  углу  рамки
		 // здесь формируются  начальные значения  номеров строк и столбцов  для
		 //  поиска  координат  углов  рамки, которые  записываются  на  места
		 //  координат  кглов  листа, поскольку  в  дальнейшем  нужно  все  будет
		 //  отмерять относительно  рамки
		 nstrHeaderEnd=0;
		 dxT=10; dxL=LMarSize;   nbTL=0;nbLL=0;nbRR=0; nbTR=0;
		 PrLeft=0; PrTopL=0;PrTopR=0;PrRight=0;
		 for(i=0;i < NumVoteType+1; i++)PrVoteType[i]=0;
         //  вычисление  угла  наклона  листа AlfaPage
		 dyALfa=PgYoR-PgYoL; dxALfa=PgXoR-PgXoL;
		 AlfaPage=(float(dyALfa)*float(NumDotY))/(float(dxALfa)*float(NumDotX));
		 SinAlfaPage=sin(AlfaPage); CosAlfaPage=cos(AlfaPage);
		 TanAlfaPage=SinAlfaPage/CosAlfaPage;
		 // формиование данных для начала поиска рамки бюллетеня
		 ncL=PgXoL+int(0.5*float(LMarSize)); // номер столбца для  поиска левой границы рамки в  строке
		 ncR=PgXoR-int(0.5*float(LMarSize)); // начальная точка строки для поиска правой границы рамки
		 ncTL=PgXoL+int(1.5*float(LMarSize)); // номер столбца для поиска верха листа слева
		 ncTR=PgXoR-int(1.5*float(LMarSize));// номер столбца для поиска правой верхней части листа
		 nstrL=PgYoL +int(0.5*float(TMarSize));
		 nstrR=PgYoR +int(0.5*float(TMarSize));

		 if(nstrR > nstrL)nstrUrWhite=nstrR+1; else nstrUrWhite=nstrL+1;//int((float(nstrL)+ float(nstrR))/2);
		 //  формирование колонок и строк для определения типа выборов с учетом угла наклона

		 // уточнение номера строки для поиска направления бюллетеня
		 // признака типа бюллетеня  с  учетом угла наклона листа
		 for(i=0;i < NumVoteType+1; i++)
		 {
			//nstrVoteType[i]=PgYoL+nstrVoteType[i]-2*NumDotY/*LineWy*/ +int(AlfaPage*float(ncVoteType[i]));
			nstrVoteType[i]=PgYoL+nstrVoteType[i]+int(AlfaPage*float(ncVoteType[i]));
			PrVoteTypeStart[i]=0;
			ncVoteType[i]=ncVoteType[i]+PgXoL;
		 }
		 ii=nstrVoteType[0];
		 if (ii > nstrVoteType[9]) ii=nstrVoteType[9];
		 nstrFrYmax = nstrVoteType[0];
		 if (nstrFrYmax < nstrVoteType[9]) nstrFrYmax=nstrVoteType[9];
		 nstrFrYmax=nstrFrYmax+LineWy*2+dyVoteType+4; // 4 точки на  всякий  случай
		 nstrBegVoteType= ii-2;
		 dyy=dyy + fabs(PgYoR-PgYoL);
		 // формирование данных для проверки конца листа
		 nstrPageEnd=PgYoL;
		 if(PgYoL > PgYoR)nstrPageEnd=PgYoL; else nstrPageEnd=PgYoR;

		 nstrPageEnd=nstrPageEnd+int(297*float(NumDotY))-TMarSize;
		 nstrLsStart=nstrVoteType[0]+dyVoteType+2*NumDotY;
		 nstrRsStart=nstrVoteType[NumVoteType]+dyVoteType+2*NumDotY;

	 } // PrPage=1
   return PrErr;
}// FindPageStart;

int FindFrame(int* PrFrame)
{ // поиск координат  начала  рамки
	 int PrErr=0;
	 if ((nstr>(nstrL+2*TMarSize))&&(nstr>(nstrR+2*TMarSize)))
	 { PrErr=1; goto fin;}
	 if((PrTopL==0)&&(nstr > nstrL))
	 {  //определение верхней границы рамки слева
		EvChangeColCol(1,nstr,1,ncTL,dxT,&nbTL,&PgYoL,&PrTopL);
		if (PrTopL==1){	nstrL=PgYoL+6;	goto fin1f;	}
	 }
	 fin1f:;
	 if((nstr > nstrL)&&(PrTopL==1)&&(PrLeft==0))
	 { //XoL=PgXoL;
	   EvChangeColRow(1,nstr,1,ncL,dxL,&nbLL,&PgXoL,&PrLeft);
	 }
		// определение координат верхней правой  части  рамки
	 if((PrTopR==0)&&(nstr > nstrR))
	 {  //определение верхней границы справа
		EvChangeColCol(1,nstr,1,ncTR,dxT,&nbTR,&PgYoR,&PrTopR);
		if (PrTopR==1) { 	nstrR=PgYoR+6;	goto fin11f;	}
	 }
	 fin11f:;
	 if ((nstr > nstrR)&&(PrTopR==1)&&(PrRight==0))
	 {  // определение правой границы рамки
		   EvChangeColRow(1,nstr,-1,ncR,dxL,&nbRR,&PgXoR,&PrRight);
	 }
		// формирование признака  окончания  поиска   начала  листа PrPage=1
	  if((PrTopR==1)&&(PrRight==1)&&(PrTopL==1)&&(PrLeft==1))
	  { *PrFrame=1;
		// уточнение  угла наклона  листа
		dyALfa=PgYoR-PgYoL; dxALfa=PgXoR-PgXoL;
		AlfaPage=(float(dyALfa)*float(NumDotY))/(float(dxALfa)*float(NumDotX));
	  }
	  fin:;
	return PrErr;
} //FindFrame

int FrameControl(void)
{ // контроль наличия  рамки  слева или справа в процессе  сканирования
	 int PrErr=0,nbK,XoK,PrK;
	 // контрольлевой границы рамки
	 LsPr=0; RsPr=0;
	 if(nstr==1081)
	 RsPr=0;
	 if (EndYoL==0)
	 {  EvChangeColRow(1,nstr,1,LsCol,Lsdx,&nbLs,&LsXo,&LsPr);
		if(LsPr==0)
		{  //  подсчет  числа подряд идущих строк, где не  найдена  граница рамки
			LsNs=LsNs+1;
		} else
		{ LsNs=0;
		  LsCol=LsXo-2*NumDotX; //EndXoL=LsXo;
		  if(PrEndXoL==0){ EndXoL=LsXo; PrEndXoL=1;}
		  //if(fabs(EndXoL-LsXo) < LineWy)EndXoL=LsXo;
		  //if ((LsXo-EndXoL) > LineWy) PrErr=11;
		  EndXoL=LsXo;
		  Lsdx=5*NumDotX;
		  // поиск первой линии для расчета коэффициента сжатия при прямом ходе
		  if((BulHead==0)&&(PrKoef==0)&&(nstr > PgYoL+2*dyVoteType))
		  { nbK=0;XoK=0;PrK=0;
			EvChangeColRow(0,nstr,1,LsXo+3,2*NumDotX,&nbK,&XoK,&PrK);
			if(PrK==0)
			{  // линия найдена расчет коэффициента
				PrKoef=1;
				if(dyLine1>1)
				{
				    float KoefYtek=float(nstr-PgYoL)/float(dyLine1-TMarSize)/CosAlfaPage;
				    KoefY=(KoefY+KoefYtek)/2;
				    cout << " Koefficient of Line 1 = " << KoefY << endl;
				   NumDotKoefY=1;
                }
            }
          }
		}

		if(LsNs>=2*NumDotY)PrErr=11;
	 }
	 //  контроль правой  границы  рамки
	 if(EndYoR==0)
	 {  EvChangeColRow(1,nstr,-1,RsCol,Rsdx,&nbRs,&RsXo,&RsPr);
		if(RsPr==0)
		{  //  подсчет  числа подряд идущих строк, где не  найдена  граница рамки
			RsNs=RsNs+1;
		} else
		{ RsNs=0;  RsCol=RsXo+2*NumDotX;
		  //if ((RsXo-EndXoR) > LineWy) PrErr=12;
		  EndXoR=RsXo;
		  Rsdx=5*NumDotX;
		}
		if (RsNs>=2*NumDotY)
		{ if(PrErr!=0) PrErr=10; else PrErr=12;
		}
	 }
	 if((RsNs==0)&&(LsNs==0))
	 { // уточнение коэффициента сжатия по оси ОХ
	   //if(nstr > dyStampArea)
	   //{
	   dyALfa=nstr-PgYoL; dxALfa=LsXo-PgXoL;
	   AlfaPage=-float(dxALfa)/float(dyALfa);
	   SinAlfaPage=sin(AlfaPage); CosAlfaPage=cos(AlfaPage);
	   TanAlfaPage=SinAlfaPage/CosAlfaPage;
       //}
	 }
  /*	 if((LsNs>=2*NumDotY)||(RsNs>=2*NumDotY))
	 { if((LsNs>=2*NumDotY)&&(RsNs>=2*NumDotY))PrErr=10;
		else { if(LsNs>=2*NumDotY) PrErr=11;else PrErr=12;  }
	 } else  PrErr=0;
  */
	return PrErr;
} //FrameControl

int FindShAreaStart(void)
{ // поиск левой тогчки начала зоны штампа по вертикали
  //при обратном ходе бюллетеня
	 int PrErr=0,dx=0,dy=0;
	 // поиск начала зоны штампа по венртикали
	 if(nstr ==4070)
	 dx=0;
	 if(nstr > nsShArStart*KoefY)
	 {  //определение верхней границы рамки слева
		colShAr=LsXo+2*NumDotX;
		EvChangeColCol(1,nstr,1,colShAr,10,&nbShAr,&ShArYo,&ShArPr);
		if (ShArPr==1)
		{ 	ShArXo= LsXo;
			//	уточнение угла наклона листа
			dyALfa=ShArYo-PgYoL; dxALfa=ShArXo-PgXoL;
			AlfaPage=-float(dxALfa)/float(dyALfa);
			SinAlfaPage=sin(AlfaPage); CosAlfaPage=cos(AlfaPage);
			TanAlfaPage=SinAlfaPage/CosAlfaPage;
			// формирование точек начала поиска штампа
			colStampLo=ShArXo+int(1.5/*1.3*/*float(NumDotX/*LineWx*/));
			nstrStampStartL=ShArYo+int(2.0/*1.8*/*float(NumDotX/*LineWy*/));
			dx=int(float(dxStampArea-int(0.5/*0.3*/*float(NumDotX)))*CosAlfaPage);
			colStampRo=ShArXo+dx;
			dy=dxStampArea*SinAlfaPage;
			nstrStampStartR=nstrStampStartL+dy;
		}
	 }
	return PrErr;
} //FindShAreaStart

int FindVoteTypeTop(int *PrPg)
{ //  поиск направления бюллетеня и типа выборов
  int Err=0,ii;
  if(nstr > nstrBegVoteType)
  {  dxT=30;
	 for(i=0;i < NumVoteType+1; i++)
	 {  if((nstr > nstrVoteType[i])&&(nstr < nstrVoteType[i]+TMarSize))//dyVoteType+48))
		{  if (PrVoteTypeStart[i]==0)
		   { EvChangeColCol(1,nstr,1,ncVoteType[i],dxT,&nbVoteT[i],&ii,&PrVoteTypeStart[i]);
			 if (PrVoteTypeStart[i]==1)
			 {
				nstrVoteType[i]=nstr-2*LineWy;
				nbVoteT[i]=0;
			 }
		   }
		}
		if((nstr > nstrVoteType[i])&&(nstr < nstrVoteType[i]+dyVoteType))
		{ if ((PrVoteTypeStart[i]==1)&&(PrVoteTypeTop[i]==0))
		   {
			   EvChangeColCol(0,nstr,1,ncVoteType[i],dxT,&nbVoteT[i],&ii,&PrVoteTypeTop[i]);
		   }
		}
	  }  // i
	  ii=0;
	  for(i=0;i < NumVoteType+1; i++)
	  if ((nstr > nstrVoteType[i]+dyVoteType)&&(PrVoteTypeStart[i]==1)) ii=ii+1;
	  if(ii ==(NumVoteType+1))
	  {
		 for(i=0;i < NumVoteType+1; i++)
		  if(PrVoteTypeTop[i]==0)PrVoteTypeTop[i]=1; else  PrVoteTypeTop[i]=0;
		 j=0;
		 for(i=1;i < NumVoteType+1; i++) if(PrVoteTypeTop[i]==0)j=j+1;
		 if(j==(NumVoteType))Err=2; else
		 {  if(j< (NumVoteType-1)) Err=3; else
			{ *PrPg=2; 	if(PrVoteTypeTop[0]==1) BulHead=0; else BulHead=1; }
		 }
	  }
	  //finVote:;
	  //return Err;
  } //nstr > nstrBegVoteType
  return Err;
}   // FindVoteTypeTop

int FindVoteTypeBot(int *PrPg)
{ //  поиск типа выборов в  конце листа
  int Err=0,ii=0,jj=0,j=0,dx=0;
  if((nstr > nstrBegVoteType)&&(*PrPg==3))
  { dxT=20;
	dx=fabs(nstrVoteType[0]-nstrVoteType[NumVoteType]);
	 for(j=nstr;j > nstr-2*TMarSize-dx; j--)
	 { // поиск начала рамки снизу вверх
		jj=0;
		for(i=0;i < NumVoteType+1; i++)
		{  //if((nstr > nstrVoteType[i])&&(nstr < nstrVoteType[i]+dyVoteType+48))
		   {  if (PrVoteTypeStart[i]==0)
			  {
				EvChangeColCol(1,j,1,ncVoteType[i],dxT,&nbVoteT[i],&ii,&PrVoteTypeStart[i]);
				if (PrVoteTypeStart[i]==1)
				{ //dy=nstrVoteType[i]-j;
				  nstrVoteType[i]=j+LineWy;
				  nbVoteT[i]=0;
				}
			  }
			}
			if((j < nstrVoteType[i])&&(j > nstrVoteType[i]-dyVoteType/*- int(0.5*float(NumDotY))*/))
			{ if ((PrVoteTypeStart[i]==1)&&(PrVoteType[i]==0))
			  {
				EvChangeColCol(0,j,1,ncVoteType[i],dxT,&nbVoteT[i],&ii,&PrVoteType[i]);
			  }
			}
		}  // i
		ii=0;
		for(i=0;i < NumVoteType+1; i++)
		if ((j < nstrVoteType[i]-dyVoteType)&&(PrVoteTypeStart[i]==1)) ii=ii+1;
		if(ii ==(NumVoteType+1))
		{
		  for(i=0;i < NumVoteType+1; i++)
		  if(PrVoteType[i]==0)PrVoteType[i]=1; else  PrVoteType[i]=0;
		  jj=0;
		  for(i=0;i < NumVoteType; i++) if(PrVoteType[i]==0)jj=jj+1;
		  if(jj==(NumVoteType)) { Err=2; goto finVote; }
		  else
		  {  if(jj< (NumVoteType-1)) { Err=3; goto finVote;}
			 else  { *PrPg=4;  goto finVote; }
		  }
		}
	 }
     	ii=0;
		for(i=0;i < NumVoteType+1; i++)
		if ((j < nstrVoteType[i]-dyVoteType)&&(PrVoteTypeStart[i]==1)) ii=ii+1;
		if(ii !=(NumVoteType+1))Err=22; // не  найдена нижняя  граница  рамки листа
	 finVote:;
  } //nstr > nstrBegVoteType
  return Err;
}   // FindVoteTypeBot

int TestPageEnd(int* PrPg)
{ //  функция поиска конца  рамки листа
	int PrErr=0;
	int yy=0,pr=0;
	 if(nstr > nstrPageEnd)
	 {  //определение окончания страницы
		EvChangeColCol(1,nstr,1,colTestPageL,dxTestPage,&nbTL,&yy,&pr);
		if (pr==1)
		{  *PrPg=1;		}
	 }
	// проверка того, что говорит о том, что идет второй лист продряд
	if(nstr > nstrPageEnd+ 2*TMarSize) PrErr=8;
	return PrErr;
} // TestPageEnd1


int FindNumYchTop(int* PrSt)
// PrSt - признак нахождения координат штампа
{ // функция определения  координат области номера УИК
  // пока только  по  левой  стороне штампа
   int Err=0,PrL=0,PrR=0;
   int XoL,XoR,YoL,YoR,j,ij,xmin,xmax,sumx;
   float dx;
   if ((nstr > nstrStampEndL+30)&&(*PrSt==0))
   { if((PrShL==0)&&(PrShR==0)) {  Err=4; goto fin; }
   }
   if(nstr > nstrStampStartL)
   { // и длину зоны поиска для левого и правого угла с  учетом
	 // угла наклона листа
	 dx=float(nstr-nstrStampStartL)*TanAlfaPage;
	 colStampL=colStampLo-dx;
	 colStampL=RsXo-dxStampArea+int(1.5*float(NumDotX));
	 colStampR=colStampRo-dx;
	 colStampR=RsXo-int(3.5*float(NumDotX));
	 dxStFindL=int(0.4*float(dxStampArea)); // количество точек для  поиска границ штампа
	 dxStFindR=dxStFindL;

	 dynstr=nstr-nstrStampStartL;
	 if(PrShL==0)
	 {  // определение координат левой стороны штампа
		if(PrFirstL!=0)  // уточнение строба поиска линии
		{ dx=XoSh1 - colStampL;  dxStFindL=dx + 3*NumDotX; 	}

		EvChangeColRowSh(1,nstr,1,colStampL,dxStFindL,&nbShL,&XoL,&PrL);
		PrXoLSh=PrL;
		if(PrL==1)
		{  XoLSh=XoL;
			if(colStampL>=XoL) // граница штампа слева сливается с рамкой бюллетеня
			{   cout << " Left side  Error nstr =  "<<nstr<<endl;
			     PrShL=-1; goto finShL; }
			//dxStFindL=XoL-colStampL+2*NumDotX;

			if(PrFirstL==0)
			{ // запоминание  начальной  точки  поиска угла штампа
			  // и угла  его  наклона
			  if (masXoL[0]<5)
			  { masXoL[0]=masXoL[0]+1; ii= masXoL[0]; masXoL[ii]=XoL; }
				else
				{ 	xmin= masXoL[1]; xmax= masXoL[1]; //imin=0; imax=0;
					//nsXoL1=nstr;
					for(j=1;j<6;j++)
					{ if(masXoL[j] < xmin){ xmin=masXoL[j];}//nsXoL1=nstr-5+i; }
					  if(masXoL[j] > xmax){ xmax=masXoL[j];}//nsXoL1=nstr-5+i; }
					}
					dx=(float(xmax)-float(xmin))/2-0.1;
					ij=0; sumx=0;
					for(j=1;j<6;j++) if(fabs(masXoL[j]-xmin) < dx /*LineWx */ )
					{ ij=ij+1; sumx=sumx+masXoL[j];  }
					//if (ij>2) XoL1=xmin; else XoL1=xmax;
					XoL=xmin;
					for(j=0;j < 6;j++)masXoL[j]=0;
					PrFirstL=1;
					PrShL=0; PrNaklShL=0;  nbyShL=0;
					XoMin=XoL;  XoSh1=XoL;  YoSh1=nstr;
					AlfaSh=0; nbyShL=0; goto finR;
				}
			} else
			{  	if (nstr > (nstrStampStartL + dyStampNum-20))
				{ if (masXoL[0]<5)
				   { masXoL[0]=masXoL[0]+1; ii= masXoL[0]; masXoL[ii]=XoL; }
					 else
				   { if(PrAlfaSh==0)
					 { // определение угла наклона
					   xmin= masXoL[1]; xmax= masXoL[1]; //imin=0; imax=0;
					   //nsXoL1=nstr;
					   for(j=1;j<6;j++)
					   { if(masXoL[j] < xmin){ xmin=masXoL[j];}//nsXoL1=nstr-5+i; }
						 if(masXoL[j] > xmax){ xmax=masXoL[j];}//nsXoL1=nstr-5+i; }
					   }
					   dx=(float(xmax)-float(xmin))/2-0.1;
					   ij=0; sumx=0;
					   for(j=1;j<6;j++) if(fabs(masXoL[j]-xmin) < dx /*LineWx */ )
					   { ij=ij+1; sumx=sumx+masXoL[j];  }
					   //if (ij>2) XoL1=xmin; else XoL1=xmax;
					   for(j=0;j < 6;j++) masXoL[j]=0;
					   XoL=xmin;
					   PrAlfaSh=1;
					   dx=nstr-YoSh1;
					   AlfaSh=float(float(XoL-XoSh1)/dx);
					   PrNaklShL=0;
					   if (AlfaSh < -0.01)  PrNaklShL=-1;
					   if (AlfaSh > 0.01) PrNaklShL=1;
					   nbXoMin=0;
					 }
				   }
				   if(PrAlfaSh==1)
				   { // угол наклона определен, начало поиска угла штампа
					 if (PrNaklShL==1)   // наклон вправо
					 {  XoSh1=XoL;    YoSh1=nstr;
					   /* if(nbXoMin==0)
						{ XoSh1=XoL;    YoSh1=nstr; nbXoMin=1;}
						if((fabs(XoL-XoSh1) <= LineWx)&&(nbXoMin==1))
						{ XoSh1=XoL;    YoSh1=nstr;}
						*/
					 }
					 if (PrNaklShL==-1)  // наклон влево
					 { // наклон влево
					   if(XoL <= XoMin)
					   { XoSh1=XoL;  YoSh1=nstr; XoMin=XoL; nbXoMin=0;} else
					   { if((XoL-XoMin)>3) nbXoMin=nbXoMin+1; 		  }
					   if (nbXoMin > 6)
					   { PrShL=1; XoShL=XoSh1;  YoShL=YoSh1;}
					 }
					 if (PrNaklShL==0)  // нет наклона
					 { if(fabs(XoL-XoSh1)>3) nbXoMin=nbXoMin+1;
					   else
					   { XoSh1=XoL; YoSh1=nstr; nbXoMin=0;     }
					   if (nbXoMin > 6)
					   { PrShL=1; XoShL=XoSh1;  YoShL=YoSh1;}
					 }
				   } //   if(PrAlfaSh==1)
				 }
			}  // PrFirst
			nbyShL=0;
		} else
		  { // PrL==0
			  if (nstr > (nstrStampStartR + dyStampNum))
			  { if(PrFirstL==0) {  Err=4; goto fin; }
				if(PrAlfaSh==1)
			   {
				  nbyShL=nbyShL+1;
				  if(nbyShL > LineWy /*LineWy*/)
				  { PrShL=1;
					/*XoShL=XoSh1;
					//if(PrNaklShL!=-1)
					YoSh1=nstr-nbyShL;
					YoShL=YoSh1;
					*/
				  }
			   }
			  }
		  } // PrL
			finShL:;
	 } //if(PrShL==0)
	 finR:;
	 if(PrShR==0)
	 { 	// определение координат правой стороны штампа
		if(PrFirstR!=0)
		{ dx=colStampR-XoRSh; dxStFindR=dx+3*NumDotX;  }

		EvChangeColRowSh(1,nstr,-1,colStampR,dxStFindR,&nbShR,&XoR,&PrR);
		PrXoRSh=PrR;
		if(PrR==1)
		{   XoRSh=XoR;
			// проверка на то, что граница штампа слева сливается с рамкой бюллетеня
			if(colStampR<=XoR)
			{ cout << " Right side  Error nstr =  "<<nstr<<"XoR=  "<<XoR<< "colStampR =  "<< colStampR<< endl;
			    PrShR=-1; goto finShR;	}
			//dxStFindR=colStampR-XoR - 2*NumDotX;
		  if(PrFirstR==0)
		  { // запоминание  начальной  точки  поиска угла штампа
			// и угла  его  наклона
			if (masXoR[0]<5)
			{ masXoR[0]=masXoR[0]+1; ii= masXoR[0]; masXoR[ii]=XoR; }
			else
			{ 	xmin= masXoR[1]; xmax= masXoR[1]; //imin=0; imax=0;
				//nsXoR1=nstr;
				for(j=1;j<6;j++)
				{ if(masXoR[j] < xmin){ xmin=masXoR[j];}//nsXoR1=nstr-5+i; };
				  if(masXoR[j] > xmax){ xmax=masXoR[j];}//nsXoR1=nstr-5+i; };
				}
				dx=(float(xmax)-float(xmin))/2-0.1;
				ij=0; sumx=0;
				for(j=1;j<6;j++) if(fabs(masXoR[j]-xmin) < dx /*LineWx */ )
				{ ij=ij+1; sumx=sumx+masXoR[j];  }
				//if (ij>2) XoR1=xmin; else XoR1=xmax;

				PrShR=0; PrNaklShR=0;  nbyShR=0;
				XoMax=XoR;  XoShR1=XoR;  YoShR1=nstr;
				PrFirstR=1;
				for(j=0;j < 6;j++) masXoR[j]=0;
				AlfaShR=0; nbyShR=0; goto fin;
			}
		  } else
		  {	if (nstr > (nstrStampStartR + dyStampNum-20))
			{  // вычисление угла наклона штампа
			   if ((masXoR[0]<6)&&(PrAlfaShR==0))
			   { masXoR[0]=masXoR[0]+1; ii= masXoR[0]; masXoR[ii]=XoR; }
			   else
			   {    // исключение ошибок, пропусков линий штампа
					if (PrAlfaShR==0)
					{
						xmin= masXoR[1]; xmax= masXoR[1]; //imin=0; imax=0;
						//nsXoR1=nstr;
						for(j=1;j<6;j++)
						{ if(masXoR[j] < xmin){ xmin=masXoR[j];}//nsXoR1=nstr-5+i; };
						  if(masXoR[j] > xmax){ xmax=masXoR[j];}//nsXoR1=nstr-5+i; };
						}
						dx=(float(xmax)-float(xmin))/2-0.1;
						ij=0; sumx=0;
						for(j=1;j<6;j++) if(fabs(masXoR[j]-xmin) < dx /*LineWx*/ )
						{ ij=ij+1; sumx=sumx+masXoR[j];  }
						//if (ij>2) XoR1=xmin; else XoR1=xmax;
						for(j=0;j < 6;j++) masXoR[j]=0;
						PrAlfaShR=1; PrNaklShR=0;
						dx=nstr-YoShR1;
						AlfaShR=float(float(XoR-XoShR1)/dx);
						if (AlfaShR < -0.01)  PrNaklShR=-1;
						if (AlfaShR > 0.01) PrNaklShR=1;
						nbXoMax=0;
					}
			   }
				if(PrAlfaShR==1)
				{ // угол наклона определен, начало поиска угла штампа
					if (PrNaklShR==1)
					{ // наклон вправо
					  if(XoR >=  XoMax)
					  { //XoShR1=XoR;
						YoShR1=nstr; XoMax=XoR;XoShR1=XoMax; nbXoMax=0;
					  }	else
					  { if((XoMax-XoR)>3) nbXoMax=nbXoMax+1;	}
					  if (nbXoMax > LineWy)
					  { PrShR=1; XoShR1=XoMax;  XoShR=XoShR1;  YoShR=YoShR1; }
					}
					if (PrNaklShR==-1)
					{ // наклон влево  или ровно

					  XoMax=XoR; XoShR1=XoR; YoShR1=nstr; //XoShR1=XoR;
					/*  if(nbXoMax==0)
					  { XoMax=XoR; XoShR1=XoR; YoShR1=nstr; nbXoMax=1;
					   //XoShR1=XoR;
					  }
					  if((fabs(XoR-XoShR1)<=LineWx)&&(nbXoMax==1))
					  { XoMax=XoR; XoShR1=XoR; YoShR1=nstr; //XoShR1=XoR;
					  }
					  */
					}
					if (PrNaklShR==0)
					{  if(fabs(XoR-XoShR1)>3) nbXoMax=nbXoMax+1;
					   else
					   { XoShR1=XoR; YoShR1=nstr; nbXoMax=0;     }
					   if (nbXoMax > 6)
					   { PrShR=1; XoShR=XoShR1;  YoShR=YoShR1;}
					}
					//}
				} //   if(PrAlfaSh==1)
			}
		  } // PrFirstR
		  nbyShR=0;
		} else
		{ // PrL==0
		  if (nstr > (nstrStampStartR + dyStampNum))
		  {  if(PrFirstR==0) {  Err=4; goto fin; }

			//if((PrAlfaShR==1)||(PrNaklShR==0))
			 { nbXoMax=nbXoMax+1;
			   if (nbXoMax > 4*LineWy)
			   { PrShR=1; XoShR1=XoMax;  YoShR1=nstr-nbXoMax; XoShR=XoShR1;  YoShR1=nstr-nbXoMax/*YoShR1*/; }
			 }
		  }
		} // PrR
		finShR: ;
	 } //if(PrShR==0)
	if((PrNaklShR !=PrNaklShL)&&(PrAlfaShR == 1)&&(PrAlfaSh == 1))
	{  PrNaklShR=0; PrNaklShL=0;	}
	if ((PrFirstR==1)&&(PrFirstL==1))
	{ dx=sqrt((XoShR1-XoSh1)*(XoShR1-XoSh1)+(YoShR1-YoSh1)*(YoShR1-YoSh1));
	  if (fabs(dxStamp-dx) > 4*NumDotX)
	  {  PrFirstR=0; PrFirstL=0; }
	  else
	  { // запоминание координат первых точек границ штампа
		PrFirstR=XoShR1; PrFirstL=XoSh1; PrFirstRy=YoShR1; PrFirstLy=YoSh1;
	  }
	}

	if((PrShL==-1)||(PrShR==-1))
	{  Err=41; goto fin; }

	if((PrShL==1)&&(PrShR==1))
	{ 	// найдены координаты обоих точек, определение
		// правильности штампа
		XoShR=XoShR1;  YoShR=YoShR1;
		XoShL=XoSh1;   YoShL=YoSh1;
		AlfaShR=float(XoShR-PrFirstR)/float(YoShR-PrFirstRy);
		AlfaSh=float(XoShL-PrFirstL)/float(YoShL-PrFirstLy);
		AlfaSh=-(AlfaSh+AlfaShR)/2;
		//AlfaSh=float(YoShR-YoShL)/float(XoShR-XoShL);
		// проверка сбоя при наличии большого пробела в  линиях штампа
		dx=sqrt((XoShR-XoShL)*(XoShR-XoShL)+(YoShR-YoShL)*(YoShR-YoShL));
		if (fabs(dxStamp-dx) > int(2.5*float(NumDotX))) 	{ Err=42; goto finPrSh;  }
		//  проверка выхода за границы верхней части штампа
		XoL=XoShL+dyStamp*sin(AlfaSh); 	YoL=YoShL-dyStamp*cos(AlfaSh);
		XoR=XoShR+dyStamp*sin(AlfaSh); 	YoR=YoShR-dyStamp*cos(AlfaSh);
		if (((XoR-NumDotX) > PgXoR)||((XoL+NumDotX) < PgXoR-dxStampArea)||
		   ((YoR-NumDotY) < PgYoR+dyVoteType + 2*LineWy)||
		   ((YoL-NumDotY) < PgYoR+dyVoteType + 2*LineWy))
		   { Err=41; goto finPrSh;}
		// формирование признака наличия штампа
		*PrSt=1;
		finPrSh:;
	}
   } //if(nstr > nstrStampStartL)
  fin:;
  return Err;
} //int FindNumYchT0p()


int FindNumYchBot(int* PrSt)
// PrSt - признак нахождения координат штампа
{ // функция определения  координат области номера УИК
  // пока только  по  левой  стороне штампа
   int Err=0,PrL=0,PrR=0;
   int XoL,XoR,YoL,YoR;
   float dx;
   if ((nstr > nstrStampEndL)&&(*PrSt==0))
   {   if((PrShL==0)&&(PrShR==0)) {  Err=4; goto fin; }     }
	 //  иногда  случайное обнаружение  фрагмента линии зоны штампа ( при наклоне)
	 // приводило к получению одной из границ там, где  ее  нет и получалось
	 // неправильное решение
	 // параметры  для поиска углов штампа
	 //  здесь нужно  уточнять начало колонки для поиска штампа
	 // и длину зоны поиска для левого и правого угла с  учетом
	 // угла наклона листа

   if(nstr > nstrStampStartL)
   { //  здесь нужно  уточнять начало колонки для поиска штампа
	 // и длину зоны поиска для левого и правого угла с  учетом
	 // угла наклона листа
	 dx=float(nstr-nstrStampStartL)*TanAlfaPage;
	 colStampL=colStampLo-int(dx)+1;
	 colStampL=LsXo+int(1.5*float(NumDotX));
	 dynstr=nstr-nstrStampStartL;
	 if((AlfaPage > 0)&&(fabs(AlfaPage) > 0.005)&&(dynstr < 8))
	 { // уточнение длины зоны захвата с  учетом наклона
	   dx=2*dynstr/sin(2*fabs(AlfaPage));
	 } else dx= int(float(dxStampArea)/2);
	 if(dx < float(dxStampArea)/2)dxStFindL=dx; else dxStFindL=int(float(dxStampArea)/2);
	 if(PrShL==0)  //nbShL >=0)// изначально  задается 1
	 {  // определение координат левой стороны штампа
		//EvChangeColRow(1,nstr,1,colStampL,dxStFindL,&nbShL,&XoL,&PrL);
		EvChangeColRowSh(1,nstr,1,colStampL,dxStFindL,&nbShL,&XoL,&PrL);
		if(PrL==1) // есть черная линия
		{ if(colStampL>=XoL) // граница штампа слева сливается с рамкой бюллетеня
		  { cout << " Left side  Error nstr =  "<<nstr<<endl;
		      PrShL=-1; goto finShL; }
		  if(PrFirstL==0)
		  { // запоминание  начальной  точки  поиска угла штампа
			// и угла  его  наклона
			if((nstr-1)==nstrStampStartL) { Err=41; goto fin;}
			PrFirstL=nstr; // первая точка найдена
			PrXMin=0; PrXMax=0;
			PrShL=0; nbyShL=0; XoMin=XoL; YoMin=nstr;
			XoSh1=XoL;  YoSh1=nstr; AlfaSh=0; nbyShL=0;  nbXoMin=0;
			if(PrNaklShR!=0) PrNaklShL=PrNaklShR;else PrNaklShL=0;
		/*	if(PrNaklShR!=0)
			{ if(fabs(YoSh1-YoShR1)/fabs(XoShR1-XoSh1) < 0.01)
			  {	PrNaklShL=2; PrNaklShR=2;
			  } else PrNaklShL=PrNaklShR;
			}else PrNaklShL=0;

		 */
			goto finR;
		  }  else
		  { // поиск минимума   впроверяемых  слева строках
			if(PrNaklShL==-1)
			{ if(XoL <  XoMin)
			  {  if(( XoMin - XoL)> 3) { /*YoMin=nstr; */ XoMin=XoL;}
				YoMin=nstr; PrXMin=1;
				XoMin=XoL;
			  } else
			  {  nbXoMin=nbXoMin+1;
				if (nbXoMin > 3)
				{ if (PrXMin==1)
				   { //левая точка найдена
					 PrShL=1;
					PrXMin=2;  XoSh1=XoMin;  YoSh1=YoMin;
				   }
				}
			  }
			}
			if(PrNaklShL==1)
			{ nbXoMin=nbXoMin+1;
			   if (nbXoMin > 0)
			  { //левая точка найдена
				PrShL=1; PrXMin=2;
				XoSh1=XoL; //int(float(XoL+XoSh1)/2);
				YoSh1=nstr;//int(float(nstr+YoSh1)/2);
			  }
			}

			if (PrNaklShL==2)
			{  nbXoMin=nbXoMin+1;
			   if((XoSh1-XoL) > NumDotY) YoSh1=nstr; XoSh1=XoL;
			   XoSh1=XoL;
			   if (nbXoMin > NumDotY) PrShL=1;
			}
		  } // PrFirstL
		} else
		{ // PrL==0   нет черной  линии
		  if (PrFirstL!=0)
		  { // если сюда попадаем после того, как найдена первая точка
			// в левой части  зоны штампа
			nbyShL=nbyShL+1;  nbXoMin=0;
			if(nbyShL > 2*NumDotY)
			{ // отсутствует левая  граница штампа
			  Err=43; goto fin;
			}
		  }
		} // PrL
		finShL:;
	 } //if(PrShL==0)
   } //if(nstr > nstrStampStartL)
	finR:;
	if(nstr > nstrStampStartR)
   {  dx=float(nstr-nstrStampStartL)*TanAlfaPage;
	  colStampR=colStampRo-int(dx)-1;
	  colStampR=LsXo+int((dxStampArea-int(2.5*float(NumDotX)))*CosAlfaPage);       /// 09_04
	  dynstr=nstr-nstrStampStartR;
	  if((AlfaPage < 0)&&(fabs(AlfaPage) > 0.005)&&(dynstr < 8))
	  { // уточнение длины зоны захвата с  учетом наклона
		dx=2*dynstr/sin(2*fabs(AlfaPage));
	  } else dx= int(float(dxStampArea)/2);
	  if(dx < float(dxStampArea)/2)dxStFindR=dx; else
			dxStFindR=int(float(dxStampArea)/2);
	 if(PrShR==0)
	 {  // определение координат правой нижней стороны штампа
		// определение координат левой стороны штампа
		EvChangeColRowSh(1,nstr,-1,colStampR,dxStFindR,&nbShR,&XoR,&PrR);
		if(PrR==1) // есть черная линия
		{  if(colStampR<=XoR) // граница штампа слева сливается с рамкой бюллетеня
			{ cout << " Right side  Error nstr =  "<<nstr<<endl;
			    PrShR=-1; goto finShR; }
			nbyShR=0; //  гашение счетчика пропуска строк с черной линией
		  if(PrFirstR==0)
		  { // запоминание  начальной  точки  поиска угла штампа
			// и угла  его  наклона
			if((nstr-1)==nstrStampStartR) { Err=41; goto fin;}
			PrFirstR=nstr; // первая точка найдена
			PrXMax=0;
			PrShR=0;  nbyShR=0; XoMax=XoR;  YoMax=nstr;
			if(PrNaklShL!=0) PrNaklShR=PrNaklShL;else PrNaklShR=0;
		 /*	if(PrNaklShL!=0)
			{ if(fabs(YoSh1-YoShR1)/fabs(XoShR1-XoSh1) < 0.01)
			  {	PrNaklShL=2; PrNaklShR=2;
			  } else PrNaklShR=PrNaklShL;
			}else PrNaklShR=0;
        */

			nbXoMax=0;
			XoShR1=XoR;  YoShR1=nstr; AlfaShR=0; nbyShR=0; goto fin;
		  }  else
		  { // поиск максимума   впроверяемых  справа строках
			if(PrNaklShR ==1)
			{ if(XoR > XoMax)
			  { if(( XoR - XoMax)>3) {/*YoMax=nstr;*/  XoMax=XoR;};YoMax=nstr; PrXMax=1;

				XoMax=XoR; YoMax=nstr; PrXMax=1;

			  }
			  else
			  {  nbXoMax=nbXoMax+1;
				if (nbXoMax > 3)
				{ PrShR=1; PrXMax=2;  XoShR1=XoMax;  YoShR1=YoMax;
				  if ((PrXMax==1)&&(XoR < XoMax)) //правая точка найдена
				  { PrShR=1; PrXMax=2;  XoShR1=XoMax;  YoShR1=YoMax; }
				}
			  }
			}
			if(PrNaklShR==-1)
			{  nbXoMax=nbXoMax+1;
			   if (nbXoMax > 0)
			  { //правая точка найдена
				PrShR=1; PrXMax=2;  XoShR1=XoR;//int(float(XoR+XoShR1)/2);
				YoShR1=nstr;//int(float(YoShR1+nstr)/2);

			  }
			}
			if (PrNaklShR==2)
			{  nbXoMax=nbXoMax+1;
			   if((XoR-XoShR1) > NumDotY) YoShR1=nstr; XoShR1=XoR;
			   if (nbXoMax > NumDotY) PrShR=1;
			}
		  } // PrFirstR
		} else
		{ // PrL==0   нет черной  линии
		  if (PrFirstR!=0)
		  { // если сюда попадаем после того, как найдена первая точка
			// в правой части  зоны штампа
			nbyShR=nbyShR+1;
			if(nbyShR > 2*NumDotY) // отсутствует правая  граница штампа
			{ Err=44; goto fin; }
		  }
		} // PrR
		finShR: ;
	 } // if(PrShR==0)
   } //if(nstr > nstrStampStartR)
   // анализ наклона штампа  при обнаружении первой  точки
   if(((PrFirstL!=0)||(PrFirstR!=0))&&((PrNaklShL==0)&&(PrNaklShR==0)))
   { if((PrFirstL!=0)&&(PrFirstR!=0)) { PrNaklShL=2; PrNaklShR=2; }
	 else
	 { if(PrFirstL!=0)
	   { PrNaklShL=1;
			/*PrShL=1; закоментировано, чтобы на следующей строке уточнить X*/
			XoSh1=XoMin;  YoSh1=YoMin;
	   }
	   if(PrFirstR!=0)
	   { PrNaklShR=-1;
			/*PrShR=1;закоментировано, чтобы на следующей строке уточнить X */
			XoShR1=XoMax;  YoShR1=YoMax;
	   }

	 }
   }
   if((PrShL==-1)||(PrShR==-1)){  Err=41; goto fin; }
	// формирование признака  окончания  поиска   начала  листа PrPage=1
	if((PrShL==1)&&(PrShR==1)&&(*PrSt==0))
	{ 	*PrSt=1;
		// вычисление угла наклона
		XoShR=XoShR1;  YoShR=YoShR1;
		XoShL=XoSh1;  YoShL=YoSh1;
		AlfaSh=float(YoShR-YoShL)/float(XoShR-XoShL);
		//  проверка сбоя при наличии большого пробела в  линиях штампа
		dx=sqrt((XoShR-XoShL)*(XoShR-XoShL)+(YoShR-YoShL)*(YoShR-YoShL));
		if (((dx-dxStamp) > int(2.2/*2*/*float(NumDotX)))||((dx-dxStamp) < -NumDotX))
			{ Err=42; goto finPrSh;  }

		//if (fabs(dxStamp-dx) > 2*NumDotX) 	{ Err=42; goto finPrSh;  }
		//  проверка выхода за границы верхней части штампа
		// смещение левой границы рамки
		dx=AlfaPage*(YoShR+YoShL)/2;
		XoL=XoShL-dyStamp*sin(AlfaSh); 	YoL=YoShL+dyStamp*cos(AlfaSh);
		XoR=XoShR-dyStamp*sin(AlfaSh); 	YoR=YoShR+dyStamp*cos(AlfaSh);
		nstrStampEndR=nstrStampStartR+dyStampArea-2*NumDotY;
		nstrStampEndL=nstrStampStartL+dyStampArea-2*NumDotY;
		if (((XoR-NumDotX) > colStampRo)||((XoL+NumDotX) < colStampLo)||
		   ((YoR-NumDotY) > nstrStampEndR)||((YoL-NumDotY) > nstrStampEndL))
		   { Err=41; goto finPrSh;}
		// формирвоание признака наличия штампа
		*PrSt=1;
		finPrSh:;
	}
  fin:;
  return Err;
} //int FindNumYchBot()


void FindVoteMarks(void)
{ // определение наличия  отметок  голосования  в  квадратах
   int ii,XTop,YTop,XLe,PrL,PrYTop,ncL,dxL,ns,Napr=0;
//   float dyy,dxy;
   if(NumSQMax[nVoteType]>0)
   for (j=1; j < NumSQMax[nVoteType]+1; j++)
   {  if (BulHead==0)Napr=1; else Napr=-1;
	  ns=int(float(SearchSQ[j][2])*KoefY);
   if((nstr > ns/*SearchSQ[j][2]*/)&&(SearchSQ[j][0]!=3))
   {   if(SearchSQ[j][0] == 0)
	   { // поиск верхней линии квадрата
			YTop=0;PrYTop=0; //jj=0;
			dxFindSQ=10;
			if(Napr==1)XTop=RsXo-SearchSQ[j][1]; else XTop=SearchSQ[j][1]+LsXo;
			XTop=XTop/CosAlfaPage;
			EvChangeColCol(1,nstr,1,XTop,10,&SearchSQ[j][5],&YTop,&PrYTop);
			if(PrYTop==1) // есть черная линия
			{ SearchSQ[j][5]=0; // обнуление счетчика обнаружения нужных элементов
			  SearchSQ[j][0]=1; // найдена верхняя линия, поиск левой границы
			  // вычисление коэффициента  сжатия
			  float zo=float(SearchSQ[j][6])*CosAlfaPage;
			  float xo = float(SearchSQ[j][1])*SinAlfaPage;
			  float KoefTek=float(YTop-PgYoL)/(zo-xo);
			  if(PrKoef==0)
			  { KoefY= KoefTek;  NumDotKoefY=1;
			  }  else
			  {  if(NumDotKoefY==0)NumDotKoefY=1;
			      KoefY=(KoefY*NumDotKoefY+KoefTek)/( NumDotKoefY+1);
			      NumDotKoefY=NumDotKoefY+1;
			      cout<<" Koefficient on Marks = "<<KoefY<< endl;
              }

			  // уточнение столбца, с которого  начинается  поиск левой стороны
			  // SearchSQ[j][1]=SearchSQ[j][1]-Napr*SearchSQ[j][3];
			  SearchSQ[j][1]=SearchSQ[j][1]+SearchSQ[j][3];
			  SearchSQ[j][2]=YTop+NumDotY+int(0.5*float(NumDotY)); // запоминание номера строки начала верхней линии
			  goto finNumSQ;
			}
	   }
	   if(SearchSQ[j][0] == 1)
	   {  // поиск левой ( правой для обратного хода) границы квадрата
		  PrL=0;
		  if(Napr==1)ncL=RsXo-SearchSQ[j][1]; else ncL=SearchSQ[j][1]+LsXo;
		  // уточнить  столбец начала поиска

		  dxL=SearchSQ[j][3];
		  EvChangeColRow(1,nstr,Napr,ncL,dxL,&SearchSQ[j][5],&XLe,&PrL);
		  if(PrL==1)
		  { // формирование данных для подсчета точек в квадрате
			 SearchSQ[j][0]=2; // найдена левая граница
			 SearchSQ[j][5]=0; // обнуление счетчика обнаружения нужных элементов
			 ii=int(0.5*float(NumDotX)); //jj=int(0.5*float(NumDotY));
			 if(Napr==1)
			 { SearchSQ[j][1]=XLe+NumDotX+ii;
			 } else
			 { SearchSQ[j][1]=XLe-SearchSQ[j][3]+ii; }
			 SearchSQ[j][3]=SearchSQ[j][3]-2*NumDotX;//-ii;
			 SearchSQ[j][4]=SearchSQ[j][4]-2*NumDotY;//-jj;
			 NumBlP[nVoteType][j]=0;
		  }
	   }
	   if((SearchSQ[j][0]==2)&&(nstr >= SearchSQ[j][2] ))
	   {
			dxSQ= SearchSQ[j][3];
			dySQ= SearchSQ[j][4];
			NumPMax[nVoteType][j]=dySQ*dxSQ;
			nc1= SearchSQ[j][1]; nc2= nc1+SearchSQ[j][3];
			nr2= SearchSQ[j][2]+SearchSQ[j][4];

			if (nstr <= nr2)
			{  // подсчет точек для текущей  строки
			   for (i=nc1; i <= nc2; i++)
			   { // подсчет числа точек
				 //=====================================
				 //Color=Form1->Image2->Picture->Bitmap->Canvas->Pixels[i][nstr];
				 //urw_ch=(GetRValue(Color)*0.3+ GetGValue(Color)*0.59+ GetBValue(Color)*0.11);
                // urw_ch=GetXYValue(i,nstr);
                 urw_ch=GetXYValue(nstr,i);
				 //if(urw_ch > 170)urw_ch=170;
				 //=====================================
				 dd=urw_ch;
				 // обращение  к функции  принятия решения о наличии  черного  в  точке
				 //g0=0;
				 g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
						   //	if(dd > 85) g0=0;/
				 if(g0==1) NumBlP[nVoteType][j]=NumBlP[nVoteType][j]+1;
			   }
			} else
			{  // определение признака наличия отметки
			   SearchSQ[j][0]=3; // признак окончания подсчета точек в квадрате
			   // берется превышением  10% заполненной площади
			   //if (float(NumBlP[nVoteType][j])/float(NumPMax[nVoteType][j]) > 0.01) g0=1; else g0=0;
			   if (float(NumBlP[nVoteType][j]) > (0.5*float(dxSQ))) g0=1; else g0=0;
			   RezRec[nVoteType][j]=g0;
			}
	   }
	   finNumSQ:;
   }
   }
} //void FindVoteMarks(void)
int ScanBulString(void)
{ //  функция управления  сканированием бюллетеня при поступлении  очередной  строки
  int ErrScan=0;

// ======================обработка  строки ===============================
/////////////////////////////////////////////////////////////////////////////
//- PrPage=0 - Н А Х О Ж Д Е Н И Е   Н А Ч А Л А    Л И С Т А  -------------
   int dxS,dyS;
   if (PrPage==0)
   { // определение  начала  листа
	 ErrScan=FindPageStart(&PrPage);
	 if(PrPage==1)
	 { FrXoL=PgXoL;   FrYoL=PgYoL;  FrXoR=PgXoR;   FrYoR=PgYoR;	}
	 if (ErrScan >0){
	 goto FinEvRow;  }
   }
   ////////////////////////////////////////////////////////////////////////////
   //- PrPage=1  Р А С П О З Н А В А Н И Е   Р А М К И   И  В И Д А  Б Ю Л Л Е Т Е Н Я  -------------
  if((PrPage==1)&&((nstr > nstrL)||(nstr > nstrR)))
   { if (nstr==nstrUrWhite)  //  уточнение уровня  белого по всей  строке
	 { nn=1;  urw=0;
	   for (i = PgXoL+100; i <= PgXoR-100; i++)
	   {  // определение  уровня  интенсивности пикселя
		  //-----------------------здесь нужно обратиться  к  массиву  точек строки-------
		  //Color=Form1->Image2->Picture->Bitmap->Canvas->Pixels[i][nstr];
		  //urw_ch=(GetRValue(Color)*0.3+ GetGValue(Color)*0.59+ GetBValue(Color)*0.11);
		  //urw_ch=GetXYValue(i,nstr);
		  urw_ch=GetXYValue(nstr,i);
		  //---------------------------------------------------
		  // определение  среднего уровня белого
			BLev(nn,urw_ch,&urw,&surw);
			//	 переход к  следующей  точке
			nn=nn+1;
	   }
		  if(surw <15) surw=15;
		  if(urw>190)urw=190;
	 }

	 if (PrFrame==0)
	 { 	ErrScan=FindFrame(&PrFrame);

	 if (ErrScan >0)
	 goto FinEvRow;
	 } //if (PrFrame==0)
	 // определение  вида  бюллетеня
	 urw1=urw;
	 urw=150;
	 if(PrPage==1)ErrScan=FindVoteTypeTop(&PrPage);

	 // анализ  ошибки при поиске рамки и вида бюллетеня
	 if((ErrScan==2)&&(PrFrame==0))ErrScan=1;
	 urw=urw1;
	 // проверка того,  что тип бюллетеня соответствует исходным данным
	 if(PrPage==2)
	 { for(i=1;i < NumVoteType+1;i++) if(PrVoteTypeTop[i]==1)nVoteType=i;
	   j=0;
	   for(i=1; i <= NumVoteType;i++)if((PrVoteTypeId[i]==1)&&(nVoteType==i))j=1;

      for(i=1; i <= NumVoteType;i++)if(PrVoteTypeId[i]==1) cout <<  "Vote Type ID   =  %d   " <<  i << endl;

        cout <<  "Vote Type  =  %d" <<  nVoteType << endl;

	   if(j==0)ErrScan=20;
	 }

	 if (ErrScan >0) goto FinEvRow;
	 if(PrPage==2)
	 {  // рамка листа найдена, вид бюллетеня определен
		// подготовка  данных для  поиска штампа и элементов голосования
		// сохранение определенного типа бюллетеня в ячейке nVoteType
		EndYoL=0;EndYoR=0;EndXoL=0; EndXoR=0; PrEndXoL=0; // координаты конца рамки внизу
		 for(i=1;i < NumVoteType+1;i++) if(PrVoteTypeTop[i]==1)nVoteType=i;
		//  уточнение параметров страницы в зависимости от типа бюллетеня
		PageHM=PagePar[nVoteType][1];  // float PageHM=148.5; // высота  страницы в  мм
		PageH=int(PageHM*float(NumDotY)); // высота  страницы  в  точках
		PageHM=PagePar[nVoteType][1];    // высота  страницы в  мм
		PageH=int(PageHM*float(NumDotY));//+0.5); // высота  страницы  в  точках
		PageWM=PagePar[nVoteType][4]; // ширина  страницы в  мм  (при загрузке ( максимальная)
		PageW=int(PageWM*float(NumDotX)); // ширина  страницы  в  точках
		dxStampAreaM=PagePar[nVoteType][2];  // ширина  зоны  печати в  мм
		dxStampArea=int(dxStampAreaM*float(NumDotX)); // ширина  зоны  печати в  точках
		dyStampAreaM=PagePar[nVoteType][3];
		dyStampArea=int(float(dyStampAreaM)*float(NumDotY)); // высота  зоны  печати в  точках
		nstrStampStartL=LineWy+dyStamp-dyStampNum; // строка начала поиска штампа
		nstrStampEndL=LineWy+dyStampArea;    // строка окончания поиска штампа
		colStampL=PageW-(LMarSize+LineWx+dxStampArea)+2;  // колонка начала поиска штампа слева
		colStampR=PageW-(LMarSize+LineWx)-2;  // колонка начала поиска штампа справа
		dxStFindL=int(0.4*float(dxStampArea)/2); // количество точек для  поиска границ штампа
		dxStFindR=int(0.4*float(dxStampArea)/2); // количество точек для  поиска границ штампа
		dyStampNum=int(dyStampNum*float(float(dxStampArea)/float(dyStamp)));
		nstrStampEndR=nstrStampStartR+dyStampArea-2*NumDotY;
		nstrStampEndL=nstrStampStartL+dyStampArea-2*NumDotY;
		nstrPageEnd=PgYoL;
		if(PgYoL > PgYoR)nstrPageEnd=PgYoL; else nstrPageEnd=PgYoR;
		nstrPageEnd=nstrPageEnd+PageH-2*TMarSize;
		//  уточнение  угла наклона листа
		dyALfa=PgYoR-PgYoL; dxALfa=PgXoR-PgXoL;
		AlfaPage=(float(dyALfa)*float(NumDotY))/(float(dxALfa)*float(NumDotX));
		SinAlfaPage=sin(AlfaPage); 	CosAlfaPage=cos(AlfaPage);
		TanAlfaPage=SinAlfaPage/CosAlfaPage;
		PrShL=0;PrShR=0; // гашение признаков
		PrAlfaSh=0; PrShFound=0;  nbShL=0; nbShR=0;
		nbyShL=0;  	nbyShR=0; 	// счетчики пропуска линий в штампе
		PrShL=0; 	PrShR=0; 		XoShL=0; 	XoSh1=0;  YoShL=0;	YoSh1=0;
		PrNaklShL=0;PrNaklShR=0; 	PrFirstL=0; PrFirstR=0;
		PrXMin=0; PrXMax=0;   NumWtP=0;
		XoShR1=0; YoShR1=0; PrAlfaShR=0; XoMax=0; YoMax=0; nbXoMax=0;
		PrFirstR=0; PrFirstL=0; PrFirstRy=0; PrFirstLy=0;
		for(j=0;j < 6;j++){ masXoR[j]=0,masXoL[j]=0;}
		// установка строки для начала  поиска нижней рамки листа
		nstrBegVoteType =nstrVoteType[0];
		if(nstrBegVoteType > nstrVoteType[NumVoteType])
					nstrBegVoteType =nstrVoteType[NumVoteType];
		if (BulHead==0)
		{ // при прямом ходе бюллетеня
		   zz=int(dyStampArea*AlfaPage);
		   colStampL=PgXoR-dxStampArea-zz+int(0.7*float(LineWx));
		   colStampR=PgXoR-LineWx-zz-int(0.7*float(LineWx));
		   zz=int(dyStamp*float(float(dxStampArea)/float(dyStamp)));
		   nstrStampStartL=PgYoL+dyVoteType+ dyStamp - dyStampNum;
		   nstrStampStartR =nstrStampStartL;
			// формирование точек начала поиска штампа
		   dyS=nstrStampStartR-PgYoR;
		   colStampRo=PgXoR-int(2.0*float(NumDotX))-int(float(dyS)*SinAlfaPage+0.5);
		   dxS=int(float(dxStampArea-int(1.0*float(NumDotX)))*CosAlfaPage);
		   colStampLo=PgXoR-dxS;
		   dyS=dyStampArea +dyVoteType;//08_03-int(2.0*float(NumDotX));
		   nstrStampEndR=PgYoR+dyS*CosAlfaPage;
		   dyS=int(float(dxStampArea-int(1.8*float(LineWx)))*SinAlfaPage);
		   nstrStampEndL=nstrStampEndR+dyS;
		 } else
		 { //при обратном ходе бюллетеня
			zz=int(float(dxStampArea)*AlfaPage);
			nstrStampStartL=PgYoL+PageH-2*TMarSize-dyVoteType-dyStampArea;
			nstrStampStartR=PgYoL+PageH-2*TMarSize-dyVoteType-dyStampArea+zz;
			nstrStampEndL=nstrStampStartL +dyStampArea-dyStamp +dyStampNum;
			nstrStampEndR=nstrStampStartR +dyStampArea-dyStamp +dyStampNum;
			DeltaX=int(float(PageH-2*TMarSize-dyStampArea)*AlfaPage);
			colStampR=PgXoL+dxStampArea-DeltaX-int(0.5*float(LineWx));
			colStampL=PgXoL+LineWx-DeltaX+LineWx;
			PrKoef=1;
		 }//if (BulHead==1)
		 dyS=PgYoL+PageH-2*TMarSize-dyVoteType-dyStampArea-8*NumDotY;
		 nsShArStart= int(float(dyS)*cos(AlfaPage));
		 nbShAr=0; ShArPr=0;
		 // начальные установки для контроля рамки бюллетеня
		 Lsdx=LMarSize;//4*NumDotX;
		 LsCol=PgXoL-2*NumDotX;
		 Rsdx=LMarSize;//4*NumDotX;
		 RsCol=PgXoR+2*NumDotX;
		 nstrLsEnd=PageH; nstrRsEnd=PageH;
		 // устанавливается максимальная зона для поиска угла наклона
		 dynstr= dyStampArea;
		 // гашение массивов подсчета количества   точек
		 for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
		 {  NumBlP[i][j]=0;	 NumPMax[i][j]=0; RezRec[i][j]=0; }
		 //  формирование  массива для работы  с каждым квадратом
		 try
		 { SearchSQ=new int*[NumSQMax[nVoteType]+1];
		   for(j=0;j < NumSQMax[nVoteType]+1;j++) SearchSQ[j]=new int [7];
		 }
		 catch (std::bad_alloc)
		 {  cout << "Нет  памяти. Массив zamXY ..."; exit(-1); }
         //  перезапись данных  для  данного  вида бюллетеня в рабочие ячейки
		 // уточнение координат  положения  квадратов  с  учетом обнаруженного
		 // места угла рамки листа и его наклона
		 for(j=1;j < NumSQMax[nVoteType]+1;j++)
		 {  // здесь уточнить екоординаты строки и столбца с учетом наклона
			dxS= MasSQ[nVoteType][j][BulHead][1];
			dyS= MasSQ[nVoteType][j][BulHead][2];
			DeltaX=dxS; DeltaY=dyS;
			if(fabs(AlfaPage) > 1/57.3)
			{  DeltaX=int(float(dxS)/CosAlfaPage);
				if(BulHead==0)
				{ // при прямом ходе
					DeltaY=dyS+int(float(dxS)*SinAlfaPage);
				} else
				{ // при обратном  ходе
					DeltaY=dyS-int(float(dxS)*SinAlfaPage);
				}
			}
			// формирование массива работы с  квадратами для данного бюллетеня
			// перезапись данных по определенному типу выборов в  рабочие ячейки сканирования
			SearchSQ[j][0]=0;
			//  для Х расстояние до рамки слева не добавляется, поскольку
			// будет добавлена  реальная величина из функции контроля
			// левой  границы рамки, что более точно
			SearchSQ[j][1]=DeltaX;
			SearchSQ[j][2]=PgYoL+DeltaY;
			SearchSQ[j][3]=MasSQ[nVoteType][j][BulHead][3];
			SearchSQ[j][4]=MasSQ[nVoteType][j][BulHead][4];
			SearchSQ[j][5]=0;
			SearchSQ[j][6]=MasSQ[nVoteType][j][BulHead][5];;
		 }

        cout << " The End of Frame Kod =    " << ErrScan    << endl;
	  } // Page==2 // переход к  следующему этапу
   } // PrPage==1

	/////////////////////////////////////////////////////////////////////////////
	//-  PrPage=2  --- Р А С П О З Н А В А Н И Е   Б Ю Л Л Е Т Е Н Я  -------------
   if(PrPage==2)
   {  // направление и тип выборов распознаны, распознавание бюллетеня
	  // контроль границ рамки  слева и справа
	  if((nstr > nstrLsStart)&&(nstr > nstrRsStart)&&
		((EndYoL < nstrLsEnd)||(EndYoR < nstrRsEnd)))
		{
		     if((nstr < (nstrLsEnd-NumDotY))&&(nstr < (nstrRsEnd-NumDotY))&&(PrKoef==1))
		     { 		if (NstrFrControl > NumDotY)
                    {   NstrFrControl=0;
                        ErrScan=FrameControl();
                    } else NstrFrControl=NstrFrControl+1;
		     } else  ErrScan=FrameControl();

		}


	  if((nstr > nstrLsEnd*KoefY)&&(nstr > nstrRsEnd*KoefY)&&(ErrScan >0))
	  { // поиск конца рамки

        cout << " The End of Frame Kod =    " << ErrScan    << endl;

		if(ErrScan==10)
		{ // наклон равен нулю, одновременно слева и справа наступил конец рамки
		  EndYoL=nstr-LsNs; EndYoR=nstr-RsNs;
		} else
		{ if(ErrScan==11) EndYoL=nstr-LsNs; if(ErrScan==12) EndYoR=nstr-RsNs;
		}
		ErrScan=0;
		if((EndYoL > 0)&&(EndYoR > 0))
		{ 	// формирование данных для  начала  поиска данных типа выборов
			//  в  конце страницы


			zz=int(dxVoteType/2);
			DeltaY=int(float(dyStampArea+dyVoteType +int(0.5*float(TMarSize)))*CosAlfaPage);
			for(i=0;i < NumVoteType+1; i++)
			{  ncVoteType[i]= EndXoL+zz+dxVoteType*i;
			   nstrVoteType[i]= EndYoL+NumDotY+int(SinAlfaPage*float(ncVoteType[i]));
			   PrVoteTypeStart[i]=0; nbVoteT[i]=0;  PrVoteType[i]=0;
			}
			nstrBegVoteType =nstrVoteType[0];
			if(nstrBegVoteType < nstrVoteType[NumVoteType]) nstrBegVoteType =nstrVoteType[9];
		}
	  }
	  if (ErrScan >0) goto FinEvRow;
	  ///////////////////////////////////////////////////////////////////////////////
	  //  ДЕЙСТВИЯ ПРИ  РАЗЛИЧНОМ  НАПРАВЛЕНИИ БЮЛЛЕТЕНЯ
	  if (BulHead==0)
	  {  // распознавание в  прямом направлении
		 //----- Определение наличия штампа и его  координат
		 if(PrCheckSh==1)
		 { //  есть признак  проверки штампа
			if (PrShFound==0)
		 	{  urbl1=urbl;
				//urbl=int((urw-urbl)/2);//65;
                //urbl=100;
                urbl=urbl+(urw-urbl)/3;
				ErrScan=FindNumYchTop(&PrShFound);
				if(PrShFound==1)
				{ // штамп печати найден, обращение для распознавания номера
				  //  ОБРАЩЕНИЕ К ФУНКЦИИ  РАСПОЗНАВАНИЯ НОМЕРА ИУ

				//while (urbl < 80)
				//{
					NumUIKRec=tryConfirmStamp(1);

					cout << " Number  UIK   ID =    " << UIKNum[0]    << endl;
					cout << " Number  UIK   =    " << NumUIKRec << endl;


					if(NumUIKRec > 0)
					{  // преобразование номера участка в  строку
					   for(i=0;i < 5;i++)NumUIKStr[i]='\0';
						for(i=0;i < 4;i++)
						{ ii=StZ[i];
						   switch (ii)
							{ case 0: strcat(NumUIKStr,"0"); break;
							  case 1: strcat(NumUIKStr,"1"); break;
							  case 2: strcat(NumUIKStr,"2"); break;
							  case 3: strcat(NumUIKStr,"3"); break;
							  case 4: strcat(NumUIKStr,"4"); break;
							  case 5: strcat(NumUIKStr,"5"); break;
							  case 6: strcat(NumUIKStr,"6"); break;
							  case 7: strcat(NumUIKStr,"7"); break;
							  case 8: strcat(NumUIKStr,"8"); break;
							  case 9: strcat(NumUIKStr,"9"); break;
							};
						}
						//goto finNumUIK1;
					} else  {ErrScan=71; goto FinEvRow;}
					/*else
					{
					  if(urbl >=80) ErrScan=71; goto FinEvRow;
					}
					urbl=urbl+10;*/
				//}
                //finNumUIK1:;


					ii=0;
					for(i=0;i < MaxNumUIK;i++)
					if((UIKNum[i]>0)&&(ii==0))
					{ if(NumUIKRec==UIKNum[i]) ii=1; };
					if (ii==0) { ErrScan=7;  } ; //goto FinEvRow; }
				}
				urbl=urbl1;
		 	} // if(PrShFound==0) поиска печати номера УИК
		 	if (ErrScan >0) goto FinEvRow;
		 }//  конец  признака  проверки штампа
		 // распознавание отметок  голосования
         urw1=urw;  urw=150;
				FindVoteMarks();
         urw=urw1;
		 // подготовка даннвх для поиска конца страницы
		 if((ShArPr==0)&&(nstr > nsShArStart))
		 {  //  начинается на  расстоянии зоны штампа в прямом
			// направлении
			ShArPr=1;
			ShArYo=nstr;	ShArXo= LsXo;
			//	уточнение угла наклона листа
			dyALfa=ShArYo-PgYoL; dxALfa=ShArXo-PgXoL;
			AlfaPage=-float(dxALfa)/float(dyALfa);
			SinAlfaPage=sin(AlfaPage); CosAlfaPage=cos(AlfaPage);
			TanAlfaPage=SinAlfaPage/CosAlfaPage;
			//  подготовка к  поиску конца  страницы
			nbTL=0;nbLL=0;nbRR=0; nbTR=0;
			DeltaY=int(float(dyStampArea)*CosAlfaPage);
			DeltaX= RsXo-LsXo;
			nstrLsEnd= ShArYo+DeltaY;//nstrVoteType[0]-int(1.2*float(TMarSize));
			nstrRsEnd= nstrLsEnd + int(DeltaX*SinAlfaPage);

		 }
		 // проверка  окончания распознавания и перехода к  поиску
		 //  нижней  рамки листа при прямом проходе бюллетеня
		 //if((nstr > nstrLsEnd)&&(nstr > nstrRsEnd)) PrPage=3;
		 if((EndYoL > 0)&&(EndYoR > 0)) PrPage=3;

	  } //BulHead==0
	  else
	  { //  распознавание  в  обратном направлении
		//  поиск начала зоны штампа
		if(ShArPr==0)
		{ ErrScan=FindShAreaStart();
		  if(ShArPr==1)
		  { //  подготовка к  поиску конца  страницы
			nbTL=0;nbLL=0;nbRR=0; nbTR=0;
			nstrLsEnd=nstrVoteType[0]-int(1.2*float(TMarSize));//dyVoteType-2*NumDotY;
			nstrRsEnd=nstrVoteType[NumVoteType]-int(1.2*float(TMarSize));//dyVoteType-2*NumDotY;
			DeltaY=int(float(dyStampArea)*CosAlfaPage);
			DeltaX= RsXo-LsXo;
			nstrLsEnd= ShArYo+DeltaY;//nstrVoteType[0]-int(1.2*float(TMarSize));
			nstrRsEnd= nstrLsEnd + int(DeltaX*SinAlfaPage);
			}
		}
		if (ErrScan >0) goto FinEvRow;
		// распознавание отметок  голосования
		         urw1=urw;  urw=150;
				   FindVoteMarks();
				            urw=urw1;
		// поиск  координат  печати УИК
		// Определение наличия штампа и его  координат
		if(PrCheckSh==1)
		{ //  есть признак  проверки штампа
			if((PrShFound==0)&&(ShArPr==1))
			{ urbl1=urbl;  //urbl=55;
			  //urw1=urw;    urw=urw-(65-urbl1);
			  //urbl=int((urw-urbl)/2);//65;
			  urbl=urbl+(urw-urbl)/3;
			  ErrScan=FindNumYchBot(&PrShFound);
			  if(PrShFound==1)
			  { //штамп печати найден, обращение для распознавания номера
				//  ОБРАЩЕНИЕ К ФУНКЦИИ  РАСПОЗНАВАНИЯ НОМЕРА ИУ
				//while (urbl < 80)
				//{
					NumUIKRec=tryConfirmStamp(0);

                    cout << " Number  UIK   ID =    " << UIKNum[0]    << endl;
					cout << " Number  UIK   =    " << NumUIKRec << endl;

					if(NumUIKRec > 0)
					{  // преобразование номера участка в  строку
					   for(i=0;i < 5;i++)NumUIKStr[i]='\0';
						for(i=0;i < 4;i++)
						{ ii=StZ[i];
						   switch (ii)
							{ case 0: strcat(NumUIKStr,"0"); break;
							  case 1: strcat(NumUIKStr,"1"); break;
							  case 2: strcat(NumUIKStr,"2"); break;
							  case 3: strcat(NumUIKStr,"3"); break;
							  case 4: strcat(NumUIKStr,"4"); break;
							  case 5: strcat(NumUIKStr,"5"); break;
							  case 6: strcat(NumUIKStr,"6"); break;
							  case 7: strcat(NumUIKStr,"7"); break;
							  case 8: strcat(NumUIKStr,"8"); break;
							  case 9: strcat(NumUIKStr,"9"); break;
							};
						}
						//goto finNumUIK;
					}else  {ErrScan=71; goto FinEvRow;}
			/*		else
					{
					  if(urbl >=80) ErrScan=71; goto FinEvRow;
					}
					//urbl=urbl+10;
				//}
				finNumUIK:;
				*/
				ii=0;
				for(i=0;i < MaxNumUIK;i++)
				if((UIKNum[i]>0)&&(ii==0))
				{ if(NumUIKRec==UIKNum[i]) ii=1; };
				if (ii==0) { ErrScan=7; };//goto FinEvRow; }
			  }
			  urbl=urbl1;
			  //urw=urw1;
			}// if(PrShFound==0) поиска печати номера УИК
			if (ErrScan >0) goto FinEvRow;
		} // конец  признака проверки штампа
		// При обратном ходе бюллетеня  переход к  проверке окончания  листа
		// осуществляется при наличии штампа, в противном случае - ошибка
		if((EndYoL > 0)&&(EndYoR > 0)) PrPage=3;
	  } //BulHead
	  /////////////////////////////////////////////////////////////////////////
	  //  ОБЩИЕ ДЕЙСТВИЯ  НЕЗАВИСИМО ОТ  НАПРАВЛЕНИЯ БЮЛЛЕТЕНЯ
	  if(PrPage==3)
	  { //  подготовка к  поиску конца  страницы
		// гашение временного массива работы с квадратами
		for(j=0;j < NumSQMax[nVoteType]+1;j++) delete [] SearchSQ[j]; delete [] SearchSQ;
		PrPageEnd=0; // гашение признака окончания  страницы для начала контроля
		//  подготовка данных для  поиска  конца листа
		nstrPageEnd =nstrVoteType[0];  colTestPageL= ncVoteType[0];
		if(nstrPageEnd < nstrVoteType[NumVoteType])
		{ nstrPageEnd =nstrVoteType[9];  colTestPageL= ncVoteType[9]; }
		nstrPageEnd=nstrPageEnd+dyVoteType;
		dxTestPage=10;
		nbTL=0;
	  }
   } //if(PrPage==2)
/////////////////////////////////////////////////////////////////////////////
//-  PrPage=3 -О П Р Е Д Е Л Е Н И Е    К О Н Ц А  Р А М К И  И  В И Д А  Б Ю Л Л Е Т Е Н Я
   if(PrPage==3)
   {  // бюллетень считан, распознан, определение конца бюллетеня
	  if(PrPage==3)
	  { urw1=urw;
		urw=150;
		ErrScan=FindVoteTypeBot(&PrPage);
		if(PrPage==4)
		{ //  сравнение  строки типа  выборов внизу и вверху
		    printf(" Start to Compare Vote Type on the Top and on the  Bottom   \n");
            j=0;
            if(BulHead==0)
            { for(i=1;i < NumVoteType+1; i++)	if (PrVoteTypeTop[i]!=PrVoteType[NumVoteType - i]) j=1;
            } else
            { for(i=0;i < NumVoteType; i++) if (PrVoteType[i]!=PrVoteTypeTop[NumVoteType-i]) j=1;  }
            if (j==1) {  ErrScan=5; goto FinEvRow;}

            if (ErrScan ==0)
            {
                printf(" Bottom  Vote Type  OK !  Form Results   \n");

                if(MasScan.MasRecOut[0]==0)
                { MasScan.MasRecOut[0]=FormRezScanBull();  }
            }
		}
		urw=urw1;
	  }
	  if (ErrScan >0) { goto FinEvRow;  }
   } //if(PrPage==3)

/////////////////////////////////////////////////////////////////////////////
//-  PrPage=4  ---О П Р Е Д Е Л Е Н И Е   К О Н Ц А  Л И С Т А-----------------
   if(PrPage==4)
   {  //  переписать  результаты  распознавания  страницы в данные
	  //  результатов  голосования
	  ////////////////////////////
	  // RezRec
//	  if(MasScan.MasRecOut[0]==0)
//	  { MasScan.MasRecOut[0]=FormRezScanBull();  }
	  /////////////////////
	  // контроль количества   строк после  определения  конца рамки бюллетеня
	  // если через установленное  время  двигатель  автоматически
	  // не  остановился, то это говорит о  том,  что может  идти второй бюллетень
	  // без зазора, поэтому  небходимо дать возможность упасть в  урну
	  //  распознанному  бюллетеню, а  затем  включить двигатель на  реверс
	  // до выбрасывания  бюллетеня  назад
	  if(PrPageEnd==0)
	  { ErrScan=TestPageEnd(&PrPageEnd);
		if (ErrScan >0) goto FinEvRow;
	  }
   } // if(PrPage==4)
   FinEvRow:; // конец обработки строки
   return ErrScan;
}//ScanBul


void GashMasVote(void)
{ // гашение динамических массивов

 // delete MasSQ;
   try
   { // гашение массив а  с  данными поквадратам голосования

	for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
	{ 	delete  [] MasSQ[i][j][0]; delete  [] MasSQ[i][j][1]; }
	for(i=0;i < NumVoteType+1;i++)
			for(j=0;j < NumSQMax[i]+1;j++) delete [] MasSQ[i][j];
	for(i=0;i < NumVoteType+1;i++)  delete [] MasSQ[i];
	delete [] MasSQ;
		// гашение числа точек в  квадратах голосования
	for(i=0;i < NumVoteType+1;i++) { delete [] NumBlP[i]; delete [] NumPMax[i];}
	delete [] NumBlP; delete [] NumPMax;
	   // гашение промежуточного  массива результатов
	for(i=0;i < NumVoteType+1;i++) delete [] RezRec[i]; delete [] RezRec;
	}
   catch (std::bad_alloc)
   {
	 //ShowMessage(" Ошибка освобождения динамической памяти");
	 cout << "Ошибка освобождения памяти. MasSQ ...";
	}

}


