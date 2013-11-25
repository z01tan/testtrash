// �����������  ���������
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <math.h>
#include <time.h>


#include <exception>

//---------------------------------------------------------------------------
#include "Ruler.h"
#include "MyX.h"
#include "cr_BMP.h"
#include "HeaderMain.h"
#include "CreateImage.h"
#include "InitAltera.h"
#include "HardProc.h"
#include "printer.h"
#include "usb.h"
#include "AlteraSocket.h"
#include "MakeTeX-L.h"

using namespace std;

//  ���  ���������� ������ �  ������� ������������
FILE    *logfR;                  // InitSocket write LOF File "TCPlog.txt" !!!!
int      PrStopScan=0;
  //  char FBMP[]        = "/www/pages/Scan.bmp";
//--------------------------------------����������------------------------------
std::string Nst[256]; //�������� ���������
std::string Nvc[256]; //���� �������� ������
std::string Mst[256]; //��������� � ������� � ��� ������ �������
std::string Mvc[256]; //���� � ����������� ������� � ��� ������ �������
std::string Golos;

// ��������������  ����
int PrConnectOK;// ������� ���������  ����� ��  ������  ��������
int Pr_Flash;
int pidMain=0;
    pid_t pid;

int PrIdError; // �������  ������ ����������  �������� ������
int PrKC; // ������� ����������  ������������  ����������� 0- ��  ��������� 1 - ���������

int NumBullAll;
int PrVoteTimeStart;
int PrVoteTimeEnd;
int PrApprove;
int PrVoteDay;
  int PrErrScan=0;
  int PrRezScan=0;
  int PrPgEnd=0;
   int Version;

   ofstream Fi;
   ofstream Fv;

  int iBlocks1[10];
  int iBlocks2[10];

   int pr_btn=-1;
   int pr_opt=-1;
   int pr_rr=-1;

   TInd Out; //������ ����� ��� ������
   TVoice OutV; //������ ��������� ���������
   bool SayYes; //�������� �� ���������

   char OutS1[128];
   char OutS2[128];
   char OutS[128];   // �������������� ���������
   bool Exit;
   std::string SourceDir; // -���� � ��������� ������
   std::string FileName;

   int NKoib; // ����������  ���� �  ������
   TKoib Koib[3]; //������, ����������� ������ ������

   TVotes Votes; // ���������� ��� �������� ������� �������� ������

   int BhourU[MaxModes]={8,22};
   int BminU[MaxModes]={0,0};

   TSnat Snat[NumBlankMax+1]; //������ ������ ������� �������
   TDataTime TekDT; // ����, ����� �������
   std::string DateS;  // ���� �������
   std::string TimeS;  // ����� �������

   int Xblank; // ���������
   int Xcand; // ���������
   int Xbl; // ���������
   int Xv; // ���������
   int XVote; // ���������
   int XVopr; //  ����� ���������������  ������� �� ������� XVote

//============================================================== ������� ==============================================================
// ��������������  ����
int Connect(void)
{  // ������� ��������  ������������  ����� ��  ������  ��������
    int pr=-1;
    pr=InitSockets();
    if(pr<0)pr=0; else pr=1;
    return pr;
}

int FindSourceFile(void)
 { // ������������ ���� �  �����
   int pr;
   pr=0;  // ��  ������ �������� ( ������ ��� SD �����)
   //  ��������  �������  ��������
  FileName=MountNRead();
  if(FileName=="-1")pr=-1;
  if(FileName=="-2")pr=-2;
  if(FileName=="-3")pr=-3;
  if(pr<0) { pr=0; FileName="./SourceDir/SourceData.XML";}
  return pr;
 } //  void FindSourceFile(void)


//  �������  ������������  ���������  (�������)
void SayVotes(int Type)
{  //  ��������� ��������� �  ��������  �����������
   if (Type>0)
   {  // �������� ��������� �� ����� ����������� ��������� ��� �����������
       SayN(OutV.VotesPath[Type]);
   }
}
//-----------------------����� St1 � St2 �� ��������� �� ����� t----------------
void Ind(std::string st1, std::string st2)
{  // ������  ���������  ��  �����
    // ������ ���������  �  ������ ������
	if (st1!="")  {      setFirstline(&st1[0],0); 	};
	// ������ ���������  ��  ������ ������
	if (st2!="")  {      setSecline(&st2[0],0); 	    };
}

void VoteScan()
{  //********������������ � ������ ��������� ����, ���� ****************
   Scptr_r = 0;
   Scptr_s = 0;
   MotorFuS = false;
   int pr1=0; int nstr11=0;
   std::string st1;
   int rez=0;
   nstr =0;
   MasScan.MasRecOut[0]=0;
    max_gelesen =0;

    cout << "MAXLINES =   " << MAXLINES <<"      Input max_gelesen = " << max_gelesen << endl;
//************************************************************************
     clock_t  t1,t2;
     t1= clock();
//************************************************************************
        PrErrScan=0;        PrRezScan=0;  PrStopScan=0;
        Page_scan();
        usleep(200000);
        //  ��������  �������  ����� ����� 0.2 ���  �����  ������� ������������ ( ���� ��� ����� ��  ������)
        get_Optrons_Imm();
        if( ((Optron[1] > NO_PAPERO[1]) ||(Optron[2] > NO_PAPERO[2]) )&&(Optron[3] > NO_PAPERO[3]) )
        {   PrStopScan=1;
             nstr=-1; //  ��� ������  ��  �����
             Page_stop(); usleep(20000); Page_back();
         }else PrStopScan=2;
        cout << "PrStopScan =   " << PrStopScan << endl;
        cout << " max_gelesen after Page scan = " << max_gelesen << endl;
        if(PrStopScan==2)
        {  //  ����  ���������  ����� ��  ������������
            while(nstr >= 0)
            { // ����  ��  �����������  �������
                nstr11=nstr;
                nstr = getNextScanLine();
                //if(nstr < 0) PrPgEnd=-1; else PrPgEnd=0;
                if(nstr <=0 )
                { cout << "nstr < 0 getNextline  last nstr = " << nstr11 << endl;
                   cout << "nstr < 0 maxgelesen  = " << max_gelesen << endl;
                }
                 // ������� �����������  ������������
                if(nstr>=0)
                {  // �������� ��������� ������ ���  �������������
                    pr1=ScanBulString();     rez=MasScan.MasRecOut[0];
               }
               if((pr1!=0)||(rez==111))
              {   //cout <<"  Line  number = " << nstr << endl;
                  if(nstr >=0) Page_stop();
                  PrStopScan=2;
                    cout << "PrStopScan =   " << PrStopScan << endl;
                  // ������ �  ������ ������ ��  �����
                 cout <<"pr1 = "<<pr1<<"      rez=  "<<rez<<"      nstr = "<<nstr<<endl;
                  nstr=-1;
                  cout <<"Out  max_gelesen = " << max_gelesen << endl;
                }
                t2 = clock();
                t2=t2 - t1;
            }
                // ���������� ��������� ����������� ������������ �  ���������� ����������
                PrErrScan=pr1;        PrRezScan=rez;
                //**********************************************
                t2 = clock();
                t2=t2 - t1;
                printf (" Scanning and recognition Time %f \n",float(t2)/CLOCKS_PER_SEC);
                printf(" PrErrScan   =    %d\n", PrErrScan);
                printf(" PrRezScan =    %d\n",PrRezScan);

        }
//**********************************************
}

//--------------------------------------------------������� ����������-----------------------------------------------------------
//------------------------------------------------�������� ���������� ���������--------------------------------------------------
void LoadInd(void)
{
   std::string stx;
   ifstream F;
   ifstream F1;
   ifstream F11;
   ifstream F2;
   ifstream F2A;
   ifstream F2B;
   ifstream F3;
   ifstream F4;
   ifstream F5;
   ifstream F6;
   ifstream F7;
   ifstream Fnum;
   ifstream F8;
   ifstream F9;
   ifstream F10;
   ifstream F12;
   ifstream F13;
   ifstream F14;
   ifstream F15;
   ifstream F16;
   int sum=0;
   char st[256];
    switch (Lang)
	{
	case 0: // ������� ����
		{
         stx="./SetUpFiles/Ind/MainRussian"+IntToStr(Version)+".dat";
		 F.open(stx.c_str(), ios::in);
         F1.open("./SetUpFiles/Ind/MenuRussian.dat", ios::in);
         F11.open("./SetUpFiles/Ind/MenuRussian1.dat", ios::in);

         F2.open("./SetUpFiles/Ind/VotesRussian.dat", ios::in);
         F2A.open("./SetUpFiles/Ind/TestRussian.dat", ios::in);
         F2B.open("./SetUpFiles/Ind/MonthsRussian.dat", ios::in);

         F3.open("./SetUpFiles/Pr/Pr_ID_Russian.dat", ios::in);
         F4.open("./SetUpFiles/Pr/Pr_Test_Russian.dat", ios::in);
         F5.open("./SetUpFiles/Pr/Pr_Rezult_Russian.dat", ios::in);
         F6.open("./SetUpFiles/Pr/Pr_Ch_Russian.dat", ios::in);
         F7.open("./SetUpFiles/Pr/Pr_IP_Russian.dat", ios::in);

         Fnum.open("./SetUpFiles/Num/Num_Russian.dat", ios::in);

         stx="./SetUpFiles/Voice/VoiceRussian"+IntToStr(Version)+".dat";
       F8.open(stx.c_str(), ios::in);


         F9.open("./SetUpFiles/Voice/VoiceRussian_Snat.dat", ios::in);
         F10.open("./SetUpFiles/Voice/VoiceRussian_ID.dat", ios::in);
         F12.open("./SetUpFiles/Voice/VoiceRussian_Num.dat", ios::in);
         F13.open("./SetUpFiles/Voice/VoiceRussian_Months.dat", ios::in);
         F14.open("./SetUpFiles/Voice/VoiceRussian_Votes.dat", ios::in);
         F15.open("./SetUpFiles/Voice/VoiceRussian_Test.dat", ios::in);
         F16.open("./SetUpFiles/Voice/VoiceRussian_Rezult.dat", ios::in);

		} break;
	case 1: // ���������� ����
		{
		 stx="./SetUpFiles/Ind/MainEnglish"+IntToStr(Version)+".dat";
		 F.open(stx.c_str(), ios::in);
         F1.open("./SetUpFiles/Ind/MenuEnglish.dat", ios::in);
         F11.open("./SetUpFiles/Ind/MenuEnglish1.dat", ios::in);
         F2.open("./SetUpFiles/Ind/VotesEnglish.dat", ios::in);
         F2A.open("./SetUpFiles/Ind/TestEnglish.dat", ios::in);
         F2B.open("./SetUpFiles/Ind/MonthsEnglish.dat", ios::in);

         F3.open("./SetUpFiles/Pr/Pr_ID_English.dat", ios::in);
         F4.open("./SetUpFiles/Pr/Pr_Test_English.dat", ios::in);
         F5.open("./SetUpFiles/Pr/Pr_Rezult_English.dat", ios::in);
         F6.open("./SetUpFiles/Pr/Pr_Ch_English.dat", ios::in);
         F7.open("./SetUpFiles/Pr/Pr_IP_English.dat", ios::in);

         Fnum.open("./SetUpFiles/Num/Num_English.dat", ios::in);

       // stx="./SetUpFiles/Voice/VoiceEnglish"+IntToStr(Version)+".dat";
      //   F8.open(stx.c_str(), ios::in);

        stx="./SetUpFiles/Voice/VoiceRussian"+IntToStr(Version)+".dat";
        F8.open(stx.c_str(), ios::in);

         F9.open("./SetUpFiles/Voice/VoiceEnglish_Snat.dat", ios::in);
         F10.open("./SetUpFiles/Voice/VoiceEnglish_ID.dat", ios::in);
         F12.open("./SetUpFiles/Voice/VoiceEnglish_Num.dat", ios::in);
         F13.open("./SetUpFiles/Voice/VoiceEnglish_Months.dat", ios::in);
         F14.open("./SetUpFiles/Voice/VoiceEnglish_Votes.dat", ios::in);
         F15.open("./SetUpFiles/Voice/VoiceEnglish_Test.dat", ios::in);
         F16.open("./SetUpFiles/Voice/VoiceEnglish_Rezult.dat", ios::in);

		} break;
    }

    // ��������� �������� � �������� ������
    sum=1;
    if(F!=NULL)// �������� �������� �����
    {
        while (! F.eof())
        { F.getline(st,128); Out.Main[sum]=st;  sum++; }
          F.close();
    }
    // ��������� �������� � ������ ���� ��� ������  ������
    sum=1;
    if(F1!=NULL)// �������� �������� �����
    {
        while (!F1.eof())
        { F1.getline(st,128); Out.Menu[sum]=st;  sum++; }
          F1.close();
    }
    // ��������� �������� � ������ ���� ��� ������  ������
    sum=1;
    if(F11!=NULL)// �������� �������� �����
    {
        while (!F11.eof())
        { F11.getline(st,128); Out.Menu1[sum]=st;  sum++; }
          F11.close();
    }
    // ��������� �������� � ������ �����������
    sum=1;
    if(F2!=NULL)// �������� �������� �����
    {
        while (! F2.eof())
        { F2.getline(st,128); Out.Votes[sum]=st;  sum++; }
          F2.close();
    }
    // ��������� �������� � ������ ����������������
    sum=1;
    if(F2A!=NULL)// �������� �������� �����
    {
        while (! F2A.eof())
        { F2A.getline(st,128); Out.Test[sum]=st;  sum++; }
          F2A.close();
    }
    // ��������� �������� �������
    sum=1;
    if(F2B!=NULL)// �������� �������� �����
    {
        while (! F2B.eof())
        { F2B.getline(st,128); Out.Months[sum]=st; Mst[sum]=st; sum++; }
          F2B.close();
    }
    // ��������� �������� ��� ������������ ��������� ��
    sum=1;
    if(F3!=NULL)// �������� �������� �����
    {
        while (! F3.eof())
        { F3.getline(st,128); Out.PrID[sum]=st;  sum++; }
          F3.close();
    }
    // ��������� �������� ��� ������������ ��������� ������������ ����
    sum=1;
    if(F4!=NULL)// �������� �������� �����
    {
        while (! F4.eof())
        { F4.getline(st,128); Out.PrTest[sum]=st;  sum++; }
          F4.close();
    }
    // ��������� �������� ��� ������������ ��������� ����������� �����������
    sum=1;
    if(F5!=NULL)// �������� �������� �����
    {
        while (! F5.eof())
        { F5.getline(st,128); Out.PrRezult[sum]=st;  sum++; }
          F5.close();
    }
    // ��������� �������� ��� ������������ ��������� ����������� ��
    sum=1;
    if(F6!=NULL)// �������� �������� �����
    {
        while (! F6.eof())
        { F6.getline(st,128); Out.PrCh[sum]=st;  sum++; }
          F6.close();
    }
    // ��������� �������� ��� ������������ ��������� ��������� �����������
    sum=1;
    if(F7!=NULL)// �������� �������� �����
    {
        while (! F7.eof())
        { F7.getline(st,128); Out.PrIP[sum]=st;  sum++; }
          F7.close();
    }
    // ��������� ������
    sum=1;
    if(Fnum!=NULL)// �������� �������� �����
    {
        while (! Fnum.eof())
        { Fnum.getline(st,128); Out.Num[sum]=st;  Nst[sum]=st;  sum++; }
          Fnum.close();
    }
    //---------------------------------------------------��������� ��������� ���������!
    // �������� ������
    sum=1;
    if(F8!=NULL)// �������� �������� �����
    {
        while (! F8.eof())
        {
          F8.getline(st,255); OutV.Main[sum]=st;
          F8.getline(st,255); OutV.MainPath[sum]=st;
          sum++;
        }
        F8.close();
    }

//printf(" After Reading   %s\n ", OutV.MainPath[1].c_str());
//OutV.MainPath[1]="./Wav/R_01.wav";

    // ������ �������
    sum=1;
    if(F9!=NULL)// �������� �������� �����
    {
        while (! F9.eof())
        {
          F9.getline(st,128); OutV.Snat[sum]=st;
          F9.getline(st,128); OutV.SnatPath[sum]=st;
          sum++;
        }
        F9.close();
    }
    // ???
    sum=1;
    if(F10!=NULL)// �������� �������� �����
    {
        while (! F10.eof())
        {
          F10.getline(st,128); OutV.ID[sum]=st;
          F10.getline(st,128); OutV.IDPath[sum]=st;
          sum++;
        }
        F10.close();
    }
    sum=1;
    if(F12!=NULL)// �������� �������� �����
    {
        while (! F12.eof())
        {
          F12.getline(st,128); OutV.Num[sum]=st;
          F12.getline(st,128); OutV.NumPath[sum]=st;  Nvc[sum]=st;
          sum++;
        }
        F12.close();
    }
    sum=1;
    if(F13!=NULL)// �������� �������� �����
    {
        while (! F13.eof())
        {
          F13.getline(st,128); OutV.Months[sum]=st;
          F13.getline(st,128); OutV.MonthsPath[sum]=st;  Mvc[sum]=st;
          sum++;
        }
        F13.close();
    }
    sum=1;
    if(F14!=NULL)// �������� �������� �����
    {
        while (! F14.eof())
        {
          F14.getline(st,128); OutV.Votes[sum]=st;
          F14.getline(st,128); OutV.VotesPath[sum]=st;
          sum++;
        }
        F14.close();
    }
    sum=1;
    if(F15!=NULL)// �������� �������� �����
    {
        while (! F15.eof())
        {
          F15.getline(st,128); OutV.Test[sum]=st;
          F15.getline(st,128); OutV.TestPath[sum]=st;
          sum++;
        }
        F15.close();
    }
    sum=1;
    if(F16!=NULL)// �������� �������� �����
    {
        while (! F16.eof())
        {
          F16.getline(st,128); OutV.Rezult[sum]=st;
          F16.getline(st,128); OutV.RezultPath[sum]=st;
          sum++;
        }
        F16.close();
    }


}//LoadInd
// ---------------------------------------------������� ���������� �����������-------------------------------------

//--------------���������� �������������--------------
int WaitRR(void)
{  // ������ ����������� ������������� ���������
    int pr1=1;
    if((PrErrScan<100)&&(PrErrScan>0))
    {  // ��������� ���������������  �����, ������� ����� �  ����������
       pr1=1;
    } else
    {  // ��������� ������������� �����
        if(PrRezScan==111)
        {  // ����������  ������������
            int NumV=MasScan.MasRecOut[3];
            int ii= MasScan.MasVoprOut[NumV][1];
            cout <<"Marks  =   "<< ii << endl;
            // ��������� ����������  ��������� ����������
            switch (ii)
            {  case -1:
                {  // ���������������� : ��� �������, �����������, �������� � ����
                    pr1=4;
                 } break;
                case -2:
                {  // ���������������� : �����  �������, �����������, �������� � ����
                    pr1=3;
                } break;
                default:
                {  //  ��������������, ���������, ��������� �� ����������
                    pr1=5;
                } break;
            }
        } else pr1=1;
    }
   return pr1;
}

//----------------------------------����� ������� �� ����------------------------
/*bool SyncData(TKoib K, bool KB,TVotes V, bool VC) //����� ������� �� ����
{
  bool SD;

  SD=true;

  return SD;
}
*/
//--------------------------����������� - ���� �� �������?----------------------
//xterm -T $TITLE -e
//------------------------------------------------------------------------------
int KoibMain(void)
// 1-���� ������� 2-���� ����������� 3-���� ����
{ // ����������� �������� �� ������ ������ ������� ( ��������� �������)
    int KM=0;
    if(CheckPrinter())
    {   KM=1;// ������� ������
         cout<< "  Printer  found. Scaner is main" <<endl;
    }  else
    {   KM=2; // ��� ��������
        cout << "There is no printer  " << endl;
        //if( PrConnectOK == 0) KM=3; // ������ ������������ ( �  ����������  ������)
    }
   //KM=1;
  return KM;
}
//--------------------------------------------------------------------------------------------------
void Zagruzka(void)
{  // �������� ���������  ������

   //ReadSetUpFiles(); //15.10.2013
   //FileName="./SourceDir/SourceData.XML"; // !!!!!
   Koib[1].PO = "1.1";
   Koib[1].N= SerNumKoib;
   Koib[1].IP= Ip_master;
   // !!!!!

/*	if (! AvarFiles() )  // ��� ����� ���������� ����������
	{

	} else { // �������� ���������� �����
      Koib[1].Avar=true;

	} // End �������� ���������� �����
*/
}
//------------------------------��������� ��� �������� ��������������� ������----------------------
//----------------------------------������ �� � ��������� ����������-------------------------------
void IDsave(void)
{
  ofstream F1;
  F1.open("./Avar/Votes.dat", ios::app|ios::trunc|ios::binary);

  F1.close();
}
//----------------------------------�������� �� �� ��������� ����������-------------------------------
void IDload(void)
{
 ifstream F1;
 F1.open("./Avar/Votes.dat", ios::app|ios::trunc|ios::binary);

 F1.close();
}
//----------------------------------������ ��������� ����� � ��������� ����������-------------------------------
//------------------N - �� ������ ����� ������ ������
void Koibsave(int N)
{
  ofstream F1;
 if (N==1)
 {
   F1.open("./Avar/Koib1.dat", ios::app|ios::trunc|ios::binary);
 }
 if (N==2)
 {
   F1.open("./Avar/Koib2.dat", ios::app|ios::trunc|ios::binary);
 }

  F1.close();
}
//----------------------------------�������� ��������� ����� �� ��������� ����������-------------------------------
//-----------------N - �� ������ ����� ������� ������
void Koibload(int N)
{
 ifstream F1;
 if (N==1)
 {
   F1.open("./Avar/Koib1.dat", ios::app|ios::trunc|ios::binary);
 }
 if (N==2)
 {
   F1.open("./Avar/Koib2.dat", ios::app|ios::trunc|ios::binary);
 }


  F1.close();
}
//--------------------------������� ��������� ����� �� ���������� Avar---------------------------
void AvarToTrash(void)
{  //  ��������  ����  ������  �  ����� ��������� ������ - �������� ���������� ���������
    system("rm -f Avar/Votes.dat");     system("rm -f Avar/Koib1.dat");     system("rm -f Avar/Koib2.dat");
}
//--------------------------�������� ������ �� ��������� ����������------------------------------

void LoadFromAvar(void)
{
  IDload();
  Koibload(1);
  Koibload(2);
}

//------------------------------�������� ������� ��������� ������---------------------------------
//----���������� true, ���� � ����� Avar ���� ��� 3 ��������� �����: Votes.dat, Koib1.dat, Koib2.dat
bool AvarFiles(void)
{
 bool AF=false;

 ifstream F1,F2,F3;
 F1.open("./Avar/Votes.dat", ios::app|ios::trunc|ios::binary);
 F2.open("./Avar/Koib1.dat", ios::app|ios::trunc|ios::binary);
 F3.open("./Avar/Koib2.dat", ios::app|ios::trunc|ios::binary);

 if ((F1!=NULL)&&(F2!=NULL)&&(F3!=NULL))
 {
   AF=true;
 }

return AF;
}
/*
//--------------------------------------------�������� �� �� ����------------------------------------
bool SendVotes(TVotes VotesX)
{
 bool SV=false;
   //���������� VotesX �� ����

 return SV;
}
//------------------------------------------��������� �� �� ���� -------------------------------------
void GetVotes(TVotes VotesX)
{
   Votes=VotesX;

}
//------------------------------------�������� ������ ���� �� ����-------------------------------------
//------To - � ����� ���� �������� KoibX
bool SendKoib(TKoib KoibX, int To)
{
   bool SK=false;
   //���������� KoibX �� ����

   return SK;
}
//------------------------------------��������� ������ ���� �� ����-------------------------------------
//------To - � ����� ���� ������ ���������� KoibX
void GetKoib(int To, TKoib KoibX)
{
    Koib[To]=KoibX;

}
*/
//--------------------------------------End ��� �������� ��������������� ������---------------------



//-----------------------����� St1 � St2 �� ��������� �� ����� t----------------
//-------------------------------------------------------------------------------------------------------------------------------------------------
int Samotestirovanie(void)    // ���������������� ������������
{ int i=0,j=0,Err=0;
    std::string  st,st1;
    // ������  ���������
    //frontLED(0,1); // ��������� ������� ��������
     //  ��������� �������� ����������
    Ind("",Out.Test[6]);  //��������� �������� ����������..
    frontLED(0,5); // ��������� ������� ��������
    sleep(1);
    frontLED(0,0); // ��������� ������� ��������
    // ��������� �������� �����������
    Ind("",Out.Test[4]);  //��������� �������� �����������..
    frontLED(5,0); // ��������� ������� ��������
    sleep(1);
    frontLED(0,0); // ���������� ������� ��������

    Ind("",Out.Test[1]);  //���� ��������� ������...
    Page_down();
    sleep(1);
         // �������� ���������
    Ind("",Out.Test[9]);  //���� �������
    Page_mark();
    // ������  ���������
    Ind("",Out.Test[3]);  //���� ��������� �����...
    Page_backTest();
    sleep(2);
    // �������� ��������
    get_Optrons_Imm() ;
       if(j==0) Ind("",Out.Test[10]);  //�������
   j=0;
   st1="";
    for(i=0;i<NumSensors;i++) if (Optron[i] < NO_PAPERO[i])
    { // ������ �������
        if(i==0) st1=IntToStr(i);else   {  st1=st1+", "+ IntToStr(i);  j=1;   Err=1;}
    }
    sleep(1);
    if(j==0) Ind("",Out.Test[11]);  else
    { // ������ ������� ����������� ��������
        st=Out.Test[12]+st1;      Ind("",st);
    }
    sleep(1);
    // �������� �������� ( ���� �� ���������)
    Ind("",Out.Test[8]);  //���� ��������
    sleep(1);
     if(CheckPrinter())
    {	 // ������� ���������
        Ind("",Out.Test[13]);
    } else
    {  // ������� �� ������
        Ind("",Out.Test[14]);   //Err=1;
    }
    sleep(1);
    return Err;
}


//-----------------------------------�������� �������� ������-------------------

//--------------------------���������� ����� ������� �� ����--------------------

// ------------------------------------����������� ����, �������------------------------------------
void TekDateTime(void)
{  std:: string s1,sh;
	TekDT=GetDateTime();
	DateS=IntToStr(TekDT.day)+"."+IntToStr(TekDT.month)+"."+IntToStr(TekDT.year);
	if(TekDT.hour <10)
	{ sh="0"; sh=sh+IntToStr(TekDT.hour); } else sh=IntToStr(TekDT.hour);
	if(TekDT.minit < 10)
	{ s1="0"; s1=s1+IntToStr(TekDT.minit);	} else s1=IntToStr(TekDT.minit);
	TimeS=sh+":"+s1;//IntToStr(TekDT.minit);//+":"+IntToStr(TekDT.sec);
}

//---------------���������� ��������������� ����������� �����������----------------
bool SavePreRezult(void)
{
  std::string Out;
  ofstream F;

  TekDateTime();
  Out="./result/preresult/PreResult-"+IntToStr(Votes.Nuik)+"-"+IntToStr(TekDT.day)+"."+IntToStr(TekDT.month)+"."+IntToStr(TekDT.year)+"_"+
                   IntToStr(TekDT.hour)+"."+IntToStr(TekDT.minit)+"."+IntToStr(TekDT.sec)+
                   ".xml";
  F.open(Out.c_str(), ios::out); //IG-����

  Out="<?xml version=\"1.0\" encoding=\"windows-1251\" ?>";
  F<<Out<<'\n';

  Out="<Xib uik=\""+IntToStr(Votes.Nuik)+"\" ";
  Out=Out+"isGasVrn=\"false\" ";
  Out=Out+"ts=\""+DateS+" "+TimeS+"\" ";
  Out=Out+">";
  F<<Out<<'\n';

  for (int i=1; i<=Votes.Nblank; i++)
  {
    for (int ii=1; ii<=Votes.Blank[i].NVopr; ii++)
    {

    Out="<Election n=\""+IntToStr(Votes.Blank[i].Vopr[ii].N)+"\" parentid=\""+
    Votes.Blank[i].Vopr[ii].PC.parentid+
    "\" e-mail=\""+Votes.Blank[i].Vopr[ii].PC.email+"\">";
    F<<Out<<'\n';

     for (int jj=1; jj<=Votes.Blank[i].Vopr[ii].NCandidates; jj++ )
     { //Candidates

        Out="<Candidate n=\""+IntToStr(jj)+"\">"+
        IntToStr(
                 Koib[1].Rezult[1].Bl[i].Vop[ii].Can[jj]+
                 Koib[2].Rezult[1].Bl[i].Vop[ii].Can[jj]+
                 Koib[1].Rezult[2].Bl[i].Vop[ii].Can[jj]+
                 Koib[2].Rezult[2].Bl[i].Vop[ii].Can[jj]
                 )+
        "</Candidate>";
        F<<Out<<'\n';
     }

  Out="</Election>";
  F<<Out<<'\n';
    }
  }

  Out="</Xib>";
  F<<Out<<'\n';
  F.close(); // ��������  �����  ��������� �� ��������� �������

return 0;
}


//-----------------------���������� �������� ����������� �����������---------------------
bool SaveRezult(int XVote)
{
  std::string Out;
  ofstream F;

  TekDateTime();
  Out="./result/Result-"+IntToStr(Votes.Nuik)+"-"+IntToStr(TekDT.day)+"."+IntToStr(TekDT.month)+"."+IntToStr(TekDT.year)+"_"+
                   IntToStr(TekDT.hour)+"."+IntToStr(TekDT.minit)+"."+IntToStr(TekDT.sec)+"_"+IntToStr(XVote)+
                   ".xml";

  F.open(Out.c_str(), ios::out); //IG-����

  Out="<?xml version=\"1.0\" encoding=\"windows-1251\" ?>";
  F<<Out<<'\n';

  Out="<Xib uik=\""+IntToStr(Votes.Nuik)+"\" ";

  Out=Out+"isGasVrn=\"";
  if (Votes.isGASvrn)
  {
     Out=Out+"true\" ";
  } else {
     Out=Out+"false\" ";
  }

  Out=Out+"ts=\""+DateS+" "+TimeS+"\" ";
  Out=Out+"version=\""+"zzz"+"\" "; //������ ���������-?
  Out=Out+">";
  F<<Out<<'\n';

  for (int i=1; i<=2; i++)
  {
    Out="<Scanner n=\""+Koib[i].N+"\" ";
    Out=Out+"nuf=\""+IntToStr(Koib[i].Rezult[1].NoValid+Koib[i].Rezult[2].NoValid)+"\" ";
    Out=Out+">";

    Out=Out+"<Bulletin id=\""+"1"+"\" ";
    Out=Out+"n=\""+"1"+"\" ";
    Out=Out+"Valid=\""+IntToStr(Koib[i].Rezult[1].Valid+Koib[i].Rezult[2].Valid)+"\" ";
    Out=Out+"NoMarks=\""+IntToStr(Koib[i].Rezult[1].NoMarks+Koib[i].Rezult[2].NoMarks)+"\" ";
    Out=Out+"TooManyMarks=\""+IntToStr(Koib[i].Rezult[1].ManyMarks+Koib[i].Rezult[2].ManyMarks)+"\" ";
    Out=Out+"BadMode=\""+"0"+"\" ";
    Out=Out+">";
    F<<Out<<'\n';

    Out="</Scanner>";
    F<<Out<<'\n';
  }

  Out="<Protocol final=\"true\">";
  F<<Out<<'\n';

  for (int i=1; i<=Votes.Blank[XVote].NVopr; i++)
  {
  Out="<Election n=\""+IntToStr(Votes.Blank[XVote].Vopr[i].N)+"\" parentid=\""+Votes.Blank[XVote].Vopr[i].PC.parentid+
  "\" e-mail=\""+Votes.Blank[XVote].Vopr[i].PC.email+"\">";
  F<<Out<<'\n';

     for (int j=1; j<=Votes.Blank[XVote].Vopr[i].NProtocoles; j++)
     {
       for (int jj=1; jj<=Votes.Blank[XVote].Vopr[i].Protocoles[j].NLines; jj++ )
       { //Lines
          Out="<Line n=\""+IntToStr(jj)+"\">"+IntToStr(Votes.Blank[XVote].Vopr[i].Protocoles[j].Lines[jj].Value)+
          "</Line>";
          F<<Out<<'\n';
       }
     }
     for (int jj=1; jj<=Votes.Blank[XVote].Vopr[i].NCandidates; jj++ )
     { //Candidates

//!!!!!!!!!!!!!!
/*
        Out="Vopr-"+IntToStr(Votes.Blank[XVote].NVopr);
        F<<Out<<'\n';
        Out="Candidate-"+IntToStr(Votes.Blank[XVote].Vopr[i].NCandidates);
        F<<Out<<'\n';
        for (int p=1; p<=Votes.Blank[XVote].Vopr[i].NCandidates ;p++)
        {
          Out="Candidate N-"+IntToStr(Votes.Blank[XVote].Vopr[i].Candidates[p].N);
          F<<Out<<'\n';
        }
*/
//!!!!!!!!!!!!!!
//Votes.Blank[XVote].Vopr[i].Candidates[jj].N

        Out="<Candidate n=\""+IntToStr(jj)+"\">"+
        IntToStr(
                 Koib[1].Rezult[1].Bl[XVote].Vop[i].Can[jj]+
                 Koib[2].Rezult[1].Bl[XVote].Vop[i].Can[jj]+
                 Koib[1].Rezult[2].Bl[XVote].Vop[i].Can[jj]+
                 Koib[2].Rezult[2].Bl[XVote].Vop[i].Can[jj]
                 )+
        "</Candidate>";
        F<<Out<<'\n';
     }

  Out="</Election>";
  F<<Out<<'\n';
  }

  Out="</Protocol>";
  F<<Out<<'\n';

  Out="<Check/>";
  F<<Out<<'\n';

  Out="</Xib>";
  F<<Out<<'\n';
  F.close(); // ��������  �����  ��������� �� ��������� �������

return 0;
}

