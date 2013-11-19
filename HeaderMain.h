//*************************//Говорить простые события****************************************************
// Общий  заголовочный файл для  объявления  прототипов переменных и функций
// используемых различными модулями реализации (разных разработчиков)
//                     О.Чудаков      23.05.2013
//******************************************************************************


#ifndef HeaderMainH
#define HeaderMainH
#include <time.h>
#include <string>

#include "MyX.h"

int Win_KoiConvert(char* St);
int Koi_WinConvert(char*  St);

//-------------глобальные  переменные, которые  должны быть в ВППО--------------
// из Ruler
const   int NumStrScan=1728; // количество точек в  строке
const   int NumVoteType=9;         //  количество  типов  выборов
const   int NumBlankMax=9;        // количество типов бланков максимальное
const   int NumParPage=7;         // количество параметров страницы
const   int NumVoprMax=10;      // максимальное количество вопросов (секций) в бюллетене
const   int NumCandMax=100;   // максимальное количество кандидатов
const   int MaxNumUIK=6;          // максимальное количество номеров штампов на выборах для данного  участка
const   int	MaxModes=3;    //максимальное количество режимов голосования. 1-стационарное, 2-переносное
const   int	NstrokMax=1000; //максимальное количество строк в протоколах
const   int	MaxProtokoles=2; //максимальное количество видов протоколов для каждых выборов
const   int	NchecksMax=20; //максимальное количество контрольных соотношений
const   int NumMenuItems=16; //  количество пунктов сервисного меню
const   int NumSensors=4;  // количество оптодатчиков
// ------------------------------------------------------------------------------

//  массивы границ  для  каждого  оптрона
extern int NO_PAPERO[NumSensors];
extern int ONE_PAPERO[NumSensors];

// взаимодействие  КОИБ
extern int PrConnectOK; // признак установки  связи со  вторым  сканером

extern   std::string FileName;
bool Parser(std::string Path); // Парсер XML-файла
extern char NUMBER[32]; //  буфер, где  формируется   число с  клавиатуры
extern  int PrEnterStart;
extern  int PrKlav;
extern  int Pr_Event;
extern int PrUprOpt; // признак состояния  оптронов
extern int  Sost_Optrons[4]; // состояние оптронов (наличие или отсутствие листов)

extern  int PrModeChange;
extern int PrNoBtnWait; // признак пропуска ожидания  кнопки
extern int pr_btn;
extern  time_t timer;
extern  struct tm* TekTime;
//------------------данные  по  странице из исходных данных---------------------
extern  char SerNumKoib[124];
extern  char SerNumKoib1[124];
extern  float ScanerWorkTime;

 // параметры КОИБ  из UnitKoibScan

extern   int UIKNum[MaxNumUIK];      //номера штампов, допустимых при выборах
extern   int Lang;  // язык интерфейса
extern   int VolumeUr; // уровень громкости
extern   int VolumeUrMax; // максималный уровень громкости
extern   char SourceDir1[128]; ////-------------глобальные  переменные, которые  должны быть в ВППО--------------

extern   char SaveDir[128]; //
extern   int PrPrint; //
extern   int PrSlave;
extern   int wngtime;
extern   int PrSaveLog;
extern   char Ip_master[16];
extern   char Ip_slave[16];
extern   char iface[16];
extern   char udp_port[16];
extern   int timeout;

extern   float MaxAngle;// максимальныц угол наклона  бланка
extern   int PrCheckBr; // признак необходимости проверки границ рамки
extern   int PrCheckSh; // признак необходимолсти проверки  штампа
extern   int PrCheckEx;// признак необходимости маркировки недействительных бюллетеней

extern   int MaxLightR; // максимальный уровень подсветки при калибровке
extern   int MaxLightG; // максимальный уровень подсветки при калибровке
extern   int MaxLightB; // максимальный уровень подсветки при калибровке

extern   float KoefY,KoefX;
extern   int PrKoefY,NumDotKoefY;
extern   int PrIdOk; // признак того, что исходные данные для сканирования переписаны в рабочин ячейки
extern   int PagePar[NumVoteType+1][NumParPage]; // массив параметров страницы
extern   int PrVoteTypeId[NumVoteType+1]; // признаки типа бюллетеней в  выборах на основе исходных данных

