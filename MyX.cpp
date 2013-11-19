//---------------------------------------------------------------------------
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstdlib>
#include <termios.h>
#include <math.h>
#include <iterator>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string>
#include <time.h>
#include <exception>
#include <unistd.h>

#include "MyX.h"
#include "Ruler.h"
using namespace std;
//------------------------------------------------------------------------------
//---------------------------------Дата, время----------------------------------
//------------------------------------------------------------------------------


//-------------------------------Возвращение даты, времени----------------------
TDataTime GetDateTime(void)
{
   TDataTime DT;
   time_t timer;
   struct tm *tblock, T;
   timer = time(NULL);
   tblock = localtime(&timer);
   T = *tblock;
	DT.year = 1900 + T.tm_year;
	DT.month = T.tm_mon + 1;
	DT.day = T.tm_mday;
	DT.hour = T.tm_hour;
	DT.minit = T.tm_min;
	DT.sec = T.tm_sec;
  return DT;
}
// ------------------------------Установка даты, времени------------------------
void SetDateTime(TDataTime DT)
{

}

// -----------------------------Задержка----------------------------------------
void Delay(int Second)
{
	TDataTime DT;
	DT=GetDateTime();
	int S = DT.hour*3600+DT.minit*60+DT.sec;
	do
	 DT=GetDateTime();
	while (Second > DT.hour*3600 + DT.minit*60 + DT.sec - S);
}


//------------------------------------------------------------------------------
//---------------------------------Строки---------------------------------------
//------------------------------------------------------------------------------
bool TrueFalseVstr(std::string st)
{
   bool TF;
   if ((st=="true") || (st=="True"))
   {
       TF=true;
   } else {
       TF=false;
   }
   return TF;
}
//-------------------------------------------------String-> <int> <float> <double>---------------------------------------------
template <typename T>
T StrToInt(const std::string& st)
{
    std::istringstream sts(st);
    T pp; sts >> pp;
    return pp;
}
//--------------------------------------Замена в str строк st1 на st2--------------------------------
std::string FiltrStr(std::string str, std::string st1, std::string st2)
{
  std::string w=str;
  int index=-1;
  int Nstr=1;
  bool Poh=false;
  if (StrVstr(st2,st1,1)) { Poh=true; }

  do
  {
     index=-1;
     for (int j=1; j<=Nstr; j++)
     {
        index = w.find(st1, index+1);
     }
     if (Poh) { Nstr++; }

     if (index >=0)
     {
        std::string w1(w,0,index);
        std::string w2(w, index+ st1.length(), w.length()-st1.length()-index);
        w=w1+st2+w2;
     }

  } while (index>=0);

return w;
}

//-------------------------------------------------_i-int  f_float _d double -> String------------------------------------------
std::string IntToStr(int pp)
{
  std::ostringstream sts;
  sts<<pp;
  return sts.str();
}

//-------------------------------------------------------------------------------------------------------------------------------
bool StrVstr(std::string st, std::string st1, int N)
{
bool TF=false;
int Nn=NStrVstr(st,st1);
if (Nn>=N) { TF=true; }

return TF;
}
//------------------------------------В строковой переменной st ищет целое число C под номером Int------------------------------
int IntVstr (std::string st, int Int)
{
  int Pop=0;
  int j;
  bool cif=false;
  int U1;
  int du;



  for (double i = 0; i <= st.length(); i++)
  {
	if ((st[i]>='0')&&(st[i]<='9'))
	{
	  if (cif)
	  {
		du++;
	  } else {
		cif=true;
		Pop++;
		U1=i; du=1;
	  }
	} else {
	  if (cif)
	  {
		cif=false;
		if (Pop==Int)
		{
         std::string w(st,U1,du); j=StrToInt<int>(w);
		  return j;
		}
	  } else {
		cif=false;
	  }
	}
  }
  return 0;
}

//----------------------------------Строка без подстроки---------------------------
std::string StrBezStr(std::string st, std::string st1) // возвращает строку st без подстроки st1
{
  int index = st.find(st1);

  if (index >=0)
  {
  std::string w1(st,0,index);
  std::string w2(st, index+ st1.length(), st.length()-st1.length()-index);
  std::string w = w1+w2;
  return w;
  }
return 0;
}
//----------------------Количество st1 в st-------------------------------------