//---------------------------����������� Expression-----------------------------
int Expression(std::string st)
{
  int Exp=0;


  return Exp;
}
// ----------------------------------���������� ������ ���������----------------------------------------
bool Protokol(std::string Prot, int Xbl, int Xv, int j)
// Xbl- ���������� ����� �������
// Xv- ���������� ����� �������
// j - ���������� ����� ���������
{
    std::string st;
    std::string Lines;
    std::string Checks;
    std::string Table;
    std::string ProtLines;

	st=StrBetweenStr12(Prot,"name=\"","\"",1);  //��� ���������
	if (st !="")
	   {
		 Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Name=st;
		 Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Enable=true;
	   } else {
		 Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Enable=false;
	   }
	   st=StrBetweenStr12(Prot,"numberWidth=\"","\"",1);
	        Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].numberWidth=StrToInt<int>(st); //
	   st=StrBetweenStr12(Prot,"nameWidth=\"","\"",1);
	        Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].nameWidth=StrToInt<int>(st); //
	   st=StrBetweenStr12(Prot,"valueWidth=\"","\"",1);
	        Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].valueWidth=StrToInt<int>(st); //
	   st=StrBetweenStr12(Prot,"textValueWidth=\"","\"",1);
	        Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].textValueWidth=StrToInt<int>(st); //
	   Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].disabledString=StrBetweenStr12(Prot,"disabledString=\"","\"",1);
	   Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Font=StrBetweenStr12(Prot,"font=\"","\"",1);
	   st=StrBetweenStr12(Prot,"fontsize=\"","\"",1);
	       if (st !="") {Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Fontsize=StrToInt<int>(st);} //
	   st=StrBetweenStr12(Prot,"doNotReadOut=\"","\"",1);
	       Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].doNotReadOut=TrueFalseVstr(st);

       Lines=StrBetweenStr12(Prot,"<Lines>","</Lines>",1);
	   Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NLines=NStrVstr(Lines,"<Line "); // ---Lines
	   if (Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NLines>0)
	   {
	   for (int jj= 1; jj<=Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NLines; jj++)
	     {
		 st=StrBetweenStr12(Lines,"name=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].Name=st;                 //��� ����� ���������
     	 st=StrBetweenStr12(Lines,"num=\"","\"",jj);
     	    if (st !="") {Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].N=StrToInt<int>(st);}    // ����� ������
		 st=StrBetweenStr12(Lines,"id=\"","\"",jj);
		    if (st !="") {Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].ID=StrToInt<int>(st);}   // ID
		 st=StrBetweenStr12(Lines,"additionalNum=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].AddNum=st;      //
		 st=StrBetweenStr12(Lines,"expression=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].expression=st; //������� ���������� ������
		 st=StrBetweenStr12(Lines,"type=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].type=st;       // type
	     }
       } //end Lines

       Checks=StrBetweenStr12(Prot,"<Checks>","</Checks>",1);
	   Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NChecks=NStrVstr(Checks,"<Check "); //----Checks
	   if (Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NChecks>0)
	   {
	     for (int jj= 1; jj<=Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NChecks; jj++)
	     {
		   st=StrBetweenStr12(Checks,"expression=\"","\"",jj);
		   st=FiltrStr(st,"&gt;",">");
		   // Filtr!
		      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Checks[jj].expression=st; //������� �������� ��
		   st=StrBetweenStr12(Checks,"enabled=\"","\"",jj);
		      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Checks[jj].Enabled=TrueFalseVstr(st);  //enabled
		   st=StrBetweenStr12(Checks,"mild=\"","\"",jj);
		      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Checks[jj].mild=TrueFalseVstr(st);  // mild
	     }
	   } //end Checks

	   int Nt=NStrVstr(Prot,"<Text "); // ---Text
	   if (Nt>0)
	   {
		 for (int jj= 1; jj<=Nt; jj++)
		 {
		  st=StrBetweenStr12(Prot,"final=\"","\"",jj);
		     Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.final=TrueFalseVstr(st); //final?
		  Table=StrBetweenStr12(Prot,"<Table>","</Table>",jj); //---Table

		  int Nvl=NStrVstr(Table,"<VoteLine "); //---VoteLines
		  Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.NVoteLine=Nvl;

		  if (Nvl>0)
		  {
			for (int jjj = 1; jjj <=Nvl; jjj++)
			{
			   std::string VL=StrBetweenStr12(Table,"VoteLine ","/>",jjj);

			   st=StrBetweenStr12(VL,"ID=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.VoteLine[jjj].ID=st;  // id
			   st=StrBetweenStr12(VL,"bold=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.VoteLine[jjj].bold=TrueFalseVstr(st);  // bold
			   st=StrBetweenStr12(VL,"italic=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.VoteLine[jjj].italic=TrueFalseVstr(st);  // italic
			   st=StrBetweenStr12(VL,"fontSize=\"","\"",1);    // fontSize
			   if (st !="")
			   {
				 Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.VoteLine[jjj].fontSize=StrToInt<int>(st);
			   }
			   st=StrBetweenStr12(VL,"fontSize=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.VoteLine[jjj].type=st;  // type
			   st=StrBetweenStr12(VL,"ElectronNum=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.VoteLine[jjj].ElectronNum=st;  // ElectronNum
			}
		  } // End VoteLines

		  ProtLines=StrBetweenStr12(Prot,"<ProtocolLines>","</ProtocolLines>",jj); // ProtokolLines
		  int Npl=NStrVstr(ProtLines,"<ProtocolLine "); // ---NProtokolLines
		  Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.NProtocolLine=Npl;

		  if (Npl>0)
		  {
			for (int jjj = 1; jjj <=Npl; jjj++)
			{
			   std::string PL=StrBetweenStr12(ProtLines,"<ProtocolLine ","</ProtocolLine>",jjj);

			   st=StrBetweenStr12(PL,"align=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].align=st;  // align
			   st=StrBetweenStr12(PL,"bold=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].bold=TrueFalseVstr(st);  // bold
			   st=StrBetweenStr12(PL,"italic=\"","\"",1);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].italic=TrueFalseVstr(st);  // italic
			   st=StrBetweenStr12(PL,"fontSize=\"","\"",1);    // fontSize
			   if (st !="")
			   {
				 Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].fontSize=StrToInt<int>(st);
			   }
			   st=StrBetweenStr12(PL,"section=\"","\"",1);
			   if (st!="")
			   {
		          Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].section=st;  // section
		          std::string st1="section=\""+st+"\">";
		          Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].text=StrBetweenStr12(PL,st1,"<",1);
		       }
			}
		  } // End Protokol Lines
		 }
	   } // End Text

 return true;
}
//---------------------------------������ XML �����-----------------------------
bool Parser(std::string path)
{
 bool Par=false;
 std::string XML=ReadFile(path);
 std::string SD;
 std::string REF;
 std::string RefX;
 std::string Mode;
 std::string MTT;
 std::string Bul;
 std::string Sect;
 std::string Model;
 std::string Elec;
 std::string ElecX;
 std::string PC;
 std::string SC;
 std::string Prot;
 std::string Table;
 std::string ProtLines;
 std::string Targets;

 std::string st;
 std::string st1;
 std::string w;

 ofstream F;
 F.open("./Log/Parser.log", ios::out); //Log-����
 //Filtr
  XML=FiltrStr(XML,"< ","<");
  XML=FiltrStr(XML,"<  ","<");
  XML=FiltrStr(XML," >",">");
  XML=FiltrStr(XML,"  >",">");

  XML=FiltrStr(XML,"</ ","</");
  XML=FiltrStr(XML,"</  ","</");

 // End Filtr

 SD=StrBetweenStr12(XML,"<SourceData",">",1); // �������� ������


 if (SD !="") //--------------------�������� �����!!!
 {
  Par=true;
  st1=StrBetweenStr12(SD,"DateTime=\"","\"",1); //Date Votes
  if (st1 !="")
  {
	 Votes.DByear=IntVstr(st1,1);  //Year
	 Votes.DBmonth=IntVstr(st1,2); // Month
	 Votes.DBday=IntVstr(st1,3); // Day
     /*
	 Votes.DByear=StrToInt<int>(st1);
	 st=StrBetweenStr12(SD,st1+"-","-",1);
	 Votes.DBmonth=StrToInt<int>(st); 	 st=st1+"-"+st;
	 st1=StrBetweenStr12(SD,st+"-","\"",1);
	 Votes.DBday=StrToInt<int>(st1);
     */

     Votes.DateV=IntToStr(Votes.DBday)+"."+IntToStr(Votes.DBmonth)+"."+IntToStr(Votes.DByear);
               F<<"Election_Date:"+Votes.DateV+"_:"+DateToString(Votes.DByear,Votes.DBmonth,Votes.DBday,Lang) <<'\n';
  } else {     F<<"Error:Votes_Date_is_epson"<<'\n'; }

  Votes.IDel=StrBetweenStr12(SD,"id=\"","\"",1);      //ID �������
  if (Votes.IDel !="")
  {
               F<<"ID="+Votes.IDel<<'\n';
  } else {     F<<"Error:ID_election_is_epson"<<'\n';}

  st=StrBetweenStr12(SD," isGasVrn=\"","\"",1);	//isGASvrn?
  if (st !="")
  {
      Votes.isGASvrn=TrueFalseVstr(st);
               F<<"isGasVrn="+Votes.isGASvrn<<'\n';
  } else {     F<<"isGASvrn_is_epson"<<'\n';}

  st=StrBetweenStr12(SD," protocolPassword=\"","\"",1); //ProtokolPassword
  if (st !="")
  {
      Votes.ProtokolPassword=st;
               F<<"ProtocolPassword="+Votes.ProtokolPassword<<'\n';
  } else {     F<<"ProtokolPassword_is_epson"<<'\n';}

  st=StrBetweenStr12(SD," password=\"","\"",1); //Password
  if (st !="")
  {
      Votes.Password=st;
               F<<"Password="+Votes.Password<<'\n';
  } else {     F<<"Password_is_epson"<<'\n';}

 } else {      F<<"Error:Main_part_is_epson_!!!"<<'\n'; } //-------------��� �������� �����!!


 //------------------------���� �� �������� ������������?
 REF=StrBetweenStr12(XML,"<Referendums>","</Referendums>",1);
   if (REF !="")
   {
	 XML=StrBezStr(XML,REF); // ������ ����������� ��� ������������
	           F<<"It_is_a_Referendum!"<<'\n';
   } else {    F<<"Referendum_part_is_epson"<<'\n';}
  //-----------------------���������� ����� �������
  Votes.Nblank = NStrVstr(XML,"</Blank>");
  if (Votes.Nblank>0)
  {
               F<<"Nblank="+IntToStr(Votes.Nblank)<<'\n';
  } else {     F<<"Error:Nblank=0!"<<'\n'; }
  //-----------------------���������� �������
  int Nel = NStrVstr(XML,"</Election>");
  Elec=StrBetweenStr12(XML,"<Elections>","</Elections>",1); // ������!
  if (Elec !="")
  {
	XML=StrBezStr(XML,Elec); // ������ ����������� ��� �������
	           F<<"It_is_Election"<<'\n';
  } else {     F<<"Error:Election_part_is_epson!"<<'\n';}
  //-----------------------������ �����������
  Mode=StrBetweenStr12(XML,"<Modes>","</Modes>",1);
  if (Mode !="")
  {
	 Votes.Nmodes = NStrVstr(Mode,"</Mode>"); // ���������� �������
	  if (Votes.Nmodes>0)
	  {
		st="Modes:";
		for (int i = 1; i <= Votes.Nmodes; i++)
		{
		  Votes.Modes[i]=StrBetweenStr12(Mode,"<Mode>","</Mode>",i);
		  st=st+Votes.Modes[i]+"_";
		}
	           F<<st<<'\n';
	  }
	} else {   F<<"Error:Modes_are_epson!"<<'\n';}
 //---------------------------����� ����� �������
 MTT=StrBetweenStr12(XML,"<ModeTimeTable>","</ModeTimeTable>",1);
	if (MTT !="")
	{
      st1="Time_of_change_modes:";
	  for (int i = 1; i <= Votes.Nmodes; i++)
	  {
	   st=StrBetweenStr12(MTT,"hour=\"","\"",i);   Votes.Bhour[i]=StrToInt<int>(st);
	   st=StrBetweenStr12(MTT,"minute=\"","\"",i); Votes.Bmin[i]=StrToInt<int>(st);
	   Votes.Bsec[i]=0;
	   Votes.TimeB[i]=IntToStr(Votes.Bhour[i])+":"+IntToStr(Votes.Bmin[i])+":"+IntToStr(Votes.Bsec[i]);
	   Votes.Unlim=false;
       st1=st1+Votes.Modes[i]+":"+Votes.TimeB[i]+"_";
	  }
	          F<<st1<<'\n';
	} else {
		Votes.Unlim=true;
		Votes.Bhour[1]=BhourU[1];  Votes.Bmin[1]=BminU[1]; Votes.Bsec[1]=0;
		Votes.Bhour[2]=BhourU[2];  Votes.Bmin[2]=BminU[2]; Votes.Bsec[2]=0;
		      F<<"Error:Time_of_change_modes_is_epson!"<<'\n';
	}
 //----------------------------------������ � ���
	Targets=StrBetweenStr12(XML,"<Targets>","</Targets>",1);
	if (Targets !="")
	{
	 Votes.Nuik=StrToInt<int>(StrBetweenStr12(Targets,"num=\"","\"",1));
	         F<<"UIK_number="+IntToStr(Votes.Nuik)<<'\n';
	 Votes.UIKname=(StrBetweenStr12(Targets,"name=\"","\"",1));
             F<<"UIK_name= "+Votes.UIKname<<'\n';
	} else { F<<"Error:UIK_information_is_epson!"<<'\n';}
 //----------------------------------������!
 Bul=StrBetweenStr12(XML,"<Blanks>","</Blanks>",1); // ���������
   if (Bul !="") // �������� ����������
   {
	 for (int i = 1; i <= Votes.Nblank; i++) // ���� �������� ����������
	 {
		Votes.Blank[i].ID=StrBetweenStr12(Bul,"num=\"","\"",i);
		Votes.Blank[i].Name=StrBetweenStr12(Bul,"name=\"","\"",i);

             F<< "Bulletin_N:"+IntToStr(i)+"_number="+Votes.Blank[i].ID+"_name="+Votes.Blank[i].Name<<'\n';

		w=StrBetweenStr12(Bul,"width=\"","\"",i);       if (w !="") {Votes.Blank[i].Width=StrToInt<int>(w);} else {}
		w=StrBetweenStr12(Bul,"height=\"","\"",i); 	    if (w !="") {Votes.Blank[i].Height=StrToInt<int>(w);} else {}
		w=StrBetweenStr12(Bul,"delta=\"","\"",i);  	    if (w !="") {Votes.Blank[i].Delta=StrToInt<int>(w);} else {}
		w=StrBetweenStr12(Bul,"orientation=\"","\"",i);	if (w !="") {Votes.Blank[i].Orientation=StrToInt<int>(w);} else {}
		w=StrBetweenStr12(Bul,"maxPShift=\"","\"",i);	if (w !="") {Votes.Blank[i].maxPShift=StrToInt<int>(w);} else {}
		w=StrBetweenStr12(Bul,"maxLShift=\"","\"",i);	if (w !="") {Votes.Blank[i].maxLShift=StrToInt<int>(w);} else {}
		w=StrBetweenStr12(Bul,"density=\"","\"",i); 	if (w !="") {Votes.Blank[i].density=StrToInt<int>(w);} else {};
		w=StrBetweenStr12(Bul,"marker=\"","\"",i);		if (w !="") {Votes.Blank[i].marker=StrToInt<int>(w);} else {}

		Votes.Blank[i].markerType=StrBetweenStr12(Bul,"markerType=\"","\"",i);
		if (Votes.Blank[i].markerType !="") {} else {}

		Sect=StrBetweenStr12(Bul,"<Sections>","</Sections>",i); // ������ ���������
		if (Sect !="") // ������ ��������� �������
		 {
		  int j=1;
		  do
		  {
		   st=StrBetweenStr12(Sect,"<int>","</int>",j);
		   if (st !="")
			 {
			   Votes.Blank[i].Vopr[j].ID=st;
			   Votes.Blank[i].Vopr[j].N=StrToInt<int>(st);
			   j++;
			 } else {};
		  }
		 while (st !="");
		 Votes.Blank[i].NVopr=j-1;
	   } else {} //������ �� �������

	   Model=StrBetweenStr12(Bul,"<model>","</model>",i); // ������ ���������
	   if (Model !="") // �������������� ������ ���������
	   {
		 Votes.Blank[i].Nlh=0;
		 Votes.Blank[i].Nlv=0;
		 Votes.Blank[i].Nsq=0;
		 int U1=1;
		 int U2=1;

		 for (double ii = 0; ii <= Model.length(); ii++)
		 {
		  if (Model[ii]=='\n')
		  {
			std::string Md(Model,U1,U2-U1);
			U1=U2; U2++;
			if (Md !="") //�����������
			{
				if (StrVstr(Md,"PG",1))  //PG
				{
				}
				if (StrVstr(Md,"MB",1))  //MB
				{
					Votes.Blank[i].MB.x=    IntVstr (Md, 1);
					Votes.Blank[i].MB.y=    IntVstr (Md, 2);
					Votes.Blank[i].MB.dx=   IntVstr (Md, 3);
					Votes.Blank[i].MB.dy=   IntVstr (Md, 4);
					Votes.Blank[i].MB.list= IntVstr (Md, 5);
				}
				if (StrVstr(Md,"LH",1))  //LH
				{
					Votes.Blank[i].Nlh++;
					Votes.Blank[i].LH[Votes.Blank[i].Nlh].x=    IntVstr (Md, 1);
					Votes.Blank[i].LH[Votes.Blank[i].Nlh].y=    IntVstr (Md, 2);
					Votes.Blank[i].LH[Votes.Blank[i].Nlh].dx=   IntVstr (Md, 3);
					Votes.Blank[i].LH[Votes.Blank[i].Nlh].dy=   IntVstr (Md, 4);
					Votes.Blank[i].LH[Votes.Blank[i].Nlh].list= IntVstr (Md, 5);

				}
				if (StrVstr(Md,"LV",1))  //LV
				{
					Votes.Blank[i].Nlv++;
					Votes.Blank[i].LV[Votes.Blank[i].Nlv].x=    IntVstr (Md, 1);
					Votes.Blank[i].LV[Votes.Blank[i].Nlv].y=    IntVstr (Md, 2);
					Votes.Blank[i].LV[Votes.Blank[i].Nlv].dx=   IntVstr (Md, 3);
					Votes.Blank[i].LV[Votes.Blank[i].Nlv].dy=   IntVstr (Md, 4);
					Votes.Blank[i].LV[Votes.Blank[i].Nlv].list= IntVstr (Md, 5);
				}
				if (StrVstr(Md,"MZ",1))  //MZ
				{
				}
				if (StrVstr(Md,"SQ",1))  //SQ
				{
					Votes.Blank[i].Nsq++;
					Votes.Blank[i].SQ[Votes.Blank[i].Nsq].x=    IntVstr (Md, 1);
					Votes.Blank[i].SQ[Votes.Blank[i].Nsq].y=    IntVstr (Md, 2);
					Votes.Blank[i].SQ[Votes.Blank[i].Nsq].dx=   IntVstr (Md, 3);
					Votes.Blank[i].SQ[Votes.Blank[i].Nsq].dy=   IntVstr (Md, 4);
					Votes.Blank[i].SQ[Votes.Blank[i].Nsq].list= IntVstr (Md, 5);
				}
			}

		  } else {
			U2++;
		  }
		}
	  } // End ������ ���������
   }  //End For Nblank
 } else { F<<"Error:Bulletins_information_are_epson!"<<'\n'; } //� �� ������ ���� ��������� �� �������???

 //----------------------------------------------------------������!
  if (Elec !="")  // Elec �� ������
  {
   for (int i = 1; i <= Nel; i++) // ���� �������� �������
   {
	ElecX=StrBetweenStr12(Elec,"<Election ","</Election>",i); // ������!

	if (ElecX !="") // ElecX �� ������
	{
	st1="Election_N:"+IntToStr(i);

	st=StrBetweenStr12(ElecX,"num=\"","\"",1);      // num

	Xbl=0; Xv=0;
	   for (int j=1; j<=Votes.Nblank; j++)
	   {
		 for (int jj=1; jj<=Votes.Blank[j].NVopr; jj++)
		 {
		   if (Votes.Blank[j].Vopr[jj].ID !="")
		   {
			 if (st==Votes.Blank[j].Vopr[jj].ID)
			 {
				Xbl=j; Xv=jj; goto end;
			 }
		   }
		 }
	   }

end: Votes.Blank[Xbl].Vopr[Xv].ID=st;
	 Votes.Blank[Xbl].Vopr[Xv].N=StrToInt<int>(st);

     st1=st1+"_ID="+st;

	 Votes.Blank[Xbl].Vopr[Xv].Name=StrBetweenStr12(ElecX,"name=\"","\"",1);    //��� �������
	 st1=st1+"_name="+st;

 	 st=StrBetweenStr12(ElecX,"maxMarks=\"","\"",1);                       //Max Marks
	 if (st !="")
	 {
	     Votes.Blank[Xbl].Vopr[Xv].maxMarks= StrToInt<int>(st);
	     st1=st1+"_MaxMarks="+st;
	 }

	PC=StrBetweenStr12(ElecX,"<ParentComittee ","/>",1);                   //Parent Committee
	if (PC !="")
	{
	  Votes.Blank[Xbl].Vopr[Xv].PC.parentid=StrBetweenStr12(PC,"parentid=\"","\"",1);
	  Votes.Blank[Xbl].Vopr[Xv].PC.name=StrBetweenStr12(PC,"name=\"","\"",1);
	  Votes.Blank[Xbl].Vopr[Xv].PC.email=StrBetweenStr12(PC,"e-mail=\"","\"",1);
	        st1=st1+"_ParentComittee:"+Votes.Blank[Xbl].Vopr[Xv].PC.name;
	} else {st1=st1+"_ParentComittee_epson!";}

	SC=StrBetweenStr12(ElecX,"<StampCommittees>","</StampCommittees>",1);                   //Stamp Commitees
	if (SC !="")
	{
	  int Nsc=NStrVstr(SC,"<StampCommittee ");
	  if (Nsc>0)
	  {
	     Votes.Blank[Xbl].Vopr[Xv].Nsc=Nsc;
	     st1="StampCommitees:_";
	     for (int j=1; j<=Nsc; j++)
	     {
	       st =StrBetweenStr12(SC,"num=\"","\"",j);
	       if (st !="")
	       {
	         Votes.Blank[Xbl].Vopr[Xv].SC[j]=StrToInt<int>(st);
	         st1+=st+"_";
           }
	     }
	  }
	} else {st1=st1+"_StampCommitees_epson!";}

	Mode=StrBetweenStr12(ElecX,"<Modes>","</Modes>",1);            //������
	if (Mode !="")
	{
	  Votes.Blank[Xbl].Vopr[Xv].Nmodes = NStrVstr(Mode,"</Mode>");  // ���������� �������
	  if (Votes.Blank[Xbl].Vopr[Xv].Nmodes>0)
	  {
	    st1=st1+"_Modes:";
		for (int i = 1; i <= Votes.Blank[Xbl].Vopr[Xv].Nmodes; i++)
		{
		  Votes.Blank[Xbl].Vopr[Xv].Modes[i]=StrBetweenStr12(Mode,"<Mode>","</Mode>",i);
		  st1=st1+"_"+Votes.Blank[Xbl].Vopr[Xv].Modes[i];
		}
	  }
	} else {st1=st1+"_Modes_epson!";} // � ����� ������ ������ ???

   Votes.Blank[Xbl].Vopr[Xv].NProtocoles=NStrVstr(ElecX,"</Protocol>"); //------���-�� ����������
   if (Votes.Blank[Xbl].Vopr[Xv].NProtocoles>0)
   {
	 for (int j=1; j<=Votes.Blank[Xbl].Vopr[Xv].NProtocoles; j++)
	 {
	 Prot=StrBetweenStr12(ElecX,"<Protocol ","</Protocol>",j); // ���������
	 if (Prot !="")
	 {

      Protokol(Prot,Xbl,Xv,j); // �������� ����������
      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].ProtX=Prot;

	 } else
        {   //  ����� ��������  ������  ���������


        } //���������� ���?
   }
  } else {st1=st1+"_PROTOCOLES_epson!";} //���������� ���!

	Votes.Blank[Xbl].Vopr[Xv].NCandidates=NStrVstr(ElecX,"<Candidate "); // ���������

	if (Votes.Blank[Xbl].Vopr[Xv].NCandidates>0)
	{
	  for (int jj= 1; jj<=Votes.Blank[Xbl].Vopr[Xv].NCandidates; jj++)
	  {
       st1=st1+"_Candidate_N:"+IntToStr(jj);
       Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].lastName=StrBetweenStr12(ElecX,"lastName=\"","\"",jj); //

	   F<<Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].lastName<<'\n';

	   Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].firstName=StrBetweenStr12(ElecX,"firstName=\"","\"",jj); //

	   F<<Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].firstName<<'\n';

	   Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].patronymic=StrBetweenStr12(ElecX,"patronymic=\"","\"",jj); //

       F<<Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].patronymic<<'\n';

       //st1=st1+"_name="+Blank[Xbl].Vopr[Xv].Candidates[jj].lastName+"_"+Blank[Xbl].Vopr[Xv].Candidates[jj].firstName+'\n';

	   st=StrBetweenStr12(ElecX,"registered=\"","\"",jj); // registered
	   if (st !="")
	   {
		 Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].registered=TrueFalseVstr(st);
	   }

	   st=StrBetweenStr12(ElecX,"disabled=\"","\"",jj);  //disabled
	   if (st !="")
	   {
		 Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].disabled=TrueFalseVstr(st);
	   }

	   Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].ID=StrBetweenStr12(ElecX,"id=\"","\"",jj);  //ID

	   st=StrBetweenStr12(ElecX,"num=\"","\"",jj);  // num
	   if (st !="")
	   {
		 Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].N=StrToInt<int>(st);
		 st1=st1+"_num="+st;
	   }

	   Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].biography=StrBetweenStr12(ElecX,"biography=\"","\"",jj); //

	   st=StrBetweenStr12(ElecX,"selfRegistered=\"","\"",jj);
	   if (st !="")
	   {
		 Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].selfRegistered=TrueFalseVstr(st); //
	   }
	   Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].party=StrBetweenStr12(ElecX,"party=\"","\"",jj);

	   st=StrBetweenStr12(ElecX,"noneAbove=\"","\"",jj);  //
	   if (st !="")
	   {
		 Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].disabled=TrueFalseVstr(st);
	   }
	 } // End For Ncandidates
   }  else {st1=st1+"_CANDIDATES_epson!";} // ���������� ���!


	//---!!!---

         F<<st1<<'\n';

  } // ElecX �� ������
  else {} //ElecX ������ - ��������, � � ��� ������ (�������) �� �������!

 }
} else { F<<"Error:ELECTION_information_are_epson!"<<'\n'; } //��������, � � ��� ������ (�������) ������ �� �������!

 //--------------------------------------------------------����������!
 if (REF !="")
 {
   int Nref = NStrVstr(REF,"</Referendum>"); // ���������� ������������

	for (int i = 1; i <= Nref; i++) // ���� �������� ������������
   {
	RefX=StrBetweenStr12(REF,"<Referendum ","</Referendum>",i); // �����������!
	if (RefX !="")
	{

	  int Nel=NStrVstr(RefX,"</Elections>"); // �������� �������
	  if (Nel>0) //������ �������
	  {
		 int j=1;
		 std::string El[10];

		 do
		 {
		 st=StrBetweenStr12(RefX,"<int>","</int>",j);
		   if (st !="")
			 {
			   El[j]=st;
			   j++;
			 } else {};
		 }
		 while (st !="");

	   bool Pravda;
	   for (int j=1; j<=Votes.Nblank; j++)
	   {
		 Pravda=true;
		 Xbl=0;

		 for (int jj=1; jj<=Votes.Blank[j].NVopr; jj++)
		 {
		   if (Votes.Blank[j].Vopr[jj].ID !="")
		   {
			   if (El[j]!=Votes.Blank[j].Vopr[jj].ID)
			   {
				 Pravda=false;
			   }
		   }
		 }
		 if (Pravda)
		 {
		   Xbl=j; goto end2;
		 }
	   }
end2:  if (Xbl>0)
	   {
		Votes.Blank[Xbl].Refer=true;
		st=StrBetweenStr12(RefX,"num=\"","\"",1);   Votes.Blank[Xbl].Ref.ID=st;    // num
		st=StrBetweenStr12(RefX,"name=\"","\"",1);  Votes.Blank[Xbl].Ref.Name=st;  //��� �����������

    PC=StrBetweenStr12(RefX,"<ParentComittee","/>",1); //Parent Committee
	if (PC !="")
	{
	  Votes.Blank[Xbl].Ref.PC.parentid=StrBetweenStr12(PC,"parentid=\"","\"",1);
	  Votes.Blank[Xbl].Ref.PC.name=StrBetweenStr12(PC,"name=\"","\"",1);
	  Votes.Blank[Xbl].Ref.PC.email=StrBetweenStr12(PC,"email=\"","\"",1);
	}

	SC=StrBetweenStr12(RefX,"StampCommittees","/>",1); //Stamp Commitees
	if (SC !="")
	{
	  st=StrBetweenStr12(SC,"num=\"","\"",1);  Votes.Blank[Xbl].Ref.SC=StrToInt<int>(st);
	}

	Mode=StrBetweenStr12(RefX,"<Modes>","</Modes>",1);    //������
	if (Mode !="")
  {
   Votes.Blank[Xbl].Ref.Nmodes = NStrVstr(Mode,"/Mode");        // ���������� �������

   for (int i = 1; i <= Votes.Blank[Xbl].Ref.Nmodes; i++)
   {
	 Votes.Blank[Xbl].Ref.Modes[i]=StrBetweenStr12(Mode,"<Mode>","</Mode>",i);
   }
  } else { } // � ����� ������ ������???


   Votes.Blank[Xbl].Ref.NProtocoles=NStrVstr(RefX,"</Protocol>");                   // ���-�� ����������
   if (Votes.Blank[Xbl].Ref.NProtocoles>0)
   {
   for (int j=1; j<=Votes.Blank[Xbl].Ref.NProtocoles; j++)
   {
	 Prot=StrBetweenStr12(RefX,"<Protocol ","</Protocol>",j); // ���������
	 if (Prot !="")
	 {
	    Protokol(Prot,Xbl,Xv,j); //�������� ����������
	    Votes.Blank[Xbl].Ref.Protocoles[j].ProtX=Prot;

	   st=StrBetweenStr12(Prot,"name=\"","\"",1);  //��� ���������
	   if (st !="")
	   {
	      Votes.Blank[Xbl].Ref.Protocoles[j].Name=st;
	      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Enable=true;
	   } else {
	      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Enable=false;
	   }


	   st=StrBetweenStr12(Prot,"numberWidth=\"","\"",1);     Votes.Blank[Xbl].Ref.Protocoles[j].numberWidth=StrToInt<int>(st); //
	   st=StrBetweenStr12(Prot,"nameWidth=\"","\"",1);       Votes.Blank[Xbl].Ref.Protocoles[j].nameWidth=StrToInt<int>(st); //
	   st=StrBetweenStr12(Prot,"valueWidth=\"","\"",1);      Votes.Blank[Xbl].Ref.Protocoles[j].valueWidth=StrToInt<int>(st); //
	   st=StrBetweenStr12(Prot,"textValueWidth=\"","\"",1);  Votes.Blank[Xbl].Ref.Protocoles[j].textValueWidth=StrToInt<int>(st); //
	   Votes.Blank[Xbl].Ref.Protocoles[j].disabledString=StrBetweenStr12(Prot,"disabledString=\"","\"",1);
	   Votes.Blank[Xbl].Ref.Protocoles[j].Font=StrBetweenStr12(Prot,"font=\"","\"",1);
	   st=StrBetweenStr12(Prot,"fontsize=\"","\"",1);        Votes.Blank[Xbl].Ref.Protocoles[j].Fontsize=StrToInt<int>(st); //

	   st=StrBetweenStr12(Prot,"doNotReadOut=\"","\"",1);    Votes.Blank[Xbl].Ref.Protocoles[j].doNotReadOut=TrueFalseVstr(st);  //

	   Votes.Blank[Xbl].Ref.Protocoles[j].NLines=NStrVstr(Prot,"<Line "); // Lines
	   if (Votes.Blank[Xbl].Ref.Protocoles[j].NLines>0)
	{
	   for (int jj= 1; jj<=Votes.Blank[Xbl].Ref.Protocoles[j].NLines; jj++)
	   {
		 st=StrBetweenStr12(Prot,"name=\"","\"",jj);          Votes.Blank[Xbl].Ref.Protocoles[j].Lines[jj].Name=st; //��� ����� ���������
		 st=StrBetweenStr12(Prot,"expression=\"","\"",jj);    Votes.Blank[Xbl].Ref.Protocoles[j].Lines[jj].expression=st; //������� ���������� ������
		 st=StrBetweenStr12(Prot,"num=\"","\"",jj);           Votes.Blank[Xbl].Ref.Protocoles[j].Lines[jj].N=StrToInt<int>(st); // ����� ������
		 st=StrBetweenStr12(Prot,"additionalNum=\"","\"",jj); Votes.Blank[Xbl].Ref.Protocoles[j].Lines[jj].AddNum=st; //
		 st=StrBetweenStr12(Prot,"id=\"","\"",jj);            Votes.Blank[Xbl].Ref.Protocoles[j].Lines[jj].ID=StrToInt<int>(st); // ID
		 st=StrBetweenStr12(Prot,"type=\"","\"",jj);          Votes.Blank[Xbl].Ref.Protocoles[j].Lines[jj].type=st; // type
	 }
   } //end Lines

	   Votes.Blank[Xbl].Ref.Protocoles[j].NChecks=NStrVstr(Prot,"<Check "); // Checks
	   if (Votes.Blank[Xbl].Ref.Protocoles[j].NChecks>0)
	{
	   for (int jj= 1; jj<=Votes.Blank[Xbl].Ref.Protocoles[j].NChecks; jj++)
	   {
		 st=StrBetweenStr12(Prot,"expression=\"","\"",jj);    Votes.Blank[Xbl].Ref.Protocoles[j].Checks[jj].expression=st; //������� �������� ��
         st=StrBetweenStr12(Prot,"enabled=\"","\"",1);        Votes.Blank[Xbl].Ref.Protocoles[j].Checks[jj].Enabled=TrueFalseVstr(st);  //enabled
 	     st=StrBetweenStr12(Prot,"mild=\"","\"",1);           Votes.Blank[Xbl].Ref.Protocoles[j].Checks[jj].mild=TrueFalseVstr(st);  // mild
	   }
	} //end Checks

       int Nt=NStrVstr(Prot,"<Text "); // ---Text
	   if (Nt>0)
	   {
		 for (int jj= 1; jj<=Nt; jj++)
		 {
		  st=StrBetweenStr12(Prot,"final=\"","\"",jj);  Votes.Blank[Xbl].Ref.Protocoles[j].Text.final=TrueFalseVstr(st); //final?
		  Table=StrBetweenStr12(Prot,"<Table>","</Table>",jj); // Table
		  int Nvl=NStrVstr(Table,"<VoteLine "); // ---VoteLines
		  Votes.Blank[Xbl].Ref.Protocoles[j].Text.NVoteLine=Nvl;

		  if (Nvl>0)
		  {
			for (int jjj = 1; jjj <=Nvl; jjj++)
			{
			   st=StrBetweenStr12(Table,"id=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.VoteLine[jjj].ID=st;  // id
			   st=StrBetweenStr12(Table,"bold=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.VoteLine[jjj].bold=TrueFalseVstr(st);  // bold
			   st=StrBetweenStr12(Table,"italic=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.VoteLine[jjj].italic=TrueFalseVstr(st);  // italic
			   st=StrBetweenStr12(Table,"fontSize=\"","\"",jjj);    // fontSize
			   if (st !="")
			   {
				 Votes.Blank[Xbl].Ref.Protocoles[j].Text.VoteLine[jjj].fontSize=StrToInt<int>(st);
			   }
			   st=StrBetweenStr12(Table,"fontSize=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.VoteLine[jjj].type=st;  // type
			   st=StrBetweenStr12(Table,"ElectronNum=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.VoteLine[jjj].ElectronNum=st;  // ElectronNum
			}
		  } // End Vote Lines

		  ProtLines=StrBetweenStr12(Prot,"<ProtocolLines>","</ProtocolLines>",jj); // ProtokolLines
		  int Npl=NStrVstr(ProtLines,"<ProtocolLine "); // ---NProtokolLines
		  Votes.Blank[Xbl].Ref.Protocoles[j].Text.NProtocolLine=Npl;

		  if (Npl>0)
		  {
			for (int jjj = 1; jjj <=Npl; jjj++)
			{
			   st=StrBetweenStr12(ProtLines,"align=\"","\"",jjj);   Votes.Blank[Xbl].Ref.Protocoles[j].Text.ProtocolLine[jjj].align=st;  // align
			   st=StrBetweenStr12(ProtLines,"bold=\"","\"",jjj);    Votes.Blank[Xbl].Ref.Protocoles[j].Text.ProtocolLine[jjj].bold=TrueFalseVstr(st);  // bold
			   st=StrBetweenStr12(ProtLines,"italic=\"","\"",jjj);  Votes.Blank[Xbl].Ref.Protocoles[j].Text.ProtocolLine[jjj].italic=TrueFalseVstr(st);  // italic
			   st=StrBetweenStr12(ProtLines,"fontSize=\"","\"",jjj);    // fontSize
			   if (st !="")
			   {
				 Votes.Blank[Xbl].Ref.Protocoles[j].Text.ProtocolLine[jjj].fontSize=StrToInt<int>(st);
			   }
			   st=StrBetweenStr12(Table,"section=\"","\"",jjj);  Votes.Blank[Xbl].Ref.Protocoles[j].Text.ProtocolLine[jjj].section=st;  // section
			}
		  } // End Protokol Lines
		 }
	   } // End Text

	 } else { } //���������� ���?
   }
   } else { } //���������� ���!

	}
   }
  }
 }

} else {} // ��� �� ����������. ��..�..

		//����������� �� ����������, ���������� ������ ������� ������
		if (Votes.Nblank>0)
		{
		for (int i = 1; i <= Votes.Nblank; i++)
		{

		Votes.Blank[i].ShAreaHeight=Votes.Blank[i].LH[2].y; // ������ ������� ������

		int sum=0;
		if (Votes.Blank[i].NVopr>0)
		{
		  for (int ii = 1; ii <= Votes.Blank[i].NVopr; ii++)
		  {
			if (Votes.Blank[i].Vopr[ii].NCandidates>0)
			{
			  for (int iii = 1; iii <= Votes.Blank[i].Vopr[ii].NCandidates; iii++)
			  {
			  sum++;
				if (sum<=Votes.Blank[i].Nsq)
				{
				  Votes.Blank[i].Vopr[ii].Candidates[iii].SQx = Votes.Blank[i].SQ[sum].x;
				  Votes.Blank[i].Vopr[ii].Candidates[iii].dx = Votes.Blank[i].SQ[sum].dx;
				  Votes.Blank[i].Vopr[ii].Candidates[iii].SQ = Votes.Blank[i].SQ[sum].dx;
				  Votes.Blank[i].Vopr[ii].Candidates[iii].SQy = Votes.Blank[i].SQ[sum].y;
				  Votes.Blank[i].Vopr[ii].Candidates[iii].dy = Votes.Blank[i].SQ[sum].dy;
				}
			  }
			}
		  }
		 }
		}
		}

		//End ����������� �� ����������, ���������� ������ ������� ������

		// ����������� ����������
		std::string ProtLineExp;
		std::string ProtCheckExp;

		if (Votes.Nblank>0)
		{
		 for (int i = 1; i <= Votes.Nblank; i++)
		 {
		  if (Votes.Blank[i].NVopr>0)
		  {
		   for (int ii = 1; ii <= Votes.Blank[i].NVopr; ii++)
		   {
			if (Votes.Blank[i].Vopr[ii].NProtocoles>0)
			{
			 for (int iii = 1; iii <= Votes.Blank[i].Vopr[ii].NProtocoles; iii++)
			 {

			  // ------------------Lines
			  if (Votes.Blank[i].Vopr[ii].Protocoles[iii].NLines>0)
			  {
			   for (int iiii = 1; iiii <= Votes.Blank[i].Vopr[ii].Protocoles[iii].NLines; iiii++)
			   {
				 ProtLineExp=Votes.Blank[i].Vopr[ii].Protocoles[iii].Lines[iiii].expression;
			   }
			  }
			  //--------------------End Lines

			  //-------------------Checks
			  if (Votes.Blank[i].Vopr[ii].Protocoles[iii].NChecks>0)
			  {
			   for (int iiii = 1; iiii <= Votes.Blank[i].Vopr[ii].Protocoles[iii].NChecks; iiii++)
			   {
				 ProtCheckExp=Votes.Blank[i].Vopr[ii].Protocoles[iii].Checks[iiii].expression;

			   }
			  }
			  //-------------------End Checks

			 }
			}
		   }
		  }
		 }
		}
		// End ����������� ����������

   F.close(); //��������� ���-����

  return Par;
}
//----------------------------------------------------------------------------
int Block(std::string B, int XVote, int Xv, bool Left)
{
  std::string st="";
  std::string st1="";
  int Npb=0;
  int SumB=0;
  int iPb[20];
  int zPb[19];

     B=FiltrStr(B,"S","[S]");
     B=FiltrStr(B,"M","[M]");

  // ��������� ���� �� ��������� ����� � ����� ����� ����
  Npb=NStrVstr(B,"[");

  for (int i=1; i<=Npb; i++) //Npb
  {
      st=StrBetweenStr12(B,"[","]",i);
      int Ns=StrToInt<int>(st);

   if (Ns==0) //�� �����
   {
      if (StrVstr(st,"B",1))
      {
        int Nb=StrToInt<int>(StrBezStr(st,"B"));
        if (Left)
        {
          SumB=iBlocks1[Nb];
        } else {
          SumB=iBlocks2[Nb];
        }
      }
      if (StrVstr(st,"S",1))
      {
        SumB=Votes.Blank[XVote].Vopr[Xv].Protocoles[1].S;
      }
      if (StrVstr(st,"M",1))
      {
        SumB=Votes.Blank[XVote].Vopr[Xv].Protocoles[1].M;
      }
   } else { // �����
     SumB=Votes.Blank[XVote].Vopr[Xv].Protocoles[1].Lines[Ns].Value;
   }

   iPb[i]=SumB;

      if (i<Npb)
      {
        st1=StrBetweenStr12(B,"]","[",i);

        if (StrVstr(st1,"*",1))
        {
          zPb[i]=1;
        }
        if (StrVstr(st1,"/",1))
        {
          zPb[i]=2;
        }
        if (StrVstr(st1,"+",1))
        {
          zPb[i]=3;
        }
        if (StrVstr(st1,"-",1))
        {
          zPb[i]=4;
        }
     }
  } //End Npb

  bool Ppb=false;
  int Nppb=0;
  int iPpb[20];
  int zPpb[20];

// ��������� �� ��������, ����� �������� ������ ����� ��� ��������
  for (int i=1; i<=Npb-1; i++)
  {
    if (Ppb)
    {
      if (zPb[i-1]==1)
      {
        iPpb[Nppb]=iPpb[Nppb]*iPb[i];
      }
      if (zPb[i-1]==2)
      {
        if (iPb[i]!=0) {iPpb[Nppb]=iPpb[Nppb]/iPb[i];}
      }
      if ((zPb[i-1]==3)||(zPb[i-1]==4))
      {
         zPpb[Nppb]=zPb[i-1];
         Nppb++; iPpb[Nppb]=iPb[i];
         if ((zPb[i]==3)||(zPb[i]==4)) { Ppb=false; zPpb[Nppb]=zPb[i]; }
      }

    } else {
      if ((zPb[i]==1)||(zPb[i]==2))
      {
         Nppb++;
         iPpb[Nppb]=iPb[i];
         Ppb=true;
      } else {
         Nppb++;
         iPpb[Nppb]=iPb[i];
         zPpb[Nppb]=zPb[i];
      }
    }
  }

    if (zPb[Npb-1]==1)
    {
       iPpb[Nppb]=iPpb[Nppb]*iPb[Npb];
    }
    if (zPb[Npb-1]==2)
    {
      if (iPb[Npb]!=0) { iPpb[Nppb]=iPpb[Nppb]/iPb[Npb]; }
    }
    if ((zPb[Npb-1]==3)||(zPb[Npb-1]==4))
    {
       Nppb++; iPpb[Nppb]=iPb[Npb];
               zPpb[Nppb]=0;
    }
//��������� � �������� ��������

   int Sum=0;
   Sum=iPpb[1];
   for (int i=2; i<=Nppb; i++)
   {
     if (zPpb[i-1]==3)
     {
        Sum=Sum+iPpb[i];
     }
     if (zPpb[i-1]==4)
     {
        Sum=Sum-iPpb[i];
     }
   }

 return Sum;
}
//--------------------------������������ �������� Checks ������ ���������, ���������� ��� ������������-----------------------------
bool Checks(std::string Ch, int XVote, int Xv)
{
   bool Yes=false;
   int Rav=0; //=1 >2 <3 <>4 >=5 <=6
   std::string RavSt="";
   //Ravenstvo
			if (StrVstr(Ch,"=",1)) { Rav=1; RavSt="=";}
			if ((StrVstr(Ch,">",1))||(StrVstr(Ch,"&gt;",1))) { Rav=2; RavSt=">"; }
			if (StrVstr(Ch,"<",1))	{ Rav=3; RavSt="<"; }
			if (StrVstr(Ch,"<>",1)) { Rav=4; RavSt="<>"; }
			if ((StrVstr(Ch,">=",1))||(StrVstr(Ch,"&gt;=",1))) { Rav=5; RavSt=">="; }
			if (StrVstr(Ch,"<=",1)) { Rav=6; RavSt="<="; }
   //End Ravenstvo
  // Left & Right
  int index = Ch.find(RavSt);
  std::string Left(Ch,0,index);
  std::string Right(Ch,index+RavSt.length(),Ch.length());
  // Left & Right
  // S & M
  int S=Koib[1].Rezult[1].Bl[XVote].Vop[Xv].Valid+
        Koib[1].Rezult[2].Bl[XVote].Vop[Xv].Valid+
        Koib[2].Rezult[1].Bl[XVote].Vop[Xv].Valid+
        Koib[2].Rezult[2].Bl[XVote].Vop[Xv].Valid; //?????
  int M=Votes.Blank[XVote].Vopr[Xv].NCandidates;   //?????

  S=100; M=200; //-?-

  Votes.Blank[XVote].Vopr[Xv].Protocoles[1].S=S;
  Votes.Blank[XVote].Vopr[Xv].Protocoles[2].S=S;
  Votes.Blank[XVote].Vopr[Xv].Protocoles[1].M=M;
  Votes.Blank[XVote].Vopr[Xv].Protocoles[2].M=M;
  // S & M
  //Left
  std::string st="";
  int NBlocks1=0;
  std::string Blocks1[10];
  std::string Left1=Left;
  do
  {
    st=StrBetweenStr12(Left,"(",")",NBlocks1+1);
    if (st!="")
    {
      NBlocks1++; Blocks1[NBlocks1]=st;
      Left1=FiltrStr(Left1,"("+st+")","[B"+IntToStr(NBlocks1)+"]");
      iBlocks1[NBlocks1]=Block(st,XVote,Xv,true);
    }
  } while (st!="");

  //Right
  int NBlocks2=0;
  std::string Blocks2[10];
  std::string Right1=Right;
  do
  {
    st=StrBetweenStr12(Right,"(",")",NBlocks2+1);
    if (st!="")
    {
      NBlocks2++; Blocks2[NBlocks2]=st;
      Right1=FiltrStr(Right1,"("+st+")","[B"+IntToStr(NBlocks2)+"]");
      iBlocks2[NBlocks2]=Block(st,XVote,Xv,false);
    }
  } while (st!="");

  int iLeft=Block(Left1,XVote,Xv,true);
  int iRight=Block(Right1,XVote,Xv,false);

  switch (Rav)
  {
     case 1: {Yes=(iLeft==iRight);}
     case 2: {Yes=(iLeft>iRight);}
     case 3: {Yes=(iLeft<iRight);}
     case 4: {Yes=(iLeft!=iRight);}
     case 5: {Yes=(iLeft>=iRight);}
     case 6: {Yes=(iLeft<=iRight);}
     default:{}
  }
 return Yes;
}

//---------------------������������ �������� Express ������ ���������, ���������� ��� ��������----------------------
int Express(std::string Exp, int XVote, int Xv, int Nkoib)
// Nkoib=1 ��� 2, ���� 0, �� ��� �����!
{
   int N=0;
   int sig=0;
   int Zn=0;
   int SumZn=0;
   std::string st;
   std::string st1;
   do
   {
   N++;

   st=StrBetweenStr12(Exp, "{", "}", N);
	 if (st !="")
	 {
	   if (StrVstr(st, "BlankType=Valid", 1))
	   // ����� ���-�� �������� �������������� ����������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[1].Bl[XVote].Vop[Xv].Valid+Koib[2].Rezult[1].Bl[XVote].Vop[Xv].Valid+
		       Koib[1].Rezult[2].Bl[XVote].Vop[Xv].Valid+Koib[2].Rezult[2].Bl[XVote].Vop[Xv].Valid;
		  } else {
		    Zn=Koib[Nkoib].Rezult[1].Bl[XVote].Vop[Xv].Valid+
		       Koib[Nkoib].Rezult[2].Bl[XVote].Vop[Xv].Valid;
		  }
	   }
	   if (StrVstr(st, "BlankType=NoMarks", 1))
	   // ����� ���-�� �������� ���������� ��� �������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[1].Bl[XVote].Vop[Xv].NoMarks+Koib[2].Rezult[1].Bl[XVote].Vop[Xv].NoMarks+
		       Koib[1].Rezult[2].Bl[XVote].Vop[Xv].NoMarks+Koib[2].Rezult[2].Bl[XVote].Vop[Xv].NoMarks;
		  } else {
		    Zn=Koib[Nkoib].Rezult[1].Bl[XVote].Vop[Xv].NoMarks+
		       Koib[Nkoib].Rezult[2].Bl[XVote].Vop[Xv].NoMarks;
		  }
	   }
	   if (StrVstr(st, "BlankType=TooManyMarks", 1))
	   // ����� ���-�� �������� ���������� � ����������� ���-�� �������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[1].Bl[XVote].Vop[Xv].ManyMarks+Koib[2].Rezult[1].Bl[XVote].Vop[Xv].ManyMarks+
		       Koib[1].Rezult[2].Bl[XVote].Vop[Xv].ManyMarks+Koib[2].Rezult[2].Bl[XVote].Vop[Xv].ManyMarks;
		  } else {
		    Zn=Koib[Nkoib].Rezult[1].Bl[XVote].Vop[Xv].ManyMarks+
		       Koib[Nkoib].Rezult[2].Bl[XVote].Vop[Xv].ManyMarks;
		  }
	   }
	   if ((StrVstr(st, "BlankType=Valid", 1))&&(StrVstr(st, "VotingMode=Main", 1)))
	   // ����� �����������-��������, ���-�� �������� �������������� ����������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[1].Bl[XVote].Vop[Xv].Valid+Koib[2].Rezult[1].Bl[XVote].Vop[Xv].Valid;
		  } else {
		    Zn=Koib[Nkoib].Rezult[1].Bl[XVote].Vop[Xv].Valid;
		  }
	   }
	   if ((StrVstr(st, "BlankType=NoMarks", 1))&&(StrVstr(st, "VotingMode=Main", 1)))
	   // ����� �����������-��������, ���-�� �������� ���������� ��� �������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[1].Bl[XVote].Vop[Xv].NoMarks+Koib[2].Rezult[1].Bl[XVote].Vop[Xv].NoMarks;
		  } else {
		    Zn=Koib[Nkoib].Rezult[1].Bl[XVote].Vop[Xv].NoMarks;
		  }
	   }
	   if ((StrVstr(st, "BlankType=TooManyMarks", 1))&&(StrVstr(st, "VotingMode=Main", 1)))
	   // ����� �����������-��������, ���-�� �������� ���������� � ����������� ���-�� �������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[1].Bl[XVote].Vop[Xv].ManyMarks+Koib[2].Rezult[1].Bl[XVote].Vop[Xv].ManyMarks;
		  } else {
		    Zn=Koib[Nkoib].Rezult[1].Bl[XVote].Vop[Xv].ManyMarks;
		  }
	   }
	   if ((StrVstr(st, "BlankType=Valid", 1))&&(StrVstr(st, "VotingMode=Portable", 1)))
	   // ����� �����������-����������, ���-�� �������� �������������� ����������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[2].Bl[XVote].Vop[Xv].Valid+Koib[2].Rezult[2].Bl[XVote].Vop[Xv].Valid;
		  } else {
		    Zn=Koib[Nkoib].Rezult[2].Bl[XVote].Vop[Xv].Valid;
		  }
	   }
	   if ((StrVstr(st, "BlankType=NoMarks", 1))&&(StrVstr(st, "VotingMode=Portable", 1)))
	   // ����� �����������-����������, ���-�� �������� ���������� ��� �������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[2].Bl[XVote].Vop[Xv].NoMarks+Koib[2].Rezult[2].Bl[XVote].Vop[Xv].NoMarks;
		  } else {
		    Zn=Koib[Nkoib].Rezult[2].Bl[XVote].Vop[Xv].NoMarks;
		  }
	   }
	   if ((StrVstr(st, "BlankType=TooManyMarks", 1))&&(StrVstr(st, "VotingMode=Portable", 1)))
	   // ����� �����������-����������, ���-�� �������� ���������� � ����������� ���-�� �������
	   {
		  if (Nkoib==0)
		  {
		    Zn=Koib[1].Rezult[2].Bl[XVote].Vop[Xv].ManyMarks+Koib[2].Rezult[2].Bl[XVote].Vop[Xv].ManyMarks;
		  } else {
		    Zn=Koib[Nkoib].Rezult[2].Bl[XVote].Vop[Xv].ManyMarks;
		  }
	   }
	   //------------------End Expression---------------
	   //------------------Signum----------------
		if (N>1)
		{
		  st1=StrBetweenStr12(Exp, "}", "{", N-1);
		  if (st1 !="")
		  {
			if (StrVstr(st1,"+",1))
			{
				sig=1;
			}
			if (StrVstr(st1,"-",1))
			{
				sig=-1;
			}
			SumZn=SumZn+sig*Zn;
		  }
		} else { SumZn=Zn; }
	   //------------------End Signum----------------
	 }
   } while (st !="");
   return SumZn;
}
//-------------------------------------������� ��������-------------------------------------
bool MakeProtokol(int XVote, int Xv)
{
  std::string ProtX;
  std::string st;
  bool prOkKC=true;
  //------������������ � �������� ����������
  if (Votes.Blank[XVote].Vopr[Xv].NProtocoles>0)
  { //Protocol Bar
    for (int i=1; i<=Votes.Blank[XVote].Vopr[Xv].NProtocoles; i++)
    {
      ProtX=Votes.Blank[XVote].Vopr[Xv].Protocoles[i].ProtX;
      st="./Protokols/"+IntToStr(i)+".XML";
      WriteFile(ProtX,st);
    }

  }// End Protocol Bar
  else { //Protokol Johtur

    std::string Prot=ReadFile("./XProtokols/1.XML");
    //NStrVstr(ProtX, "</Protocol>");
    Votes.Blank[XVote].Vopr[Xv].NProtocoles=1;
    //std::string Prot=StrBetweenStr12(ProtX,"<Protocol ","</Protocol>",1);
    Protokol(Prot,XVote,Xv,1);

  } //End Protokol Johtur
    //------End ������������ � �������� ����������

  for (int i=1; i<=Votes.Blank[XVote].Vopr[Xv].NProtocoles; i++) // 1..Nprotokoles
  {
    //�������� �� �������� Lines
    if (Votes.Blank[XVote].Vopr[Xv].Protocoles[i].NLines>0)
    {
       for (int j=1; j<=Votes.Blank[XVote].Vopr[Xv].Protocoles[i].NLines; j++) //1..Nlines
       {
          //�������� ������ �� ��� �����
          if (Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Lines[j].expression!="")
          {
            Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Lines[j].Value=
            Express(Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Lines[j].expression,
                    XVote, Xv, 0);
          }
       } //End 1..Nlines
    }
    //End �������� �� �������� Lines

    //�������� � ������������ ������������� Checks
    if (Votes.Blank[XVote].Vopr[Xv].Protocoles[i].NChecks>0)
    {
       for (int j=1; j<=Votes.Blank[XVote].Vopr[Xv].Protocoles[i].NChecks; j++) //1..NChecks
       {
          if ((Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Checks[j].expression!="")&&
              (Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Checks[j].Enabled ))
          {
           Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Checks[j].Itog=
           Checks(Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Checks[j].expression,
                  XVote,Xv);

          }
          if(Votes.Blank[XVote].Vopr[Xv].Protocoles[i].Checks[j].Itog==false)prOkKC=false;
       }

    }
    //End �������� � ������������ ������������� Checks

  } // End 1..Nprotokoles

  return prOkKC;
}