extern   float Max_Bull_Height; // максимальная длина бюллетеня
extern   float Max_Bull_Width; // максимальная длина бюллетеня
extern   float Dx_Max; // допустимое отклонение ширины листа
extern   float Dy_Max; //  допустимое отклонение высоты  листа
extern   float MaxAngle;// максимальныц угол наклона  бланка
extern   float Dig_Height; //  высота  цифр в  бюллетене
extern   float Dig_Width; //  высота  цифр в  бюллетене
extern   float Dig_Interv; //  расстояние между  цифрами штампа
extern   float Max_ShAngle; // максимальный угол наклона штампа

extern   float PageWM; // ширина  страницы в  мм
extern   float PageHM; // высота  страницы в  мм
extern   float dxVoteTypeM; // ширина зоны типа выборов по горизонтали в  милиметрах
extern   float dyVoteTypeM;    // ширина зоны типа выборов по вертикали в  милиметрах
extern   float LineWM;    // толщина  линий зоны типа выборов в  милиметрах
extern   float LMarSizeM; // размер полей  бюллетеня в  миллиметрах по горизонтали
extern   float TMarSizeM; // размер полей  бюллетеня в  миллиметрах  по вертикали
extern   	int LMarSize; // размер полей  бюллетеня в  точках по горизонтали
extern   	int TMarSize; // размер полей  бюллетеня в  точках  по вертикали
extern   float dxStampAreaM; // ширина  зоны  печати в  мм
extern   float dyStampAreaM; // высота  зоны  печати в  мм
extern 	 int nsShArStart,nbShAr,ShArXo,ShArYo,ShArPr,colShAr;
extern   float dyHeaderM; // высота заголовка  в мм
extern	int nVoteType;


extern int SymWidth ; // ??
extern int SymHeigt ; // ??
extern int SymDist[3] ;

struct TScanOut // выходной массив результатов сканирования  листа
{ int MasRecOut[5]; //[0] – код ошибки; [1] - номер типа бланка;
  // [2] - номер распознанного участка   [3] - количество вопросов на бланке
  int MasVoprOut[NumVoprMax][3];
		// [0] – наличие вопроса в бюллетене, определяется максимальным допустимым количеством отметок;
		// [1] - характеристика результата ( действителен, недействителен, причины
		// [2] - количество выбранных кандидатов (отметок)
  int MasCandOut[NumVoprMax+1][NumCandMax+1]; // номер ячейки равен номеру кандидата
		// [0] -  всего кандидатов  в данном  вопросе
		// [1..NumCandMax] - "1" - если есть отметка, "0" - если  нет  отметки
};
extern TScanOut MasScan;
// ------------------переменные  для  определения  цвета  точки (черный-белый)
extern	int UrWhite0; // эталон уровня белого
extern	int UrBlack0; // эталон уровня черного

extern 	int	NumV;   // угол наклона  листа
extern 	int MaxMarks;   // угол наклона  листа

extern 	double	AlfaPage;   // угол наклона  листа
extern  float dxStampM; // ширина  зоны  печати в  мм
extern  float dyStampM; // высота  зоны  печати в  мм
extern  float dyNumZoneM;// высота  зоны номера  в мм

extern	unsigned urw_ch; // значение интенсивности точки
extern	int NumDotX; // количество  точек на  мм  по горизонтали листа
extern  int NumDotY; // количество  точек на  мм  по вертикали   листа
extern  float p00,   // вероятность того,  что  точка  имеет  черный  цвет
		p11,    // вероятность того,  что  точка  имеет  белый  цвет
		alfa,   // вероятность ошибки  первого  рода ( пропуска  черного)
		beta,// вероятность ошибки  второго рода (принрятия белого за  черный)
		g0,     // гипотеза "Черный" "1" - цвет  черный
 		g1,     // гипотеза "Белый"  "1" - цвет  белый
		surw,   // Оценка СКО мат. ожидания  уровня  белого (среднее значение)
 		surbl,  // Оценка СКО мат. ожидания  уровня  черного (среднее значение)
 		urw,    // Оценка мат. ожидания  уровня  белого (среднее значение)
		urbl,   // Оценка мат. ожидания  уровня  черного (среднее значение)
		UrWtSh,   // Оценка мат. ожидания  уровня  белого  для  поиска штампа(среднее значение)
		UrBlSh;   // Оценка мат. ожидания  уровня  черного для поиска штампа(среднее значение)
