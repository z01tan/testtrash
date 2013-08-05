// ---------------------------------------------------------------------------

#ifndef RulerH
#define RulerH

#include "MyX.h"
#include "HeaderMain.h"

// ----------------------------статические данные--------------------------------
// ------------------------------------------------------------------------------
extern int NumBullAll; // суммарное  количество принятых бюллетеней
extern int pr_btn;
extern int pr_opt;
extern int pr_rr;
/* 10.06.13 !!!!!! */ extern  char OutS1[128];
/* 10.06.13 !!!!!! */ extern  char OutS2[128];
/* 10.06.13 !!!!!! */ extern  char OutS[128];  // информационные сообщения


//------------------------------------------------------------------------------
//-----------------------------------функции------------------------------------
//------------------------------------------------------------------------------
std::string dtx1(int m1 , int m2, int m3, int flag, int Lang);
std::string CisloToStringOrVoice(double Cislo, bool String);
std::string DateToStringOrVoice(int year,int month,int day, bool String);
std::string TimeToStringOrVoice(int Chas,bool C,int minit,bool M,int sec,bool S,bool String);

//void LoadInd(void); // Загрузка выводимых на табло значений
int WaitOptrons(int Vektor);// Ожидание изменения состояния оптронов
int ImmOptrons(void);// Опрос изменения состояния оптронов на лету
int WaitYesNo(void); // Ожидание нажатия клавиш Да Нет
int ImmYesNo(void); // Моментальный опрос нажатия клавиш Да Нет
bool DriveGO(int Vektor, bool Solenoid); // Управление ШД
int WaitRR(void); //Ожидание результатов распознавания бюллетеня

bool SaveIG(int XVote); //Запись результатов на Flash
bool LoadSetup(void); // Загрузка первоначальных данных
bool AvarFiles(void);  // Загрузка из файла аварийного выключения
//bool CheckPrinter(void); //Определение - есть ли принтер?
bool SyncData(TKoib K, bool KB,TVotes V, bool VC); //Обмен данными по сети
int KoibMain(void); // Определяет, главный ли КОИБ
void Ind(std::string st1, std::string st2, int t); // Вывод данных на индикатор
void Zagruzka(void);      // Загрузка начальных параметров
int Expression(std::string st); //
void Delay(int Second);   // Задержка
int Samotestirovanie(void);    // Самотестирование оборудования
bool Catch(void); // Проверка соединения КОИБов

//void TekDateTime(void);

void IDsave(void); //Запись ИД в аварийную директорию
void IDload(void); //Загрузка ИД из аварийной директории
bool Protokol(std::string Prot, int Xbl, int Xv, int j); //Считывание данных протокола
//bool Parser(std::string Path); // Парсер XML-файла
int MakeFile(char* NameF);
bool Print(int Type); // Печать на принтере

void SayN(std::string st1); //Говорить сложные события
void Say(int Type); //Говорить простые события

//void Say5(std::string str1, std::string str2, std::string str3,std::string str4,std::string str5); //Говорить 5 простых событий
bool Speak(int Type); // Генератор речи для сложных событий
void KoibZero(void); // Обнуление динамических переменных КОИБа

bool NextXVopr(void); //Определяет следующую корректируемую строку протокола, если нет - следующие выборы
int Sostoanie(void); // Обработка вектора состояния КОИБ
int Golosovanie(void); // Обработчик вектора состояния при голосовании
bool Timer(bool GoSG, int day, int chas, int min); // От таймера
void DeShifr(void);

void SetMainTimer(void); //Установка главного таймера MainTimer
void Start(bool S);
bool RUN(void); // Точка входа программы


//------------------------------------------------------------------------------
//----------------------------------переменные----------------------------------
//------------------------------------------------------------------------------


//----end описание непосредственно голосования




//------------------------------------Константы---------------------------------
 const unsigned int MainTimer=1;
 const unsigned int SocketTimer=2;
 const std::string newstr("\n");

 extern bool Exit;
 extern int Xblank; // Указатель
 extern int Xcand; // Указатель
 extern int Xbl; // Указатель
 extern int Xv; // Указатель
 extern int XVote; // Указатель



#endif