int NStrVstr(std::string st, std::string st1)
{
  int N=0;
  std::size_t index=st.find(st1);
  if ((index >=0)&&(index<st.length()))
  {
     N++;
     do
     {
      index = st.find(st1, index+1);
        if ((index >=0)&&(index<st.length()))
        {
          N++;
        }
     }
     while ((index >=0)&&(index<st.length()));
  }
  return N;
}

//---------------------------------Строка между строк (String)---------------------------
std::string StrBetweenStr12(std::string st, std::string st1, std::string st2, int N)
{

  std::size_t index1=0;
  std::size_t index2=0;

  index1=st.find(st1);
  //index2=st.find(st2);
  if (N>1)
  {

    for (int i=2; i<=N; i++)
    {
     index1=st.find(st1,index1+2);
     //index2=st.find(st2,index2+1);
    }
  }
  index2=st.find(st2,index1+st1.length());

  if ( ((index1>=0)&&(index1<=st.length()))  &&  ((index2>=0)&&(index2<=st.length())) )
  {
    std::string sts(st,index1+st1.length(),index2-index1-st1.length());
    return sts;
  } else {
  return "";
  }

}

//------------------------------------------------------------------------------
//---------------------------------Файлы----------------------------------------
//------------------------------------------------------------------------------

//---------------------------------Чтение из файла------------------------------
std::string ReadFile(std::string path)
{
   std::ifstream w (path.c_str());
   std::string a;
   w.unsetf(std::ios_base::skipws);
   std::copy(std::istream_iterator <char> (w), std::istream_iterator <char> (), std::back_inserter(a));
   return a;
 }

//---------------------------------Запись в файл--------------------------------
void WriteFile(std::string Text, std::string path)
{
  ofstream F;
  F.open(path.c_str(), ios::out); //Log-файл
  F<<Text<<'\n';
  F.close();
}
//-----------------------------Возвращение текущего каталога--------------------
std::string GetCurDir(void)
{
   char CurDir[255];
   int length=0;

   if (getcwd(CurDir, length))
   {
     return CurDir;
   }
   return 0;
}