//------------------------------------------------------------------------------
void SumRep(int Rep_cntX, int Typ, int Alg, int Bold, int Tab, int Vsps,
			std::string Num, std::string NCand,
			std::string Txt, std::string V_prop, int Value,
			int Size,
			int c1,int c2,int c3,int c4,int c5)
{  // Rep_cntX  -  ����� ������
    Rep_st[Rep_cntX].Typ=Typ;
    Rep_st[Rep_cntX].Alg=Alg;
    Rep_st[Rep_cntX].Bold=Bold;
    Rep_st[Rep_cntX].Tab=Tab;
    Rep_st[Rep_cntX].Vsps=Vsps;

    Rep_st[Rep_cntX].Num=Num;
    Rep_st[Rep_cntX].NCand=NCand; // �����  ���������
    Rep_st[Rep_cntX].Txt=Txt;
    Rep_st[Rep_cntX].V_prop=V_prop;
    Rep_st[Rep_cntX].Value=Value;

    Rep_st[Rep_cntX].Size=Size;

    Rep_st[Rep_cntX].Cols[0]=c1;
    Rep_st[Rep_cntX].Cols[1]=c2;
    Rep_st[Rep_cntX].Cols[2]=c3;
    Rep_st[Rep_cntX].Cols[3]=c4;
    Rep_st[Rep_cntX].Cols[4]=c5;
}
//------------------------------------������------------------------------------
bool PrintDoc(int Type, char* sst, char* st5)
// Type- ��� ���������
// sst-���� .tex
// st5-���� .pdf
{

  char st1[128];
  char st2[128];
    char st3[128];
        char st4[128];
  MakeFile(sst); // ������������ tex �����

 // ������������  pdf �����

  strcpy(st1,"pdflatex ");
  strcat(st1,sst);  //strcat(st1,st5);
  strcat(st1," >null");
  /*!!   ��������� � LaTex ��� ������������ ����� *.pdf */
   system(st1);
   system(st1);
  // ������������ ����� *.pdf � �� ����� *.tex

        // ������������ ������� ���  ���������� ����� *.pdf � �����
        strcpy(st1,"");  strcat(st1,"cp ");  strcat(st1,st5);
        strcat(st1," ./Protokols/"); strcat(st1,st5);        strcat(st1," >null");
        system(st1); // ���������� ����� � ����� Protokols
        // ������������  �������  ��  �������� ����� *.pdf   ��  �������� ��������
        strcpy(st1,"rm -f "); strcat(st1,st5); // �������� ����� ��  ������� ����������
        system(st1); // ��������  ����� �� �������� ��������
        //   ��������  ���������������  ������ �� ������� ����������
       int i=strlen(st5)-3;      strncpy(st2,st5,i); strcpy(st4,st2);
        //  ��������  ���������������� �����  latex
        strcpy(st3,"aux");    strcat(st2,st3);       strcpy(st1,"rm -f "); strcat(st1,st2);
        system(st1); // ��������  ���������������� ����� LaTex �� �������� ��������
        //  ��������  log - �����  latex
        strcpy(st3,"log");    strcat(st4,st3);    strcpy(st1,"rm -f "); strcat(st1,st4);
        system(st1); // ��������   log - �����  latex �� �������� ��������
        //    �����  ����� - ���������  ��  �������
        strcpy(st2,"lp ");   strcat(st2," ./Protokols/"); strcat(st2,st5);
        //system(st2);
   return 1;
}
//------------------------------------������������ ����������---------------------------
bool Print(int Type)
{
  ofstream F;
  char sst[128];
  std::string Printer;
  std::string st;
  std::string F1;
  std::string F2;
  std::string F3;
  std::string F4;

  char st5[128];

  Rep_cnt=0; //  �������  �������  �����
  std::string Font=Votes.Blank[1].Vopr[1].Protocoles[1].Font;
  int FontSize=Votes.Blank[1].Vopr[1].Protocoles[1].Fontsize;
  if (FontSize==0) {FontSize=8;}

  //Page Footer
  std::string PF1,PF2,PF3,PF4,PF22,PF222,PF5;
  PF1="������������ ���������� ������������� ��������\0";
  PF2= " (�������, ��������)\0";
  PF22= "( ������� ���� ������� ����������,\0";
  PF222="      ������� �� ������ ������)\0";
  PF3="����c������ ������������ ��������\0";
  PF4="��������� ��������\0";

	switch (Type)
	{
	case 1: //--------------------------------------------------�������� ������
	  {
		  F.open("./Log/Protokol_ID.log", ios::out); //Log-����
		  //1
		  Printer=Out.PrID[1]+" "+IntToStr(Votes.Nuik); //�������� ������ ��� ��� N
		  SumRep(Rep_cnt, 1,2,2,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  //Header, Centr, Bold
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //2
		  Printer=Out.PrID[2]+ Koib[1].N;  if(Koib[2].N!="")Printer=Printer +", "+Koib[2].N;
		  //������ ������
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //3
		  Printer=""; //����� �������:
		  if (Koib[1].Tren)
		       {Printer+=Out.PrID[27];} else //����������
		       {Printer+=Out.PrID[28];} //�����������
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //4
		  Printer=Out.PrID[4]+" "+Koib[1].N; //������ N:
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //5
		  Printer=Out.PrID[5]+" "+DateToStringOrVoice(Votes.DByear,Votes.DBmonth,Votes.DBday,true);
		  //���� �����������:
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //6
		  Printer=Out.PrID[6]+" "+IntToStr(Votes.Nuik); //������ ������������� ��� ��� N
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //7
		  std::string stCom;
		  stCom=Votes.Blank[1].Vopr[1].PC.parentid;
		  if(stCom=="")stCom=Out.PrID[29];// ���
         Printer=Out.PrID[7]+stCom; //!!!??? ������ ����������� ��������:
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //8
		  Printer=Out.PrID[8]+" "+IntToStr(Votes.Nblank);//���������� ����� ����������:
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //9
		  Printer=Out.PrID[9]; //������������������ ����� �������:
		  if (Votes.Nmodes>0)
		  {
			  for (int i = 1; i <= Votes.Nmodes; i++)
			  {
				if (Votes.Modes[i]=="Main") {Printer=Printer+Out.PrID[35]+", ";} //������������
				if (Votes.Modes[i]=="Portable") {Printer=Printer+Out.PrID[36];} //����������
			  }
		  }
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //10,11
		  if (! Votes.Unlim)
		  {
			//10
			if (Votes.TimeB[1]!="")
			{
			  Printer=Out.PrID[10]+" "+Votes.TimeB[1]; //����� ������ ������������� ������:
			  SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
			  Rep_cnt++;
			  F<<Printer<<'\n';
			}
			//11
			if (Votes.TimeB[2]!="")
			{
			  Printer=Out.PrID[11]+" "+Votes.TimeB[2]; //����� ������ ����������� ������:
			  SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
			  Rep_cnt++;
			  F<<Printer<<'\n';
			}
		  }
		  //������!
	   if (Votes.Nblank>0)
	   {
		  //12
		  Printer=Out.PrID[12]; //���������!
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  for (int i = 1; i <= Votes.Nblank; i++)
		  {
			//13
			 std::string nums; nums=IntToStr(i);
			Printer=Out.PrID[13]+" "+IntToStr(i); //��������� N
			SumRep(Rep_cnt, 3,1,1,1,1,"","",Printer,"",0, FontSize,1,0,0,0,0);
			Rep_cnt++;
			F<<Printer<<'\n';
			//14
			if (Votes.Blank[i].Refer)
			{
			  Printer=Out.PrID[14]+" "+Votes.Blank[i].Vopr[1].Name; //����������:
			} else {
			  Printer=Out.PrID[16]+" "+Votes.Blank[i].Vopr[1].Name; //������:
			}
			SumRep(Rep_cnt, 3,1,1,1,1,"","",Printer,"",0, FontSize,1,0,0,0,0);
			Rep_cnt++;
			F<<Printer<<'\n';
		  }

		  //15
		  Printer=Out.PrID[19]; //������!
		  SumRep(Rep_cnt, 3,1,1,0,2,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';

		  for (int i = 1; i <= Votes.Nblank; i++)
		  {
		  //16...
		  Printer=IntToStr(i)+".  "+Votes.Blank[i].Vopr[1].Name;
		  //������ N
		  SumRep(Rep_cnt, 3,1,2,0,2,"1. ","",Printer,"",0, FontSize,1,0,0,0,0);
		  //Bold
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //17
		  Printer=Out.PrID[22]+" "; //������ �����������:
		  if (Votes.Blank[i].Vopr[1].Nmodes>0)
		  {
			  for (int j = 1; j <= Votes.Blank[i].Vopr[1].Nmodes; j++)
			  {
			    if (Votes.Blank[i].Vopr[1].Modes[j]=="Main") {Printer=Printer+Out.PrID[35]+", ";} //������������
				if (Votes.Blank[i].Vopr[1].Modes[j]=="Portable") {Printer=Printer+Out.PrID[36];} //����������
			  }
		  }
		  SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //18
		  Printer=Out.PrID[23]+" "+IntToStr(Votes.Blank[i].Vopr[1].maxMarks);
		  //����� ��������:
		  SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //19
		  Printer=Out.PrID[25]+" "+IntToStr(Votes.Blank[i].Vopr[1].NCandidates);
		  //����� �������:
		  SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //20
		  Printer=Out.PrID[26]; //�������:
		  SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
			  if (Votes.Blank[i].Vopr[1].NCandidates>0)
			  {
				for (int j=1; j<=Votes.Blank[i].Vopr[1].NCandidates; j++)
				{
				//21...
				Printer=IntToStr(j)+". ";
				  if (Votes.Blank[i].Vopr[1].Candidates[j].lastName !="")
				  {
					Printer=Printer+Votes.Blank[i].Vopr[1].Candidates[j].lastName+" ";
				  }
				  if (Votes.Blank[i].Vopr[1].Candidates[j].firstName !="")
				  {
					Printer=Printer+Votes.Blank[i].Vopr[1].Candidates[j].firstName+" ";
				  }
				  if (Votes.Blank[i].Vopr[1].Candidates[j].patronymic !="")
				  {
					Printer=Printer+Votes.Blank[i].Vopr[1].Candidates[j].patronymic;
				  }
				  if (Votes.Blank[i].Vopr[1].Candidates[j].disabled)
				  {
				    Printer=Printer+" "+Out.PrID[39]; //������� �����
				  }

				 SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
				 Rep_cnt++;
				 F<<Printer<<'\n';

			    }
			  }

                  if (Votes.Blank[i].Vopr[1].NProtocoles>0)
                  {
				          //22...
				          Printer=Out.PrID[31]; //���������
                          //SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		                  //Rep_cnt++;
		                  F<<Printer<<'\n';

                   for (int j=1; j<=Votes.Blank[i].Vopr[1].NProtocoles; j++)
				   {
				     if (Votes.Blank[i].Vopr[1].Protocoles[j].Enable)
				     {
				           //23...
				           Printer=Out.PrID[32]+" "+Votes.Blank[i].Vopr[1].Protocoles[j].Name;
                           //������������ ���������:
                           SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
                           Rep_cnt++;
		                   F<<Printer<<'\n';
				           //24...
				           Printer=Out.PrID[24]; //������ ������� �� ���������� � �����
                           SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
                           Rep_cnt++;
		                   F<<Printer<<'\n';

                          if (Votes.Blank[i].Vopr[1].Protocoles[j].NLines>0)
                          {
				           //25
				           Printer=Out.PrID[33]; //������ ���������:
                           SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
                           Rep_cnt++;
		                   F<<Printer<<'\n';

                           for (int jj=1; jj<=Votes.Blank[i].Vopr[1].Protocoles[j].NLines; jj++)
				           {
                           //26...
                           Printer=IntToStr(jj)+". "+Votes.Blank[i].Vopr[1].Protocoles[j].Lines[jj].Name;
                           SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
                           Rep_cnt++;
		                   F<<Printer<<'\n';
				           }
                          }
                        if (Votes.Blank[i].Vopr[1].Protocoles[j].NChecks>0)
                        {
				          //27
				          Printer=Out.PrID[34]; //����������� �����������:
				          SumRep(Rep_cnt, 3,1,1,1,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
				          Rep_cnt++;
		                  F<<Printer<<'\n';

                          for (int jj=1; jj<=Votes.Blank[i].Vopr[1].Protocoles[j].NChecks; jj++)
				          {
                          //28...
                          Printer=Votes.Blank[i].Vopr[1].Protocoles[j].Checks[jj].expression;
                          SumRep(Rep_cnt, 3,1,1,2,0,"","",Printer,"",0, FontSize,1,0,0,0,0);
                          Rep_cnt++;
		                  F<<Printer<<'\n';
				          }
                        }
                    }
				 }
              }
		   }
		}
  //PF2= " (�������, ��������)\0";
 // PF22="( ������� ���� ������� ����������, ������� �� ������ ������)\0";
 //  PF3="���������� ������������ ��������\0";
 // PF4="��������� ��������\0";
 FontSize=8;
		    // Page Footer �������
/*  SumRep(Rep_cnt, 4,1,0,0,0,"","","","",0, FontSize, 1,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","",PF1,"",0, FontSize, 1,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,3,0,0,0,"","", "_____________________   _________________________________\0","",0,2, 1,0,0,0,0);  Rep_cnt++;
   SumRep(Rep_cnt, 4,3,0,0,0,"","","  �������, ��������      ������� ���� ������� ���������� \0,","",0,4, 1,0,0,0,0);  Rep_cnt++;
                               SumRep(Rep_cnt, 4,3,0,0,0,"","","������� �� ������ ������     \0","",0,4, 1,0,0,0,0);  Rep_cnt++;

  SumRep(Rep_cnt, 4,1,0,0,0,"","",PF3,"",0, FontSize, 1,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,3,0,0,0,"","", "_____________________   _________________________________\0","",0,2, 1,0,0,0,0);  Rep_cnt++;


  SumRep(Rep_cnt, 4,1,0,0,0,"","",PF4,"",0, FontSize, 1,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,3,0,0,0,"","", "_____________________   _________________________________\0","",0,2, 1,0,0,0,0);  Rep_cnt++;
*/
  //SumRep(Rep_cnt, 4,1,0,0,5,"","","","",0, FontSize, 1,0,0,0,0);  Rep_cnt++;
  //SumRep(Rep_cnt, 3,1,0,0,5,"","","","",1, FontSize, 0,0,0,0,0);  Rep_cnt++;
  //SumRep(Rep_cnt, 4,1,0,0,0,"","","\\vspace{10mm}","",0, FontSize, 0,0,0,0,0);  Rep_cnt++;
  //End Page Footer �������


    // Page Footer  longtable
  SumRep(Rep_cnt, 4,1,0,0,0,"","","","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF1,"","","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"", "____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,2,0,0,0,"",PF2,PF22,"",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,2,0,0,3,"","",PF222,"",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,3,PF3,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF4,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","","","",0, FontSize, 1,0,0,0,0);  Rep_cnt++;


    //SumRep(Rep_cnt, 3,1,0,0,5,"","","","",1, FontSize, 0,0,0,0,0);  Rep_cnt++;
  //SumRep(Rep_cnt, 4,1,0,0,0,"","","\\vspace{10mm}","",0, FontSize, 0,0,0,0,0);  Rep_cnt++;
  //End Page Footer longtable

	F.close();
    strcpy(sst,"./Tex/Pr_ID.tex");
    strcpy(st5,"Pr_ID.pdf");

    PrintDoc(1, sst, st5);

	} break; // End �������� ������

case 2: //---------------------------------------------------------�������� ������������
	{
		 F.open("./Log/Protokol_Test.log", ios::out); //Log-����

		  //1
		  Printer=Out.PrTest[1]; //���������� ������������ �������(��)
		  SumRep(Rep_cnt, 3,1,1,0,4,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //2
		  TekDateTime();
		  Printer=Out.PrTest[2]+" "+DateS+" "+TimeS;
		  //������� ���� � �����:
		  SumRep(Rep_cnt, 3,1,1,0,4,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //4
		  //---  Ns1   Ns2

         if (!Koib[1].Main)
		  {
		    F1=Out.PrTest[4]+Koib[1].N;
		    F2=Out.PrTest[5]+Koib[2].N;
		    F3=Out.PrTest[6];
		    SumRep(Rep_cnt, 2,1,1,0,0,"",F1,F2,F3,0, FontSize,75,30,30,0,20);
	  	    Rep_cnt++;
		    F<<Printer<<'\n';
            F1="("+Koib[1].IP+")";
            F2="("+Koib[2].IP+")";
		    SumRep(Rep_cnt, 2,1,1,0,10,"",F1,F2,"",0, FontSize,75,30,30,0,20);
	  	    Rep_cnt++;
		    F<<Printer<<'\n';

		  } else {
		    F1=Out.PrTest[4]+Koib[1].N;
		    F2=Out.PrTest[5]+Out.PrTest[7];
            F3=Out.PrTest[6];
		    SumRep(Rep_cnt, 2,1,1,0,0,"",F1,F2,F3,0, FontSize,75,30,30,0,20);
		    Rep_cnt++;
		    F<<Printer<<'\n';
            F1="("+Koib[1].IP+")";
            F2="()";
		    SumRep(Rep_cnt, 2,1,1,0,10,"",F1,F2,"",0, FontSize,75,30,30,0,20);
	  	    Rep_cnt++;
		    F<<Printer<<'\n';

		  }
		  //5
		  //�������
		  F1=Out.PrTest[8];
		  F2=IntToStr(Koib[1].Rezult[0].All);
		  F3=IntToStr(Koib[2].Rezult[0].All);
		  F4=IntToStr(Koib[1].Rezult[0].All+Koib[2].Rezult[0].All);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		   //����������������
		  F1=Out.PrTest[11];
		  F2=IntToStr(Koib[1].Rezult[0].NoValid);
		  F3=IntToStr(Koib[2].Rezult[0].NoValid);
		  F4=IntToStr(Koib[1].Rezult[0].NoValid+Koib[2].Rezult[0].NoValid);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0,FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';

		  //11
		  //��� �����
          F1="    "+Out.PrTest[13];
          F2=IntToStr(Koib[1].Rezult[0].NoMarks);
          F3=IntToStr(Koib[2].Rezult[0].NoMarks);
          F4=IntToStr(Koib[1].Rezult[0].NoMarks+Koib[2].Rezult[0].NoMarks);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);

		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //12
		  //���������� ����� �����
		  F1="    "+Out.PrTest[14];
		  F2=IntToStr(Koib[1].Rezult[0].ManyMarks);
		  F3=IntToStr(Koib[2].Rezult[0].ManyMarks);
		  F4=IntToStr(Koib[1].Rezult[0].ManyMarks+Koib[2].Rezult[0].ManyMarks);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //12
		  //��������������
		  F1=Out.PrTest[15];
		  F2=IntToStr(Koib[1].Rezult[0].Valid);
		  F3=IntToStr(Koib[2].Rezult[0].Valid);
		  F4=IntToStr(Koib[1].Rezult[0].Valid+Koib[2].Rezult[0].Valid);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';

	  if (Votes.Nblank>0)
	  {
		  for (int i = 1; i <= Votes.Nblank; i++)
		  {
		  //13
		  Printer=Votes.Blank[i].Vopr[1].Name; //������ �
		  SumRep(Rep_cnt, 3,1,2,0,4,"","",Printer,"",0, FontSize,1,0,0,0,0);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //14
		  //�������
		  F1=Out.PrTest[16];
		  F2=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].All);
        F3=IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].All);
		  F4=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].All+Koib[2].Rezult[0].Bl[i].Vop[1].All);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //16

		  //����������������
		  F1=Out.PrTest[18];
		  F2=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoValid);
		  F3=IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].NoValid);
		  F4=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoValid+Koib[2].Rezult[0].Bl[i].Vop[1].NoValid);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0,FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';

		  //19
		  //��� �����
		  F1="    "+Out.PrTest[20];
		  F2=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoMarks);
		  F3=IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].NoMarks);
		  F4=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoMarks+Koib[2].Rezult[0].Bl[i].Vop[1].NoMarks);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //20
		  //���������� ����� �����
		  F1="    "+Out.PrTest[21];
		  F2=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].ManyMarks);
		  F3=IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].ManyMarks);
		  F4=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].ManyMarks+Koib[2].Rezult[0].Bl[i].Vop[1].ManyMarks);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0,FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //��������������
		  F1=Out.PrTest[15];
		  F2=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Valid);
		  F3=IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].Valid);
		  F4=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Valid+Koib[2].Rezult[0].Bl[i].Vop[1].Valid);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);
		  Rep_cnt++;
		  F<<Printer<<'\n';

			if (Votes.Blank[i].Vopr[1].NCandidates>0)
			  {
				for (int j = 1; j <=Votes.Blank[i].Vopr[1].NCandidates; j++)
				{
				//21...
		  F1=IntToStr(j)+". "+Out.PrTest[22]+" "+Votes.Blank[i].Vopr[1].Candidates[j].firstName+
		                                         Votes.Blank[i].Vopr[1].Candidates[j].lastName+
		                                         Votes.Blank[i].Vopr[1].Candidates[j].patronymic;
		  F2=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Can[j]);
		  F3=IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].Can[j]);
		  F4=IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Can[j]+Koib[2].Rezult[0].Bl[i].Vop[1].Can[j]);
		  SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,-75,-30,-30,0,20);
				Rep_cnt++;
                F<<Printer<<'\n';
				}
			  }

		  }
	   }
   //Begin Page Footer
  SumRep(Rep_cnt, 4,1,0,0,0,"","","","",1, FontSize, 70,32,50,-20,-15);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF1,"","","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"", "____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,2,0,0,5,"",PF2,PF22,"",1, 4, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,5,PF3,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF4,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,5,"","","","",0, FontSize, 1,0,0,0,0);  Rep_cnt++;
  //End Page Footer
	  F.close();
	  strcpy(sst,"./Tex/Pr_Test.tex");
	  strcpy(st5,"Pr_Test.pdf");

	  PrintDoc(2, sst, st5);

	} break; // End �������� ������������

	case 3: //---------------------------------------------���������� ����������� �� ������� XVote
	{
            st="./Log/Protokol_VR"+IntToStr(XVote)+".log";
            F.open(st.c_str(), ios::out); //Log-����

           //1
            Printer=Out.PrRezult[1]; // ���������� �����������
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            //Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //2
            Printer=Out.PrRezult[2]+IntToStr(XVote)+": "+Votes.Blank[XVote].Vopr[XVopr].Name;
            //������ N
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            Rep_cnt++;
            F<<Printer<<'\n';
            //3
            TekDateTime();
            Printer=DateToString(TekDT.year,TekDT.month,TekDT.day,Lang); //����
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            Rep_cnt++;
            F<<Printer<<'\n';
            //4
            Printer=Out.PrRezult[4]+IntToStr(Votes.Nuik); //������������� ������� N
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            Rep_cnt++;
            F<<Printer<<'\n';
            //5...
            if (Votes.Blank[XVote].Vopr[1].NCandidates>0)
            {
                for (int i=1; i<=Votes.Blank[XVote].Vopr[1].NCandidates; i++)
                {
                    F1=IntToStr(i);
                    F2=     Votes.Blank[XVote].Vopr[1].Candidates[i].firstName+" "+
                            Votes.Blank[XVote].Vopr[1].Candidates[i].lastName+" "+
                            Votes.Blank[XVote].Vopr[1].Candidates[i].patronymic;
                    int sum=(Koib[1].Rezult[1].Bl[XVote].Vop[1].Can[i]+
                            Koib[2].Rezult[1].Bl[XVote].Vop[1].Can[i]+
                            Koib[1].Rezult[2].Bl[XVote].Vop[1].Can[i]+
                            Koib[2].Rezult[2].Bl[XVote].Vop[1].Can[i]); //???

                    F3=IntToStr(sum);
                    F4=" ("+CisloToStringOrVoice(sum,true) +")";
                    SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,4,60,5,0,40);
                    Rep_cnt++;
                    F<<Printer<<'\n';
                }

                    F1="";
                    F2=Out.PrRezult[5]; //����� ������� ����������
                    int sum=Koib[1].Rezult[1].Bl[XVote].Vop[XVopr].All+
                            Koib[2].Rezult[1].Bl[XVote].Vop[XVopr].All+
                            Koib[1].Rezult[2].Bl[XVote].Vop[XVopr].All+
                            Koib[2].Rezult[2].Bl[XVote].Vop[XVopr].All;
                    F3=IntToStr(sum);
                    F4=" ("+CisloToStringOrVoice(sum,true) +")";
                    SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,4,60,5,0,40);
                    Rep_cnt++;
                    F<<Printer<<'\n';

                    F1="";
                    F2=Out.PrRezult[6]; //����� �������������� ����������
                    sum=Koib[1].Rezult[1].Bl[XVote].Vop[XVopr].Valid+
                            Koib[2].Rezult[1].Bl[XVote].Vop[XVopr].Valid+
                            Koib[1].Rezult[2].Bl[XVote].Vop[XVopr].Valid+
                            Koib[2].Rezult[2].Bl[XVote].Vop[XVopr].Valid;
                    F3=IntToStr(sum);
                    F4=" ("+CisloToStringOrVoice(sum,true) +")";
                    SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,4,60,5,0,40);
                    Rep_cnt++;
                    F<<Printer<<'\n';
                    F1="";
                    F2=Out.PrRezult[7]; //����� ���������������� ����������
                    sum=Koib[1].Rezult[1].Bl[XVote].Vop[XVopr].NoValid+
                            Koib[2].Rezult[1].Bl[XVote].Vop[XVopr].NoValid+
                            Koib[1].Rezult[2].Bl[XVote].Vop[XVopr].NoValid+
                            Koib[2].Rezult[2].Bl[XVote].Vop[XVopr].NoValid;
                    F3=IntToStr(sum);
                    F4=" ("+CisloToStringOrVoice(sum,true) +")";
                    SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,4,60,5,0,40);
                    Rep_cnt++;
                    F<<Printer<<'\n';
                    F1="";
                    F2=Out.PrRezult[8]; //���������� � ������������ ������
                    sum=Koib[1].Rezult[1].Bl[XVote].Vop[XVopr].All+
                            Koib[2].Rezult[1].Bl[XVote].Vop[XVopr].All;
                    F3=IntToStr(sum);
                    F4=" ("+CisloToStringOrVoice(sum,true) +")";
                    SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,4,60,5,0,40);
                    Rep_cnt++;
                    F<<Printer<<'\n';

                    F1="";
                    F2=Out.PrRezult[9]; //���������� � ���������� ������
                    sum=Koib[1].Rezult[2].Bl[XVote].Vop[XVopr].All+
                            Koib[2].Rezult[2].Bl[XVote].Vop[XVopr].All;
                    F3=IntToStr(sum);
                    F4=" ("+CisloToStringOrVoice(sum,true) +")";
                    SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,F4, 0, FontSize,4,60,5,0,40);
                    Rep_cnt++;
                    F<<Printer<<'\n';

            }
  //Begin Page Footer
  SumRep(Rep_cnt, 4,1,0,0,0,"","","","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF1,"","","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"", "____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,2,0,0,5,"",PF2,PF22,"",1, 4, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,5,PF3,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,5,PF4,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
    //SumRep(Rep_cnt, 4,0,0,0,5,"","","","",0, FontSize, 0,0,0,0,0);  Rep_cnt++;
    SumRep(Rep_cnt, 4,1,0,0,5,"","","","",0, FontSize, 1,0,0,0,0);  Rep_cnt++;

  PF3="��     ��������� \"___\"__________20___���� � ____ ����� ____�����\0";
  PF4="     �����������:\"___\"__________20___���� � ____ ����� ____�����\0";

  SumRep(Rep_cnt, 4,1,1,0,2,"","","","",0, FontSize, 0,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,1,0,2,"","","","",0, 8,0,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,1,0,2,"","",PF3,"",0, FontSize, 0,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,1,0,2,"","",PF4,"",0, FontSize, 0,0,0,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,5,"","","","",0, FontSize, 0,0,0,0,0);  Rep_cnt++;




  //End Page Footer

            F.close();
	        st="./Tex/Pr_Result"+IntToStr(XVote)+".tex"; strcpy(sst,st.c_str());
	        st="Pr_Result"+IntToStr(XVote)+".pdf";       strcpy(st5,st.c_str());

	        PrintDoc(3, sst, st5);

    } break; // End �������� � ������������ �����������

	case 4://----------------------------------------------------------------�� ���������� ��
	{
           F.open("./Log/Protokol_KS.log", ios::out); //Log-����

            //1
            Printer=Out.PrCh[1]; // ������������� ����������� �����������
            SumRep(Rep_cnt,1,2,1,0,10, "","",Printer,"", 0, FontSize,1,0,0,0,0); //Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //2
            TekDateTime();
            Printer=Out.PrCh[2]+" "+DateS+" "+TimeS; // ������� ���� � �����:
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);//Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //3
            Printer=Votes.Blank[XVote].Name; // ��� �������
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);//Centr
            Rep_cnt++;
            F<<Printer<<'\n';

            if (Votes.Blank[XVote].Vopr[1].NProtocoles>0)
            {
              for (int i=1; i<=Votes.Blank[XVote].Vopr[1].NProtocoles; i++)
              {
                if (Votes.Blank[XVote].Vopr[1].Protocoles[i].Text.final)
                //�������� ��������
                {
            //4
            Printer=Out.PrCh[3]; //������ ���������:
            SumRep(Rep_cnt,3,1,1,0,4, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            Rep_cnt++;
            F<<Printer<<'\n';

                   if (Votes.Blank[XVote].Vopr[1].Protocoles[i].NLines>0)
                   {
                      for (int j=1; j<=Votes.Blank[XVote].Vopr[1].Protocoles[i].NLines; j++)
                      {
                        //5...
                        F1=IntToStr(Votes.Blank[XVote].Vopr[1].Protocoles[i].Lines[j].N);
                        //???
                        F2=Votes.Blank[XVote].Vopr[1].Protocoles[i].Lines[j].Name;
                        F3=IntToStr(Votes.Blank[XVote].Vopr[1].Protocoles[i].Lines[j].Value);
                        SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,"", 0, FontSize,10,60,20,0,0);
                        Rep_cnt++;
                        F<<Printer<<'\n';
                      }
                   }
            //6
            Printer=Out.PrCh[4]; //����������� �����������:
            SumRep(Rep_cnt,3,1,1,0,4, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            Rep_cnt++;
            F<<Printer<<'\n';
                   if (Votes.Blank[XVote].Vopr[1].Protocoles[i].NChecks>0)
                   {
                      for (int j=1; j<=Votes.Blank[XVote].Vopr[1].Protocoles[i].NChecks; j++)
                      {
                        //7...
                        F1=IntToStr(j);
                        F2=Votes.Blank[XVote].Vopr[1].Protocoles[i].Checks[j].expression;
                        if (Votes.Blank[XVote].Vopr[1].Protocoles[i].Checks[j].Itog)
                        {
                            F3=Printer=Out.PrCh[5]; //���������
                        } else {
                            F3=Printer=Out.PrCh[6]+  //�� ��������� + ���������
                            Votes.Blank[XVote].Vopr[1].Protocoles[i].Checks[j].ItogExpression;
                        }
                        SumRep(Rep_cnt,2,1,1,0,2, F1,F2,F3,"", 0, FontSize,10,40,40,0,0);
                        Rep_cnt++;
                        F<<Printer<<'\n';

            //8
            Printer=Out.PrCh[7]+IntToStr(Votes.Blank[XVote].Vopr[1].Protocoles[i].S);
            // S(����� �������, �������� �� ��� ������������)=
            SumRep(Rep_cnt,3,1,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            Rep_cnt++;
            F<<Printer<<'\n';

            //9
            Printer=Out.PrCh[8]+IntToStr(Votes.Blank[XVote].Vopr[1].Protocoles[i].M);
            // M(���������� ��������)=
            SumRep(Rep_cnt,3,1,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);
            Rep_cnt++;
            F<<Printer<<'\n';

                      }
                   }
                }
              }
            }

            F.close();
            strcpy(sst,"./Tex/Pr_Rezult_KS.tex");
            strcpy(st5,"Pr_Rezult_KS.pdf");

            PrintDoc(4, sst, st5);

	} break; // End �� ���������� ��
	case 5: //-------------------------------------------------------------�������� ��������
	{
            st="./Log/Protokol_Itog"+IntToStr(XVote)+".log";
            F.open(st.c_str(), ios::out); //Log-����

            if (Votes.Blank[XVote].Vopr[XVopr].NProtocoles>0)
            {
              for (int i=1; i<=Votes.Blank[XVote].Vopr[XVopr].NProtocoles; i++)
              {
 	               if (Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Enable)
	               {
		             std::string Font=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Font;

		             int NumW=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].numberWidth;
		             int NameW=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].nameWidth;
                     int ValueW=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].valueWidth;
		             int ValueTextW=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].valueWidth;

		             //��� ����� �������!!!
		    //1
            Printer=Votes.Blank[XVote].Vopr[XVopr].Name; // ������������ �����������
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);//Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //2
            TekDateTime();
            Printer=Out.PrCh[2]+" "+DateS; // ������� ����:
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);//Centr
            Rep_cnt++;
            F<<Printer<<'\n';
		    //3
            Printer=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Name; // ������������ ���������
            SumRep(Rep_cnt,1,2,1,0,2, "","",Printer,"", 0, FontSize,1,0,0,0,0);//Centr
            Rep_cnt++;
            F<<Printer<<'\n';

		             int NPlines=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.NProtocolLine;
                     if (NPlines>0)
                     {
                        for (int j=1; j<=NPlines; j++)
                        {
                        //N protokol lines

                        st=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.ProtocolLine[j].text;

                        TekDateTime();
                        if (StrVstr(st,"{ElectionName}",1))
                        {
                           st=FiltrStr(st,"{ElectionName}",Votes.ElName);
                        }
                        if (StrVstr(st,"{VotingDate}",1))
                        {
                            st=FiltrStr(st,"{VotingDate}",DateToStringOrVoice(TekDT.year,TekDT.month,TekDT.day, true));
                        }
                        if (StrVstr(st,"{UIK}",1))
                        {
                            st=FiltrStr(st,"{UIK}"," "+IntToStr(Votes.Nuik)+" ");
                        }

                        int fs=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.ProtocolLine[j].fontSize;
                        if (fs==0) {fs=8;}
                        bool it=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.ProtocolLine[j].italic;
                        bool bl=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.ProtocolLine[j].bold;
                        std::string sec=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.ProtocolLine[j].section;
                        std::string al=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.ProtocolLine[j].align;

                        int Alg=1;
                        if ((al=="Center"))
                        {
                           Alg=2;
                        }
                        if ((al=="Right"))
                        {
                           Alg=3;
                        }

                        int Typ=0;
                        if ((sec=="PageHeader")||(sec=="Header"))
                        {
                           Typ=1;
                        }
                        if (sec=="Body")
                        {
                           Typ=2;
                        }
                        if (sec=="Footer")
                        {
                           Typ=3;
                        }
                        if (sec=="PageFooter")
                        {
                           Typ=4;
                        }

                        int Bold=1;
                        if (bl)
                        {
                          Bold=2;
                        }
                        if (it)
                        {
                          Bold=3;
                        }
                        // � ���� � bold � Italic???

                        Printer=st;
                        SumRep(Rep_cnt,Typ,Alg,Bold,0,2,"","",Printer,"",0, fs,1,0,0,0,0);
                        Rep_cnt++;
                        F<<Printer<<'\n';

                        }
                     }
                    // End ��� ����� �������!!!

                    //� ������ �������!!!
		             int Nlines=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].NLines;

                     if (Nlines>0)
                     {
                        for (int j=1; j<=Nlines; j++)
                        {
                        st=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Lines[j].Name;
                        std::string NumSt=IntToStr(Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Lines[j].N)+
                                                   Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Lines[j].AddNum;
                        int Value=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Lines[j].Value;
                        std::string ValueSt=IntToStr(Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Lines[j].Value);
                        std::string ValuePropSt="("+CisloToStringOrVoice(Value,true)+")";

                        int fs=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.VoteLine[j].fontSize;
                        if (fs==0) {fs=8;}
                        bool it=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.VoteLine[j].italic;
                        bool bl=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.VoteLine[j].bold;
                        std::string type=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Text.VoteLine[j].type;

                        int Typ=2;
                        int Alg=1;
                        int Bold=1;
                        if (bl)
                        {
                          Bold=2;
                        }
                        if (it)
                        {
                          Bold=3;
                        }
                        // � ���� � bold � Italic???

                        Printer=st;
             SumRep(Rep_cnt,Typ,Alg,Bold,0,2,NumSt,"",Printer,ValuePropSt,Value, fs,
             NumW,0,NameW,ValueW,ValueTextW);
                        Rep_cnt++;
                        F<<Printer<<'\n';

                        }
                     }
                     //End � ������ �������!!!


 std::string PF1,PF2,PF3,PF4,PF22,PF5;
  PF1="������������ ���������� ������������� ��������\0";
  PF2= " (�������, ��������)\0";
  PF22="( ������� ���� ������� ����������, ������� �� ������ ������)\0";
  PF3="���������� ������������ ��������\0";
  PF4="��������� ��������\0";
  PF5="�����  ��������\0";
  SumRep(Rep_cnt, 4,1,0,0,0,"","","","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF1,"","","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"", "____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,2,0,0,0,"",PF2,PF22,"",1, 4, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF3,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF4,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,PF5,"____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,0,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
  SumRep(Rep_cnt, 4,1,0,0,10,"","____________________\0","________________________________\0","",1, FontSize, 70,32,50,0,0);  Rep_cnt++;
    //SumRep(Rep_cnt, 3,1,0,0,5,"","","","",1, FontSize, 0,0,0,0,0);  Rep_cnt++;
  //End Page Footer
  PF1="";
  PF3="��     ��������� \"___\"__________20___���� � ____ ����� ____�����\0";


  SumRep(Rep_cnt, 4,1,1,0,2,"","",PF1,"",0, FontSize, 1,0,0,0,0);  Rep_cnt++;

  SumRep(Rep_cnt, 4,1,1,0,2,"","",PF3,"",0, FontSize, 1,0,0,0,0);  Rep_cnt++;
                    F.close();
                    st="./Tex/Pr_Itog"+IntToStr(XVote)+".tex";   strcpy(sst,st.c_str());
                    st="Pr_Itog"+IntToStr(XVote)+".pdf";         strcpy(st5,st.c_str());

                    PrintDoc(5, sst, st5);
	              }
              }
		  }
	} break; // End �������� ��������

 } //End Switch

 return 0;
}