extern  float AlfaSh;
extern  int XoShL,XoShR,YoShL,YoShR; // координаты левого и правого нижних углов штампа
extern  int nstr;   // номер  текущей  строки
extern  int InfSTablo1,InfSTablo2; // номера сообщений на  табло в строку 1,2
// из UnitKoibScan
unsigned GetXYValue(int xi,int yi); // получение  значения  уровня  сигнала в  точке
void EvChangeColCol(int BlWt,int nstr,int step,int ncT,int dxT,int* nbT,int* yo,int* prT);
void EvChangeColRow(int BlWt,int nstr,int step,int ncL,int dxL,int* nbL,int* xo,int* prL);

int RecBlack(float alfa,float beta,float urw,float surw,float urbl,float surbl,
			 float urs,float* p00,float* p11); // определение  черного
void BLev(int nn,unsigned urw_ch,float* ur,float* sur); // уровень черный-белый
void GashBeforeScan(void); // гашение  перед сканированием бюллетеня
int ScanBulString(void);//  функция управления  сканированием бюллетеня при поступлении  очередной  строки
// ---------------------------------------------------------------------------
// из UnitServiceMenu
extern  float MasSenValue[NumSensors][3];//массив значений порогов датчиков для листов
void TestScan(); // функция  сканирования
int FindSourceFile(void); // формирование  пути к  файлу исходных данных
void VoteScan(); //  сканирование  в  режиме  голосования
void SetUpKoibWrite(void);
int SetUpKoibRead(void);
void SetUpUprWrite(void);
int SetUpUprRead(void);
void SaveRecParToFile(void);
int SetUpRecRead(void);
void SetUpBullWrite(void);
int SetUpBullRead(void);
void SaveKoeffSens(void);
int ReadKoeffSens(void);

int ReadSetUpFiles(void); // считывания файлов исходных данных
void FormIdScan(void);
int FormRezScanBull(void);

void GetPzsScan(int ur);// сканирование  для  калибровки
void GetSensorsValue(int n);
void SendInfToTablo2(void);
void SendInfToTablo1(void);
int ServMenu(void); // функция работы в сервисном меню
// ---------------------------------------------------------------------------
// из Detect
extern int StZ[4];   // All Symbols of Stamp
int tryConfirmStamp(int trend);
// ---------------------------------------------------------------------------


//--------RULER------------------------Начало описания Исходных Данных-----------------------------------------

extern int PrVoteTimeStart;
extern int PrVoteTimeEnd;
extern int PrVoteDay;

struct TCandidates // класс, описывающий кандидатов
{
	int N; // номер кандидата
	std::string ID; // идентификатор кандидата
	std::string lastName; // фамилия (да/нет в референдуме) кандидата
	std::string firstName; // - имя
	std::string patronymic; // - отчество
	bool registered;
	// - Кандидат зарегистрирован и будет размещен на бюллетене?
	bool disabled; // - Кандидат снят с голосования?
	bool disabledLocally;
	// - Кандидат снят с голосования на УИК (Оператором КОИБ)?
	std::string biography; // – биография кандидата
	bool selfRegistered; // - Признак самовыдвижения
	std::string party; // – Членство в политической партии
	bool noneAbove; // - Наличие отметки "Против Всех"
	int SQx; // – смещение квадрата кандидата на бюллетене по оси X , 0,1 мм
	int dx; // – ширина стороны квадрата, 0,1 мм
	int SQy; // – смещение квадрата кандидата на бюллетене по оси Y , 0,1 мм
	int dy; // – высота стороны квадрата, 0,1 мм
	int SQ; // – оставил, чтобы не ломать существующую структуру=dx
};

struct TPC // описываем вышестоящую комиссию
{
	std::string parentid; // - идентификатор комиссии из ГАС "Выборы"
	std::string name; // Название комиссии
	std::string email; // - мыло комиссии
};

struct TVoteLine // описываем строки
{
	bool bold;
	bool italic;
	int fontSize;
	std::string ID;
	std::string type;
	std::string ElectronNum;
};

struct TProtocolLine
{
  std::string align;
  bool bold;
  bool italic;
  int fontSize;
  std::string section;
  std::string text;
};

struct Ttext // описываем текст протокола
{
	bool final; // отвечает за принадлежность шаблона 1-итоговый, 0-о распределении голосов
	int NVoteLine; // кол-во строк таблицы данных Table
	TVoteLine VoteLine[NstrokMax+1]; // описание строк таблицы данных Table
	int NProtocolLine; // кол-во строк протокола
	TProtocolLine ProtocolLine[NstrokMax+1]; // описание строк протокола

};