//------------------------------------------------------------------------------------------------
std::string dtx(int m1 , int m2, int m3, int flag, int Lang)
{
    std::string str="";

    switch(m1)
    {
        case 1:
			if (Lang==0) str+="сто ";
            if (Lang==1) str+="one hundred ";
            break;
		case 2:
            if (Lang==0) str+="двести ";
            if (Lang==1) str+="two hundred ";
            break;
        case 3:
            if (Lang==0) str+="триста ";
            if (Lang==1) str+="three hundred ";
			break;
        case 4:
            if (Lang==0) str+="четыреста ";
            if (Lang==1) str+="four hundred ";
            break;
        case 5:
            if (Lang==0) str+="пятьсот ";
            if (Lang==1) str+="five hundred ";
            break;
        case 6:
            if (Lang==0) str+="шестьсот ";
            if (Lang==1) str+="six hundred ";
            break;
        case 7:
            if (Lang==0) str+="семьсот ";
            if (Lang==1) str+="seven hundred ";
            break;
        case 8:
            if (Lang==0) str+="восемьсот ";
            if (Lang==1) str+="eith hundred ";
            break;
        case 9:
            if (Lang==0) str+="девятьсот ";
            if (Lang==1) str+="nine hundred ";
            break;
    }
    if( m2 == 0 || m2 > 1 )
    {
        if( m2 > 1 )
        {
            switch( m2 )
            {
                case 2:
					if (Lang==0) str+="двадцать ";
					if (Lang==1) str+="twenty ";
                    break;
                case 3:
                    if (Lang==0) str+="тридцать ";
                    if (Lang==1) str+="thirty ";
                    break;
                case 4:
                    if (Lang==0) str+="сорок ";
                    if (Lang==1) str+="fourty ";
                    break;
                case 5:
                    if (Lang==0) str+="пятьдесят ";
                    if (Lang==1) str+="fifty ";
                    break;
                case 6:
                    if (Lang==0) str+="шестьдесят ";
                    if (Lang==1) str+="sixty ";
                    break;
                case 7:
                    if (Lang==0) str+="семьдесят ";
                    if (Lang==1) str+="seventy ";
                    break;
                case 8:
                    if (Lang==0) str+="восемьдесят ";
                    if (Lang==1) str+="eithy ";
                    break;
                case 9:
                    if (Lang==0) str+="девяносто ";
                    if (Lang==1) str+="ninety ";
                    break;
            }
        }
        switch( m3 )
        {
            case 1:
                if (Lang==0)
                {
                  if( flag == 2 ) str+="одна ";
                  else str+="один ";
                }
                if (Lang==1) str+="one ";
                break;
            case 2:
                if (Lang==0)
                {
                  if( flag == 2 ) str+="две ";
                  else str+="два ";
                }
                if (Lang==1) str+="two ";

                break;
            case 3:
                if (Lang==0)
                {
                  str+="три ";
                }
                if (Lang==1) str+="three ";

                break;
            case 4:
                if (Lang==0)
                {
                  str+="четыре ";
                }
                if (Lang==1) str+="four ";

                break;
            case 5:
                if (Lang==0)
                {
                  str+="пять ";
                }
                if (Lang==1) str+="five ";

                break;
            case 6:
                if (Lang==0)
                {
                  str+="шесть ";
                }
                if (Lang==1) str+="six ";

                break;
            case 7:
                if (Lang==0)
                {
                  str+="семь ";
                }
                if (Lang==1) str+="seven ";

                break;
            case 8:
                if (Lang==0)
                {
                  str+="восемь ";
                }
                if (Lang==1) str+="eith ";

                break;
            case 9:
                if (Lang==0)
                {
                  str+="девять ";
                }
                if (Lang==1) str+="nine ";

                break;
        }
    }
    else if( m2 == 1 )
    {
        switch( m3 )
        {
            case 0:
                if (Lang==0) str+="десять ";
                if (Lang==1) str+="ten ";
                break;
            case 1:
                if (Lang==0) str+="одиннадцать ";
                if (Lang==1) str+="illeven ";
                break;
            case 2:
                if (Lang==0) str+="двенадцать ";
                if (Lang==1) str+="twelve ";
                break;
            case 3:
                if (Lang==0) str+="тринадцать ";
                if (Lang==1) str+="thirteen ";
                break;
            case 4:
                if (Lang==0) str+="четырнадцать ";
                if (Lang==1) str+="thorteen ";
                break;
            case 5:
                if (Lang==0) str+="пятнадцать ";
                if (Lang==1) str+="thifteen ";
                break;
            case 6:
                if (Lang==0) str+="шестнадцать ";
                if (Lang==1) str+="sixteen ";
                break;
            case 7:
                if (Lang==0) str+="семнадцать ";
                if (Lang==1) str+="seventeen ";
                break;
            case 8:
                if (Lang==0) str+="восемнадцать ";
                if (Lang==1) str+="eiteen ";
                break;
            case 9:
                if (Lang==0) str+="девятнадцать ";
                if (Lang==1) str+="nineteen ";
                break;
        }
    }
    if( flag == 1 )
    {
        if( m3 == 0 && m2 == 0 && m1 == 0 ) str+="";
        else if( m2 == 1 )
        {
            if (Lang==0) str+="миллионов ";
            if (Lang==1) str+="millions ";
        }
        else
        {
                if (Lang==0)
                {
                  if( m3 == 1 ) str+="миллион ";
                  else if( m3 >= 2 && m3 <= 4 ) str+="миллиона ";
                  else str+="миллионов ";
                }
                if (Lang==1) str+="millions ";
        }
    }
    else if( flag == 2 )
    {
        if( m3 == 0 && m2 == 0 && m1 == 0 ) str+="";
        else if( m2 == 1 )
        {
            if (Lang==0) str+="тысяч ";
            if (Lang==1) str+="thousand ";
        }
        else
        {
                if (Lang==0)
                {
                  if( m3 == 1 ) str+="тысяча ";
                  else
                  { if( m3 >= 2 && m3 <= 4 )
                      {  if (Lang==0) str+="тысячи ";
                          else str+="тысяч ";
                      }
                  }
                }
                if (Lang==1) str+="thousand ";

        }
    }
    else
    {
		if( m3 == 0 && m2 == 0 && m1 == 0 )
		{
		  if (Lang==0) str+="ноль ";
		  if (Lang==1) str+="zero ";
		} else if( m2 == 1 )
		{
			str+=" ";
		} else {
				if( m3 == 1 ) str+=" ";
				else if( m3 >= 2 && m3 <= 4 ) str+=" ";
				else str+=" ";
        }
    }

 return str;
}

