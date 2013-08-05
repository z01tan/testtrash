//---------------------------------------------------------------------------

#ifndef MyXH
#define MyXH


#include<iterator>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <exception>

//---------------------------------------------------------------------------
struct TDataTime // структура даты и времени
{
   int year;
   int month;
   int day;
   int hour;
   int minit;
   int sec;
};
           //Задержка в секундах

//---------Строки---------
std::string IntToStr(int pp); //_i-int  f_float _d double -> String
bool TrueFalseVstr(std::string st); //Правда или ложь в строке?
template <typename T> T StrToInt(const std::string& st); //String-> <int> <float> <double>

bool StrVstr(std::string St, std::string St1, int N); //
int IntVstr (std::string st, int Int); //В строковой переменной st ищет целое число C под номером Int
std::string StrBezStr(std::string st, std::string st1); //возвращает строку st без подстроки st1
std::string StrBetweenStr12(std::string st, std::string st1, std::string st2, int N);//возвращает подстроку из строки st между строками st1 и st1 по порядку N
int NStrVstr(std::string st, std::string st1); //возвращает количество вхождений подстроки st1 в строке st

std::string TimeToString(int Chas,bool C,int minit,bool M,int sec,bool S,int Lang);
std::string DateToString(int year,int month,int day, int Lang); //Дата в пропись
std::string dtx(int m1 , int m2, int m3, int flag, int Lang);
std::string CisloToString(double Cislo, int Lang); // Число прописью Lang:0-Russian, 1-English

//-------Файлы---------
std::string GetCurDir(void);                          //  Возвращение текущего каталога
std::string ReadFile(std::string path);               // Загрузка из файла в char
void WriteFile(std::string Text, std::string path);   // Запись в файл

//---------Время----------
TDataTime GetDateTime(void);       //Возвращение даты, времени
void SetDateTime(TDataTime DT);    //Установка даты, времени
void Delay(int Second);

#endif