struct TChecks // описываем контрольные соотношения
{
	std::string expression; // - формула проверки контрольного соотношения
	bool Enabled; // - включено ли контрольное соотношение
	bool mild;	// - признак того, что КС мягкое, т.е. допускается отправка протокола, если такое КС не выполнено
    bool Itog; // Выполнено ли в итоге контрольное соотношение
    std::string ItogExpression; //Итоговая формула
};

struct TLines // описываем строки протоколов
{
	int N; // – номер строки протокола
	int ID; // – ID строки протокола
	std::string Name; // – наименование строки
	std::string expression; // формула вычисления значения строки
	std::string AddNum; // – дополнительный номер строки
	std::string type; // – тип строки (1 - DontQueryUser – не запрашивать ввод строки у пользователя;
	                  //               2- Voting – строка для голосования;
	                  //               3- Blank – строка для бланка;
	                  //               4 - Election – строка для выборов)
	int Value;// Итоговое значение строки
};

struct TProtocoles // класс, описывающий протоколы выборов
{
	bool Enable; // Есть или нет описания протокола
	std::string Name; // – наименование протокола
	int numberWidth; // - Ширина колонки с номером строки
	int nameWidth; // - Ширина колонки с именем строки
	int valueWidth; // - Ширина колонки со значением строки
	int textValueWidth; // - Ширина колонки со значением строки прописью
	std::string disabledString; // - Строка - способ отображения снятых кандидатов.
	std::string Font; // – 1..3 (1-Default, 2-Verdana, 3-ArialNarrow)
	int Fontsize; // – размер шрифта
	bool doNotReadOut; // - Не зачитывать протокол?
	int NLines; // – количество строк в протоколе
	TLines Lines[NstrokMax+1]; // – массив описания строк протокола
	int NChecks; // - количество контрольных соотношений
    int S; // Сумма голосов, поданных за все альтернативы
    int M; // Количество мандатов
	TChecks Checks[NchecksMax+1]; // – массив описания контрольных соотношений
	Ttext Text;   // - описание шаблона протокола
	std::string ProtX; //Текст протокола
};

struct TVopr // Структура, описывающая заданный вопрос
{
	int N; // номер выборов (референдума), к которым относится вопрос
	std::string ID; // уникальный идентификатор выборов (референдума), к которым относится вопрос
	std::string Name; // название выборов (референдума), к которым относится вопрос
	TPC PC; // Данные о вышестоящей комиссии
	int Nsc; //Количество вышестоящих комиссий
	int SC[5]; // Номера вышестоящих комиссий
	int Nmodes; // кол-во режимов голосования
	std::string Modes[MaxModes]; // режимы голос-я
	int maxMarks; // максимальное количество мандатов
	bool Anarchy; // 1-отметка против всех есть, 0-нет
	int NCandidates; // количество кандидатов на выборах (альтернатив в референдуме)
	TCandidates Candidates[NumCandMax+1]; // массив описания кандидатов на выборах (альтернатив в референдуме)
	int NProtocoles; // – количество протоколов для выборов
	TProtocoles Protocoles[MaxProtokoles+1];	// массив описания протоколов для выборов
};
struct TRef // Структура, описывающая параметры референдум
{
	int N; // номер выборов (референдума), к которым относится вопрос
	std::string ID; // уникальный идентификатор выборов (референдума), к которым относится вопрос
	std::string Name; // название выборов (референдума), к которым относится вопрос
	TPC PC; // Данные о вышестоящей комиссии
	int SC; // Номер комиссии
	int Nmodes; // кол-во режимов голосования
	std::string Modes[MaxModes]; // режимы голос-я
	int NProtocoles; // – количество протоколов для референдума
	TProtocoles Protocoles[MaxProtokoles+1]; // массив описания протоколов для референдума
};

struct Tpole // описание поля
{
  int x;   //расстояние от левого края в 0.1 мм
  int y;   //расстояние от верха в 0.1 мм
  int dx;  //протяженность по горизонтали в 0.1 мм
  int dy;  //расстояние по вертикали в 0.1 мм
  int list; //на каком листе располагается
};