//------------------------------------�������� N ������� �������----------------------------------
void SayNSys(std::string st1)
{
    std::string st="aplay   ";
    st=st+st1+">/dev/null";
    system(st.c_str());
}

int SayN(std::string st1)
{
    std::string st="aplay";
    pid_t pid = fork();
    switch(pid)
    { case  0: execlp(&st[0],&st[0],&st1[0],NULL); exit(1); break;
      case -1: cout << "error!" << endl; break;
    }
return 0;
}
//------------------------------------�������� ������� �������----------------------------------
void Say(int Type)
{  // ��������� ��������� �  ���� ���. ���������
    if (Type>0)
    {  // �������� ��������� �� ����� ����������� ��������� �������� ���������
            SayN(OutV.MainPath[Type]);
    }
}

void SaySys(int Type)
{  // ��������� ��������� �  ���� ���. ���������
    if (Type>0)
    {  // �������� ��������� �� ����� ����������� ��������� �������� ���������
            SayNSys(OutV.MainPath[Type]);
    }
}

//-----------------------------��������� ���� ��� ������� �������---------------------------------
bool Speak(int Type)
{
	ofstream F;
	// std::string Golos;  // ������� ��������� ��� ����� �����
	switch (Type)
	{
	  case 1: //---------------------------------------------�������� ������
	  { // �����������  �������� ������
        F.open("./Log/Protokol_Voice_ID.log", ios::out); //Log-����

        Golos=OutV.IDPath[1]; //�������� ������
        SayNSys(Golos);
        F<<Golos<<'\n';

        if (Votes.Nuik>0)
        {
            Golos=OutV.IDPath[2]+" "+CisloToStringOrVoice(Votes.Nuik,false) ; //����� ���
            SayNSys(Golos);
            F<<Golos<<'\n';
        }

        if (Votes.DateV !="")
        {
            Golos=OutV.IDPath[3];//+" "+DateToStringOrVoice(Votes.DByear,Votes.DBmonth,Votes.DBday,false);
            //���� �������
            Golos=  Golos+" "+ SpeakElectionDate();
            //st1+" "+st2+" "+st3+" "+st4+" "+st5+" "+OutV.MonthsPath[13]; // ����
            SayNSys(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.Nblank>0)
        {
            Golos=OutV.IDPath[4]+" "+CisloToStringOrVoice(Votes.Nblank,false); //���������� �������
            SayNSys(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.TimeB[1] !="")
        {
            Golos=OutV.IDPath[5]+" "+TimeToStringOrVoice(Votes.Bhour[1],true,Votes.Bmin[1],true,Votes.Bsec[1],false,false);
            //������ �����������
            SayNSys(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.TimeB[2] !="")
        {
            Golos=OutV.IDPath[6]+" "+TimeToStringOrVoice(Votes.Bhour[2],true,Votes.Bmin[2],true,Votes.Bsec[2],false,false);
            //��������� �����������
            SayNSys(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.Nblank>0) //���������
        {
            Golos=OutV.IDPath[7]+" "+CisloToStringOrVoice(Votes.Nblank,false); //���������� ����������
            SayNSys(Golos);
            F<<Golos<<'\n';
            for (int j=1; j<=Votes.Nblank; j++)
            {
                Golos=OutV.IDPath[8]+" "+CisloToStringOrVoice(j,false)+" "; //����� N
                if (Votes.Blank[j].Refer) { Golos+=OutV.IDPath[10]; } // ��� ����������
                                          else { Golos+=OutV.IDPath[9]; } // ��� ������
                SayNSys(Golos);
                F<<Golos<<'\n';

                if (Votes.Blank[j].Refer) //--------------------------����������
                {
                    if (Votes.Blank[j].NVopr>0)
                    {
                      Golos=OutV.IDPath[11]+" "+CisloToStringOrVoice(Votes.Blank[j].NVopr,false);
                      //���������� ��������
                      SayNSys(Golos);
                      F<<Golos<<'\n';
                    }
                } //End ����������
                if (! Votes.Blank[j].Refer) //--------------------------������
                {
                    if (Votes.Blank[j].Vopr[1].NCandidates>0)
                    {
                      Golos=OutV.IDPath[12]+" "+CisloToStringOrVoice(Votes.Blank[j].Vopr[1].NCandidates,false);
                      //���������� ����������
                      SayNSys(Golos);
                      F<<Golos<<'\n';
                    }
                } //End ������
            }
        }//End ���������
	  } break;
	  case 2:  //---------------------------------------------����������� ������ ������� �������
	  {
			F.open("./Log/Protokol_Voice_Snat.log", ios::out); //Log-����
            // �������� �������  ������ �������
             bool S=false;
             for (int i = 1; i <=Votes.Nblank; i++) if (Snat[i].Nsnat>0) { S=true; }

             if (S) //�������!!!
             {  // ����  �����  ������� �� ������ �������
                Golos=OutV.SnatPath[1];//����� ��������� ������� � ��������� ������������
                SayNSys(Golos);
                F<<Golos<<'\n'; //  ������ �  log  ����
                for (int i = 1; i <=Votes.Nblank; i++)
                if (Snat[i].Nsnat>0)
                {  // ����� �������
                    Golos=OutV.SnatPath[2]+" "+CisloToStringOrVoice(i,false); //����������� N
                    SayNSys(Golos);
                    F<<Golos<<'\n';
                    Golos=OutV.SnatPath[3]+" "; //������ ������ �������
                    for (int j = 1; j <=Snat[i].Nsnat; j++)
                    {
                       Golos+=CisloToStringOrVoice(Snat[i].Snat[j],false)+" ";
                    }
                    SayNSys(Golos);
                    F<<Golos<<'\n';
                }
             } else
             { //�� ������� ������� �� ������ �������!!!
                Golos=OutV.SnatPath[4]; //������ �� �����!
                SayNSys(Golos);
                F<<Golos<<'\n';
             }
	  } break;
	  case 3: //---------------------------------------�������� ������������----------------------
	  {
        F.open("./Log/Protokol_Voice_Test.log", ios::out); //Log-����

        Golos=OutV.TestPath[1];//�������� ������������ ����
        SayNSys(Golos);
        F<<Golos<<'\n';

        TekDateTime();
        Golos=OutV.TestPath[2];//+" "+DateToStringOrVoice(TekDT.year,TekDT.month,TekDT.day,false); //����
        Golos=  Golos+" "+ SpeakElectionDate();
        SayNSys(Golos);
        F<<Golos<<'\n';

        //����� ������� ����������
        Golos=OutV.TestPath[3]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].All+Koib[2].Rezult[0].All,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //�� ���
        Golos=OutV.TestPath[4];
        SayNSys(Golos);
        F<<Golos<<'\n';

        //��������������� �����
        Golos=OutV.TestPath[5]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].Nuf+Koib[2].Rezult[0].Nuf,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //����������������
        Golos=OutV.TestPath[6]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].NoValid+Koib[2].Rezult[0].NoValid,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //� ��� �����
        Golos=OutV.TestPath[7];
        SayNSys(Golos);
        F<<Golos<<'\n';

        //��� �������
        Golos=OutV.TestPath[8]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].NoMarks+Koib[2].Rezult[0].NoMarks,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //����� �������
        Golos=OutV.TestPath[9]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].ManyMarks+Koib[2].Rezult[0].ManyMarks,false);
        SayNSys(Golos);
        F<<Golos<<'\n';


     if (Votes.Nblank>0)
     {

        //�������� �� �������
        Golos=OutV.TestPath[10];
        SayNSys(Golos);
        F<<Golos<<'\n';

        for (int i=1; i<=Votes.Nblank; i++)
        {
        //������ �����
        Golos=OutV.TestPath[11]+" "+CisloToStringOrVoice(i,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //����� ������� ����������
        Golos=OutV.TestPath[3]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].Bl[i].Vop[1].All+Koib[2].Rezult[0].Bl[i].Vop[1].All,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //�� ���
        Golos=OutV.TestPath[4];
        SayNSys(Golos);
        F<<Golos<<'\n';

        //��������������� �����
        Golos=OutV.TestPath[5]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].Bl[i].Vop[1].Nuf+Koib[2].Rezult[0].Bl[i].Vop[1].Nuf,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //����������������
        Golos=OutV.TestPath[6]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].Bl[i].Vop[1].NoValid+Koib[2].Rezult[0].Bl[i].Vop[1].NoValid,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //� ��� �����
        Golos=OutV.TestPath[7];
        SayNSys(Golos);
        F<<Golos<<'\n';

        //��� �������
        Golos=OutV.TestPath[8]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].Bl[i].Vop[1].NoMarks+Koib[2].Rezult[0].Bl[i].Vop[1].NoMarks,false);
        SayNSys(Golos);
        F<<Golos<<'\n';

        //����� �������
        Golos=OutV.TestPath[9]+" "+
              CisloToStringOrVoice(Koib[1].Rezult[0].Bl[i].Vop[1].ManyMarks+Koib[2].Rezult[0].Bl[i].Vop[1].ManyMarks,false);
        SayNSys(Golos);
        F<<Golos<<'\n';


           if (Votes.Blank[i].Vopr[1].NCandidates>0)
           {
           //�������� �� ����������
           Golos=OutV.TestPath[12];
           SayNSys(Golos);
           F<<Golos<<'\n';

              for (int j=1; j<=Votes.Blank[i].Vopr[1].NCandidates; j++)
              {
                //�������� �����
                Golos=OutV.TestPath[13]+" "+CisloToStringOrVoice(j,false);
                SayNSys(Golos);
                F<<Golos<<'\n';

                //������ �������
                Golos=OutV.TestPath[14]+" "+
                      CisloToStringOrVoice(Koib[1].Rezult[0].Bl[i].Vop[1].Can[j]+Koib[2].Rezult[0].Bl[i].Vop[1].Can[j],false);
                SayNSys(Golos);
                F<<Golos<<'\n';

              }
           }
        }
     }
   } break;
	  case 4: //-------------------------------------���������� ����������� XVote----------------------
	  {
        F.open("./Log/Protokol_Voice_Rezult.log", ios::out); //Log-����

        Golos=OutV.RezultPath[1]+" "+CisloToStringOrVoice(XVote,false);
        //���������� ����������� �� ������� �����
        F<<Golos<<'\n';
        SayNSys(Golos);

        TekDateTime();
        Golos=OutV.RezultPath[2];//+" "+DateToStringOrVoice(TekDT.year,TekDT.month,TekDT.day,false); //����
        Golos=  Golos+" "+ SpeakElectionDate();
        SayNSys(Golos);
        F<<Golos<<'\n';

        Golos=OutV.RezultPath[3]+" "+CisloToStringOrVoice(Votes.Nuik,false); //������������� ������� �����
        SayNSys(Golos);
        F<<Golos<<'\n';

           if (Votes.Blank[XVote].Vopr[1].NCandidates>0)
           {
           Golos=OutV.RezultPath[4]+" "+CisloToStringOrVoice(Votes.Blank[XVote].Vopr[1].NCandidates,false);
           //����������
           SayNSys(Golos);
           F<<Golos<<'\n';

              for (int j=1; j<=Votes.Blank[XVote].Vopr[1].NCandidates; j++)
              {
                //�� ��������� �����
                Golos=OutV.RezultPath[5]+" "+CisloToStringOrVoice(j,false);
                SayNSys(Golos);
                F<<Golos<<'\n';

                int K1=Koib[1].Rezult[1].Bl[XVote].Vop[1].Can[j]+Koib[1].Rezult[2].Bl[XVote].Vop[1].Can[j];
                int K2=Koib[2].Rezult[1].Bl[XVote].Vop[1].Can[j]+Koib[2].Rezult[2].Bl[XVote].Vop[1].Can[j];
                //������
                Golos=OutV.RezultPath[6]+" "+CisloToStringOrVoice(K1+K2,false);
                SayNSys(Golos);
                F<<Golos<<'\n';

                // ����������
                Golos=OutV.RezultPath[7];
                SayNSys(Golos);
                F<<Golos<<'\n';

              }
           }
	  } break;

	} // end switch

  F.close();
 return 0;
}