//------------------------------------------------------------------------------
std::string CisloToString(double Cislo, int Lang)
{
	int m1, m2, m3;
	int sum=Cislo;
	//unsigned char c;
	std::string str="";
	m1=sum/100000000; //100 миллионов
	m2=sum/10000000-(sum/100000000)*10;
	m3=sum/1000000-(sum/10000000)*10;
	str=str+dtx(m1, m2, m3, 1,Lang);

	m1=sum/100000-(sum/1000000)*10;
	m2=sum/10000-(sum/100000)*10;
	m3=sum/1000-(sum/10000)*10;
	str=str+dtx(m1, m2, m3, 2,Lang);

	m1=sum/100-(sum/1000)*10;
	m2=sum/10-(sum/100)*10;
	m3=sum/1-(sum/10)*10;
	str=str+dtx(m1, m2, m3, 3,Lang);

	return str;
}
//----------------------------------------Date To String-------------------------------------------
std::string DateToString(int year,int month,int day, int Lang)
{
std::string Date="";
std::string str1="";
std::string str2="";

	switch (month)
	{
	case 1:
	  {
        if (Lang==0) str1="января";
        if (Lang==1) str1="january";
      } break;
	case 2:
	  {
        if (Lang==0) str1="февраля";
        if (Lang==1) str1="february";
      } break;
	case 3:
	  {
        if (Lang==0) str1="марта";
        if (Lang==1) str1="march";
      } break;
 	case 4:
	  {
        if (Lang==0) str1="апреля";
        if (Lang==1) str1="april";
      } break;
 	case 5:
	  {
        if (Lang==0) str1="мая";
        if (Lang==1) str1="may";
      } break;
 	case 6:
	  {
        if (Lang==0) str1="июня";
        if (Lang==1) str1="june";
      } break;
 	case 7:
	  {
        if (Lang==0) str1="июля";
        if (Lang==1) str1="julay";
      } break;
 	case 8:
	  {
        if (Lang==0) str1="августа";
        if (Lang==1) str1="august";
      } break;
 	case 9:
	  {
        if (Lang==0) str1="сентября";
        if (Lang==1) str1="september";
      } break;
 	case 10:
	  {
        if (Lang==0) str1="октября";
        if (Lang==1) str1="octouber";
      } break;
 	case 11:
	  {
        if (Lang==0) str1="ноября";
        if (Lang==1) str1="november";
      } break;
 	case 12:
	  {
        if (Lang==0) str1="декабря";
        if (Lang==1) str1="december";
      } break;
    }
     Date=IntToStr(day)+" "+str1+" "+IntToStr(year);

     if (Lang==0) Date+=" года ";
     if (Lang==1) Date+=" year ";

   return Date;

}
//------------------------------------------------------------------------------------------------
std::string TimeToString(int Chas,bool C,int minit,bool M,int sec,bool S,int Lang)
{
  std::string str="";
  std::string strx="";
  std::string stry="";
  std::string strz="";
  std::string stra="";

if (C)
{
  strx=CisloToString(Chas, Lang);

  if ((Chas==1)||(Chas==21))
  {
    if (Lang==0) { stry="час "; } //Mst[14]
    if (Lang==1) { stry="hour "; }
  }
  if (((Chas>=2)&&(Chas<=4)) || ((Chas>=22)&&(Chas<=24)))
  {
    if (Lang==0) { stry="часа "; } //Mst[15]
    if (Lang==1) { stry="hours "; }
  }
  if ((Chas>=5)&&(Chas<=20))
  {
    if (Lang==0) { stry="часов "; } //Mst[16]
    if (Lang==1) { stry="hours "; }
  }

}
if (M)
{
   strz=CisloToString(minit, Lang);

   if ((minit=1)||(minit=21)||(minit=31)||(minit=41)||(minit=51))
   {
    if (Lang==0) { stra="минута "; } //Mst[17]
    if (Lang==1) { stra="minits "; }
   }
   if ( ((minit>=2)&&(minit<=4))||((minit>=22)&&(minit<=24))||((minit>=32)&&(minit<=34))||
        ((minit>=42)&&(minit<=44))||((minit>=52)&&(minit<=54)) )
   {
    if (Lang==0) { stra="минуты "; } //Mst[18]
    if (Lang==1) { stra="minits "; }
   }
   if ( (minit=0)||((minit>=5)&&(minit<=20))||((minit>=25)&&(minit<=30))||((minit>=35)&&(minit<=40))||
        ((minit>=45)&&(minit<=50))||((minit>=55)&&(minit<=60)) )
   {
    if (Lang==0) { stra="минут "; } //Mst[19]
    if (Lang==1) { stra="minits "; }
   }
}
if (S)
{

}

str=strx+stry+strz+stra;

return str;

}

