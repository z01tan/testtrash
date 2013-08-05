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
struct TDataTime // ��������� ���� � �������
{
   int year;
   int month;
   int day;
   int hour;
   int minit;
   int sec;
};
           //�������� � ��������

//---------������---------
std::string IntToStr(int pp); //_i-int  f_float _d double -> String
bool TrueFalseVstr(std::string st); //������ ��� ���� � ������?
template <typename T> T StrToInt(const std::string& st); //String-> <int> <float> <double>

bool StrVstr(std::string St, std::string St1, int N); //
int IntVstr (std::string st, int Int); //� ��������� ���������� st ���� ����� ����� C ��� ������� Int
std::string StrBezStr(std::string st, std::string st1); //���������� ������ st ��� ��������� st1
std::string StrBetweenStr12(std::string st, std::string st1, std::string st2, int N);//���������� ��������� �� ������ st ����� �������� st1 � st1 �� ������� N
int NStrVstr(std::string st, std::string st1); //���������� ���������� ��������� ��������� st1 � ������ st

std::string TimeToString(int Chas,bool C,int minit,bool M,int sec,bool S,int Lang);
std::string DateToString(int year,int month,int day, int Lang); //���� � �������
std::string dtx(int m1 , int m2, int m3, int flag, int Lang);
std::string CisloToString(double Cislo, int Lang); // ����� �������� Lang:0-Russian, 1-English

//-------�����---------
std::string GetCurDir(void);                          //  ����������� �������� ��������
std::string ReadFile(std::string path);               // �������� �� ����� � char
void WriteFile(std::string Text, std::string path);   // ������ � ����

//---------�����----------
TDataTime GetDateTime(void);       //����������� ����, �������
void SetDateTime(TDataTime DT);    //��������� ����, �������
void Delay(int Second);

#endif