//------------------------��������� ������������ ���������� �����----------------------------------------
void KoibZero(void)
{   //  ������� ��������  ���� ����� ������� �����������

    for (int i = 0; i <= 2; i++)
    {
        Koib[1].Rezult[i].All=0;
        Koib[1].Rezult[i].Nuf=0;
        Koib[1].Rezult[i].ManyMarks=0;
        Koib[1].Rezult[i].NoMarks=0;
        Koib[1].Rezult[i].NoValid=0;
        Koib[1].Rezult[i].Valid=0;

    for (int ii = 1; ii <= Votes.Nblank; ii++)
    {
        for (int j = 1; j <= Votes.Blank[ii].NVopr; j++)
        {
            Koib[1].Rezult[i].Bl[ii].Vop[j].All=0;
            Koib[1].Rezult[i].Bl[ii].Vop[j].Nuf=0;
            Koib[1].Rezult[i].Bl[ii].Vop[j].ManyMarks=0;
            Koib[1].Rezult[i].Bl[ii].Vop[j].NoMarks=0;
            Koib[1].Rezult[i].Bl[ii].Vop[j].NoValid=0;
            Koib[1].Rezult[i].Bl[ii].Vop[j].Valid=0;

            for (int q=1; q<=Votes.Blank[ii].Vopr[j].NCandidates; q++)
            {
                Koib[1].Rezult[i].Bl[ii].Vop[j].Can[q]=0;
            }
        }
    }
  }

}
//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------------���������� ������� ���������-----------------------------------------
//------------------���������� ��������� �������������� ������ ���������, ���� ��� - ��������� ������---------
bool NextStrokaProt(void)
{  // ����������� ��������� ������ ���  ����� ���. ��������
    bool NXV=false;
    std::string st;
    if (Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].NLines>0)
    {
        for (int i=1; i<=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].NLines; i++)
        {
            st=Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Lines[i].Value;
            // if ((st=="Blank") && (i>Xv))
            if (i>Xv)   {   Xv=i;    NXV=true;   goto end;      }
        }
    }
    end:
    //if (! NXV)  XVote++;
    return NXV;
}
//---------------------------------------------------------------------------------------------------------------
void Sos_3(void)
{
	Ind(Out.Main[99],Out.Main[100]); // ���������� ��������� �����/ ���������� ��������? ��/���
	SaySys(56); // ���������� ��������� �����. ���������� ��������?
}
void Sos_5(void)
{   // ���������� ��  ������// ��������  �����������
    std::string st1;    std::string st2;
	st1=Out.Main[9]; 	st2=Out.Main[11]; 	Ind(st1,st2); 	//SaySys(3);
}
void Sos_6(void)
{  // ������� �  �����  ������  �  Flash
    std::string st1;     std::string st2;
    //�������� Flash //������� ��
	st1=Out.Main[15];    	st2=Out.Main[16];       Ind(st1,st2);     SaySys(7);
}
void Sos_7(void)
{   //�������� �������� ������...
    Ind(Out.Main[17]," ");    SaySys(8);
}

std::string  SpeakElectionDate(void)
{  // �����������  ����  �����������
        std::string st1;     std::string st2;
        std::string st3; std::string st4; std::string st5;
        std:: string SayStr;
        std::string stDig;
        char FirstDig,SecDig;
        switch(Votes.DBday)
        {  // ����������� ����� ���
            case 1:  st1= OutV.NumPath[94];   break;     case 2:  st1= OutV.NumPath[95]; break;          case 3:  st1= OutV.NumPath[96]; break;
            case 4:  st1= OutV.NumPath[97];   break;     case 5:  st1= OutV.NumPath[98]; break;          case 6:  st1= OutV.NumPath[99]; break;
            case 7:  st1= OutV.NumPath[100];  break;     case 8:  st1= OutV.NumPath[101]; break;       case 9:  st1= OutV.NumPath[102]; break;
            case 10:  st1= OutV.NumPath[103]; break;    case 11:  st1= OutV.NumPath[104]; break;     case 12:  st1= OutV.NumPath[105]; break;
            case 13:  st1= OutV.NumPath[106]; break;    case 14:  st1= OutV.NumPath[107]; break;     case 15:  st1= OutV.NumPath[108]; break;
            case 16:  st1= OutV.NumPath[109]; break;    case 17:  st1= OutV.NumPath[110]; break;     case 18:  st1= OutV.NumPath[111]; break;
            case 19:  st1= OutV.NumPath[112]; break;    case 20:  st1= OutV.NumPath[113]; break;    case 21:  st1= OutV.NumPath[114]; break;
            case 22:  st1= OutV.NumPath[115]; break;    case 23:  st1= OutV.NumPath[116]; break;    case 24:  st1= OutV.NumPath[117]; break;
            case 25:  st1= OutV.NumPath[118]; break;    case 26:  st1= OutV.NumPath[119]; break;    case 27:  st1= OutV.NumPath[120]; break;
            case 28:  st1= OutV.NumPath[121]; break;    case 29:  st1= OutV.NumPath[122]; break;    case 30:  st1= OutV.NumPath[113]; break;
            case 31:  st1= OutV.NumPath[124]; break;
        }
        switch(Votes.DBmonth)
        {  // ����������� ����� ���������
            case 1:  st2= OutV.MonthsPath[1]; break;          case 2:  st2= OutV.MonthsPath[2]; break;          case 3:  st2= OutV.MonthsPath[3]; break;
            case 4:  st2= OutV.MonthsPath[4]; break;          case 5:  st2= OutV.MonthsPath[5]; break;          case 6:  st2= OutV.MonthsPath[6]; break;
            case 7:  st2= OutV.MonthsPath[7]; break;         case 8:  st2= OutV.MonthsPath[8]; break;       case 9:  st2= OutV.MonthsPath[9]; break;
            case 10:  st2= OutV.MonthsPath[10]; break;    case 11:  st2= OutV.MonthsPath[11]; break;     case 12:  st2= OutV.MonthsPath[12]; break;
        }
        // ������������ ������ �����������  ���� �����������
        stDig=IntToStr(Votes.DByear);
        FirstDig=stDig[0];// ����� ����� ���
        switch (FirstDig)
        {    case '1':  { st3=OutV.NumPath[74]; } break;  // ���� ������
                case '2':  { st3=OutV.NumPath[75]; } break;  // ��� ������
                case '3':  { st3=OutV.NumPath[76]; } break;  // ��� ������
        }//switch
        FirstDig=stDig[1];// ����� ����� ���
        switch (FirstDig)
        {    case '0':  { st4=""; } break;  // ���� �����
             case '1':  { st4=OutV.NumPath[1]; } break;  // ���
             case '2':  { st4=OutV.NumPath[2]; } break;  // ������
             case '3':  { st4=OutV.NumPath[3]; } break;  // ������
             case '4':  { st4=OutV.NumPath[1]; } break;  // ���������
             case '5':  { st4=OutV.NumPath[2]; } break;  // �������
             case '6':  { st4=OutV.NumPath[3]; } break;  // ��������
             case '7':  { st4=OutV.NumPath[1]; } break;  // �������
             case '8':  { st4=OutV.NumPath[2]; } break;  // ���������
             case '9':  { st4=OutV.NumPath[3]; } break;  // ���������
        }//switch
        FirstDig=stDig[2];// ����� �������� ���
        SecDig  =stDig[3];// ����� ������ ���
        if(FirstDig > '1')
        {  switch (FirstDig)
            {   case '2':  {  if(SecDig ==0) st5=OutV.NumPath[67];  else st5=OutV.NumPath[10];   } break;  // ����������/��������
                case '3':  {  if(SecDig ==0) st5=OutV.NumPath[68];  else st5=OutV.NumPath[11];   } break;  // ����������/��������
                case '4':  {  if(SecDig ==0) st5=OutV.NumPath[69];  else st5=OutV.NumPath[12];   } break;  // ����������/��������
                    case '5':  {  if(SecDig ==0) st5=OutV.NumPath[70];  else st5=OutV.NumPath[13];   } break;  // ����������/��������
                case '6':  {  if(SecDig ==0) st5=OutV.NumPath[71];  else st5=OutV.NumPath[14];   } break;  // ����������/��������
                case '7':  {  if(SecDig ==0) st5=OutV.NumPath[72];  else st5=OutV.NumPath[15];   } break;  // ����������/��������
                case '8':  {  if(SecDig ==0) st5=OutV.NumPath[73];  else st5=OutV.NumPath[16];   } break;  // ����������/��������
                case '9':  {  if(SecDig ==0) st5=OutV.NumPath[125];  else st5=OutV.NumPath[17];   } break;  // ����������/��������
            }//switch
            switch (SecDig)
            {   case '1':  {  st5=st5+" "+ OutV.NumPath[48];     } break;  // �������
                case '2':  {  st5=st5+" "+ OutV.NumPath[49];     } break;  // �������
                case '3':  {  st5=st5+" "+ OutV.NumPath[59];     } break;  // ��������
                case '4':  {  st5=st5+" "+ OutV.NumPath[51];     } break;  // ����������
                case '5':  {  st5=st5+" "+ OutV.NumPath[52];     } break;  // ������
                case '6':  {  st5=st5+" "+ OutV.NumPath[53];     } break;  // �������
                case '7':  {  st5=st5+" "+ OutV.NumPath[54];     } break;  // ��������
                case '8':  {  st5=st5+" "+ OutV.NumPath[55];     } break;  // ��������
                case '9':  {  st5=st5+" "+ OutV.NumPath[56];     } break;  // ��������
        }//switch
        } else
        {  if(FirstDig == '0')
            { //"0"
                switch (SecDig)
                {  case '0':  {  st5= OutV.NumPath[126];     } break;  // ������
                    case '1':  {  st5= OutV.NumPath[48];     } break;  // �������
                    case '2':  {  st5= OutV.NumPath[49];     } break;  // �������
                    case '3':  {  st5= OutV.NumPath[59];     } break;  // ��������
                    case '4':  {  st5= OutV.NumPath[51];     } break;  // ����������
                    case '5':  {  st5= OutV.NumPath[52];     } break;  // ������
                    case '6':  {  st5= OutV.NumPath[53];     } break;  // �������
                    case '7':  {  st5= OutV.NumPath[54];     } break;  // ��������
                    case '8':  {  st5= OutV.NumPath[55];     } break;  // ��������
                    case '9':  {  st5= OutV.NumPath[56];     } break;  // ��������
                }//switch
            }else
            {   // "1"
                switch (SecDig)
                {  case '0':  {  st5= OutV.NumPath[57];     } break;  // ��������
                    case '1':  {  st5= OutV.NumPath[58];     } break;  // �������������
                    case '2':  {  st5= OutV.NumPath[59];     } break;  // ������������
                    case '3':  {  st5= OutV.NumPath[60];     } break;  // ������������
                    case '4':  {  st5= OutV.NumPath[61];     } break;  // ��������������
                    case '5':  {  st5= OutV.NumPath[62];     } break;  // ������������
                    case '6':  {  st5= OutV.NumPath[63];     } break;  // �������������
                    case '7':  {  st5= OutV.NumPath[64];     } break;  // ������������
                    case '8':  {  st5= OutV.NumPath[65];     } break;  // ��������������
                    case '9':  {  st5= OutV.NumPath[66];     } break;  // ��������������
                }//switch
            }
        }
       // SaySys(12); //���� �����������
        SayStr="";
        SayStr=st1+" "+st2+" "+st3+" "+st4+" "+st5+" "+OutV.MonthsPath[13]; // ����

   /*     pid_t pid = fork();
        switch(pid)
        {   case  0: execlp("aplay","aplay",&st1[0],&st2[0],&st3[0],&st4[0],&st5[0],&OutV.MonthsPath[13],NULL); exit(1); break;
            case -1: cout << "error!" << endl; break;
        }
*/

       return SayStr;
}// SpeakElectionDate(void)

void Sos_8(void)
{
    std::string st1;     std::string st2;
    std::string st3; std::string st4; std::string st5;
    std::string stDig;
    TekDateTime(); //������� ���� � �����
    PrVoteDay=0; //  ��������� ��������� ����� ��������� �������
     if (TekDT.year==Votes.DByear)
     {      if (TekDT.month == Votes.DBmonth)
            {   if (TekDT.day ==Votes.DBday)
                 {     //  ����  �����������
                        PrVoteDay=1;
                        Koib[1].VS = 9;
                        Ind(Out.Main[23]," "); //����������� �������� ������? ��/���
                        SaySys(14); //����������� �������� ������?
                 }  else
                 {      if (TekDT.day  > Votes.DBday)
                        {  //  ����  �����������  ������
                            PrVoteDay=-1;
                        }
                 }
            } else
            {       if (TekDT.month > Votes.DBmonth)
                    {  //  ����  �����������  ������
                        PrVoteDay=-1;
                    }
            }
     } else
     {  if (TekDT.year  > Votes.DByear)
         {       //  ����  �����������  ������
                PrVoteDay=-1;
         }
     }
    if(PrVoteDay!=1)
    {  // ���� ��  ����  �����������
        st1=Out.Main[21]+" "+Votes.DateV; // ���� ����������� ��.��.��
        switch ( PrVoteDay)
        {  case -1:  {  st2=Out.Main[20];    } break;
            case  0:  {  st2=Out.Main[19];   } break;
            case  1:  {   st2=""; } break;
        }
        Ind(st1,st2);
        // ������� ��������� ����  �����������
        SaySys(12); //���� �����������
        Golos="  ";    //Golos= SpeakElectionDate();  //SayNSys(Golos);
        if(PrVoteDay==-1) SaySys(11);  if(PrVoteDay==0) SaySys(9);
        //������ ������ � ������������� ������ �������?
        st1=Out.Main[22];       st2=Out.Main[118];       Ind(st1,st2);
        SaySys(13); //������ ������ � ������������� ������ �������?
    }
}

void Sos_9(void)
{     // ����������
      Ind(Out.Main[23]," "); //����������� �������� ������? ��/���
      SaySys(14); //����������� �������� ������?
}
void Sos_10(void)
{
      Ind(Out.Main[24]," "); //����������� ����� �������� ������? ��/���
      SaySys(15); //����������� ����� �������� ������?
}
void Sos_11(void)
{
      Ind(Out.Main[25]," "); //�������� �������� ������? ��/���
      SaySys(16); //�������� �������� ������?
}
void Sos_12(void)
{
      Ind(Out.Main[26]," "); //�������� �������� ������ ��� ���? ��/���
      SaySys(17); //�������� �������� ������ ��� ���???
}
void Sos_13(void)
{
      Ind(Out.Main[27]," "); // �������� ������ �����? ��/���
      SaySys(18); //�������� ������ �����?
}
void Sos_14(void)
{
    std::string st1;    std::string st2;    std::string stDig;
      if (Koib[1].Tren) // �� ���� �����������
      {
        Koib[1].VS = 14;
          if(PrVoteDay==0)
          {  // ���� ����������� ��� ��  ��������
              int Days=(Votes.DByear-TekDT.year)*365+(Votes.DBmonth-TekDT.month)*30+(Votes.DBday-TekDT.day);
              char FirstDig;
              int ii,jj;
              stDig=IntToStr(Days);
              FirstDig=stDig[stDig.length()-1];
              ii=int(FirstDig);
              if(((Days >4)&&(Days<21))||((Days >104)&&(Days<121))||((Days >204)&&(Days<221))||((Days >304)&&(Days<321)))
              { // ����
                  st2=Out.Main[83]; jj=24;
              } else
              {     if(ii==49)
                    {  // ����
                        st2=Out.Main[81];   jj=22;}
                  else
                  {  // ���
                      st2=Out.Main[82];  jj=23;
                  }
              }
               st1=IntToStr(Days)+"  "+st2 +" "+Out.Main[28];
               Ind(st1," "); // DD  ���� �� ������ �����������
              Golos="";
	          Golos=CisloToStringOrVoice(Days,false) +" "+ OutV.MonthsPath[jj]+" "+OutV.MainPath[19];
               SayNSys(Golos);
          }
        //  ������������  ��������� � ���, ������� ����  �������� �� ������ �����������
        Ind(Out.Main[31]," "); //�������������...
       } else {
         Koib[1].VS = 14;
          Ind(st1, Out.Main[31]); //�������������...
       }
}
void Sos_15(void)
{
	Ind(Out.Main[32],Out.Main[33]); //��� �������� � ����� ������������ ������� ��. ��� ������ ������� ������� ���
    SaySys(21); //��� �������� � ����� ������������ ������� ��/ ��� ������ ������� ������� ���
}
void Sos_16(void)
{
    std::string st1;
    st1=Out.Main[34]+" "+IntToStr(Xblank);
    // ������ �������  // ������� ������� ��� ������� N..?
    Ind(st1," ");
    Golos="";
	Golos=OutV.MainPath[22]+" "+CisloToStringOrVoice(Xblank,false) ; //����� �������
    SayNSys(Golos);
}
void Sos_17(void)
{
    std::string st2;
      st2=Out.Main[36]+" "+IntToStr(Xcand);//+" "+Out.Main[37]+" "+IntToStr(Xblank)+" "+Out.Main[35];
      // ������ �������
      Ind("",st2); //������� ������� N...��� ������� N...? ��/���
      Golos="";
     Golos=OutV.MainPath[23]+" "+CisloToStringOrVoice(Xcand,false);//+" "+OutV.MainPath[24]+" "+CisloToStringOrVoice(Xblank,false) ; // ����� �������
    SayNSys(Golos);
}
void Sos_18(void)
{
    std::string st2;
      st2=Out.Main[38]+" "+IntToStr(Xcand)+" "/*+Out.Main[37]+" "+IntToStr(Xblank)+" "*/+Out.Main[39];
      // ������ �������
      Ind("",st2); // � ##  �����!
      Golos=OutV.MainPath[25]+" "+CisloToStringOrVoice(Xcand,false)+ " "+ // ����� �������
                            //OutV.MainPath[24]+" "+CisloToStringOrVoice(Xblank,false) +" "+ // ��� ������� ����� *
                            OutV.MainPath[26]; // �����!
    SayNSys(Golos);
}
void Sos_19(void)
{
    std::string st1;
    std::string st2;
 	if (Snat[Xblank].Nsnat>0)
	{
        st1=Out.Main[37]+" "+IntToStr(Xblank); // ������
        st2=Out.Main[40]+" ";//+IntToStr(Xblank); // �����_���������_�������_��_�������_N_
         Golos="";
         Golos=OutV.MainPath[91]+" "+CisloToStringOrVoice(Xblank,false)+" "+OutV.MainPath[94];
        for (int i = 1; i <=Snat[Xblank].Nsnat; i++)
	   {
		 st2=st2+IntToStr(Snat[Xblank].Snat[i])+", ";
		 Golos=Golos+" "+CisloToStringOrVoice(Snat[Xblank].Snat[i],false);
       }
	   Ind(st1,st2); // ������ �## // �����: ##,##,##,...
        //  ������������ ����������  ��������� � ������ ��������
        SayNSys(Golos);
    }
}
void Sos_20(void)
{
	  Ind(Out.Main[41]," "); //��������� �������� ������,
	  sleep(1);
	  Ind(Out.Main[23]," "); //�����������_��������_������?_��/���
      SaySys(59); //��������� �������� ������
      SaySys(14); //�����������_��������_������?
}
void Sos_21(void)
{
     Ind(Out.Main[43]," "); //����������� ������ �������? ��/���
     SaySys(27); //����������� ������ �������?
}
void Sos_22(void)
{
     Ind(Out.Main[44]," "); //������������� ��������� ?
     SaySys(61); //�������������_���������_�_��������_������?
}