struct TBlank // описываем бланк
{
	std::string Name; // – наименование бланка
	std::string ID; // – ID бланка
	int Width; // – ширина бланка (портретная ориентация), 0,1 мм
	int Height; // – высота бланка (портретная ориентация), 0,1 мм
	int Delta;	// - Допустимая дельта для расчета минимальной и максимальной длины бланка, 0,1 мм
	int Orientation; // – (1-портретная, 2-альбомная, 3-портретная и альбомная)
	int maxPShift;	// - Максимально допустимое смещение относительно левого края для портретной ориентации (если не задано, то допускается любое смещение)
	int maxLShift;	// - Максимально допустимое смещение относительно левого края для альбомной ориентации (если не задано, то допускается любое смещение)
	int ShAreaHeight;
	int density; // - Абсолютный порог плотности бумаги бюллетеней данного типа
	int marker; // - Тип маркера: цифровой или штрих-маркер
	std::string markerType; // - ???
	bool Refer;// Если 1, то вопрос относится к референдуму, если 0 - то к выборам
	TRef Ref; // Описание данных референдума (если это референдум)
	int NVopr; // количество вопросов (альтернатив), которые описывает бюллетень
	TVopr Vopr[NumVoprMax+1]; // массив описания вопросов (альтернатив) на бланке

	Tpole MB; //описание маркера бюллетеня
	int Nlh; //кол-во горизонтальных линий в бюллетене
	Tpole LH[20]; //описание горизонтальных линий в бюллетене
	int Nlv; //кол-во вертикальных линий в бюллетене
	Tpole LV[20]; //описание вертикальных линий в бюллетене
	int Nsq; //кол-во квадратиков в бюллетене
	Tpole SQ[NumCandMax]; //описание квадратиков в бюллетене

};

struct TVotes //Описание выборов
{
    std::string ElName; //наименование выборов ! ???
    std::string UIKname; //Имя текущего УИК
    int Nuik; //Номер текущего УИК
    int DByear; // – дата выборов
    int DBmonth;
    int DBday;
    std::string DateV; // дата выборов
    std::string IDel;  //ID выборов !
    bool isGASvrn; // признак использования ИК ВРН из ГАС Выборы
    std::string ProtokolPassword;
    std::string Password;
    int Nmodes; // кол-во режимов голосования
    std::string Modes[MaxModes]; // наименование режимов голос-я
    int Bhour[MaxModes]; // Время начала голосования 1-СГ, 2-ПГ
    int Bmin[MaxModes];
    int Bsec[MaxModes];
    std::string TimeB[MaxModes]; //Время начала голосования 1-СГ, 2-ПГ
    bool Unlim; //если время голосоввания не указано
    int Nblank;  // количество типов бланков в день голосования
    TBlank Blank[NumBlankMax+1];  // массив описания типов бланков в день голосования

    int NUIKnum; // количество номеров ИК
    int UIKNum[MaxNumUIK];  //номера штампов, допустимых при выборах


};
//--------------------------------Конец описания Исходных Данных-----------------------------------------
 extern TVotes Votes; // Переменная для описания выборов

//--------------------------------Начало описания динамики голосования-----------------------------------

struct TVop
	// Описание голосов, поданных в тех или иных вопросах за тех или иных кандидатов
{
	int All; // – общее количество поданных бюллетеней в вопросе
	int Nuf; //Неустановленной формы
	int NoValid; // – количество бюллетеней недействительных -//-
	int NoMarks; // – количество бюллетеней без отметок  -//-
	int ManyMarks; // – количество бюллетеней с превышением количества отметок -//-
	int Valid; // – количество действительных бюллетеней -//-
	int Can[NumCandMax+1]; // массив описания голосов, поданных за тех или иных кандидатов
};

struct TBl
	// Описание голосов, поданных в тех или иных вопросах за тех или иных кандидатов на N-ном бланке
{
	TVop Vop[NumVoprMax+1]; // массив описания голосов на опр. вопросах
};

struct TRezult // - структура, описывающая режим голосования
{
	int All; // – общее количество поданных бюллетеней
 	int Nuf; //Неустановленной формы
	int NoValid; // – количество бюллетеней недействительных -//-
	int NoMarks; // – количество бюллетеней без отметок  -//-
	int ManyMarks; // – количество бюллетеней с превышением количества отметок -//-
	int Valid; // – количество действительных бюллетеней -//-
	TBl Bl[NumBlankMax+1]; // - массив описания голосов на опр. бланках
};