void ReadTime(int pr0, int hour,int minut)
{  // �����������  �������� �������
    // ������������ ������� ��� �����������
    std:: string st1,st2,stDig;
    char FirstDig,SecDig;
    switch (hour)
    {  // ���� ���  �����������
        case 0:    { if(pr0==1){ st1= OutV.NumPath[47]+" "+ OutV.MonthsPath[16];} else st1="";  }   break;
        case 1:    { st1= OutV.NumPath[19]+" "+ OutV.MonthsPath[14];  } break;
        case 2:    { st1= OutV.NumPath[21]+" "+ OutV.MonthsPath[15];  } break;
        case 3:    { st1= OutV.NumPath[22]+" "+ OutV.MonthsPath[15];  } break;
        case 4:    { st1= OutV.NumPath[23]+" "+ OutV.MonthsPath[15];  }  break;
        case 5:    { st1= OutV.NumPath[24]+" "+ OutV.MonthsPath[16];  }  break;
        case 6:    { st1= OutV.NumPath[25]+" "+ OutV.MonthsPath[16];  }   break;
        case 7:    { st1= OutV.NumPath[26]+" "+ OutV.MonthsPath[16];  }   break;
        case 8:    { st1= OutV.NumPath[27]+" "+ OutV.MonthsPath[16];  }   break;
        case 9:    { st1= OutV.NumPath[28]+" "+ OutV.MonthsPath[16];  }  break;
        case 10:    { st1= OutV.NumPath[29]+" "+ OutV.MonthsPath[16];  }   break;
        case 11:    { st1= OutV.NumPath[30]+" "+ OutV.MonthsPath[16];  }   break;
        case 12:    { st1= OutV.NumPath[31]+" "+ OutV.MonthsPath[16];  }   break;
        case 13:    { st1= OutV.NumPath[32]+" "+ OutV.MonthsPath[16];  }  break;
        case 14:    { st1= OutV.NumPath[33]+" "+ OutV.MonthsPath[16];  }   break;
        case 15:    { st1= OutV.NumPath[34]+" "+ OutV.MonthsPath[16];  }  break;
        case 16:    { st1= OutV.NumPath[35]+" "+ OutV.MonthsPath[16];  }   break;
        case 17:    { st1= OutV.NumPath[36]+" "+ OutV.MonthsPath[16];  }   break;
        case 18:    { st1= OutV.NumPath[37]+" "+ OutV.MonthsPath[16];  }   break;
        case 19:    { st1= OutV.NumPath[38]+" "+ OutV.MonthsPath[16];  }   break;
        case 20:    { st1= OutV.NumPath[10]+" "+ OutV.MonthsPath[16];  }  break;
        case 21:    { st1= OutV.NumPath[10]+" "+OutV.NumPath[19]+" "+ OutV.MonthsPath[14];  }  break;
        case 22:    { st1= OutV.NumPath[10]+" "+OutV.NumPath[21]+" "+ OutV.MonthsPath[15];  }   break;
        case 23:    { st1= OutV.NumPath[10]+" "+OutV.NumPath[22]+" "+ OutV.MonthsPath[15];  }  break;
    }
    if(minut >19)
    {    // ������ 19 �����
           stDig=IntToStr(minut);
           FirstDig=stDig[0];// ����� ��������  �����
           SecDig  =stDig[1];// ����� ������ �����

        if(FirstDig > '1')
        {  switch (FirstDig)
            {   case '2':  { st2=OutV.NumPath[10];   } break;  // ��������
                case '3':  {  st2=OutV.NumPath[11];   } break;  // ��������
                case '4':  {  st2=OutV.NumPath[12];   } break;  // �����
                case '5':  {   st2=OutV.NumPath[13];   } break;  // ���������
            }//switch
            switch (SecDig)
            {   case '0':  {  st2=st2+" "+ OutV.MonthsPath[19];       } break;  // �����
                case '1':  {  st2=st2+" "+ OutV.NumPath[18] +" "+ OutV.MonthsPath[17];       } break;  // ���� ������
                case '2':  {  st2=st2+" "+ OutV.NumPath[20] +" "+ OutV.MonthsPath[18];       } break;  // ��� ������
                case '3':  {  st2=st2+" "+ OutV.NumPath[22] +" "+ OutV.MonthsPath[18];       } break;  // ��� ������
                case '4':  {  st2=st2+" "+ OutV.NumPath[23] +" "+ OutV.MonthsPath[18];       } break;  // ������ ������
                case '5':  {  st2=st2+" "+ OutV.NumPath[24] +" "+ OutV.MonthsPath[19];       } break;  // ���� �����
                case '6':  {  st2=st2+" "+ OutV.NumPath[25] +" "+ OutV.MonthsPath[19];       }  break;  // ����� �����
                case '7':  {  st2=st2+" "+ OutV.NumPath[26] +" "+ OutV.MonthsPath[19];       }  break;  // ���� �����
                case '8':  {  st2=st2+" "+ OutV.NumPath[27] +" "+ OutV.MonthsPath[19];       }  break;  // ������ �����
                case '9':  {  st2=st2+" "+ OutV.NumPath[28] +" "+ OutV.MonthsPath[19];       }  break;  // �����������
            }//switch
        }
    } else
    {  // �� 20 �����
        switch (minut)
        {  // ������ ���  �����������
            case 0:    st2= ""; break;
            case 1:    { st2= OutV.NumPath[18] +" "+ OutV.MonthsPath[17];  }  break;
            case 2:    { st2= OutV.NumPath[20] +" "+ OutV.MonthsPath[18];  } break;
            case 3:    { st2= OutV.NumPath[22] +" "+ OutV.MonthsPath[18];  } break;
            case 4:    { st2= OutV.NumPath[23] +" "+ OutV.MonthsPath[18];  } break;
            case 5:    { st2= OutV.NumPath[24] +" "+ OutV.MonthsPath[19];  }  break;
            case 6:    { st2= OutV.NumPath[25] +" "+ OutV.MonthsPath[19];  }  break;
            case 7:    { st2= OutV.NumPath[26] +" "+ OutV.MonthsPath[19];  }  break;
            case 8:    { st2= OutV.NumPath[27] +" "+ OutV.MonthsPath[19];  }  break;
            case 9:    { st2= OutV.NumPath[28] +" "+ OutV.MonthsPath[19];  }  break;
            case 10:    { st2= OutV.NumPath[29] +" "+ OutV.MonthsPath[19];  }  break;
            case 11:    { st2= OutV.NumPath[30] +" "+ OutV.MonthsPath[19];  }  break;
            case 12:    { st2= OutV.NumPath[31] +" "+ OutV.MonthsPath[19];  }  break;
            case 13:    { st2= OutV.NumPath[32] +" "+ OutV.MonthsPath[19];  }  break;
            case 14:    { st2= OutV.NumPath[33] +" "+ OutV.MonthsPath[19];  }  break;
            case 15:    { st2= OutV.NumPath[34] +" "+ OutV.MonthsPath[19];  }  break;
            case 16:    { st2= OutV.NumPath[35] +" "+ OutV.MonthsPath[19];  }  break;
            case 17:    { st2= OutV.NumPath[36] +" "+ OutV.MonthsPath[19];  }  break;
            case 18:    { st2= OutV.NumPath[37] +" "+ OutV.MonthsPath[19];  }  break;
            case 19:    { st2= OutV.NumPath[38] +" "+ OutV.MonthsPath[19];  }  break;
        }
    }
    Golos="  ";         Golos=st1+" "+st2;        SayNSys(Golos);
} //  ReadTime

void Sos_50(void)
{  // ���������� ����������
      std::string st1;  st1=Out.Main[47];
      if (NumBullAll >= 0)   st1=st1+" "+IntToStr(NumBullAll);
      Ind(st1," ");   //Ind("",st1);

}
void Sos_23(void)
{   //������������! ��������� �������������. �����
    std::string st1,st2;
        TekDateTime();    int h =TekDT.hour;    int m=TekDT.minit;
            st1=Out.Main[45]; st2=Out.Main[48]+TimeS; // �����: ��:��
    Ind(st1,st2);
    //sleep(1);
    SaySys(28);
    ReadTime(1,h,m);
    //st1=Out.Main[47] + " 0";  Ind(st1," "); // 15.10.2013//st1=Out.Main[46] ;  st2=Out.Main[47] + " 0";  Ind(st1,st2);
    Sos_50();
}

void Sos_24(void)
{
      Ind(Out.Main[49]," "); //��������� ����? ��/���
  	  SaySys(29); //��������� ����?
}

void ObrTestBull(void)
{  // ������������ ������ ������ ��� ��������� �������� ����������
        std::string st1="";         std::string st2="";         std::string st3="";
        Ind(Out.Main[127]," "); // ������������...
        //SayVotes(1);  // ������������
        pr_rr=0;
        Golosovanie();  // ����� � ������������� ���������
        //st1=IntToStr(NumBullAll)+" : "+IntToStr(MasScan.MasRecOut[2]); // ���������� �������� ����������// ����� ���
         st1=IntToStr(MasScan.MasRecOut[4])+" : "+IntToStr(MasScan.MasRecOut[2]); // ��� ��������� (����� �������)// ����� ���
        //Golos="";    Golos=OutV.VotesPath[11]+" "+CisloToStringOrVoice(NumBullAll,false) ; //���������� �������� ����������
        //SayNSys(Golos);
        if ((pr_rr==1)||(pr_rr==2))//���//��������� ��������������� �����!
        {   st1=st1+" "+Out.Votes[2];   Ind(st1,"");     SayVotes(2);  SayVotes(7);  }
        if ((pr_rr==3))//��������� ����������������!//����� �������
        {    st1=st1+" "+Out.Votes[3];    Ind(st1,""); SayVotes(3);       }
        if ((pr_rr==4))//��������� ����������������!//��� �������
        {    st1=st1+" "+Out.Votes[4];    Ind(st1,"");    SayVotes(4);    }
        if ((pr_rr==5))
        {   //��������� ��������������!//��� � �������
             //SayVotes(5); //
            int i=MasScan.MasRecOut[1]; // ����� ���� ������ ������������� ���������
            int NumV=MasScan.MasRecOut[3]; // ���������� ��������  ��  ������ ( ��� ����������� >= 1. ��� �������  1)
            //int NumMarker=MasScan.MasRecOut[4]; // ����� ������� ������
            st1=st1+" "+Out.Votes[5]; // ��������������
            if(NumV>1)st3=Out.Votes[9]+" N : ";  // ������ �����
            for(int j=1;j <= NumV;j++) //  �� ���� ������� (��������) ��  ������
            if(Votes.Blank[i].Vopr[j].ID!="0")
            {   st2="";
                 Golos= "";
                //Golos= OutV.VotesPath[9]+" "+CisloToStringOrVoice(j,false) ;
               int jj=0;
               for(int k=1; k <= Votes.Blank[i].Vopr[j].NCandidates;k++) if(MasScan.MasCandOut[j][k]!=0)jj=jj+1;
               if(jj==1)
               {  // ����  ������� �� ��������
                        if(NumV>1)st2=st3+IntToStr(j)+"  "+Out.Votes[10]+" N : ";
                        if(NumV==1) st2=Out.Votes[10]+" N : ";
                        Golos= Golos+" "+OutV.VotesPath[10];
               } else
               {  // ������ �����  �������
                        if(NumV>1)st2=st3+IntToStr(j)+"  "+Out.Votes[13]+" N : ";
                        if(NumV==1) st2=Out.Votes[13]+" N : ";
                        Golos= Golos+" "+OutV.VotesPath[13] ;
                }
                for(int k=1; k <= Votes.Blank[i].Vopr[j].NCandidates;k++)
                {  //��  ���������� ��� ������ �������
                    if(MasScan.MasCandOut[j][k]!=0)
                    {  // ������������ ������ ����������, �� ������� ���� �������
                        if(k>1) // ���������
                        {   st2=st2+IntToStr(k);
                             if(k!=Votes.Blank[i].Vopr[j].NCandidates) st2=st2+", "; else st2=st2+".";
                            Golos= Golos+" "+CisloToStringOrVoice(k,false) ;
                        } else // ��������
                        {   st2=st2 + IntToStr(k);
                            Golos= Golos+" "+CisloToStringOrVoice(k,false) ;
                        }
                    }
                }
            }
            Ind(st1,st2);   SayVotes(5); // ��������� ��������������
            //SayNSys(Golos);
        }
       Sos_50();
          // ���������� ������  ��������� ��������� �  �����  .bmp
    if(MakeBMPFile(&FBMP[0],SCAN_LINEW,max_gelesen)==0)  printf("\n BMP_File is written\n");
    else printf("\n BMP_File open error\n");
    /*!!!!!!!!!!!!!!!!!!!!!!!*/
                if((PrErrScan >0)&&(PrErrScan <100))
                {     st1=" Scan Error "+IntToStr(PrErrScan);
                        Ind("",st1);  //  ������  ����  ������  ������������
                }
} //ObrTestBull

void Sos_25(void)
{
      Ind(Out.Main[50]," "); //����������� �������� ������������? ��/���
      SaySys(32);//����������� �������� ������������?
}
void Sos_26(void)
{
      Ind(Out.Main[24]," "); //����������� ����� ��������� ������������? ��/���
      SaySys(33); //����������� �����?
}
void Sos_27(void)
{
      Ind(Out.Main[52],Out.Main[53]); //�������� �������� ������������? ��/���
      SaySys(34); //�������� �������� ������������?
}
void Sos_28(void)
{
      Ind(Out.Main[52],Out.Main[53]); //�������� �������� ������������ ��� ���? ��/���
      SaySys(35);//�������� �������� ������������ ��� ���?
}
void Sos_29(void)
{
      Ind(Out.Main[54],Out.Main[55]);
      SaySys(36); //��� �������� � ������������ ����� ����������� ������� ��
}
void Sos_30(void)
{  //  ��  ������ �����������  �������� *** ����� *** �����
     std::string  st1,st2,st3;
     TekDateTime();
      int ho=Votes.Bhour[1]-TekDT.hour;
      int mi=Votes.Bmin[1]-TekDT.minit;
      //int zz=(ho-1)*60+mi;
      st2="";      st1=Out.Main[56];     st2=IntToStr(ho);
      switch(ho)
      {   case 1: st2=st2+" "+Out.Main[98]; break;
          case 2: st2=st2+" "+Out.Main[99]; break;
          default: st2=st2+" "+Out.Main[100]; break;
      }
      st3="";       st3=IntToStr(mi);
      if((mi==0)||((mi>4)&&(mi<21))||((mi>24)&&(mi<31))||((mi>34)&&(mi<41))||((mi>44)&&(mi<51))||((mi>54)&&(mi<61)))
      st3=st3+" "+Out.Main[101]; else
      { if((mi==1)||(mi==21)||(mi==31)||(mi==41)||(mi==51)) st3=st3+" "+Out.Main[102]; else st3=st3+" "+Out.Main[103];    }
      st2=st2+" "+st3;
      Ind(st1,st2); // �� ������ ����������� ��������

      ReadTime(0,ho,mi); // "0" - ��  ��������  ���� �����
      //SaySys(38); // ��  ������ ����������� ��������
}

void Sos_60(void)
{
      std::string st1;
     // st1=Out.Main[57]+TimeS; // ������������ �����������! ����� *.*
      if (NumBullAll >=0 )
      {
           st1=Out.Main[58]+IntToStr(NumBullAll); // �������:...
      }
      Ind(st1," ");
}

void Sos_31(void)
{
      std::string st1;
      std::string st2;
      SaySys(39); //������������ �����������. �����
              TekDateTime();
      int h =TekDT.hour;
      int m=TekDT.minit;
      st1=Out.Main[57]; // ������������ �����������
      st2= Out.Main[48]+" "+IntToStr(h)+":"+IntToStr(m); // ����� ��:��
      Ind(st1,st2);

      //  ������������  ����������� �����: ** ����� ** �����
            ReadTime(1,h,m);

      //ReadTime(1,h,m);
      //sleep(2);

      Sos_60();//st1= Out.Main[58]+" 0"; // ������� : 0
     // Ind(st1," ");


}



void Sos_32(void)
{
      Ind(Out.Main[59],Out.Main[60]);
      // �����_�������������_�����������_�������! ���_�������������_�����_���������_�������_��
      SaySys(41); //�����_�������������_�����������_�������!
      SaySys(42); //���_�������������_�����_���������_�������_��
}
void Sos_33(void)
{
      Ind(Out.Main[61],Out.Main[64]); //��������� ������������ �����������? ��/���
 	  SaySys(43); //��������� ������������ �����������?
}

void Sos_70(void)
{  // ������� ���������� ��� ���������� �����������
      std::string st1;
       if (NumBullAll >=0 )    {  st1=Out.Main[63]+" "+IntToStr(NumBullAll);          Ind(st1," ");       }
}

void Sos_34(void)
{  //  ����������  �����������
	  std::string st1;
	  std::string st2;
	   SaySys(44); //���������� �����������. ����� :
       TekDateTime();      int h =TekDT.hour;     int m=TekDT.minit;
       	  st1=Out.Main[62]+" "+TimeS; //����������_�����������!_�����:_
      int All=Koib[1].Rezult[XVote].All;
	  st2=Out.Main[63]+" "+IntToStr(All); // ����������:_
	  Ind(st1,st2);

     ReadTime(1,h,m);


     Sos_70();
}



void Sos_35(void)
{
      Ind(Out.Main[61],Out.Main[65]);
      //��� ���������� ����������� ������������ ������� �� /��� �������� � ������ ���������� ������� ���
  	  SaySys(45);

}
void Sos_36(void)
{
      Ind(Out.Main[66],Out.Main[67]);
      // ��� �������� � ���������� ������ ����������� ������� ��/ ��� �������� � ����������� ������� ���
      SaySys(48); // ��� �������� � ���������� ������ ����������� ������� ��
      SaySys(49); // ��� �������� � ����������� ������� ���
}
void Sos_37(void)
{
      std::string st;
      st=Out.Main[51];//+IntToStr(XVote)+Out.Main[35];
      Ind(st," "); //����������� �����
      SaySys(33); //����������� ����� ?
}
void Sos_38(void)
{
  	  Ind(Out.Main[69]," "); //���������� �������� ����������
      SaySys(51); //���������� �������� ����������, ������� ��
}
void Sos_39(void)
{
        std::string st;
        //��������_����������_��_�������_N_XVote _?_��/���
        st=Out.Main[70]+" "+IntToStr(XVote);//+" "+Out.Main[35];
        Ind(st,Out.Main[104]); // ���������� ����������
        //  ������� Speak
        Golos="";
        Golos=OutV.MainPath[52]+" "+CisloToStringOrVoice(XVote,false) ; //����� �������
        SayNSys(Golos);
}
void Sos_40(void)
{
  	  std::string st;
  	  st=Out.Main[71]+" "+IntToStr(XVote)+" "+Out.Main[72];
	  Ind(st," "); //�������� ���������� �� ������� Xvote ��� ���?
	  Golos="";
	  Golos=OutV.MainPath[52]+" "+CisloToStringOrVoice(XVote,false) +" "+OutV.MainPath[53];
      SayNSys(Golos);
}
void Sos_41(void)
{
      std::string st1;   std::string st2;
      st1=Out.Main[73];  st2=Out.Main[75]+" "+IntToStr(XVote)+". ";   Ind(st1,st2);
      Golos="";
	  Golos=OutV.MainPath[54]+" "+CisloToStringOrVoice(XVote,false); //���� �������������� �������� ��  ������� �
      SayNSys(Golos);
}
void Sos_42(void)
{
      std::string st;   std::string st1;
      st=Out.Main[37]+" "+IntToStr(XVote)+" "+Out.Main[76]+"  "+IntToStr(Xv);
         // ������ N...XVote. ������ N...Xv
      st1=IntToStr(Votes.Blank[XVote].Vopr[XVopr].Protocoles[1].Lines[Xv].Value);
      // ������  �������� ������  ��  ������ ������ �����
      Ind(st,st1);
}

void Sos_44(void)
{
	  std::string st;
	  std::string st1;
	  st=Out.Main[78];//+" "+IntToStr(XVote)+" "; //������_�����������_�����������_��_�������_N_XVote

	  if (PrKC==1) //�������� �� �� ������� N
	  {  //����������� ����������� ���������
		 st1=Out.Main[79];
		 SaySys(63);
	   } else
	   { //����������� ����������� �� ���������
		 Print(4); //���������� ������������� �� �� ������� XVote
		 st1=Out.Main[80];
		 SaySys(64);
       }
	   Ind(st,st1);
}
void Sos_45(void)
{
        std::string st;
        SaySys(65);
        st=Out.Main[85];  Ind(st," ");
		//���������_�_���������_��������_��������_��_�������_N_XVote
		        sleep(2);
		SaySys(66);
		Ind(Out.Main[86]," "); //�������� ��������?


}
void Sos_46(void)
{
	    std::string st1,st2;
        SaySys(67);
	    st1=Out.Main[87];//+" "+IntToStr(XVote)+" "+Out.Main[35];
	    st2=Out.Main[91];
        Ind(st1,st2); //����������� �������������� //����� ���������?

}
void Sos_47(void)
{
        std::string st;
		SaySys(68);
        st=Out.Main[88];//+" "+IntToStr(XVote)+" "+Out.Main[89];
		Ind(st," "); //���������� �����

}
void Sos_49(void)
{
 	    std::string st;
 	    st=Out.Main[90];//+" "+IntToStr(XVote)+" "+Out.Main[91];
		Ind(st," "); //����_������_�����������_�����������_��_�������_N_XVote_��_Flash

}

int Golosovanie(void)
{
	if (Koib[1].Opros)
    {    // ������ ������������
		GashBeforeScan();
        frontLED(2,0);   // ��������� ������� �������� ���� ������� ������������ � ���������
        pr_rr=0;
        ScanReady=true;
        VoteScan();
        ScanReady=false;
        if(PrStopScan==2)
        {       pr_rr=WaitRR();
                if ((pr_rr==1)||(pr_rr==2)) //���
                {  //��������� ��������������� �����!
                        Page_back(); // �������� �� �����
                        printf(" Defected Bulleten \n");      // ��������� ���������������  �����
                }
                if((pr_rr > 2)&&(pr_rr < 6))
                {       // ��������� �������������  �����
                        NumBullAll= NumBullAll+1; // ���������� �������� ����������
                        cout << " Recognized nVoteType = " << nVoteType << endl;
                        XVote= MasScan.MasRecOut[3];  // ����� ���� ������������  ������
                        cout << " Vote Type (XVote) = " << XVote << endl;
                        cout << "  Blank Number  = " << Votes.Blank[MasScan.MasRecOut[1]].marker << endl;
                        cout << " Marker Blank Vote Type  = " << Votes.Blank[MasScan.MasRecOut[3]].marker << endl;
                        if ((pr_rr==3))
                        {  //����� �������
                                Koib[1].Rezult[XVote].All++;
                                Koib[1].Rezult[XVote].ManyMarks++;
                                Page_mark();
                                printf(" Too  many Marks \n");      // ��������� ���������������  �����
                        }
                        if ((pr_rr==4))
                        {  //��� �������
                                Koib[1].Rezult[XVote].All++;
                                Koib[1].Rezult[XVote].NoMarks++;
                                Page_mark();
                                printf("  No Marks \n");      // ��������� ���������������  �����
                        }
                        if ((pr_rr==5))
                        {  //��� � �������
                                Koib[1].Rezult[XVote].All++;
                                Koib[1].Rezult[XVote].Valid++;
                                Page_down();
                                printf("  Bulleten has been recognized  One Mark  \n ");      // ��������� �������������  �����
                        }
                        Koib[1].Rezult[0].All=NumBullAll;
                        // �������  ������  �  ������  ����
/*!����!!!!*/                  //  ��������  �����������  ��������� ���������� �  ������  ����
                        char *ptrSend;
                        ptrSend=(char *)&Koib[1];
                        SendtoAnother(19,ptrSend,sizeof(Koib[1]));
                        Koibsave(1);  // ������ ��������� ��������� ����
		 //Koibsave(2);  // ������ ��������� ������������ ���� (  � HardProc.cpp  ���  ��������� ������ �� ������� �������

                    }
                    //if(Koib[1].VS==50)  Sos_50();
                    if((Koib[1].VS==60)||(Koib[1].VS==32))  Sos_60();
                    if(Koib[1].VS==70)  Sos_70();

                }
                 frontLED(0,2);   // ��������� ������� �������� ���� ������� ������������ � ���������
        }
        return 0;
} //Golosovanie
//---------------------------------------------------------------------------------------------------------------------
int Sostoanie(void)
{
    std::string st;  std::string  st1;  std::string st2;
	switch (Koib[1].VS)
	{

	case 1: //----------------------------------------------��������� ����+��������+���������������� ����
		{  //�������������  -  ��������  ��  �����  ���������� ��� ��������� �������
		  st1=Out.Main[1];  Ind(st1," ");   sleep(3);
		  //�������� �������
		  st1=Out.Main[2];  Ind(st1," ");
		   sleep(5);
		  Zagruzka(); //  �������� ��������� ������ ���� (������������ �����)
		  st=Out.Main[3]+"  (2013)";  st1=Out.Main[4]+" "+SerNumKoib;    Ind(st, st1);//��_����_���.// ������ N
          sleep(1);
          Pr_Flash=0; // ������� �������� ������� ���� �� ������ ����
		  st1=Out.Main[5];  Ind(st1," ");  //����������������...

          int S=Samotestirovanie();
          if (S==0)
          {  // ���������������� ���������
              st=Out.Main[4]+" "+SerNumKoib; st1=Out.Main[6];  Ind(st,st1); // ������ N// �����
			  SaySys(1); //������ �����
			  Koib[1].VS = 2;
			  PrNoBtnWait=1;
          }
          else
          {  if(pr_btn==1) // Yes  ����������  ������
            {   //  ����������� ������
                Koib[1].VS = 2;
                PrNoBtnWait=1;
			} else
			{     st=Out.Main[4]+" "+SerNumKoib; st1=Out.Main[7]; Ind(st,st1); // ������ N  // ��  �����
                  SaySys(2); //������ �� �����
			}
          }
		}break;
	case 2: //------------------------------------------------�������� ������� ��������� ������
		{
          if (AvarFiles())
          {  // ���� ��������� ���� ������,  ���������� ���������� ����� (��������� 3)
			 Koib[1].VS=3;
			 Sos_3(); //3
          } else {
             Koib[1].VS=4;
              Ind(" "," "); // ������� �����
          }
             PrNoBtnWait=1;
		} break;
	case 3: //-------------------------------------------------��������� �� ���������� ����� ?
		{
              Koib[1].VS = 4;
             Ind(" "," "); // ������� �����
              //  ���������  �  �������  �������� ������ �� ����������  �����
              LoadFromAvar();
              NumBullAll=Koib[1].Rezult[0].All; //  �������� ����������  �  �������  ���������
              PrNoBtnWait=1;
		} break;
	case 4: //-------------------------------------------------���������� �� 2-� ����
		{
            // �����������  �������  ��������
            have_Printer = CheckPrinter(); // 1- ������� ����, ���� - �������, 0 - ���� ����������
		    st1=Out.Main[8];     Ind(st1,""); // �����...
            // !!! �������� ���������� �� ������ ��������
            PrConnectOK= Connect();
             //PrConnectOK =1 ;//- ����������  �����������
             //have_Printer=1;
			cout << " Priznak connection = " << PrConnectOK << "  Priznak KM = " << have_Printer<< endl;
			if ((have_Printer==1)||(Pr_Flash==1)) //���� ���� �������, ������� ���������
			{   Koib[1].Avto=false;     Koib[1].Main=true;
				//st1=Out.Main[9]; // ���������� �� ������
                // !!! �������� ������� ���������� �� ������ ��������
                if(PrConnectOK==1)
                {  // ���������� �� ������ // �������� �����������
                    Sos_5();   SaySys(6); // ���������� �� 2-� �������� �����������
/*!��������� ���������� � ������ �������*/  // Koib[2] // � ������ HardProc
                 //   int j=int(Koib[2].IP.length());
                //    for(int i=0;i<  j ;i++)  Ip_slave[i]=Koib[2].IP[i];

                    // ������� �  ����������  ���������
                    Koib[1].VS = 6;
                     Sos_6(); // ������� �  �����  �������� ������
                } else
                { // ���������� �� ������ �������� (�����������)��  �����������
                    st2=Out.Main[10];   Ind("",st2);  // ��� - �������� �����
                   // ����� ���������� �  �������� ������� ������ "���"
                   if(get_Buttons_Imm()==1)
                   {if (Buttons[1]==1)
                     {   //  ������  ������ "���" - �������  �  ����������  �����
                            st1=Out.Main[12];      st2=Out.Main[13];       Ind(st1, st2);
                            Koib[1].Avto=true;
                            SaySys(5);
                            Koib[1].VS = 6;
                            Sos_6(); // ������� �  �����  �������� ������
                            strcpy(Ip_slave,"");
                     }
                   }
                   usleep(25000);  //25msec!! // ��� ������ ������ ����� ���� ����������
                   PrNoBtnWait=1; // ���  �����������  ������  ������� ��� ������� ������ "���"
                };
			}else
			//if (have_Printer==0)
			{  //  ������ ��  ��������, ������� ��  ���������
               Koib[1].Avto=false;   Koib[1].Main=false;
			    // !!! �������� ������� ���������� �� ������ ��������
                if(PrConnectOK==1)
                {  // ���������� �� ������ (��������)  �������� �����������
                    Sos_5();   SaySys(6);
                    Koib[1].VS = 6;// ������ �����������, ���������� �����������
                    //Sos_6(); Koib[1].Avto=true;
                    PrNoBtnWait=1;
                } else
                {	// ������� ��  ���������, ������ ���
                    // ����������  ��  ������� (��������)  �������� ��  �����������
                    //st2=Out.Main[10];   Ind("",st2);  // ��� - �������� �����
                    Koib[1].VS = 4;
                    // ��������� ��  ����������� �������� ��� Flash
                    FileName=MountNRead();
                    if(FileName!="-1")
                    { // ������� ���� ��� ���������  �������
                      //������� ����������  �  ����������  �����
                        Pr_Flash=1;
                    } else Pr_Flash=0;
                    usleep(25000);  //25msec!! ��� ������ ����������� �������� ��� ������
                    PrNoBtnWait=1;  // ���  �����������  ������  �������
 // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*! �� ������ ������ */             Pr_Flash=1;
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
			}
		} break;
	case 5: //----------------------------------------�������� ����� ����������?
		{
           if(PrModeChange==1 )    {   Sos_5();   PrModeChange=0; 	}
           if ((Koib[1].Main)||(Koib[1].Avto))
			{
                if(pr_btn==0) // No �������� �����, ������� �  ����������  �����
                {  //������ �������� � ���������� ������
                    st1=Out.Main[12];      st2=Out.Main[13];       Ind(st1, st2);
                    Koib[1].Avto=true;
                    SaySys(5);

                    Koib[1].VS = 6;
                    Sos_6(); // ������� �  �����  �������� ������
                }
			}
			PrNoBtnWait=0;
			pr_btn=-1;
		} break;
	case 6: //---------------------------------------------------�������� ��������� ��
		{
			if ((Koib[1].Main)||(Koib[1].Avto))
			{
                  if(PrModeChange==1 )    {    Sos_6();   PrModeChange=0; 	}
                if(pr_btn==1) // Yes
                {   //  ����� ������� ��. �� �����������  ���� � ����� �������� ������
                    Sos_7();  // �������� �������� ������
                    // ������������ ���� �  �����
                    PrIdError=FindSourceFile();
                    switch ( PrIdError)
                    {  case 0: //  ���� ������  ������  ��  ��������, ������� � ������ �����
                            {   Koib[1].VS = 7;   } break;
                        case -1: //  �������� ��  ������
                            {   st1=Out.Main[120];      st2=Out.Main[119];       Ind(st1, st2);   SaySys(87);
                            } break;
                        case -2: //  ����  ��  ������
                            {   st1=Out.Main[14];      st2=Out.Main[119];       Ind(st1, st2);    SaySys(86);   } break;
                        case -3: //  ������ �������� ������
                            {   st1=Out.Main[121];      st2=Out.Main[119];       Ind(st1, st2);    SaySys(88);   } break;
                    } // switch
                    PrNoBtnWait=1;
                }
                if(pr_btn==0) // No // ��������� ������
                {  Koib[1].VS = 500;     PrNoBtnWait=1;         }
                pr_btn=-1;
            } else
            {  // ������  ��������� "����������" ���  ������������  �������
                st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 7: //----------------------------------------------------------------�������� ��
		{
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if (PrModeChange==1 )  {     Sos_7();   PrModeChange=0; 	}
			cout << FileName << endl;
			if (! Parser(FileName))
			{  //�� ����������� ������ � FLASH
			    if(pr_btn==-1)
			    {  Ind(Out.Main[122],Out.Main[119]); //������ ������. ��������� ������?
                    SaySys(89); //������ �������� ������������. ��������� ������?
			    }
				if(pr_btn==1) //Yes  //  ������� �  ���������� ��������� ������� ����� �� ��  ����
                    {   Koib[1].VS = 6;				  Sos_6(); 				  PrNoBtnWait=1;			}
				if(pr_btn==0) //No  // ������� �  ��������� ������
                    {	  Koib[1].VS = 500;  PrNoBtnWait=1;    }//����� �� ��������!
				pr_btn=-1;
			}
			else
			{ // ������ � FLASH ���������!
//!!!!!!!!!!!!!!!!!!!!!!!
   //Print(1); Print(2); Print(3); Print(4); Print(5);


//!!!!!!!!!!!!!!!!!!!!!!
                Koib[1].VS = 8;
				Ind(Out.Main[18]," "); //  ������  ��������
				SaySys(10);
				Sos_8(); // ������� � 8 ���������
                PrVoteTimeStart=0;     PrVoteTimeEnd=0;

                //Koib[1].VS = 38;  //  ���  �������� � ����������� ( ������� ����������)


			}
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                 //st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 8: //--------------------------------------������� � ����� ����������, ���� �� ���� �����������?
		{
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		    if(PrModeChange==1) {     Sos_8();   PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{  // �������  �  ������������� �����
			   Koib[1].Tren = true;
			   Koib[1].VS = 9;
			   Sos_9();
			  }
             if(pr_btn==0) //No
             {  // ��  ������
               Koib[1].Tren = false;
               Koib[1].VS = 11;
               Sos_11();
             }
			 pr_btn=-1;
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                // st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 9://---------------------------------------------------���������� ���o���� ������
		{
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 )  {     Sos_9();  PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{   //  ���������� ��������  ������
			    // �������� �������  ��������
                have_Printer=1;
                if(have_Printer)
			    //if(CheckPrinter())
			    {	    Ind(Out.Main[114]," "); //  ����������
                        Print(1);
                        Ind(Out.Main[123],Out.Main[124]); //  ���������� ��  ������ // �����
                        Koib[1].VS = 10;                   Sos_10();                    PrNoBtnWait=1;
			    } else
			    {  // ������� �� ������
			         Ind(Out.Main[125],Out.Main[126]); //  ������� ��  ������
			    }
            }
            if(pr_btn==0) //No
            {
                Koib[1].VS = 11;         Sos_11();
            }
            pr_btn=-1;
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                //   st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 10://------------------------------------------------���������� ����� �������� ������
		{
            if ((Koib[1].Main)||(Koib[1].Avto))
            {
                if(PrModeChange==1)  {     Sos_10();  PrModeChange=0; 	}
                if(pr_btn==1) //Yes
                {  // �������� �������  ��������
                    have_Printer=1;
                    if(have_Printer)
                    //if(CheckPrinter())
                    {	    Ind(Out.Main[114]," "); //  ����������
                            Print(1);
                            Ind(Out.Main[123],Out.Main[124]); //  ���������� ��  ������ // �����
                            Koib[1].VS = 10;
                            Sos_10();
                            PrNoBtnWait=1;
                    } else
			    {  // ������� �� ������
			         Ind(Out.Main[125],Out.Main[126]); //  ���������� ��  ������ // �����
			    }
			}
			if(pr_btn==0) //No
			{      Koib[1].VS = 11;
                    Sos_11();
                    PrNoBtnWait=1;
			}
			pr_btn=-1;
          } else
            {  // ������  ��������� "����������" ���  ������������  �������
               //  st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 11://--------------------------------------------------------�������� �������� ������?
		{

		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 ) 	{    Sos_11();   PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{ // �����������  �������� ������
			  Speak(1);
			  Koib[1].VS = 12;
              Sos_12(); // �������� �������� ������  ��� ��� ?
			}
			 if(pr_btn==0) //No
			 {  // ������� � ������� �������� �������� ������
			   Koib[1].VS = 13;
			   Sos_13();  // �������� ������ ����� ?
			 }
			 pr_btn=-1;
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                 //st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 12://-------------------------------------------------�������� �������� ������ ��� ���?
		{
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1) {     Sos_12();  PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{   Speak(1);     Sos_12();     PrNoBtnWait=1; 	}
			if(pr_btn==0) //No
			{   Koib[1].VS = 13;   Sos_13(); // �������� ������ ����� ?
			    PrNoBtnWait=1;
			}
            pr_btn=-1;
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
              //   st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 13://------------------------------------------------------�������� ������ �����?
		{
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 )  {     Sos_13();   PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{  // ������ �����! ������������  �������� ������  �  ������  ������������
                FormIdScan();
                Koib[1].VS = 14;      Sos_14();     PrNoBtnWait=1;
			}
			if(pr_btn==0) //No // ������� �  ����  �������� ������
			{    Koib[1].VS = 6;     PrModeChange=1;     PrNoBtnWait=1; 			}
			pr_btn=-1;
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                // st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 14://--------------------------------------------------------���� �� ���� �����������.
		{
		  if (Koib[1].Main)
		  {
            if(PrModeChange==1 )    {     Sos_14();  PrModeChange=0; 	}
              // !!!!               //   �������  ������������� ( ������ �������� ������  �� ������  ������)
              // ������������  ������� � ��������� 14 ��� ������ ��������� "�������������"
/*!����!!!!*/ //  ��������  ������  ��  ������  ���� ( �����������) ���������� Votes  ���� TVotes (15 ����� ��������)
            char *ptrSend;
            ptrSend=(char *)&Votes;
             SendtoAnother(15,ptrSend,sizeof(Votes));
/*!����!!!!*/
		  } else
		  {  // ��������� "�������������..." ��  �����������  �������
		       st1=Out.Main[31];    Ind(st1," ");
		  }
		  //
		 IDsave(); //�������� ������ �� HDD � Avar
		 Koibsave(1);  // ������ ��������� ��������� ����
		 Koibsave(2);  // ������ ��������� ������������ ����
		 Koib[1].VS = 15;
         Sos_15();
		} break;
	case 15://---------------------------------������� � ����� ������������ ��� ������ �������?
		{  //  ������� � ����� ������������ ��� ������ �������?
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if (Votes.Nblank > 0)
			{
				if(PrModeChange==1 ) 	{   Sos_15();  PrModeChange=0; 	}
                if(pr_btn==1) //Yes
                { // �������  �  �����  ������������
                  Koib[1].VS = 23;       Sos_23();     PrNoBtnWait=1;
                }
                if(pr_btn==0) //No
                {   // ������� � ������ ���������� �  �����������
                    Xblank = 1;    for (int i = 1; i <=NumCandMax; i++)     Snat[i].Nsnat=0;
                    Koib[1].VS = 16;     Sos_16();        PrNoBtnWait=1;
                }
                pr_btn=-1;
			}
		  }else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1,"");
            }
		} break;
	case 16: //-----------------------------------------------������� ������� ��� ������� Xblank?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if ((Votes.Blank[Xblank].Vopr[1].NCandidates > 0)&&(! Votes.Blank[Xblank].Refer))
			{
			    if(PrModeChange==1 )   {    Sos_16();  PrModeChange=0; 	}

                if(pr_btn==1) //Yes
                {  // ������� �  ������ ������� ��� ��������� �������
                   Xcand = 1;
                   Koib[1].VS = 17;
                   Sos_17();
                   PrNoBtnWait=1;
                }
                if(pr_btn==0) //No
                {  //  ������� �  ���������  ������� ��� ������ �������
                   if (Xblank + 1 <= Votes.Nblank)
                   {
                        Xblank++;
                        				//Koib[1].VS = 16;
                        Sos_16();
                   }
                   else
                   {  //  ���  ������  ��������
                        Koib[1].VS = 19;
                        Sos_19();
                   }
                   PrNoBtnWait=1;
                }
                pr_btn=-1;
			} else
			{
				if (Xblank + 1 <= Votes.Nblank)
				{
					Xblank++;  						//Koib[1].VS = 16;
				    Sos_16();
                } else {
					Koib[1].VS = 20; // �� ������ ��
					Sos_20();
				}
			}
		  }else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1," ");
            }
} break;
	case 17: //---------------------------------------������� ������� Xcand ��� ������� Xvopr?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if (! Votes.Blank[Xblank].Vopr[1].Candidates[Xcand].disabled)
			{
			   if(PrModeChange==1 ) {  Sos_17();   PrModeChange=0; 	}

               if (pr_btn==1) //Yes
               {  //  ������  �������, ������� � ���������� ���������
                   Koib[1].VS = 18;
                   Sos_18();
                   PrNoBtnWait=1;
               }
               if (pr_btn==0) //No
               {   // ������� ��  ���������, �������  �  ��������� �������
                   if (Xcand + 1 <= Votes.Blank[Xblank].Vopr[1].NCandidates)
                   {
                      Xcand++; PrModeChange=1;
                   } else {

                         Koib[1].VS = 19;
                         Sos_19();
               /*      if (Xblank + 1 <= Votes.Nblank)
                     {
                         Xblank++;
                         Koib[1].VS = 16;
                         Sos_16();
                     } else {
                         Koib[1].VS = 19;
                         Sos_19();
                     }
                    */
                   }
                   PrNoBtnWait=1;
                } //No
                pr_btn=-1;
			} else {
				if (Xcand + 1 <= Votes.Blank[Xblank].Vopr[1].NCandidates)
				{
					Xcand++;
				} else {
					Koib[1].VS = 19;
					Sos_19();
				}
			}
		  }else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1," ");
            }
		} break;
	case 18: //------------------------------------------������� Xcand ��� ������� Xblank �����!
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			Snat[Xblank].Nsnat++;
			Snat[Xblank].Snat[Snat[Xblank].Nsnat]=Xcand;
            if(PrModeChange==1 )   {  Sos_18();   PrModeChange=0; 	}
			   if (Xcand + 1 <= Votes.Blank[Xblank].Vopr[1].NCandidates)
			   {  // ������� �  ����������  ���������
				  Xcand++;
				  Koib[1].VS = 17;
				  Sos_17();
 			   } else
 			   {  // �� ������ ������� ��� ���������  �����������,
 			       //����� ������ ������� �� ������ ������� � ������� �  ��������� �������
                    Koib[1].VS = 19;
                    Sos_19();
			   }
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1," ");
            }
		} break;
	case 19: //---------------------------------------------------------��� ������� ... ����� �������...!
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 )  {   Sos_19();  PrModeChange=0; 	}
            //  �����  ������  ������� ��� ������������ �������
			if (Xblank + 1 <= Votes.Nblank)
			{  // ������� �  ��������� �������
				Xblank++;
				Koib[1].VS = 16;
				Sos_16();
			}
			else
			{  // ��� ������ �����������, ������� �  ������
				Koib[1].VS = 20; // �� ������ ��
                Sos_20();
			}
		  }else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1," ");
            }
		} break;
	case 20: //---------------------------------------���������� ������������ �������� ������
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		     if(PrModeChange==1 )   {  Sos_20();  PrModeChange=0; 	}

               if(pr_btn==1) //Yes
               { // ������ � ������� ������
                have_Printer=1;
                if(have_Printer)
                   //if(CheckPrinter())
			    {	    Ind(Out.Main[114]," "); //  ����������
                        Print(1);
                        Ind(Out.Main[123],Out.Main[124]); //  ���������� ��  ������ // �����
                        //Koib[1].VS = 10;
                        //Sos_10();
                        Koib[1].VS = 21;
                        Sos_21();
                        //PrNoBtnWait=1;
			    } else
			    {  // ������� �� ������
			         Ind(Out.Main[125],Out.Main[126]); //  ������� �� ������
			    }
			      PrNoBtnWait=1;
               }
               if(pr_btn==0) //No
               {  // ������� ������  ��� ������
                   Koib[1].VS = 21;
                    Sos_21();
                 // PrNoBtnWait=1;
               }
               pr_btn=-1;

		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1," ");
            }
		} break;
	case 21: //---------------------------------------------------����������� ������ �������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		      if(PrModeChange==1 )  {   Sos_21();  PrModeChange=0; 	}

               if(pr_btn==1) //Yes
               {  // ������������  ������ �������,������� � ������������� ���������
                   Koib[1].VS = 22;
                   Speak(2);
                    usleep(200000);  Sos_22();
                   PrNoBtnWait=1;
               }
               if(pr_btn==0) //No
               { //  �� ����������� ������ �������, ������� � ������������� ���������
                   Koib[1].VS = 22;  Sos_22();  PrNoBtnWait=1;
               }
               pr_btn=-1;
		  }else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1," ");
            }
		} break;
	case 22: //----------------------------------------------------������������� ��������� � �������� ������?
		{
         if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		      if(PrModeChange==1 )   {   Sos_22();  PrModeChange=0; 	}

              if(pr_btn==1) //Yes
              {  //  ������������  ������  �������, �������� � �������� ������
                 for (int i=1; i <= Votes.Nblank; i++)
                 {
                   if (Snat[i].Nsnat>0)
                   {
                     for (int j=1; j<= Snat[i].Nsnat; j++)
                     {
                       Votes.Blank[i].Vopr[1].Candidates[Snat[i].Snat[j]].registered = false; //????
                       Votes.Blank[i].Vopr[1].Candidates[Snat[i].Snat[j]].disabled = true;    //????
                     }
                   }
                  }
                  Koib[1].VS = 15; PrNoBtnWait=1; PrModeChange=1;
               }
               if(pr_btn==0) //No
               {  // ������ ������� ��  ����������
                   for (int i = 1; i <=NumCandMax; i++)  Snat[i].Nsnat=0;
                    //Ind(Out.Main[45],""); //������ ������� �� ����������!
                    SaySys(62); //������ ������� �� ����������!
                    Koib[1].VS = 15;PrNoBtnWait=1; PrModeChange=1;
               }
               pr_btn=-1;
		  } else
            {  // ������  ��������� "����������" ���  ������������  �������
                 st1=Out.Main[114];    Ind(st1," ");
            }
		} break;
	case 23: //------------------------------------------------------������ ������ ������������
		{

          if (Koib[1].Main)
		  {	//������� �������� ������ �������

/*!����!!!!*/                // �������� ��������� 23 ��  �����������  ������
                char *ptrSend;
                ptrSend=(char *)&Koib[1].VS;
                SendtoAnother(1,ptrSend,sizeof(Koib[1].VS));
                // �������� �������� ������ ��  �����������  ������
                //  ���  ��������  �  ��������� 14
		  }
		  /*else
		  { //������ ����� ������, ���������� ������ ��� �������� � �������
		     // ReceiveFromAnother ������ HardProc.cpp
		  }
		  */
			//TekDateTime(); //������� ���� � �����

			Koib[1].TestV = true;  XVote=0; //Tect
			Koib[1].StacV = false;
			Koib[1].PortV = false;
			Koib[1].Opros = true;
			NumBullAll=0; // ��������� �������� ����������
			KoibZero(); //��������� ������������ ���������� �����
            if(PrModeChange==1 ) {     Sos_23();   PrModeChange=0;   }
			Koib[1].VS = 50; // ����-?
			PrApprove=0;
            frontLED(0,2);   // ��������� ������� �������� ���� ������� ������������ � ���������
			//st1=Out.Main[47]+" 0"; //����:...
           // Ind(Out.Main[102],st1); //����� �� ������ ������������ - ���

		} break;
	case 50: //----------------------------------------------------------��� ������ ������������
		{
            if(PrModeChange==1 )   {   Sos_50();  PrModeChange=0; 	}
	        if(pr_btn==0) //No
           {  // ���������  ���� �������� ����������
  	           Koib[1].VS = 24;    Sos_24();   pr_btn=-1;
  /*!����!!!!*/	              // �������� ��������� 23 ��  �����������  ������
                char *ptrSend;
                ptrSend=(char *)&Koib[1].VS;
                SendtoAnother(1,ptrSend,sizeof(Koib[1].VS));

  	       } else
  	       {  //�������� �� ����������� ��������� �����������!!!
               if((PrVoteTimeStart ==2)&&(PrVoteDay==1))
               {  // ��� ����������� ������� ����������� ���� ���������� � ������ ������
                  // �  �������������� ������� ��
                  if((pr_btn==1)||((PrApprove==1)&&(pr_opt==6)))//Yes
                  {    // ���� ���������� � ��������������
                        if (pr_btn==1) PrApprove=1;  else
                        {  // ������������� ����� ��������� ��������� ��� �����������
                            // ������� �������������  �����������
                            PrApprove=0; // ������� ������������� ����� ���  ���������� ���������
                            // ��������� �������� ����������
                            ObrTestBull();
                        }
                    }
                    if(pr_opt==100)
                    {   //-------------��������� �����, ��������� �������
                       SayVotes(6); // ������� �� ������ ���������
                       for(int i=0;i<4;i++) Optron[i]=0;
                    }
                    if((pr_opt>1)&&(pr_opt < 6))
                    {       //Ind(Out.Votes[7],""); //��������  ���������  ���������
                        // ��������� ���������
                        SayVotes(7); // �������� ��������� ����� ������� ����!
                        for(int i=0;i<4;i++) Optron[i]=0;
                        //sleep(1);
                    }
                } else
                {  // ����� ��� ���� �����������  ��� ��  ���������
                    // �������� �������� ����������� �����������
                    if(pr_opt==6) //Yes
                    {   // ��������� �������� ����������
                        ObrTestBull();
                    }
                    if(pr_opt==100)
                    {   //-------------��������� �����, ��������� �������
                        SayVotes(6); // ������� �� ������ ���������
                        for(int i=0;i<4;i++) Optron[i]=0;

                    }
                    if((pr_opt>1)&&(pr_opt < 6))
                    {       //Ind(Out.Votes[7],""); //��������  ���������  ���������
                        // ��������� ���������
                        SayVotes(7); // �������� ��������� ����� ������� ����!
                        for(int i=0;i<4;i++) Optron[i]=0;
                        //sleep(1);
                    }
                }
               //get_Optrons_Imm();
           }
             /*!!!!!!!!!!!!!!!!!!!!!!!*/
                if((logfR = fopen("./Log/Recogn.log","w"))==NULL)   printf("RecognLogFile open eroor!!");
                if(logfR!=NULL)
                { // ������ � ����  ������  ������������
                        fprintf(logfR," PrScan Error \n");
                        if((PrErrScan >0)&&(PrErrScan <100))
                        {  st1="";
                            switch(PrErrScan)
                            { // ������ �������������
                                case 1:     fprintf(logfR," ����������� ���� ��  �������� ���������� \n");  break;
                                case 10:    fprintf(logfR," �������� ����� ��������� �  ���� ������ \n");  break;
                                case 11:    fprintf(logfR," �������� ����� ��������� �  �����  ������ \n");  break;
                                case 12:    fprintf(logfR," �������� ����� ��������� �  ������ ������� \n");  break;
                                case 2:     fprintf(logfR," �� ���������  ���  ���������  \n");  break;
                                case 20:    fprintf(logfR," ��� ���������  ��  �������������  �������� ������ \n");  break;
                                case 21:    fprintf(logfR," ��  ��������� ���  �  �����  ��������� \n");  break;
                                case 22:    fprintf(logfR," �������� ����� ���������  �  ������ ����� \n");  break;
                                case 3:     fprintf(logfR," ���������� ������  ������ ���� ��������� \n");  break;
                                case 4:     fprintf(logfR," ��������� ��  �����  ������ �������� \n");  break;
                                case 41:    fprintf(logfR," ������� ������ ���������� ����� ��������� \n");  break;
                                case 42:    fprintf(logfR," ��������������  ����� \n");  break;
                                case 43:    fprintf(logfR," ��������������  �����  �������  ������ \n");  break;
                                case 44:    fprintf(logfR," ��������������  ������ ������� ������ \n");  break;
                                case 5:     fprintf(logfR," ��� ��������� � ������ � �  ����� ����� ��  ��������� \n");  break;
                                case 6:     fprintf(logfR," �� ������� ����� ��������� �  ����� �������� \n");  break;
                                case 7:     fprintf(logfR," ����� �������������� ������� ��  ������������� �������� ������ \n");  break;
                                case 71:    fprintf(logfR," �� ��������� ����� ��������������  ������� \n");  break;
                                case 8:     fprintf(logfR," ���������  ���  ����� \n");  break;
                            }// switch
                        }
                fclose(logfR);
                }




		} break;
	case 24: //------------------------------------------------------------����� �� ������ ������������?
		{
                if ((Koib[1].Main)||(Koib[1].Avto))
                {   if(PrModeChange==1 )   {    Sos_24();    PrModeChange=0; 	}
                    if(pr_btn==1) //Yes
                    { // ����������  ������������
                        Koib[1].TestV = false;			Koib[1].StacV = false;
                        Koib[1].PortV = false;         Koib[1].Opros = false;
                        Koib[1].VS = 25;
                        Sos_25();
                    }
                    if(pr_btn==0) //No
                    {  // ������� � ������������
                        Koib[1].VS = 50;
                    }
                    pr_btn=-1;
                } else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
		} break;
	case 25: //---------------------------------------------------����������� �������� ������������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 ) {    Sos_25();  PrModeChange=0; 	}
  	        if(pr_btn==1) //Yes
  	        {  Print(2);    Koib[1].VS = 26;   Sos_26(); 	        }
  	        if(pr_btn==0) //No
  	        { Koib[1].VS = 27;           Sos_27();  	       }
  	       pr_btn=-1;
		  }
		  else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
		} break;
	case 26: //-----------------------------------------------------����������� ����� ��������� ������������? ��/���
		{
         if ((Koib[1].Main)||(Koib[1].Avto))
          {
                if(PrModeChange==1 )      {     Sos_26();    PrModeChange=0; 	}
                if(pr_btn==1) //Yes
                { Print(2);    Sos_26();   }
                if(pr_btn==0) //No
                {  Koib[1].VS = 27;   Sos_27();  }
                pr_btn=-1;
		  } else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
		} break;
	case 27: //--------------------------------------------------------�������� �������� ������������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
          {
			if(PrModeChange==1 )	{    Sos_27();		PrModeChange=0; 	}
  	        if(pr_btn==1) //Yes
  	        {  Speak(3);  Koib[1].VS = 28;   Sos_28();         }
  	        if(pr_btn==0) //No
  	        {  Koib[1].VS = 29;   Sos_29(); 		    }
            pr_btn=-1;
		  }else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
		} break;
	case 28: //----------------------------------------------------------�������� �������� ������������ ��� ���?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
            if(PrModeChange==1 )  {  Sos_28();     PrModeChange=0; 	}
  	        if(pr_btn==1) //Yes
  	        {  Speak(3);   Sos_28();   PrNoBtnWait=1; 	        }
  	        if(pr_btn==0) //No
  	        {  Koib[1].VS = 29;  Sos_29(); PrNoBtnWait=1;       }
            pr_btn=-1;
		  }else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
		} break;
	case 29: //--------------------------------------------------------------������������ ����������� ��� ��������� ��?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
                if(PrModeChange==1 )  {    Sos_29();  PrModeChange=0; 	}
  	            if(pr_btn==1) //Yes
  	            {  // ������� � �������������  �����������
  	               if(PrVoteDay==1)
                   { //�������� ����.�����������
                       Koib[1].VS = 30;
                   } else { //����.�����������  ��  �  ���� ����������� (����. �����)
                       Koib[1].VS = 31;
                       Sos_31();
                   }
  /* ! !!*/                 // �������� ������� �������  ��������� �������������  ����������� Koib[1].VS;
 /*!����!!!!*/               // �������� ��������� 23 ��  �����������  ������
                char *ptrSend;
                ptrSend=(char *)&Koib[1].VS;
                SendtoAnother(1,ptrSend,sizeof(Koib[1].VS));
                   PrNoBtnWait=1;
               }
  	           if(pr_btn==0) //No
  	           {  // ������� �  �������� ������
  	               //Koib[1].VS =15;// 6;
  	              // PrModeChange=1;

                   Koib[1].VS =50; //  ������� � ��������� ���������� �  �������� ������
  	               PrNoBtnWait=1; PrModeChange=1;
			   }
			   pr_btn=-1;
		  } else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
	    } break;

	case 30: //-------------------------------------------------------------------����� �������� ��
		{
            if ((Koib[1].Main)||(Koib[1].Avto))
		  {
             if(PrModeChange==1 )  {   Sos_30();       PrModeChange=0; 	}
		     //  ������  ��������� " �� ������ �����������  �������� 10 �����"

             if(PrVoteTimeStart==1) { PrVoteTimeStart=11; Sos_30(); }
             if(PrVoteTimeStart==12) { PrVoteTimeStart=13; Sos_30(); }
		     if(PrVoteTimeStart==2)
			 {  // ����� ����������� ��������� �������  �  ����������� � ���� �����������
			    Koib[1].VS = 31;
			    Sos_31();
			    PrNoBtnWait=1;
/* ! !!*/                 // �������� ������� �������  ��������� �������������  �����������(31) Koib[1].VS;
 //              char *ptrSend;
 //               ptrSend=(char *)&Koib[1].VS;
   //             SendtoAnother(1,ptrSend,sizeof(Koib[1].VS));

			 }
		  }
		} break;
	case 31: //--------------------------------------------------------------------������������ �����������!
		{   if(PrModeChange==1 ){   Sos_31();		PrModeChange=0; 	}
			Koib[1].TestV = false; //XVote=1; //��
			Koib[1].StacV = true;
			Koib[1].PortV = false;
			Koib[1].Opros = true;
			KoibZero();
			NumBullAll=Koib[1].Rezult[0].All; // ��������� �������� ���������� 30_07_2013
			Koib[1].VS = 60;
            frontLED(0,2);   // ��������� ������� �������� ���� ������� ������������ � ���������
		}break;
	case 60: //--------------------------------------------------------------------��� ������������� �����������
		{
            if(PrModeChange==1 ){   Sos_60();		PrModeChange=0; 	}
            if(pr_btn==2) //����� ��  ������ ������������� ����������� �������������� ��+���
  	        {
  	            if ((Koib[1].Main)||(Koib[1].Avto))
  	            {  // ���������� ����� ���� ������ ��  �������� ������� ��� �  ����������  ������
                    if((PrVoteTimeEnd==2)||(PrVoteDay!=1))
                    {  	Koib[1].VS = 33; //�������������� �����?
                        Sos_33();
                        PrNoBtnWait=1;
                    }
                    if((PrVoteTimeEnd!=2)&&(PrVoteDay==1))
                    {   // ����� ������������� ����������� ��� �� �������
                        // �������  ������������� ����� ��  ������ �� ���������  �������
                        // ����������
                        TekDateTime(); //������� ���� � �����
                        int ho=Votes.Bhour[2]-TekDT.hour-1;
                        int mi=TekDT.minit;
                        //int Minuts=(Votes.Bhour[2]-TekDT.hour)*60- 60+TekDT.minit;
  /*!!!*/	        //st2=Out.Main[106]+" "+IntToStr(Minuts)+" "+Out.Main[101]; Ind("",st2);
                        // ���������  ��������� �� ����
                        Golos="";  Golos=OutV.MainPath[74]; SayNSys(Golos);// ��  ���������  ����������� ��������
                        ReadTime(0,ho,mi); //  **  ����� **  �����
                    }
  	            }
               pr_btn=-1;
 	        } else
 	        {  //  ��������� ��  ���������  ����������� ����� 10 �����
 	            if ((Koib[1].Main)||(Koib[1].Avto))
 	            {
 	                 if(PrVoteTimeEnd==1)
                    {   PrVoteTimeEnd=11; PrVoteTimeEnd=2;
                       //st1=Out.Main[106]+" 10 "+Out.Main[101]; Ind(st1,"");
                     // ���������  ��������� � ���, � �� �� ����� ����������� �������� 10 �����
                        Golos="";
                        Golos=OutV.MainPath[74]+" "+ OutV.NumPath[29]+" "+ OutV.MainPath[72];// 10 �����
                        SayNSys(Golos);
                    }
                    if(PrVoteTimeEnd==12)
                    {   PrVoteTimeEnd=13; PrVoteTimeEnd=2; Ind(Out.Main[106],"");
                        //st1=Out.Main[106]+" 5 "+Out.Main[101];   Ind(st1,"");
                        // ���������  ��������� � ���, � �� �� ����� ����������� �������� 5 �����
                        Golos="";
                        Golos="";
                        Golos=OutV.MainPath[74]+" "+ OutV.NumPath[24]+" "+ OutV.MainPath[72];// 5 �����
                        SayNSys(Golos);
                    }
                    // st=Out.Main[57]+TimeS; // ������������ �����������! ����� *.*
 	            }
                if((PrVoteTimeEnd==2)&&(PrVoteDay==1))
                {  // ����� ���� �����������   ������� ( �  ���� �����������)
                    //st1=Out.Main[59];   Ind(st1,"");
                    if ((Koib[1].Main)||(Koib[1].Avto))
                    {
                        Koib[1].VS = 32;
                        Sos_32();
                        SaySys(41);   SaySys(42);
                        PrApprove=0;
                        cout << " Vote Time is over"<< endl;
                    }
                } else
                { // ���������� ����� �����������
				   if(pr_opt==6)
				  { // ��������� �������� ���������
                    SayVotes(1);  // ������������
                    Golosovanie();
                    if(PrStopScan==2)SayVotes(8);// �������
				  }
				  if(pr_opt==100)
                    {   //-------------��������� �����, ��������� �������
                        SayVotes(6); // ������� �� ������ ���������
                        for(int i=0;i<4;i++) Optron[i]=0;

                    }
                    if((pr_opt>1)&&(pr_opt < 6))
                    {       //Ind(Out.Votes[7],""); //��������  ���������  ���������
                        // ��������� ���������
                        SayVotes(7); // �������� ��������� ����� ������� ����!
                        for(int i=0;i<4;i++) Optron[i]=0;
                        //sleep(1);
                    }
                }
            }
                          // get_Optrons_Imm();
		} break;
	case 32: //--------------------------��������� ������������� �����������
		{
		if(PrModeChange==1 )	{   Sos_32();	PrModeChange=0; 	}

  	        if((pr_btn==1)||((PrApprove==1)&&(pr_opt==6)))//Yes
  	        { // ������������� ������ ��������� ����� ��������� ������� �����������
  	           // Ind(Out.Votes[1],""); // ������������
				  // ���������  ���������, "������������!"
                //waitBothOpt();// �������� ������ �����
                if(pr_btn==1)
                { PrApprove=1 ; } else
                {  // ����� ���������
                    PrApprove=0;
                   SayVotes(1); // ������������
  	               Golosovanie();
                           if(PrStopScan==2)SayVotes(8);// �������
                }
  	        }
  	        if(pr_opt==100)
            {   //-------------��������� �����, ��������� �������
                SayVotes(6); // ������� �� ������ ���������
                for(int i=0;i<4;i++) Optron[i]=0;

            }
            if((pr_opt>1)&&(pr_opt < 6))
            {  //Ind(Out.Votes[7],""); //��������  ���������  ���������
                // ��������� ���������
                SayVotes(7); // �������� ��������� ����� ������� ����!
                for(int i=0;i<4;i++) Optron[i]=0;

            }
            if (pr_btn==2) //No
            {  // ���������  ������������  �����������
                Koib[1].VS = 33;
                Sos_33();
                PrNoBtnWait=1;
                Koib[1].VS = 33; //�������������� �����?
            }
                             //  get_Optrons_Imm();
 	        pr_btn=-1;
		} break;
	case 33: //---------������� �  �����������  ����������� ��� ������� �  �������������
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 )		{    Sos_33();	PrModeChange=0; 	}
			if(pr_btn==1) //Yes // ������������� �������� � �����������  �����������
			{     Koib[1].VS = 34;		   /*Sos_34();	*/   PrNoBtnWait=1;

  /*!����!!!!*/              // �������� ��������� 34 ��  �����������  ������
                char *ptrSend;
                ptrSend=(char *)&Koib[1].VS;
                SendtoAnother(1,ptrSend,sizeof(Koib[1].VS));
			}
			if(pr_btn==0) //No
  	        {
  	           Koib[1].VS = 32;
  	           Sos_32();
  	           PrNoBtnWait=1;
			}
			pr_btn=-1;
		  } else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
		} break;
	case 34: //--------------------------����� ����������� �����������!
		{
			if(PrModeChange==1 ){   Sos_34(); PrModeChange=0; 	}
			Sos_34();
			Koib[1].TestV = false; //XVote=2; //���������� �����������
			Koib[1].StacV = false;
			Koib[1].PortV = true;
			Koib[1].Opros = true;
            //KoibZero();
			Koib[1].VS = 70;
		} break;
	case 70: //------------------------------------------------------------
	    {  //���� ���������� �����������
	       if(PrModeChange==1 ){   Sos_34(); PrModeChange=0; 	}
	        if(pr_btn==2) //Yes+No ����� �� ������ �����������  �����������
  	        {
  	           Koib[1].VS = 35; // ��������� ����������  �����������
  	           Sos_35();
 	        } else
            {   if(pr_opt==6) //Yes
                {      //SayVotes(1);  // ������������
                        Golosovanie();
                         //       if(PrStopScan==2)SayVotes(8);// �������
                }
                if(pr_opt==100)
                {   //-------------��������� �����, ��������� �������
                    SayVotes(6); // ������� �� ������ ���������
                    for(int i=0;i<4;i++) Optron[i]=0;

                }
                if((pr_opt>1)&&(pr_opt < 6))
                {  //Ind(Out.Votes[7],""); //��������  ���������  ���������
                    // ��������� ���������
                    SayVotes(7); // �������� ��������� ����� ������� ����!
                    for(int i=0;i<4;i++) Optron[i]=0;
                }
             }
                           // get_Optrons_Imm();
             pr_btn=-1;
	    } break;

	case 35: //------------------------���������� ����������� �����������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 ){    Sos_35();	PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{   // ������������� ���������  �����������  �����������
			    Koib[1].Opros = false;
                Koib[1].VS = 36;
                Sos_36();
   /*!����!!!!*/                // �������� ��������� 36 ��  �����������  ������
                char *ptrSend;
                ptrSend=(char *)&Koib[1].VS;
                SendtoAnother(1,ptrSend,sizeof(Koib[1].VS));

                //XVote=1; //����� ������� = 1
			}
  	    	if(pr_btn==0) //No
  	        {  // ������� �  �����������  �����������
  	           //Koib[1].VS = 70;
  	           Koib[1].VS = 34; PrModeChange=1; PrNoBtnWait=1;
			}
            pr_btn=-1;
		  } else {
                //  ������� ����������
                std::string st1;
                if (NumBullAll >=0 )    {  st1=Out.Main[63]+" "+IntToStr(NumBullAll);          Ind(st1," ");       }
		  }
		} break;
	case 36: //---------------------������� � ���������� ������ �����������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 ) 	{  Sos_36(); PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{   //  ������ �����������  ����������� �� ���� �������
                for (int i=1; i<=Votes.Nblank; i++)
                {
                 XVote=i; Print(3);
                }
                SavePreRezult();

                XVote=1;
                Koib[1].VS = 37;
                Sos_37();