struct TKoib // – класс, описывающий работу КОИБов
{
	std::string N; // - серийный номер КОИБа
	std::string PO; // - версия ПО КОИБа
	std::string IP; // - IP адрес КОИБа
	bool Avar; // Если 1, то КОИБ восстает после аварийного режима
	bool Main; // - КОИБ главный?
	bool Avto; // - КОИБ работает в автономном режиме?
	int VS; // - вектор состояния КОИБА
	bool Tren; // - режим голосования в режиме тренировки?
	bool TestV; // - режим тестового голосования?
	bool StacV; // - режим стационарного голосования?
	bool PortV; // - режим переносного голосования?
	bool Opros;	// флаг, указывающий на необходимость опроса оптодатчиков (режим сканирования и распознавания бюллетеня ON)

	TRezult Rezult[3]; // результаты голосования 0-тест, 1-стац., 2-перенос.
	// - результаты работы КОИБ в режиме: 1- стационарного голосования, 2-переносного голосования
};
//--------------------------------Конец описания динамики голосования-----------------------------------
struct TSnat // снятые позиции выборов
{
   int Nsnat; // кол-во снятых позиций
   int Snat[NumCandMax+1]; // перечень снятых позиций в голосовании
};

struct TInd
{
    std::string Main[256]; // Индикация на табло в основном режиме
    std::string Menu[256]; // Индикация на табло в режиме меню
    std::string Menu1[256]; // Индикация на табло в режиме меню
    std::string Votes[256]; // Индикация на табло в режиме голосования
    std::string Test[256]; // Индикация на табло в режиме самотестирования

    std::string Num[256]; // Числительные
    std::string Months[13]; //Месяцы

    std::string PrID[256]; // Сообщения для формирования протокола ИД
    std::string PrTest[256]; // Сообщения для формирования протокола тестирования КОИБ
    std::string PrRezult[256]; // Сообщения для формирования протокола результатов голосования
    std::string PrCh[256]; // Сообщения для формирования протокола несхождения КС
    std::string PrIP[256]; // Сообщения для формирования итогового протокола голосования

};
struct TVoice
{
    std::string Main[256]; //Текст голосовых сообщений
    std::string MainPath[256]; //Путь к их wav-файлам

    std::string Snat[256]; //Текст голосовых сообщений
    std::string SnatPath[256]; //Путь к их wav-файлам

    std::string ID[256]; //Текст голосовых сообщений
    std::string IDPath[256]; //Путь к их wav-файлам

    std::string Num[256]; //Текст голосовых сообщений
    std::string NumPath[256]; //Путь к их wav-файлам

    std::string Months[256]; //Текст голосовых сообщений
    std::string MonthsPath[256]; //Путь к их wav-файлам

    std::string Votes[256]; //Текст голосовых сообщений
    std::string VotesPath[256]; //Путь к их wav-файлам

    std::string Test[256]; //Текст голосовых сообщений
    std::string TestPath[256]; //Путь к их wav-файлам

    std::string Rezult[256]; //Текст голосовых сообщений
    std::string RezultPath[256]; //Путь к их wav-файлам

};
    // Lang; // 0 - русский 1 - английский

   extern std::string Nst[256]; //числовые сообщения
   extern std::string Nvc[256]; //пути звуковых файлов
   extern std::string Mst[256]; //сообщения о месяцах
   extern std::string Mvc[256]; //пути к озвучиванию месяцев

   extern TInd Out; //Массив строковых переменных выводимых на индикатор
   extern TVoice OutV; //Массив голосовых сообщений
   extern bool SayYes; // Говорить ли сообщения
//-----------------------------------Протоколы (От АИ)--------------------------

//-----------------------------------------------------------------------------------------------------------
 extern int Version; //Номер версии сообщений!
 extern std::string SourceDir; // -Путь к источнику данных
 extern int NKoib; // – количество КОИБов в связке (2)
 extern TKoib Koib[3]; // - массив, описывающий работу КОИБов
 extern TSnat Snat[NumBlankMax+1];//массив снятых позиций выборов
 extern int BhourU[MaxModes];
 extern int BminU[MaxModes];
 extern std::string TimeBU[MaxModes];
 extern TDataTime TekDT; // Дата, время текущие
 extern std::string DateS;  // Дата текущая
 extern std::string TimeS;  // Время текущее

 void LoadInd(void); // Загрузка выводимых на табло значений
 void TekDateTime(void);
 void SayVotes(int Type);
// ---------------------------------------------------------------------------





#endif