/*!����!!!!*/                          // �������� ��������� 36 ��  �����������  ������
                char *ptrSend;
                ptrSend=(char *)&Koib[1].VS;
                SendtoAnother(1,ptrSend,sizeof(Koib[1].VS));


			}
  	    	if(pr_btn==0) //No
  	        {   // ������� � �����������  �����������
  	            //Koib[1].VS = 70;
  	            Koib[1].VS = 34; PrModeChange=1; PrNoBtnWait=1;
			}
			pr_btn=-1;
		  } else {
                  //  ������� ����������
                std::string st1;
                if (NumBullAll >=0 )    {  st1=Out.Main[63]+" "+IntToStr(NumBullAll);          Ind(st1," ");       }
		  }
		} break;
	case 37: //------------------���������� �����  ���������  �����������  �����������
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
             if(PrModeChange==1 )  {   Sos_37(); PrModeChange=0; 	}
			 if(pr_btn==1) //Yes
			 {
			    //���������� ����� ��������� ����������� ����������� �� ������� XVote
                for (int i=1; i<=Votes.Nblank; i++)
                {
                 XVote=i; Print(3);


                }
                Sos_37();
                PrNoBtnWait=1;
                XVote=1;
			 }
  	    	 if(pr_btn==0) //No
  	         {
  	            Koib[1].VS = 38;
  	            Sos_38();
  	            PrNoBtnWait=1;
  	            XVote=1;
 	         }
 	         pr_btn=-1;
		  } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
	case 38: //-----------------------���������� �������� ����������!
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
            if(PrModeChange==1)  {  Sos_38();   PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{  //  �������� �����������  ��������  ����������

			    if(PrKlav==1)
			    {  // ���������� ����������

			     //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			        for (int i=1; i<=Votes.Nblank; i++)
			        {
			            MakeProtokol(i,1);
			            XVote=i; XVopr=1;
			            Print(5);
			            SaveRezult(XVote);
			        }
			        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


			       	Koib[1].VS = 39;
			       	XVote=1; XVopr=1;
			        Sos_39();
 			    }else
			    {  // ��������  ���������� �� ����������
			       Ind("",Out.Main[104]); //����������
			       Sos_38();
			    }
			    //PrNoBtnWait=1;
			}
  	    	if(pr_btn==0) //No
  	        {
  	        /*    Koib[1].VS = 41;
  	            Sos_41();
  	            PrNoBtnWait=1;*/
 	        }
 	        pr_btn=-1;

		  } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
	case 39: //------------------------�������� ���������� �� ������� Xvote?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		     if(PrModeChange==1 )  {    Sos_39();  PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{
               Speak(4);
			   Koib[1].VS = 40;
			   Sos_40();
			}
  	    	if(pr_btn==0) //No
  	        {
  	           Koib[1].VS = 41;
  	           Sos_41();
			}
            pr_btn=-1;
		  } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
	case 40: //-------------------�������� ���������� �� ������� Xvote ��� ���?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		    if(PrModeChange==1 ) 	{    Sos_40(); PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{
                Speak(4);
			   	Sos_40();
		    }
  	    	if(pr_btn==0) //No
  	        {   // ������� � ����� �������������� ��������
  	            Koib[1].VS = 41;
                Sos_41();
			// ���������� �  ������ ������ �  ��������� ���������
			}
            pr_btn=-1;
		  } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
	case 41: //------------------���� �������������� �������� �� ������� Xvopr
		{  // ����  �������������� ��������
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		     PrKC=0; // �������  ��������  ����������  �����������  �����������
		     if (PrModeChange==1)   {   Sos_41(); PrModeChange=0; 	}
			 if(pr_btn==1) //Yes
			 {
                 Xv=0; //��������� �������� ����� ��������� ������� XVote ������� XVopr � 0
                 Koib[1].VS = 42;
                 Sos_42();
                 PrNoBtnWait=1;
                 PrEnterStart=0;
			}
            pr_btn=-1;
		  } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
	case 42: //----------------------------------------------���������� ������ XVopr ��������� ����������� �� ������� XVote
		{
            if ((Koib[1].Main)||(Koib[1].Avto))
            {   int zz=0;
                if(PrModeChange==1) {  Sos_42(); PrModeChange=0; 	}
                if(pr_btn==3) //  ������ ? ��  ����������
                { // ����������� ��  ���������� ������
                  if(Xv > 0){ Xv=Xv-1;   Sos_42();   PrEnterStart=0;}
                } else
                {  st2=IntToStr(Votes.Blank[XVote].Vopr[1].Protocoles[1].Lines[Xv].Value);
                   Ind("",st2); // ������ ��������  ��  �����
                   *NUMBER=st2[0];
                   strcat(NUMBER,"\0"); // ��������� ������ ������ ���� ����� ����
                   if((pr_btn==0)||(PrEnterStart==1))
                    {  // ���������� ���������� ��������
                        if(PrEnterStart==1)  //((pr_btn!=0)&&(pr_btn!=3))
                        {
                            get_KeyString(NUMBER);
                            Ind("",NUMBER); // ������ �������� ����� �����
                            zz=atof(NUMBER);
                            Votes.Blank[XVote].Vopr[1].Protocoles[1].Lines[Xv].Value=zz;
                            PrEnterStart=0;
                            if (NextStrokaProt()) // ���������� ��������� �������������� ������ ���������, ���� ��� - ��������� ������
                            { // �������  �  ��������� ������
                                Sos_42();  //PrEnterStart=1;
                                PrNoBtnWait=0;
                            } else
                            {  // ������������  ��������� �  ���������  ����������  �����������  �����������
                                PrKC=MakeProtokol(XVote,Xv);
PrKC=1;
                                Koib[1].VS=44;
                                Sos_44();
                                PrEnterStart=0;PrNoBtnWait=1;
                            }
                        }else
                        {
                            PrEnterStart=1;PrNoBtnWait=1;
                        }
                        pr_btn=-1;
                    };
                    if(pr_btn==1)
                    {   if (NextStrokaProt()) // ���������� ��������� �������������� ������ ���������, ���� ��� - ��������� ������
                        { // �������  �  ��������� ������
                            Sos_42();  //PrEnterStart=1;
                            PrNoBtnWait=0;
                        } else
                        {  // ������������  ��������� �  ���������  ����������  �����������  �����������
                           PrKC=MakeProtokol(XVote,XVopr);
PrKC=1;
                           Koib[1].VS=44;
                           Sos_44();
                           PrEnterStart=0;PrNoBtnWait=1;
                        }
                    }

             /*       if(pr_btn==0) //No
                    {  // ����� �� ����� ���. �������� ��� ������ �������
                        Koib[1].VS = 44;
                        Sos_44();
                        PrEnterStart=0;
                    }
                    */
                    pr_btn=-1;
                }
            } else
            {
		    Ind(Out.Main[84]," "); //����������  ������
            }
		} break;
	case 43: //----------------------------------------------Rezerv
		{

		} break;
	case 44: //---------------------------------------------------------------������ ����������� �����������
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		     if(PrModeChange==1) { Sos_44();  PrModeChange=0; 	}
            if(PrKC==0)
            {// �����������  �����������  �� ���������
                Print(4); // ������ ��� ������������ ��
                if(pr_btn==1) //Yes
                {   // ������� � ����������  ����� �������������� ��������
                    Koib[1].VS = 41;
                    Sos_41();
                    PrNoBtnWait=1;
                }
            } else
			{   //  ����������  �����������  ���������
/*!			    for (int i=1; i<=Votes.Nblank; i++)
                {
                  XVote=i;  Print(5);
                }
*/
                Koib[1].VS = 45;
                // ����������  ��������� 2  ���������� ���������
                Print(5); Print(5);
                Sos_45();
                PrNoBtnWait=1;
			}
            pr_btn=-1;
	      } else {
		    Ind(Out.Main[84],""); //����������  ������
		  }
		} break;
	case 45: //-------------�������� �������� ����������� ��������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 ) {  Sos_45(); 	PrModeChange=0; 	}
	        if(pr_btn==1) //Yes
			{
			    Koib[1].VS = 46;
			    Sos_46();
			    PrNoBtnWait=1;
			}
			if(pr_btn==0) //No
  	        {
  	            Koib[1].VS = 41;
  	            Sos_41();
  	            PrNoBtnWait=1;
  	        }
            pr_btn=-1;
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
	case 46: //---------����������� ����� ��������� ��������� �����������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		      if(PrModeChange==1 ) { Sos_46(); PrModeChange=0; 	}
			 if(pr_btn==1) //Yes
			 {//  ������� �  ������  �����  ���������
			     Koib[1].VS = 47;
			     Sos_47();
			     PrNoBtnWait=1; PrEnterStart=1;
			 }
  	    	 if(pr_btn==0) //No
  	         { // �������  � ���������� ���� ������� ��� ��������� ���������� ������
  	             Koib[1].VS = 48;
  	             PrNoBtnWait=1;
 	         }
 	         pr_btn=-1;
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
	case 47: //---------���-�� ����� ��������� ��������� �����������...
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
		      if(PrModeChange==1) {  Sos_47();  PrModeChange=0; 	}

			if((pr_btn==1)||(PrEnterStart==1))
			{   // ���� ���������� �����  ��������� �� ������� �
			    strcpy(NUMBER,"\0");
                get_KeyString(NUMBER);
                int zz=atof(NUMBER);
/*! ������ �����  ����������  �� *.pdf  ����� ����� Protokols    */                //���������� N ����� ��

               for(int i=1; i<=zz;i++)Print(5);

/*! ������ �����  ����������      */
                PrEnterStart=0;
                Koib[1].VS = 48;
                st=Out.Main[93]+" "+IntToStr(XVote)+" "+Out.Main[94];
			    //���_����������_������_��_�������_N_XVote+1_�������_��
			    st1=" ";
			    Ind(st,st1);
			}
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		}// break;

	case 48: //------------------------
		{
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  { SaveRezult(XVote); //  ���������� �����������  �����������

			if (XVote< Votes.Nblank)
			{  // �������  �  ����������  ����  �������

			    if(XVopr< Votes.Blank[XVote].NVopr)
			    {  //  �������  �  ����������  �������
                    XVopr++;
			    } else
			    {  // ������� �  ��������� �������
			        XVote++;
			    }

			    Koib[1].VS = 39;
			    PrNoBtnWait=1;
			    PrModeChange=1;
		    } else
		    {   // ��� ���� ������� ���������� ��� ���������� ������ �����������
		        st=Out.Main[95];  //��� �������� � ���������� ������ ������� ��
			    st1=Out.Main[96]; //���_����������_�����_�����������_�_������_������_�����_�������_�������_���
			    Ind(st,st1);
			    SaySys(70);
                Koib[1].VS =49;
			    PrNoBtnWait=1;
			}
	      } else
	      {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
		case 49: //------------������ ����������� ����������� �� ������� XVote �� ����
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  { // ������ ����������� � ������� � ����� �������
            if(pr_btn==1) //Yes
			{  // ������� � ���������� ������ �����������
			    Koib[1].VS = 35;    PrNoBtnWait=1; PrModeChange=1;
			}
  	    	if(pr_btn==0) //No
  	        {  // ���������� ����������� ������� �������
  	            Sos_49(); // ������ ���������  �  ������  �����������  �� ����-�����
                AvarToTrash(); //�������� ��������� ������
                //  ���������� �����������  ����������� ��  ����
                   system("mkdir /mnt/usb/result"); // �������� ����� ��  ���������
/* ������ ��  ��������� */                system("cp -a result  /mnt/usb/result");

  	            Ind(Out.Main[92],""); //����������_��������_��_Flash
                Ind(Out.Main[92]," ");
                SaySys(69); // ���������� ���������!
  	            // ��������� � ������������� ��������  ���������� � �����
                UMount("/mnt/usb"); // ���������� ����-�����
  	            Koib[1].VS = 51;
                st1=Out.Main[109];        Ind(st1," ");        // ������� ���������� � ����
                SaySys(76);
 	        }
 	        pr_btn=-1;
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
		case 51: //------------������ ����������� ����������� �� ������� XVote �� ����
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  { // ������������� ����, ��� ���������� � �����  ������
            if(pr_btn==1) //Yes
			{  // ������� � ���������� ������ �����������
			    Koib[1].VS = 52;
 	            st2=Out.Main[107] ; st2=Out.Main[108]; Ind(st1, st2); // ����� ���� - ��, ������� � ���������� ������ - ���
  	            SaySys(75);
			}
 	        pr_btn=-1;
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
		case 52: //----------����� ���������� ���������� � ������������ ������
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  { // ������ ����������� � ������� � ����� �������
            if(pr_btn==1) //Yes
			{  // ������� � ����������  ��������� ����� ��������� ����� �����������
			    Koib[1].VS = 1;
 	            PrNoBtnWait=1;
 /* !!!! */ // �������� �  �����������  ������  ��������� ��������� ��������


			}
  	    	if(pr_btn==0) //No
  	        {  // ��� �������� � ���������� ������ ����������� ������� ��,
  	            // ��� �������� � ����������� ������� ���
  	            Koib[1].VS = 36;
  	            PrModeChange=1;
			    //Ind(Out.Main[66],Out.Main[67]);
			    PrNoBtnWait=1;
 	        }
 	        pr_btn=-1;
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;

  case 500: //------------------------
		{  Exit=1; Koib[1].VS =1;
		   PrNoBtnWait=1;
		} break;

	}
  return Exit;
}

//----------------------��������� �������� ������� MainTimer--------------------

/*
//-------------------------���������� �� ������� MainTimer----------------------
void __stdcall TimerStop(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	RisDateTime();
}
void SetMainTimer(void)
{
  SetTimer(hWnd, MainTimer, 1000, TimerStop);
}
*/

// ---------------------------------------RUN !!!--------------------------------
void Start(bool S)
{
  if (S)
  {
    Fi.open("./Log/Ind.log", ios::out); //Log-����
    Fv.open("./Log/Voice.log", ios::out); //Log-����
  } else {
    Fi.close();
    Fv.close();
  }

}

//---------------------�������������� ����� � ������� ��� ����---------------------------
/*
���
������
������
���������
�������
��������
�������
���������
���������
��������
��������
�����
���������
����������
���������
�����������
���������
����
����
���
���
���
������
����
�����
����
������
������
������
�����������
����������
����������
������������
����������
�����������
����������
������������
������������
���������
�������
��������
���������
�����
������
������
�����
����

one hundred
two hundred
three hundred
four hundred
five hundred
six hundred
seven hundred
eith hundred
nine hundred
twenty
thirty
fourty
fifty
sixty
seventy
eithy
ninety
one
one
two
two
three
four
five
six
seven
eith
nine
ten
illeven
twelve
thirteen
thorteen
thifteen
sixteen
seventeen
eiteen
nineteen
millions
millions
millions
millions
thousand
thousand
thousand
thousand
zero

*/

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
std::string dtx1(int m1 , int m2, int m3, int flag, bool String)
{
    std::string str="";

    switch(m1)
    {
        case 1:
            if (String) {str+=Nst[1]+" ";} else {str+=Nvc[1]+" ";}
			break;
		case 2:
            if (String) {str+=Nst[2]+" ";} else {str+=Nvc[2]+" ";}
            break;
        case 3:
            if (String) {str+=Nst[3]+" ";} else {str+=Nvc[3]+" ";}
			break;
        case 4:
            if (String) {str+=Nst[4]+" ";} else {str+=Nvc[4]+" ";}
            break;
        case 5:
            if (String) {str+=Nst[5]+" ";} else {str+=Nvc[5]+" ";}
            break;
        case 6:
            if (String) {str+=Nst[6]+" ";} else {str+=Nvc[6]+" ";}
            break;
        case 7:
            if (String) {str+=Nst[7]+" ";} else {str+=Nvc[7]+" ";}
            break;
        case 8:
            if (String) {str+=Nst[8]+" ";} else {str+=Nvc[8]+" ";}
            break;
        case 9:
            if (String) {str+=Nst[9]+" ";} else {str+=Nvc[9]+" ";}
            break;
    }
    if( m2 == 0 || m2 > 1 )
    {
        if( m2 > 1 )
        {
            switch( m2 )
            {
                case 2:
					if (String) {str+=Nst[10]+" ";} else {str+=Nvc[10]+" ";}
                    break;
                case 3:
                    if (String) {str+=Nst[11]+" ";} else {str+=Nvc[11]+" ";}
                    break;
                case 4:
                    if (String) {str+=Nst[12]+" ";} else {str+=Nvc[12]+" ";}
                    break;
                case 5:
                    if (String) {str+=Nst[13]+" ";} else {str+=Nvc[13]+" ";}
                    break;
                case 6:
                    if (String) {str+=Nst[14]+" ";} else {str+=Nvc[14]+" ";}
                    break;
                case 7:
                    if (String) {str+=Nst[15]+" ";} else {str+=Nvc[15]+" ";}
                    break;
                case 8:
                    if (String) {str+=Nst[16]+" ";} else {str+=Nvc[16]+" ";}
                    break;
                case 9:
                    if (String) {str+=Nst[17]+" ";} else {str+=Nvc[17]+" ";}
                    break;
            }
        }
        switch( m3 )
        {
            case 1:
                  if( flag == 2 )
                  {
                      if (String) {str+=Nst[18]+" ";} else {str+=Nvc[18]+" ";}
                  } else {
                      if (String) {str+=Nst[19]+" ";} else {str+=Nvc[19]+" ";}
                  }

                break;
            case 2:
                  if( flag == 2 )
                  {
                      if (String) {str+=Nst[20]+" ";} else {str+=Nvc[20]+" ";}
                  } else {
                      if (String) {str+=Nst[21]+" ";} else {str+=Nvc[21]+" ";}
                  }

                break;
            case 3:
                if (String) {str+=Nst[22]+" ";} else {str+=Nvc[22]+" ";}

                break;
            case 4:
                if (String) {str+=Nst[23]+" ";} else {str+=Nvc[23]+" ";}

                break;
            case 5:
                if (String) {str+=Nst[24]+" ";} else {str+=Nvc[24]+" ";}

                break;
            case 6:
                if (String) {str+=Nst[25]+" ";} else {str+=Nvc[25]+" ";}

                break;
            case 7:
                if (String) {str+=Nst[26]+" ";} else {str+=Nvc[26]+" ";}

                break;
            case 8:
                if (String) {str+=Nst[27]+" ";} else {str+=Nvc[27]+" ";}

                break;
            case 9:
                if (String) {str+=Nst[28]+" ";} else {str+=Nvc[28]+" ";}

                break;
        }
    }
    else if( m2 == 1 )
    {
        switch( m3 )
        {
            case 0:
                if (String) {str+=Nst[29]+" ";} else {str+=Nvc[29]+" ";}
                break;
            case 1:
                if (String) {str+=Nst[30]+" ";} else {str+=Nvc[30]+" ";}
                break;
            case 2:
                if (String) {str+=Nst[31]+" ";} else {str+=Nvc[31]+" ";}
                break;
            case 3:
                if (String) {str+=Nst[32]+" ";} else {str+=Nvc[32]+" ";}
                break;
            case 4:
                if (String) {str+=Nst[33]+" ";} else {str+=Nvc[33]+" ";}
                break;
            case 5:
                if (String) {str+=Nst[34]+" ";} else {str+=Nvc[34]+" ";}
                break;
            case 6:
                if (String) {str+=Nst[35]+" ";} else {str+=Nvc[35]+" ";}
                break;
            case 7:
                if (String) {str+=Nst[36]+" ";} else {str+=Nvc[36]+" ";}
                break;
            case 8:
                if (String) {str+=Nst[37]+" ";} else {str+=Nvc[37]+" ";}
                break;
            case 9:
                if (String) {str+=Nst[38]+" ";} else {str+=Nvc[38]+" ";}
                break;
        }
    }
    if( flag == 1 )
    {
        if( m3 == 0 && m2 == 0 && m1 == 0 ) str+="";
        else if( m2 == 1 )
        {
            if (String) {str+=Nst[39]+" ";} else {str+=Nvc[39]+" ";}
        }
        else
        {
         if( m3 == 1 )
         {
           if (String) {str+=Nst[40]+" ";} else {str+=Nvc[40]+" ";}
         } else {
           if( m3 >= 2 && m3 <= 4 ) if (String) {str+=Nst[41]+" ";} else {str+=Nvc[41]+" ";}
           else if (String) {str+=Nst[42]+" ";} else {str+=Nvc[42]+" ";}
         }
        }
    }
    else if( flag == 2 )
    {
        if( m3 == 0 && m2 == 0 && m1 == 0 ) str+="";
        else if( m2 == 1 )
        {
            if (String) {str+=Nst[43]+" ";} else {str+=Nvc[43]+" ";}
        }
        else
        {

                  if( m3 == 1 ) if (String) {str+=Nst[44]+" ";} else {str+=Nvc[44]+" ";}
                  else
                  {
                      if( m3 >= 2 && m3 <= 4 )
                      {  if (String) {str+=Nst[45]+" ";} else {str+=Nvc[45]+" ";}
                      } else if (String) {str+=Nst[46]+" ";} else {str+=Nvc[46]+" ";}
                  }

        }
    }
    else
    {
		if( m3 == 0 && m2 == 0 && m1 == 0 )
		{
		  if (String) {str+=Nst[47]+" ";} else {str+=Nvc[47]+" ";}
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
std::string CisloToStringOrVoice(double Cislo, bool String)
{
	int m1, m2, m3;
	int sum=Cislo;
	std::string str="";

	m1=sum/100000000; //100 ���������
	m2=sum/10000000-(sum/100000000)*10;
	m3=sum/1000000-(sum/10000000)*10;
	str=str+dtx1(m1, m2, m3, 1, String);

	m1=sum/100000-(sum/1000000)*10;
	m2=sum/10000-(sum/100000)*10;
	m3=sum/1000-(sum/10000)*10;
	str=str+dtx1(m1, m2, m3, 2,String);

	m1=sum/100-(sum/1000)*10;
	m2=sum/10-(sum/100)*10;
	m3=sum/1-(sum/10)*10;
	str=str+dtx1(m1, m2, m3, 3,String);

	return str;
}
//----------------------------------------Date To String or Voice------------------------------------------
/*
������
�������
�����
������
���
����
����
�������
��������
�������
������
�������
����
���
����
�����
������
�����
�������
������

*/
std::string DateToStringOrVoice(int year,int month,int day, bool String)
{
std::string Date=IntToStr(day)+" ";

	if (String) { Date=IntToStr(day)+" "; } else
	            { Date=CisloToStringOrVoice(day,String)+" "; }


	if (String) { Date=Date+Mst[month]+" "; } else
	            { Date=Date+Mvc[month]+" "; }

    if (String) { Date=Date+IntToStr(year)+" "+Mst[13]; } else
                { Date=Date+CisloToStringOrVoice(year,String)+" "+Mvc[13]; }

   return Date;
}
//--------------------------------------------------------------------------------------------------------------
std::string TimeToStringOrVoice(int Chas,bool C,int minit,bool M,int sec,bool S,bool String)
{
  std::string str="";
  std::string st="";
  std::string st1="";

if (C)
{
  if ((Chas==1)||(Chas==21))
  {
    if (String) { st=Mst[14]; } //Mst[14]
           else { st=Mvc[14]; }
  }
  if (((Chas>=2)&&(Chas<=4)) || ((Chas>=22)&&(Chas<=24)))
  {
    if (String) { st=Mst[15]; } //Mst[15]
           else { st=Mvc[15]; }
  }
  if ((Chas>=5)&&(Chas<=20))
  {
    if (String) { st=Mst[16]; } //Mst[16]
           else { st=Mvc[16]; }
  }
  st1=CisloToStringOrVoice(Chas, String);
  str=st1+" "+st+" ";

}
if (M)
{
   if ((minit==1)||(minit==21)||(minit==31)||(minit==41)||(minit==51))
   {
    if (String) { st=Mst[17]; } //Mst[17]
           else { st=Mvc[17]; }
   }
   if ( ((minit>=2)&&(minit<=4))||((minit>=22)&&(minit<=24))||((minit>=32)&&(minit<=34))||
        ((minit>=42)&&(minit<=44))||((minit>=52)&&(minit<=54)) )
   {
    if (String) { st=Mst[18]; } //Mst[18]
           else { st=Mvc[18]; }
   }
   if ( (minit==0)||((minit>=5)&&(minit<=20))||((minit>=25)&&(minit<=30))||((minit>=35)&&(minit<=40))||
        ((minit>=45)&&(minit<=50))||((minit>=55)&&(minit<=60)) )
   {
    if (String) { st=Mst[19]; } //Mst[19]
           else { st=Mvc[19]; }

   }
   st1=CisloToStringOrVoice(minit, String);
   str+=st1+" "+st+" ";
}
if (S)
{

}

return str;

}


//--------------------------------------------------------------------------------------------------------
bool RUN(void)
{  // �������� ����������� ��������� ������
  std:: string sst;

  /*Parser("./SourceDir/SourceData.XML");
  MakeProtokol(1,1);
  XVote=1;
  Print(5);
*/
  /*
  Votes.Nuik=1256;
  Votes.Nblank=5;
  Votes.DateV="12.03.2013";  Votes.DBday=12; Votes.DBmonth=3; Votes.DByear=2013;
  Votes.TimeB[1]="9:00:00";  Votes.Bhour[1]=9; Votes.Bmin[1]=0;
  Votes.TimeB[2]="16:00:00"; Votes.Bhour[2]=16; Votes.Bmin[2]=0;

  Votes.Blank[1].Refer=true;  Votes.Blank[1].NVopr=3;
  Votes.Blank[2].Vopr[1].NCandidates=5;
  Votes.Blank[3].Vopr[1].NCandidates=7;
  Votes.Blank[4].Vopr[1].NCandidates=12;
  Votes.Blank[5].Refer=true; Votes.Blank[5].NVopr=5;

  //Speak(1);


  Snat[1].Nsnat=1;
  Snat[1].Snat[1]=3;

  //Speak(2);


  // Speak(3);

  XVote=2;
  Speak(4);
  */

  //Print(3);
  //Speak(4);
//  int p=Block("[B1]*[2]+[5]*[3]*[2]-[5]+S-[B2]",1,1,true);
//    int p=Block("[7]-[10]/[5]*[7]-[3]+[12]",1,1,true);
//    cout<<p<<endl;
 //bool c=Checks("[2]+[3]*([1]+[6]/[2]*[3])*[4]-[5]+S>=[6]+[2]*([7]-[10]/[5]*[7]-[3]+[12])",1,1);

 //if (c) {cout<< 1 <<endl;}else {cout<<0<<endl;}

    //�������� ������� ������ �������

    //Parser(FileName);
    // Speak(3);
    /*
    std::string st="1234567890123456789012345678901234567890";
    st=FiltrStr(st,"456","---");
    cout <<st<<endl;
    */
    /*
    //�������� ������� ������ �������
    Votes.Nblank=1;
    Snat[1].Nsnat=1;
    Snat[1].Snat[1]=1;

    Speak(2);
    */
 /*
 std::string XML=ReadFile("./SourceDir/SourceData.XML");
 cout <<XML<<endl;
 //Filtr
 //XML=(XML,"< ","<");
 //XML=(XML,"<  ","<");
 XML=FiltrStr(XML,"&&&&&&&",">");
 //XML=(XML,"  >",">");
cout <<"--------------------------------"<<endl;
 cout <<XML<<endl;
 */
  std::string st;  std::string  st1;  std::string st2;
    Exit=false;
    Exit=Sostoanie();

   return Exit;
}



