//---------------------------------------------------------------------------
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string> //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <iostream>
#include <math.h>
#include <time.h>
//---------------------------------------------------------------------------
#include "Ruler.h"
#include "MyX.h"
#include "HeaderMain.h"
#include "CreateImage.h"
#include "InitAltera.h"
#include "HardProc.h"
#include "printer.h"
#include "usb.h"
using namespace std;

//--------------------------------------����������------------------------------
std::string Nst[256]; //�������� ���������
std::string Nvc[256]; //���� �������� ������
std::string Mst[256]; //��������� � ������� � ��� ������ �������
std::string Mvc[256]; //���� � ����������� ������� � ��� ������ �������
std::string Golos;

// ��������������  ����
int PrConnectOK;// ������� ���������  ����� ��  ������  ��������

int PrIdError; // �������  ������ ����������  �������� ������

int NumBullAll;
int PrVoteTimeStart;
int PrVoteTimeEnd;
int PrApprove;
int PrVoteDay;
  int PrErrScan=0;
  int PrRezScan=0;
   int Version;

   ofstream Fi;
   ofstream Fv;

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

   TVotes Votes; // ���������� ��� �������� �������

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
   //-------------------------------------------------���������� ������ ���������
   int Rep_cnt; // ���-�� ����� � ���������
   RepS Rep_st[NstrokMax+1]; //������ ������� ����� ���������
   int NumTEX = 0;
   char fn[] = "Test.tex";

    char DocBeg[20][62] = {
	{"% LaTeX report document; Erstellt von GAI"},
	{"\\documentclass[10pt]{article}"},
	{"\\usepackage{geometry}"},
	{" \\geometry{a4paper}"},
	{" \\oddsidemargin = 0pt"},
	{" \\topmargin = -1,5cm \\headheight = 0pt \\textwidth = 180mm"},
	{" \\textheight = 14cm"},
	// //!!!1mm = 2.65pt!!!!!!!!!!175 mm =~ 496pt!!!14cm = 396pt!!!!!
	{"\\headsep = 10pt"},
	{" \\flushbottom"},
	{"\\usepackage{latexsym}"},
	{"\\usepackage[T2A]{fontenc}"},
	{"\\usepackage{ucs}"},
	{"\\usepackage {longtable}"},
	{"\\usepackage[cp1251]{inputenc}"},//\\usepackage[koi8-r]{inputenc}"},
	{"\\usepackage[russian]{babel}"},
	{"\\frenchspacing"},
	{"\\usepackage{fancyhdr}"},
	{"\\pagestyle{fancyplain}"},
	{"\\renewcommand{\\headrulewidth}{0pt}"},
	{"\\fancyhf{}"}};
//-------------------------------------------------------------
char b_ffoot[] = "\\fancyfoot[C]{";
char e_ffoot[] = "\\center\\rm ���� \\thepage}";

char b_doc[] = "\\begin{document}";
char e_doc[] = "\\end{document}";
char b_tbl[] = "\\begin{longtable}";
//% col =160/ summ(Columns); p{x} = col * sym; example:  p{65*(160/100)}
char t_tabl[]= "{p{18pt}p{280pt}p{42pt}p{112pt}}";
char e_tbl[] = "\\end{longtable}";

char a_left[] = "\\flushleft ";
char a_centr[] = "\\center ";
char a_right[] = "\\flushright ";

char s_norm[] = "\\rm ";
char s_ital[] = "\\it ";
char s_slave[] = "\\sl ";
char s_bold[] = "\\bf ";
char s_tt[] = "\\tt ";

char k_norm[] = "\\normalsize ";
char k_small[] = "\\small ";
char k_ssm[] = "\\footnotesize ";
char k_large[] = "\\large ";
char k_Large[] = "\\Large ";
char k_LARGE[] = "\\LARGE ";

char c_under[] = "\\tt \\underline{";
char c_space[] = "\\hspace{";
char c_quad[] = "\\quad ";
char c_verb[] = "\\verb'";
char c_eol[] = "\\\\";
char comment[] = "%%COMMEMT";

std::string s;
char si[400];
int Kegel;	//������� ������: 10 / 9 / 8 / 12 / 14

FILE* ft;
//============================================================== ������� ==============================================================
// ��������������  ����
int Connect(void)
{  // ������� ��������  ������������  ����� ��  ������  ��������
    int pr=0;

   usleep(timeout);
   pr=1;
    return pr;
}

int FindSourceFile(void)
 { // ������������ ���� �  �����
   int pr;
   pr=0;  // ��  ������ �������� ( ������ ��� SD �����)
   //  ��������  �������  ��������
  //std:: string sst;
  //sst="/mnt/usb/SourceData.bin";
  //pr=MountNRead("/mnt/usb/SourceData.bin","/mnt/usb");
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

void VoteScan()
{  //********������������ � ������ ��������� ����, ���� ****************
   Scptr_r = 0;
   Scptr_s = 0;
   MotorFuS = false;
   int pr1=0;
   int rez=0;
   nstr =0;
   MasScan.MasRecOut[0]=0;
    max_gelesen =0;

    cout << "MAXLINES =   " << MAXLINES <<"      Input max_gelesen = " << max_gelesen << endl;
//************************************************************************
     clock_t  t1,t2;
     t1= clock();
//************************************************************************
   SayVotes(1);  // ������������
   //get_Optrons_Imm();
   //if((Optron[1] < NO_PAPER) &&(Optron[2] < NO_PAPER) )
 //  {
          Page_scan();

        while(nstr >= 0)
        { // ����  ��  �����������  �������
                nstr = getNextScanLine();
                // ������� �����������  ������������
                if(nstr>=0)
                {  // �������� ��������� ������ ���  �������������
                    pr1=ScanBulString();
                    rez=MasScan.MasRecOut[0];
               }
               if((pr1!=0)||(rez==111))
              {       nstr=-1;
                // ������ �  ������ ������ ��  �����
                    cout <<"pr1 = "<<pr1<<"      rez=  "<<rez<<"      nstr = "<<nstr<<endl;
                        cout <<"Out  max_gelesen = " << max_gelesen << endl;
                }
        }



//**********************************************
         t2 = clock();
        t2=t2 - t1;
        printf (" Scanning and recognition Time %f \n",float(t2)/CLOCKS_PER_SEC);
//************************************************
        PrErrScan=pr1;
        PrRezScan=rez;

//**********************************************
        printf(" PrErrScan   =    %d\n", PrErrScan);
                printf(" PrRezScan =    %d\n",PrRezScan);
//**********************************************
  /*  }else
    {  // ��������� � ���, ��� ���������� ���������  ������ ���������
    }*/
    return;
}

//--------------------------------------------------������� ������ ���������-----------------------------------------------------------
int Translate(char* f)
{
 int m=0;
 int i,k,pr;
  pr=0;k=0;   m=0;
  for (i = 0; i < 256; i++)
  {
   if (*(f+i)==0)
	{if (pr>0){pr=0;si[i+k]=0x7D;}return(m);}
   if (*(f+i)!='_')
	{if (pr>0){pr=0;si[i+k]=0x7D;k++;}
	 if (*(f+i)!=' ') si[i+k] =*(f+i);
	 else {memmove(&si[i+k],"\\ ",3);k+=1;}
	}
   else
   {
	if (pr>0) {memmove(&si[i+k],"\\ ",3);k+=1;}
	if (pr==0)
		{pr=1;m=1;memmove(&si[i+k],&c_under,17);k+=15;si[i+k]='~';}
   }
  }   return(m);
}
//............................... Check attributes of Line ....................................
int FormLine(int i)
{  	int k;
	int c;

	k = Rep_st[i].Size;
		if (k != Kegel)     // ���� ������ �� ��������� � �������
		 switch(k){
		  case 8 :{fprintf(ft,"%s",k_ssm);Kegel = 8; break;}
		  case 9 :{fprintf(ft,"%s",k_small);Kegel = 9; break;}
		  case 10:{fprintf(ft,"%s",k_norm);Kegel = 10; break;}
		  case 12:{fprintf(ft,"%s",k_large);Kegel = 12; break;}
		  case 14:{fprintf(ft,"%s",k_Large);Kegel = 14; break;}
		  case 16:{fprintf(ft,"%s",k_LARGE);Kegel = 16; break;}
		  default:{fprintf(ft,"%s",k_norm);Kegel = 10;}
		 }
		c = Rep_st[i].Alg;   // ��������� ������������
		switch (c) {
		 case 1:{fprintf(ft,"%s",a_left);break;}
		 case 2:{fprintf(ft,"%s",a_centr);break;}
		 case 3:{fprintf(ft,"%s",a_right);break;}
		 default:fprintf(ft,"%s\n",comment);
		}
		c = Rep_st[i].Bold;  // ��������� ����������
        switch (c) {
		 case 2:{fprintf(ft,"%s",s_bold);break;}
		 case 3:{fprintf(ft,"%s",s_ital);break;}
		 case 1:{fprintf(ft,"%s",s_norm);break;}
		 case 0:{fprintf(ft,"%s",s_slave);break;}
		 case 4:{fprintf(ft,"%s",s_tt);break;}
		 default:fprintf(ft,"%s\n",comment);
		}
	return(1);
}
//---------------------......................------------------------------------------------------
//---------------------------------------------------------------------------
int MakeFile(char* NameF)
{
int i,m, k,P_cnt;
int isTab;
	if ((ft = fopen(NameF, "w")) == NULL) return(-1);
  // 	 ������������ ����� - ��������� ���������
	for (i = 0; i < 20; i++)
	{
		fprintf(ft, "%s\n", DocBeg[i]);
	}
//......................................................
	 fprintf(ft,"%s\n",b_ffoot);
	 fprintf(ft,"%s\n",k_ssm);
	 Kegel = 8;
	 P_cnt = 0;
//.................. PageFooter Type ...........................................
	 for (i=0; i<Rep_cnt; i++) // ���� Pagefooter
	 {
	  if ( Rep_st[i].Typ== 4)  //PageFooter Type
	  {
		bzero(&si[0], 400);
		k = Translate(&Rep_st[i].Txt[0]);
		if (k) Rep_st[i].Bold = 4;
 	//	memmove(&si[0],&Rep_st[i].Txt[0],k);
       FormLine(i);
	   fprintf(ft,"%s %s\n",si,c_eol);      // ��������� ������ �������!
	   P_cnt++;
	  } // Type == P
	 } fprintf(ft,"%s\n",e_ffoot);
//......................End of PageFooter.......................................
	 fprintf(ft,"%s %i %s\n"," \\textheight = ",int(255-(Kegel*2.4*P_cnt)/2.65+0.5),"mm");

// ..................!!!!!! Begin DOKUMENT  !!!!!!!!!!! ........................
	fprintf(ft,"%s\n",b_doc);
	Kegel = 10;
	isTab = 0;
	 for (i=0; i<Rep_cnt; i++)     // �������� ��� Head
	 {
	   if ( Rep_st[i].Typ== 1) { // Header Type
	     bzero(&si[0], 400);
		  k = Translate(&Rep_st[i].Txt[0]);
		  FormLine(i);
		  fprintf(ft,"%s %s\n",si,c_eol);
	   }  // end of Header
	  }  // End cicle for Head
//............................................................................
	   for (i=0; i<Rep_cnt; i++)     // �������� ���  Body  (TABLE)
	   {
	   if ( Rep_st[i].Typ== 2)  // Body Type
	   {  	bzero(&si[0], 400);
			if (!isTab)
			{   isTab = 1;
			    FormLine(i);
				fprintf(ft,"%s",b_tbl);
				k = Rep_st[i].Cols[0]+Rep_st[i].Cols[1]+Rep_st[i].Cols[2]+ Rep_st[i].Cols[3]+Rep_st[i].Cols[4];
				double d = 160./k;
				fprintf(ft,"%s"," {");
				for	(m=0;m<5;m++)
				 if (Rep_st[i].Cols[m]!=0)
				   {
					  fprintf(ft,"%s %i %s","p{",(int)(d*Rep_st[i].Cols[m]),"mm}");
				   }
				fprintf(ft,"%s\n"," }");
			}
                         for	(m=0;m<5;m++)
				 if (Rep_st[i].Cols[m]!=0)
				   {  switch (m)
					 { case 0:{if(Rep_st[i].Cols[0]==1) fprintf(ft,"%s ",Rep_st[i].Txt.c_str());
                               else fprintf(ft,"%s ",Rep_st[i].Num.c_str());
                               break;}
					   case 1:{fprintf(ft,"%s","&");
						if (Rep_st[i].NCand.length()>0)fprintf(ft,"%s %s",
						Rep_st[i].NCand.c_str(),". ");break;}
                       case 2:{fprintf(ft,"%s","&");fprintf(ft,"%s",Rep_st[i].Txt.c_str());break;}
					   case 3:{fprintf(ft,"%s %i","&",Rep_st[i].Value);break;}
					   case 4:{fprintf(ft,"%s %s","&",Rep_st[i].V_prop.c_str());break;}
					   default:;
					 }
					}  fprintf(ft,"%s \n",c_eol);
	   }  // end of Body
	  } // End cikle for Body
	  if (isTab) {fprintf(ft,"%s \n",e_tbl);isTab=0;}
//..............................................................................
	 for (i=0; i<Rep_cnt; i++)     // �������� ���  Footer
	 {
	 if ( Rep_st[i].Typ== 3)  // Footer Type
	   {  bzero(&si[0], 400);
		  k = Translate(&Rep_st[i].Txt[0]);
		  FormLine(i);
		  fprintf(ft,"%s %s\n",si,c_eol);
	   }
	  }   // end of Footer
//..............................................................................
// !!! End of DOKUMENT  !!!!!!
	fprintf(ft,"%s\n",e_doc);
// ����� ��� ����� - ���������!
	fprintf(ft,"%s \n",c_eol);
	fclose(ft);

return(1);
};
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

  // Lang=0;

   int sum=0;
   char st[100];
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
          F8.getline(st,128); OutV.Main[sum]=st;
          F8.getline(st,128); OutV.MainPath[sum]=st;
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
          F16.getline(st,128); OutV.TestPath[sum]=st;
          sum++;
        }
        F16.close();
    }


}//LoadInd
// ---------------------------------------------������� ���������� �����������-------------------------------------

//--------------���������� �������������--------------
int WaitRR(void)
{  // ������ ����������� ������������� ���������
    int pr1=0;
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
        }
    }
   return pr1;
}

//----------------------------------����� ������� �� ����------------------------
bool SyncData(TKoib K, bool KB,TVotes V, bool VC) //����� ������� �� ����
{
  bool SD;

  SD=true;

  return SD;
}

//--------------------------����������� - ���� �� �������?----------------------

//------------------------------------------------------------------------------
int KoibMain(void)
// 1-���� ������� 2-���� ����������� 3-���� ����
{ // ����������� �������� �� ������ ������ ������� ( ��������� �������)
    int KM=0;
    if(CheckPrinter()!=-1)
    {   KM=1;// ������� ������
        printf("Printer on board\n");
        cout<< "  Printer  found. Scaner is main" <<endl;

    }  else
    {   KM=2; // ��� ��������
        printf("there is no printer\n");
        if( PrConnectOK==0) KM=3; // ������ ������������ ( �  ����������  ������)
    }
   //KM=1;
  return KM;
}
//------------------------������� ����� ���������� ����������-------------------
bool AvarFiles(void)
{
 bool AF;


 ifstream F;
 F.open("./Avar/Votes.dat", ios::app|ios::trunc|ios::binary);
 if (F!=NULL)// �������� �������� �����
 {
   F.read((char *)&Votes ,sizeof(TVotes) );

   cout << "Avar Load!" <<endl;
 }


  /*
  FILE* f = fopen("./Avar/Votes.dat", "rb");
  fread((char*)&Votes, sizeof(Votes), 1, f);
  fclose(f);
  */


 AF=false; //!!!!
return AF;
}
//--------------------------------------------------------------------------------------------------
void Zagruzka(void)
{  // �������� ���������  ������

   ReadSetUpFiles();
   FileName="./SourceDir/SourceData.XML"; // !!!!!
   Koib[1].PO = "1.1";                                     // !!!!!

	if (! AvarFiles() )  // ��� ����� ���������� ����������
	{

	} else { // �������� ���������� �����
      Koib[1].Avar=true;

	} // End �������� ���������� �����

}

//----------------------------------������ �� � ��������� ����������-------------------------------
void IDsave(void)
{
  /*
  FILE* f=fopen("./Avar/Votes.dat","wb");
  fwrite((char *)&Votes, sizeof(Votes), 1, f);
  fclose(f);
  */

  ofstream F;
  //F.open("./Avar/Votes.dat", ios::app|ios::beg|ios::trunc|ios::binary);

  F.open("./Avar/Votes.dat", ios::app|ios::trunc|ios::binary);

  F.write((char *)&Votes ,sizeof(TVotes) );

  F.close();

}
//----------------------------------�������� �� �� ��������� ����������-------------------------------
void IDload(void)
{
 // ifstream F("./Avar/Votes.dat", ios::app|ios::beg|ios::trunc|ios::binary);
 // F.read((char *)&Votes ,sizeof(TVotes) );

}





//-----------------------����� St1 � St2 �� ��������� �� ����� t----------------
void Ind(std::string st1, std::string st2)
{  // ������  ���������  ��  �����
    // ������ ���������  �  ������ ������
	if (st1!="")  {      setFirstline(&st1[0],0); 	};
	// ������ ���������  ��  ������ ������
	if (st2!="")  {      setSecline(&st2[0],0); 	    };
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
int Samotestirovanie(void)    // ���������������� ������������
{ int i=0;
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
    // �������� �������� ( ���� �� ���������)
    Ind("",Out.Test[8]);  //���� ��������


sleep(1);
    return i;
}


//-----------------------------------�������� �������� ������-------------------

//--------------------------���������� ����� ������� �� ����--------------------
bool Catch(void)
{

 // UstanovkaSoedinenia(Koib[1].Main);

return true;
}



// ------------------------------------����������� ����, �������------------------------------------
void TekDateTime(void)
{
	TekDT=GetDateTime();
	DateS=IntToStr(TekDT.day)+"."+IntToStr(TekDT.month)+"."+IntToStr(TekDT.year);
	TimeS=IntToStr(TekDT.hour)+":"+IntToStr(TekDT.minit)+":"+IntToStr(TekDT.sec);
}

//-----------------------���������� ����������� �����������---------------------
bool SaveIG(int XVote)
{
  std::string Out;
  ofstream F;

  TekDateTime();
  Out="./Result/Result-"+IntToStr(Votes.Nuik)+"-"+IntToStr(TekDT.day)+"."+IntToStr(TekDT.month)+"."+IntToStr(TekDT.year)+"_"+
                   IntToStr(TekDT.hour)+"."+IntToStr(TekDT.minit)+"."+IntToStr(TekDT.sec)+"_"+IntToStr(XVote)+
                   ".xml";

  F.open(Out.c_str(), ios::out); //IG-����

  Out="<?xml version=\"1.0\" encoding=\"windows-1251\" ?>";
  F<<Out<<'\n';

  Out=Out+"<Xib uik=\""+IntToStr(Votes.Nuik)+"\" ";

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
    Out=Out+"nuf=\""+IntToStr(Koib[i].Rezult[XVote].NoValid)+"\" ";
    Out=Out+">";

    Out=Out+"<Bulletin id=\""+"1"+"\" ";
    Out=Out+"n=\""+"1"+"\" ";
    Out=Out+"Valid=\""+IntToStr(Koib[i].Rezult[XVote].Valid)+"\" ";
    Out=Out+"NoMarks=\""+IntToStr(Koib[i].Rezult[XVote].NoMarks)+"\" ";
    Out=Out+"TooManyMarks=\""+IntToStr(Koib[i].Rezult[XVote].ManyMarks)+"\" ";
    Out=Out+"BadMode=\""+"0"+"\" ";
    Out=Out+">";
    Out=Out+"</Scanner>";

    F<<Out<<'\n';
  }

  Out="<Protocol final=\"true\">";

//<Election n="82" parentid="113031" e-mail="">

//<Line n="1">0</Line>

//<Candidate n="36943">0</Candidate>

//</Election>

  Out=Out+"</Protocol>";
  F<<Out<<'\n';

  Out=Out+"<Check/></Xib>";
  F<<Out<<'\n';

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

	   Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NLines=NStrVstr(Prot,"<Line "); // ---Lines
	   if (Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NLines>0)
	   {
	   for (int jj= 1; jj<=Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NLines; jj++)
	     {
		 st=StrBetweenStr12(Prot,"name=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].Name=st;                 //��� ����� ���������
     	 st=StrBetweenStr12(Prot,"num=\"","\"",jj);
     	    if (st !="") {Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].N=StrToInt<int>(st);}    // ����� ������
		 st=StrBetweenStr12(Prot,"id=\"","\"",jj);
		    if (st !="") {Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].ID=StrToInt<int>(st);}   // ID
		 st=StrBetweenStr12(Prot,"additionalNum=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].AddNum=st;      //
		 st=StrBetweenStr12(Prot,"expression=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].expression=st; //������� ���������� ������
		 st=StrBetweenStr12(Prot,"type=\"","\"",jj);
		    Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Lines[jj].type=st;       // type
	     }
       } //end Lines

	   Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NChecks=NStrVstr(Prot,"<Check "); //----Checks
	   if (Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NChecks>0)
	   {
	     for (int jj= 1; jj<=Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].NChecks; jj++)
	     {
		   st=StrBetweenStr12(Prot,"expression=\"","\"",jj);
		      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Checks[jj].expression=st; //������� �������� ��
		   st=StrBetweenStr12(Prot,"enabled=\"","\"",jj);
		      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Checks[jj].Enabled=TrueFalseVstr(st);  //enabled
		   st=StrBetweenStr12(Prot,"mild=\"","\"",jj);
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
		  Table=StrBetweenStr12(Prot,"<Table>","</Table>",jj); // Table

		  int Nvl=NStrVstr(Table,"<VoteLine "); // ---VoteLines
		  Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.Table.NVoteLine=Nvl;

		  if (Nvl>0)
		  {
			for (int jjj = 1; jjj <=Nvl; jjj++)
			{
			   st=StrBetweenStr12(Table,"id=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.Table.VoteLine[jjj].ID=st;  // id
			   st=StrBetweenStr12(Table,"bold=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.Table.VoteLine[jjj].bold=TrueFalseVstr(st);  // bold
			   st=StrBetweenStr12(Table,"italic=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.Table.VoteLine[jjj].italic=TrueFalseVstr(st);  // italic
			   st=StrBetweenStr12(Table,"fontSize=\"","\"",jjj);    // fontSize
			   if (st !="")
			   {
				 Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.Table.VoteLine[jjj].fontSize=StrToInt<int>(st);
			   }
			   st=StrBetweenStr12(Table,"fontSize=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.Table.VoteLine[jjj].type=st;  // type
			   st=StrBetweenStr12(Table,"ElectronNum=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.Table.VoteLine[jjj].ElectronNum=st;  // ElectronNum
			}
		  } // End Vote Lines

		  ProtLines=StrBetweenStr12(Prot,"<ProtocolLines>","</ProtocolLines>",jj); // ProtokolLines
		  int Npl=NStrVstr(ProtLines,"<ProtocolLine "); // ---NProtokolLines
		  Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.NProtocolLine=Npl;

		  if (Npl>0)
		  {
			for (int jjj = 1; jjj <=Npl; jjj++)
			{
			   st=StrBetweenStr12(ProtLines,"align=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].align=st;  // align
			   st=StrBetweenStr12(ProtLines,"bold=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].bold=TrueFalseVstr(st);  // bold
			   st=StrBetweenStr12(ProtLines,"italic=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].italic=TrueFalseVstr(st);  // italic
			   st=StrBetweenStr12(ProtLines,"fontSize=\"","\"",jjj);    // fontSize
			   if (st !="")
			   {
				 Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].fontSize=StrToInt<int>(st);
			   }
			   st=StrBetweenStr12(Table,"section=\"","\"",jjj);
			      Votes.Blank[Xbl].Vopr[Xv].Protocoles[j].Text.ProtocolLine[jjj].section=st;  // section
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

 SD=StrBetweenStr12(XML,"<SourceData",">",1); // �������� ������


 if (SD !="") //--------------------�������� �����!!!
 { Par=true;
  st1=StrBetweenStr12(SD,"DateTime=\"","-",1); //Date Votes
  if (st1 !="")
  {
	 //Votes.DByear=StrToInt<int>(st1);
	 //Votes.DBmonth=StrToInt<int>(StrBetweenStr12(SD,"-","-",1));
	 //Votes.DBday=StrToInt<int>(StrBetweenStr12(SD,"-","\"",1));

	 Votes.DByear=StrToInt<int>(st1);
	 st=StrBetweenStr12(SD,st1+"-","-",1);
	 Votes.DBmonth=StrToInt<int>(st); 	 st=st1+"-"+st;
	 st1=StrBetweenStr12(SD,st+"-","\"",1);
	 Votes.DBday=StrToInt<int>(st1);


     Votes.DateV=IntToStr(Votes.DBday)+"."+IntToStr(Votes.DBmonth)+"."+IntToStr(Votes.DByear);
               F<<"Election_Date:"+Votes.DateV+"_:"+DateToString(Votes.DByear,Votes.DBmonth,Votes.DBday,Lang) <<'\n';
  } else {     F<<"Error:Votes_Date_is_epson"<<'\n'; }

  Votes.IDel=StrBetweenStr12(SD,"id=\"","\"",1);      //ID �������
  if (Votes.IDel !="")
  {
               F<<Votes.IDel<<'\n';
  } else {     F<<"Error:ID_election_is_epson"<<'\n';}

  st=StrBetweenStr12(SD," isGasVrn=\"","\"",1);	//isGASvrn?
  if (st !="")
  {
      Votes.isGASvrn=TrueFalseVstr(st);
               F<<Votes.isGASvrn<<'\n';
  } else {     F<<"isGASvrn_is_epson"<<'\n';}

 } else {      F<<"Error:Main_part_is_epson"<<'\n'; } //-----------------��� �������� �����!!

 //------------------------���� �� �������� ������������?
 REF=StrBetweenStr12(XML,"Referendums","/Referendums",1);
   if (REF !="")
   {
	 XML=StrBezStr(XML,REF); // ������ ����������� ��� ������������
	           F<<"It_is_a_Referendum!"<<'\n';
   } else {    F<<"Referendum_part_is_epson"<<'\n';}
  //-----------------------���������� ����� �������
  Votes.Nblank = NStrVstr(XML,"/Blank>");
  if (Votes.Nblank>0)
  {
               F<<"Nblank="+IntToStr(Votes.Nblank)<<'\n';
  } else {     F<<"Error:Nblank=0!"<<'\n'; }
  //-----------------------���������� �������
  int Nel = NStrVstr(XML,"/Election>");
  Elec=StrBetweenStr12(XML,"<Elections","/Elections>",1); // ������!
  if (Elec !="")
  {
	XML=StrBezStr(XML,Elec); // ������ ����������� ��� �������
	           F<<"It_is_Election"<<'\n';
  } else {     F<<"Error:Election_part_is_epson!"<<'\n';}
  //-----------------------������ �����������
  Mode=StrBetweenStr12(XML,"<Modes>","</Modes>",1);
  if (Mode !="")
  {
	 Votes.Nmodes = NStrVstr(Mode,"/Mode"); // ���������� �������
	  if (Votes.Nmodes>0)
	  {
		st="Modes: ";
		for (int i = 1; i <= Votes.Nmodes; i++)
		{
		  Votes.Modes[i]=StrBetweenStr12(Mode,"<Mode>","</Mode>",i);
		  st=st+Votes.Modes[i]+" ";
		}
	           F<<st<<'\n';
	  }
	} else {   F<<"Error:Modes_are_epson!"<<'\n';}
 //---------------------------����� ����� �������
 MTT=StrBetweenStr12(XML,"ModeTimeTable","/ModeTimeTable",1);
	if (MTT !="")
	{
      st1="Time_of_change_modes:";
	  for (int i = 1; i <= Votes.Nmodes; i++)
	  {

	   st=StrBetweenStr12(MTT,"hour=\"","\"",i);
	   Votes.Bhour[i]=StrToInt<int>(st);
	   st=StrBetweenStr12(MTT,"minute=\"","\"",i);
	   Votes.Bmin[i]=StrToInt<int>(st);
	   Votes.Bsec[i]=0;
	   Votes.TimeB[i]=IntToStr(Votes.Bhour[i])+":"+IntToStr(Votes.Bmin[i])+":"+IntToStr(Votes.Bsec[i]);
	   Votes.Unlim=false;
       st1=st1+Votes.TimeB[i];
	  }
	          F<<st1<<'\n';
	} else {
		Votes.Unlim=true;
		Votes.Bhour[1]=BhourU[1];  Votes.Bmin[1]=BminU[1]; Votes.Bsec[1]=0;
		Votes.Bhour[2]=BhourU[2];  Votes.Bmin[2]=BminU[2]; Votes.Bsec[2]=0;
		      F<<"Error:Time_of_change_modes_is_epson!"<<'\n';
	}
 //----------------------------------������ � ���
	Targets=StrBetweenStr12(XML,"<Targets >","</Targets>",1);
	if (Targets !="")
	{
	 Votes.Nuik=StrToInt<int>(StrBetweenStr12(Targets,"num=\"","\"",1));
	         F<<"UIK_number="+IntToStr(Votes.Nuik)<<'\n';
	 Votes.UIKname=(StrBetweenStr12(Targets,"name=\"","\"",1));
             F<<"UIK name= "+Votes.UIKname<<'\n';
	} else { F<<"Error:UIK_information_is_epson!"<<'\n';}
 //----------------------------------������!
 Bul=StrBetweenStr12(XML,"Blanks","/Blanks",1); // ���������
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

	   Model=StrBetweenStr12(Bul,"<model>","</model",i); // ������ ���������
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

	PC=StrBetweenStr12(ElecX,"<ParentComittee","/>",1);                   //Parent Committee
	if (PC !="")
	{
	  Votes.Blank[Xbl].Vopr[Xv].PC.parentid=StrBetweenStr12(PC,"parentid=\"","\"",1);
	  Votes.Blank[Xbl].Vopr[Xv].PC.name=StrBetweenStr12(PC,"name=\"","\"",1);
	  Votes.Blank[Xbl].Vopr[Xv].PC.email=StrBetweenStr12(PC,"e-mail=\"","\"",1);
	        st1=st1+"_ParentComittee:"+Votes.Blank[Xbl].Vopr[Xv].PC.name;
	} else {st1=st1+"_ParentComittee_epson!";}

	SC=StrBetweenStr12(ElecX,"StampCommittees","/>",1);                   //Stamp Commitees
	if (SC !="")
	{
	  st =StrBetweenStr12(SC,"num=\"","\"",1);
	  if (st !="")
	  {
	      Votes.Blank[Xbl].Vopr[Xv].SC=StrToInt<int>(st);
	      st1=st1+"_StampCommitees:"+st;
      }
	} else {st1=st1+"_StampCommitees_epson!";}

	Mode=StrBetweenStr12(ElecX,"<Modes>","</Modes>",1);            //������
	if (Mode !="")
	{
	  Votes.Blank[Xbl].Vopr[Xv].Nmodes = NStrVstr(Mode,"/Mode>");  // ���������� �������
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

	 } else { } //���������� ���?
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

	   Votes.Blank[Xbl].Vopr[Xv].Candidates[jj].biography=new char[255]; //?
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
   int Nref = NStrVstr(REF,"/Referendum>"); // ���������� ������������

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
		  Votes.Blank[Xbl].Ref.Protocoles[j].Text.Table.NVoteLine=Nvl;

		  if (Nvl>0)
		  {
			for (int jjj = 1; jjj <=Nvl; jjj++)
			{
			   st=StrBetweenStr12(Table,"id=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.Table.VoteLine[jjj].ID=st;  // id
			   st=StrBetweenStr12(Table,"bold=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.Table.VoteLine[jjj].bold=TrueFalseVstr(st);  // bold
			   st=StrBetweenStr12(Table,"italic=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.Table.VoteLine[jjj].italic=TrueFalseVstr(st);  // italic
			   st=StrBetweenStr12(Table,"fontSize=\"","\"",jjj);    // fontSize
			   if (st !="")
			   {
				 Votes.Blank[Xbl].Ref.Protocoles[j].Text.Table.VoteLine[jjj].fontSize=StrToInt<int>(st);
			   }
			   st=StrBetweenStr12(Table,"fontSize=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.Table.VoteLine[jjj].type=st;  // type
			   st=StrBetweenStr12(Table,"ElectronNum=\"","\"",jjj);
			      Votes.Blank[Xbl].Ref.Protocoles[j].Text.Table.VoteLine[jjj].ElectronNum=st;  // ElectronNum
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

//--------------------------������������ �������� Checks ������ ���������, ���������� ��� ������������-----------------------------
bool Checks(std::string Ch)
{
   int SumZn=0;
   int N=0;
//   int Left=0;
//   bool L=true;
   int Zn=0;
//   int Right=0;
   int Rav=0; //=1 >2 <3 <>4 >=5 <=6
   std::string st;
   std::string st1;
   N++;

   st=StrBetweenStr12(Ch, "[", "]", N);
	 if (st !="")
	 {
	   Zn=StrToInt<int>(st);
	 }
	   //----------------Signum----------------
		if (N>1)
		{
		  st1=StrBetweenStr12(Ch, "]", "[", N-1);
		  if (st1 !="")
		  {
			if (StrVstr(st1,"=",1))
			{
			  Rav=1;
			}
			if (StrVstr(st1,">",1))
			{
			  Rav=2;
			}
			if (StrVstr(st1,"<",1))
			{
			  Rav=3;
			}
			if (StrVstr(st1,"<>",1))
			{
			  Rav=4;
			}
			if (StrVstr(st1,">=",1))
			{
			  Rav=5;
			}
			if (StrVstr(st1,"<=",1))
			{
			  Rav=6;
			}
			//-----
		  }
		} else { SumZn=Zn; }
	   //------------------End Signum----------------
	Rav=Rav;
 return SumZn;
}

//---------------------������������ �������� Express ������ ���������, ���������� ��� ��������----------------------
int Express(std::string Exp)
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
		  Zn=0;
	   }
	   if (StrVstr(st, "BlankType=NoMarks", 1))
	   // ����� ���-�� �������� ���������� ��� �������
	   {
		  Zn=0;
	   }
	   if (StrVstr(st, "BlankType=TooManyMarks", 1))
	   // ����� ���-�� �������� ���������� � ����������� ���-�� �������
	   {
		  Zn=0;
	   }
	   if ((StrVstr(st, "BlankType=Valid", 1))&&(StrVstr(st, "VotingMode=Main", 1)))
	   // ����� �����������-��������, ���-�� �������� �������������� ����������
	   {
		  Zn=0;
	   }
	   if ((StrVstr(st, "BlankType=NoMarks", 1))&&(StrVstr(st, "VotingMode=Main", 1)))
	   // ����� �����������-��������, ���-�� �������� ���������� ��� �������
	   {
		  Zn=0;
	   }
	   if ((StrVstr(st, "BlankType=TooManyMarks", 1))&&(StrVstr(st, "VotingMode=Main", 1)))
	   // ����� �����������-��������, ���-�� �������� ���������� � ����������� ���-�� �������
	   {
		  Zn=0;
	   }
	   if ((StrVstr(st, "BlankType=Valid", 1))&&(StrVstr(st, "VotingMode=Portable", 1)))
	   // ����� �����������-����������, ���-�� �������� �������������� ����������
	   {
		  Zn=0;
	   }
	   if ((StrVstr(st, "BlankType=NoMarks", 1))&&(StrVstr(st, "VotingMode=Portable", 1)))
	   // ����� �����������-����������, ���-�� �������� ���������� ��� �������
	   {
		  Zn=0;
	   }
	   if ((StrVstr(st, "BlankType=TooManyMarks", 1))&&(StrVstr(st, "VotingMode=Portable", 1)))
	   // ����� �����������-����������, ���-�� �������� ���������� � ����������� ���-�� �������
	   {
		  Zn=0;
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
//--------------------------------------------------------------------------
bool MakeProtokol(int XVote)
{
  //------������������ � �������� ����������
  if (Votes.Blank[XVote].Vopr[1].NProtocoles>0)
  { //Protocol Bar

  }// End Protocol Bar
  else { //Protokol Johtur

    std::string ProtX=ReadFile("./Protokols/Protokols.XML");
    int Nprot=NStrVstr(ProtX, "</Protocol>");

    std::string Prot=StrBetweenStr12(ProtX,"<Protocol ","</Protocol>",1); //����� ���������� ���������
    Protokol(Prot,XVote,1,1);

  } //End Protokol Johtur
  //------End ������������ � �������� ����������
  return true;
}

//------------------------------------------------------------------------------
void SumRep(int Rep_cntX, std::string Printer, int Typ, int Alg, int Bold,
			std::string Num, std::string NCand, std::string V_prop,
			int Value, int c1,int c2,int c3,int c4,int c5, int c6, int Size)
{
  Rep_st[Rep_cntX].Txt=Printer;
  Rep_st[Rep_cntX].Typ=Typ;
  Rep_st[Rep_cntX].Alg=Alg;
  Rep_st[Rep_cntX].Bold=Bold;

  Rep_st[Rep_cntX].Num=Num;
  Rep_st[Rep_cntX].NCand=NCand;
  Rep_st[Rep_cntX].V_prop=V_prop;

  Rep_st[Rep_cntX].Value=Value;
  Rep_st[Rep_cntX].Cols[0]=c1;
  Rep_st[Rep_cntX].Cols[1]=c2;
  Rep_st[Rep_cntX].Cols[2]=c3;
  Rep_st[Rep_cntX].Cols[3]=c4;
  Rep_st[Rep_cntX].Cols[4]=c5;

  Rep_st[Rep_cntX].Size=Size;
}
//------------------------------------������------------------------------------
bool Print(int Type)
{
  ofstream F;
  char sst[128];
  std::string Printer;
  Rep_cnt=0;
  std::string Font=Votes.Blank[1].Vopr[1].Protocoles[1].Font;
  int FontSize=Votes.Blank[1].Vopr[1].Protocoles[1].Fontsize;
  if (FontSize==0) {FontSize=10;}

  std::string PF1,PF2,PF3,PF4;
  PF1="������������ ���������� ������������� �������� ___________________________________________________________ ____________________";
  PF2="                                               (�������, ��������) (������� ���� ������� ����������, ������� �� ������ ������) ";
  PF3="���������� ������������ ��������               ___________________________________________________________ ____________________";
  PF4="��������� ��������                             ___________________________________________________________ ____________________";


	switch (Type)
	{
	case 1: //--------------------------------------------------�������� ������
	  {
		  strcpy(sst,"./Tex/Pr_ID.tex");
          F.open("./Log/Protokol_ID.log", ios::out); //Log-����

		  //1
		  Printer=Out.PrID[1]+" "+IntToStr(Votes.Nuik); //�������� ������ ��� ��� N
		  SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  //Centr
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //2
		  Printer=Out.PrID[2]; //������ ���:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //3
		  Printer=Out.PrID[3]+" "; //����� �������:
		  if (Koib[1].Tren)
		       {Printer+=Out.PrID[27];} else //����������
		       {Printer+=Out.PrID[28];} //�����������
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //4
		  Printer=Out.PrID[4]+" "+Koib[1].N; //������ N:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //5
		  Printer=Out.PrID[5]+" "+DateToStringOrVoice(Votes.DByear,Votes.DBmonth,Votes.DBday,true);
		  //���� �����������:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //6
		  Printer=Out.PrID[6]+" "+IntToStr(Votes.Nuik); //������ ������������� ��� ��� N
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //7
		  Printer=Out.PrID[7]; //!!!??? ������ ����������� ��������:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //8
		  Printer=Out.PrID[8]+" "+IntToStr(Votes.Nblank);//���������� ����� ����������:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //9
		  Printer=Out.PrID[9]; //������������������ ����� �������:
		  if (Votes.Nmodes>0)
		  {
			  for (int i = 1; i <= Votes.Nmodes; i++)
			  {
				Printer=Printer+Votes.Modes[i]+" ";
			  }
		  }
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //10,11
		  if (! Votes.Unlim)
		  {
			//10
			Printer=Out.PrID[10]+" "+Votes.TimeB[1]; //����� ������ ������������� ������:
			SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
			//11
			Printer=Out.PrID[11]+" "+Votes.TimeB[2]; //����� ������ ����������� ������:
			SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
		  }
		  //������!
	   if (Votes.Nblank>0)
	   {
		  //12
		  Printer=Out.PrID[12]; //���������!
		  SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize); //Centr
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  for (int i = 1; i <= Votes.Nblank; i++)
		  {
			//13
			Printer=Out.PrID[13]+" "+IntToStr(i); //��������� N
			SumRep(Rep_cnt, Printer,1,1,1,Printer,Printer,Printer,0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
			//14
			Printer=Out.PrID[14]+" "+Votes.Blank[i].Name; //������������:
			SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
			//15
			Printer=Out.PrID[15]; //������������� �������:
			if (Votes.Blank[i].ID !="")
			   { Printer=Printer+" "+Votes.Blank[i].ID; } else
			   { Printer=Printer+" "+Out.PrID[29]; } //���
			SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
			//16
			Printer=Out.PrID[16]+" "+Votes.Blank[i].Vopr[1].Name; //������:
			SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
			//17
			Printer=Out.PrID[17]; //!!!??? ������ ��������� ���������:
			SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
			//18
			Printer=Out.PrID[18]; //!!!??? ������ ��������� � ������������� �������:
			SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
			Rep_cnt++;
			F<<Printer<<'\n';
		  }

		  //19
		  Printer=Out.PrID[19]; //������!
		  SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize);//Centr
		  Rep_cnt++;
		  F<<Printer<<'\n';

		  for (int i = 1; i <= Votes.Nblank; i++)
		  {
		  //20...

		  Printer=Out.PrID[20]+" "+IntToStr(i)+": "+Votes.Blank[i].Vopr[1].Name; //������ N
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //21
		  Printer=Out.PrID[21]; //!!!??? ����������� ��������:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //22
		  Printer=Out.PrID[22]+" "; //������ �����������:
		  if (Votes.Blank[i].Vopr[1].Nmodes>0)
		  {
			  for (int j = 1; j <= Votes.Blank[i].Vopr[1].Nmodes; j++)
			  {
				Printer=Printer+Votes.Blank[i].Vopr[1].Modes[j]+" ";
			  }
		  }
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //23
		  Printer=Out.PrID[23]+" "+IntToStr(Votes.Blank[i].Vopr[1].maxMarks); //����� ��������:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //24
		  Printer=Out.PrID[24]+" "; //!!!??? ������� ������� "������ ����":
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //25
		  Printer=Out.PrID[25]+" "+IntToStr(Votes.Blank[i].Vopr[1].NCandidates); //����� �������:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //26
		  Printer=Out.PrID[26]; //�������:
		  SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
			  if (Votes.Blank[i].Vopr[1].NCandidates>0)
			  {
				for (int j=1; j<=Votes.Blank[i].Vopr[1].NCandidates; j++)
				{
				//27...
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
				 SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
				 Rep_cnt++;
				 F<<Printer<<'\n';

			    }
			  }

                  if (Votes.Blank[i].Vopr[1].NProtocoles>0)
                  {
				          //28...
				          Printer=Out.PrID[31]; //���������
                          SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		                  Rep_cnt++;
		                  F<<Printer<<'\n';

                   for (int j=1; j<=Votes.Blank[i].Vopr[1].NProtocoles; j++)
				   {
				     if (Votes.Blank[i].Vopr[1].Protocoles[j].Enable)
				     {
				           //29...
				           Printer=Out.PrID[32]+" "+Votes.Blank[i].Vopr[1].Protocoles[j].Name;
                           //������������ ���������:
                           SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		                   Rep_cnt++;
		                   F<<Printer<<'\n';
                           if (Votes.Blank[i].Vopr[1].Protocoles[j].NLines>0)
                          {
				           //31
				           Printer=Out.PrID[33]; //������ ���������:
                           SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		                   Rep_cnt++;
		                   F<<Printer<<'\n';

                           for (int jj=1; j<=Votes.Blank[i].Vopr[1].Protocoles[j].NLines; j++)
				           {
                           //32...
                           Printer=IntToStr(jj)+". "+Votes.Blank[i].Vopr[1].Protocoles[j].Lines[jj].Name;
                           SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		                   Rep_cnt++;
		                   F<<Printer<<'\n';
				           }
                          }

                           if (Votes.Blank[i].Vopr[1].Protocoles[j].NChecks>0)
                        {
				          //33
				          Printer=Out.PrID[34]; //����������� �����������:
				          SumRep(Rep_cnt, Printer,2,1,1,"","","",0, 1,0,0,0,0,0 ,FontSize);
		                  Rep_cnt++;
		                  F<<Printer<<'\n';

                          for (int jj=1; j<=Votes.Blank[i].Vopr[1].Protocoles[j].NChecks; j++)
				          {
                          //34...
                          Printer=IntToStr(jj)+". "+Votes.Blank[i].Vopr[1].Protocoles[j].Checks[jj].expression;
                          SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		                  Rep_cnt++;
		                  F<<Printer<<'\n';
				          }
                        }

                    }
				 }
              }
		   }

		}

	} break; // End �������� ������

case 2: //---------------------------------------------------------�������� ������������
	{
		 strcpy(sst,"./Tex/Pr_Test.tex");
		 F.open("./Log/Protokol_Test.log", ios::out); //Log-����

		  //1
		  Printer=Out.PrTest[1]; //�������� ������������ ����
		  SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  //Centr
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //2
		  TekDateTime();
		  Printer=Out.PrTest[2]+" "+DateToStringOrVoice(TekDT.year,TekDT.month,TekDT.day,true); //����
		  SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  //Centr
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //3
		  Printer=Out.PrTest[3]+" "+IntToStr(Votes.Nuik); //������������� ������� N
		  SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize); //Centr
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //4
		  //---  ������ N1, ������ N2, �����
		  SumRep(Rep_cnt, Out.PrID[5],2,1,1," ",Out.PrTest[4],Out.PrID[6],0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //5
		  //---  Ns1   Ns2

		  if (!Koib[1].Main)
		  {
		    SumRep(Rep_cnt, Koib[1].N,2,1,1," "," ",Koib[2].N,0, 20,15,15,1,15,0 ,FontSize);
		  } else {
		    SumRep(Rep_cnt, Koib[1].N,2,1,1," "," ",Out.PrID[7],0, 20,15,15,1,15,0 ,FontSize);
		  }
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //6
		  //������� ����������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].All),2,1,1,Out.PrTest[8],IntToStr(Koib[1].Rezult[0].All+Koib[2].Rezult[0].All),
                 IntToStr(Koib[2].Rezult[0].All), 0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //7
		  //�� ���:
		  SumRep(Rep_cnt,"",2,1,1,Out.PrTest[9],"","",0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //8
		  //��������������� �����:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].Nuf),2,1,1,Out.PrTest[10],IntToStr(Koib[1].Rezult[0].Nuf+Koib[2].Rezult[0].Nuf),
                 IntToStr(Koib[2].Rezult[0].Nuf),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //9
		  //����������������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].NoValid),2,1,1,Out.PrTest[11],IntToStr(Koib[1].Rezult[0].NoValid+Koib[2].Rezult[0].NoValid),
                 IntToStr(Koib[2].Rezult[0].NoValid),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //10
		  //� ��� �����:
		  SumRep(Rep_cnt,"",2,1,1,Out.PrTest[12],"","",0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //11
		  //- ��� �������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].NoMarks),2,1,1,Out.PrTest[13],IntToStr(Koib[1].Rezult[0].NoMarks+Koib[2].Rezult[0].NoMarks),
                 IntToStr(Koib[2].Rezult[0].NoMarks),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //12
		  //- ����� �������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].ManyMarks),2,1,1,Out.PrTest[14],IntToStr(Koib[1].Rezult[0].ManyMarks+Koib[2].Rezult[0].ManyMarks),
                 IntToStr(Koib[2].Rezult[0].ManyMarks),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';


	  if (Votes.Nblank>0)
	  {
		  for (int i = 1; i <= Votes.Nblank; i++)
		  {
		  //13
		  Printer=Out.PrTest[14]+IntToStr(i)+". "+Votes.Blank[i].Vopr[1].Name; //��� ������� N
		  SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //14
		  //������� ����������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].All),2,1,1,Out.PrTest[15],
                 IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].All+Koib[2].Rezult[0].Bl[i].Vop[1].All),
                 IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].All), 0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //15
		  //�� ���:
		  SumRep(Rep_cnt,"",2,1,1,Out.PrTest[9],"","",0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //16
		  //��������������� �����:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Nuf),2,1,1,Out.PrTest[16],
                 IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Nuf+Koib[2].Rezult[0].Bl[i].Vop[1].Nuf),
                 IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].Nuf),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //17
		  //����������������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoValid),2,1,1,Out.PrTest[17],
                 IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoValid+Koib[2].Rezult[0].Bl[i].Vop[1].NoValid),
                 IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].NoValid),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //18
		  //� ��� �����:
		  SumRep(Rep_cnt,"",2,1,1,Out.PrTest[12],"","",0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //19
		  //- ��� �������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoMarks),2,1,1,Out.PrTest[18],
                 IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].NoMarks+Koib[2].Rezult[0].Bl[i].Vop[1].NoMarks),
                 IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].NoMarks),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';
		  //20
		  //- ����� �������:
		  SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].ManyMarks),2,1,1,Out.PrTest[19],
                 IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].ManyMarks+Koib[2].Rezult[0].Bl[i].Vop[1].ManyMarks),
                 IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].ManyMarks),0, 20,15,15,1,15,0 ,FontSize);
		  Rep_cnt++;
		  F<<Printer<<'\n';

			if (Votes.Blank[i].Vopr[1].NCandidates>0)
			  {
				for (int j = 1; j <=Votes.Blank[i].Vopr[1].NCandidates; j++)
				{
				//21...
				Printer=Out.PrTest[21]+IntToStr(j)+":"; //�������� N
				SumRep(Rep_cnt,IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Can[j]),2,1,1,Printer,
                       IntToStr(Koib[1].Rezult[0].Bl[i].Vop[1].Can[j]+Koib[2].Rezult[0].Bl[i].Vop[1].Can[j]),
                       IntToStr(Koib[2].Rezult[0].Bl[i].Vop[1].Can[j]),0, 1,1,1,1,0,0 ,FontSize);
				Rep_cnt++;
                F<<Printer<<'\n';
				}
			  }

		  }
	   }
	} break; // End �������� ������������

	case 3: //---------------------------------------------���������� ����������� �� ������� XVote
	{              strcpy(sst,"./Tex/Pr_Rezult.tex");
            F.open("./Log/Protokol_VR.log", ios::out); //Log-����
        for(int i=1;i<=Votes.Nblank; i++)
       {  // ������������ ����� �����  ����������� �� ���� �������
            XVote=i;
      // ������������ ����� ������  ����������� �� ���� �������

            //1
            Printer=Out.PrRezult[1]; // ���������� �����������
            SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize); //Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //2
            Printer=Out.PrRezult[2]+IntToStr(XVote)+": "+Votes.Blank[XVote].Name; //������ N
            SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  //Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //3
            TekDateTime();
            Printer=Out.PrRezult[3]+DateToString(TekDT.year,TekDT.month,TekDT.day,Lang); //����
            SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize); //Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //4
            Printer=Out.PrRezult[4]+IntToStr(Votes.Nuik); //������������� ������� N
            SumRep(Rep_cnt, Printer,1,2,1,"","","",0, 0,0,0,0,0,0 ,FontSize); //Centr
            Rep_cnt++;
            F<<Printer<<'\n';
            //5...
            if (Votes.Blank[XVote].Vopr[1].NCandidates>0)
            {
                for (int i=1; i<=Votes.Blank[XVote].Vopr[1].NCandidates; i++)
                {
                    Printer=IntToStr(i)+". ";
                    Printer+=Votes.Blank[XVote].Vopr[1].Candidates[i].firstName+" "+
                            Votes.Blank[XVote].Vopr[1].Candidates[i].lastName+" "+
                            Votes.Blank[XVote].Vopr[1].Candidates[i].patronymic+": ";
                    int sum=(Koib[1].Rezult[1].Bl[XVote].Vop[1].Can[i]+
                            Koib[2].Rezult[1].Bl[XVote].Vop[1].Can[i]+
                            Koib[1].Rezult[2].Bl[XVote].Vop[1].Can[i]+
                            Koib[2].Rezult[2].Bl[XVote].Vop[1].Can[i]); //???

                    Printer+=" "+IntToStr(sum)+" ("+CisloToStringOrVoice(sum,true) +")";

                    SumRep(Rep_cnt, Printer,1,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);
                    Rep_cnt++;
                    F<<Printer<<'\n';
                }
            }
        }

    } break; // End �������� � ������������ �����������

	case 4://----------------------------------------------------------------�� ���������� ��
	{
     F.open("./Log/Protokol_KS.log", ios::out); //Log-����


	} break; // End �� ���������� ��
	case 5: //--------------------------------------------------------------------------�������� ��������
	{
	 F.open("./Log/Protokol_Itog.log", ios::out); //Log-����

	 if (Votes.Blank[1].Vopr[1].Protocoles[1].Enable)
	 {
		 std::string Font=Votes.Blank[1].Vopr[1].Protocoles[1].Font;
		 /*
		 int NumW=Votes.Blank[1].Vopr[1].Protocoles[1].numberWidth;
		 int NameW=Votes.Blank[1].Vopr[1].Protocoles[1].nameWidth;
		 int ValueW=Votes.Blank[1].Vopr[1].Protocoles[1].valueWidth;
		 int Nlines=Votes.Blank[1].Vopr[1].Protocoles[1].NLines;
		 int Nchecks=Votes.Blank[1].Vopr[1].Protocoles[1].NChecks;
         */

		  if (Votes.Blank[1].Vopr[1].Protocoles[1].Lines[1].type=="DontQueryUser")
		  {
			// Blank[1].Vopr[1].Protocoles[1].Lines[1].Value=Express(Blank[1].Vopr[1].Protocoles[1].Lines[1].)

		  }
	   };
	} break; // End �������� ��������

 } //End Switch

 //Page Footer

		  SumRep(Rep_cnt,PF1, 4,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  Rep_cnt++;
		  SumRep(Rep_cnt,PF2, 4,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  Rep_cnt++;
		  SumRep(Rep_cnt,PF3, 4,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  Rep_cnt++;
		  SumRep(Rep_cnt,PF4, 4,1,1,"","","",0, 0,0,0,0,0,0 ,FontSize);  Rep_cnt++;

          MakeFile(sst);


 F.close();
 return 0;
}

//------------------------------------�������� N ������� �������----------------------------------
void SayN(std::string st1)
{
  std::string st="aplay   ";
  st=st+st1+">/dev/null";
  system(st.c_str());
}
//------------------------------------�������� ������� �������----------------------------------
void Say(int Type)
{  // ��������� ��������� �  ���� ���. ���������
    if (Type>0)
    {  // �������� ��������� �� ����� ����������� ��������� �������� ���������
        SayN(OutV.MainPath[Type]);
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
        SayN(Golos);
        F<<Golos<<'\n';

        if (Votes.Nuik>0)
        {
            Golos=OutV.IDPath[2]+" "+CisloToStringOrVoice(Votes.Nuik,false) ; //����� ���
            SayN(Golos);
            F<<Golos<<'\n';
        }

        if (Votes.DateV !="")
        {
            Golos=OutV.IDPath[3]+" "+DateToStringOrVoice(Votes.DByear,Votes.DBmonth,Votes.DBday,false);
            //���� �������
            SayN(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.Nblank>0)
        {
            Golos=OutV.IDPath[4]+" "+CisloToStringOrVoice(Votes.Nblank,false); //���������� �������
            SayN(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.TimeB[1] !="")
        {
            Golos=OutV.IDPath[5]+" "+TimeToStringOrVoice(Votes.Bhour[1],true,Votes.Bmin[1],true,Votes.Bsec[1],false,false);
            //������ �����������
            SayN(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.TimeB[2] !="")
        {
            Golos=OutV.IDPath[6]+" "+TimeToStringOrVoice(Votes.Bhour[2],true,Votes.Bmin[2],true,Votes.Bsec[2],false,false);
            //��������� �����������
            SayN(Golos);
            F<<Golos<<'\n';
        }
        if (Votes.Nblank>0) //���������
        {
            Golos=OutV.IDPath[7]+" "+CisloToStringOrVoice(Votes.Nblank,false); //���������� ����������
            SayN(Golos);
            F<<Golos<<'\n';
            for (int j=1; j<=Votes.Nblank; j++)
            {
                Golos=OutV.IDPath[8]+" "+CisloToStringOrVoice(j,false)+" "; //����� N
                if (Votes.Blank[j].Refer) { Golos+=OutV.IDPath[10]; } // ��� ����������
                                          else { Golos+=OutV.IDPath[9]; } // ��� ������
                SayN(Golos);
                F<<Golos<<'\n';

                if (Votes.Blank[j].Refer) //--------------------------����������
                {
                    if (Votes.Blank[j].NVopr>0)
                    {
                      Golos=OutV.IDPath[11]+" "+CisloToStringOrVoice(Votes.Blank[j].NVopr,false);
                      //���������� ��������
                      SayN(Golos);
                      F<<Golos<<'\n';
                    }
                } //End ����������
                if (! Votes.Blank[j].Refer) //--------------------------������
                {
                    if (Votes.Blank[j].Vopr[1].NCandidates>0)
                    {
                      Golos=OutV.IDPath[12]+" "+CisloToStringOrVoice(Votes.Blank[j].Vopr[1].NCandidates,false);
                      //���������� ����������
                      SayN(Golos);
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
                SayN(Golos);
                F<<Golos<<'\n'; //  ������ �  log  ����
                for (int i = 1; i <=Votes.Nblank; i++)
                if (Snat[i].Nsnat>0)
                {  // ����� �������
                    Golos=OutV.SnatPath[2]+" "+CisloToStringOrVoice(i,false); //����������� N
                    SayN(Golos);
                    F<<Golos<<'\n';
                    Golos=OutV.SnatPath[3]+" "; //������ ������ �������
                    for (int j = 1; j <=Snat[i].Nsnat; j++)
                    {
                       Golos+=CisloToStringOrVoice(Snat[i].Snat[j],false)+" ";
                    }
                    SayN(Golos);
                    F<<Golos<<'\n';
                }
             } else
             { //�� ������� ������� �� ������ �������!!!
                Golos=OutV.SnatPath[4]; //������ �� �����!
                SayN(Golos);
                F<<Golos<<'\n';
             }
	  } break;
	  case 3: //---------------------------------------�������� ������������----------------------
	  {
        F.open("./Log/Protokol_Voice_Test.log", ios::out); //Log-����

        Golos=OutV.TestPath[1];//�������� ������������ ����
        SayN(Golos);
        F<<Golos<<'\n';

        TekDateTime();
        Golos=OutV.TestPath[2]+" "+DateToStringOrVoice(TekDT.year,TekDT.month,TekDT.day,false); //����
        SayN(Golos);
        F<<Golos<<'\n';

        //����� ������� ����������
        Golos=OutV.TestPath[3]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //�� ���
        Golos=OutV.TestPath[4];
        SayN(Golos);
        F<<Golos<<'\n';

        //��������������� �����
        Golos=OutV.TestPath[5]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //����������������
        Golos=OutV.TestPath[6]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //� ��� �����
        Golos=OutV.TestPath[7];
        SayN(Golos);
        F<<Golos<<'\n';

        //��� �������
        Golos=OutV.TestPath[8]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //����� �������
        Golos=OutV.TestPath[9]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //�������� �� �������
        Golos=OutV.TestPath[10];
        SayN(Golos);
        F<<Golos<<'\n';

     if (Votes.Nblank>0)
     {
        for (int i=1; i<=Votes.Nblank; i++)
        {
        //������ �����
        Golos=OutV.TestPath[11]+" "+CisloToStringOrVoice(i,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //����� ������� ����������
        Golos=OutV.TestPath[3]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //�� ���
        Golos=OutV.TestPath[4];
        SayN(Golos);
        F<<Golos<<'\n';

        //��������������� �����
        Golos=OutV.TestPath[5]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //����������������
        Golos=OutV.TestPath[6]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //� ��� �����
        Golos=OutV.TestPath[7];
        SayN(Golos);
        F<<Golos<<'\n';

        //��� �������
        Golos=OutV.TestPath[8]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';

        //����� �������
        Golos=OutV.TestPath[9]+" "+CisloToStringOrVoice(0,false);
        SayN(Golos);
        F<<Golos<<'\n';


           if (Votes.Blank[i].Vopr[1].NCandidates>0)
           {
           //�������� �� ����������
           Golos=OutV.TestPath[12];
           SayN(Golos);
           F<<Golos<<'\n';

              for (int j=1; j<=Votes.Blank[i].Vopr[1].NCandidates; j++)
              {
                //�������� �����
                Golos=OutV.TestPath[13]+" "+CisloToStringOrVoice(j,false);
                SayN(Golos);
                F<<Golos<<'\n';

                //������ �������
                Golos=OutV.TestPath[14]+" "+CisloToStringOrVoice(0,false);
                SayN(Golos);
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
        SayN(Golos);

        TekDateTime();
        Golos=OutV.RezultPath[2]+" "+DateToStringOrVoice(TekDT.year,TekDT.month,TekDT.day,false); //����
        SayN(Golos);
        F<<Golos<<'\n';

        Golos=OutV.TestPath[3]+" "+CisloToStringOrVoice(Votes.Nuik,false); //������������� ������� �����
        SayN(Golos);
        F<<Golos<<'\n';

           if (Votes.Blank[XVote].Vopr[1].NCandidates>0)
           {
           Golos=OutV.TestPath[4]+" "+CisloToStringOrVoice(Votes.Blank[XVote].Vopr[1].NCandidates,false);
           //����������
           SayN(Golos);
           F<<Golos<<'\n';

              for (int j=1; j<=Votes.Blank[XVote].Vopr[1].NCandidates; j++)
              {
                //�� ��������� �����
                Golos=OutV.TestPath[5]+" "+CisloToStringOrVoice(j,false);
                SayN(Golos);
                F<<Golos<<'\n';

                int K1=Koib[1].Rezult[1].Bl[XVote].Vop[1].Can[j]+Koib[1].Rezult[2].Bl[XVote].Vop[1].Can[j];
                int K2=Koib[2].Rezult[1].Bl[XVote].Vop[1].Can[j]+Koib[2].Rezult[2].Bl[XVote].Vop[1].Can[j];
                //������
                Golos=OutV.TestPath[6]+" "+CisloToStringOrVoice(K1+K2,false);
                SayN(Golos);
                F<<Golos<<'\n';

                // ����������
                Golos=OutV.TestPath[7];
                SayN(Golos);
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
    for (int i = 0; i <= Votes.Nblank; i++)
    {
        Koib[1].Rezult[i].All=0;
        Koib[1].Rezult[i].ManyMarks=0;
        Koib[1].Rezult[i].NoMarks=0;
        Koib[1].Rezult[i].NoValid=0;
        Koib[1].Rezult[i].Valid=0;

        for (int j=1; j<= Votes.Blank[i].NVopr; j++)
        {
            for (int q=1; q<=Votes.Blank[i].Vopr[j].NCandidates; q++)
            {
                Koib[1].Rezult[XVote].Bl[i].Vop[j].Can[q]=0;
            }
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------------���������� ������� ���������-----------------------------------------
//------------------���������� ��������� �������������� ������ ���������, ���� ��� - ��������� ������---------
bool NextXVopr(void)
{  // ����������� ��������� ������ ���  ����� ���. ��������
    bool NXV=false;
    std::string st;
    if (Votes.Blank[XVote].Vopr[1].Protocoles[1].NLines>0)
    {
        for (int i=1; i<=Votes.Blank[XVote].Vopr[1].Protocoles[1].NLines; i++)
        {
            st=Votes.Blank[XVote].Vopr[1].Protocoles[1].Lines[i].Value;
            // if ((st=="Blank") && (i>Xv))
            if (i>Xv)   {   Xv=i;    NXV=true;   goto end;      }
        }
    }
    end:
    if (! NXV)  XVote++;
    return NXV;
}
//---------------------------------------------------------------------------------------------------------------
void Sos_3(void)
{
	Ind(Out.Main[99],Out.Main[100]); // ���������� ��������� �����/ ���������� ��������? ��/���
	Say(56); // ���������� ��������� �����. ���������� ��������?
}
void Sos_5(void)
{   // ���������� ��  ������// ��������  �����������
    std::string st1;    std::string st2;
	st1=Out.Main[9]; 	st2=Out.Main[11]; 	Ind(st1,st2); 	//Say(3);
}
void Sos_6(void)
{  // ������� �  �����  ������  �  Flash
    std::string st1;     std::string st2;
    //�������� Flash //������� ��
	st1=Out.Main[15];    	st2=Out.Main[16];       Ind(st1,st2);     Say(7);
}
void Sos_7(void)
{   //�������� �������� ������...
    Ind(Out.Main[17]," ");    Say(8);
}
void Sos_8(void)
{
    std::string st1;
    std::string st2;
    TekDateTime(); //������� ���� � �����
    st1=Out.Main[21]+" "+Votes.DateV; // ���� ����������� ��.��.��
    PrVoteDay=0; //  ��������� ��������� ����� ��������� �������
     if (TekDT.year==Votes.DByear)
     {      if (TekDT.month == Votes.DBmonth)
            {   if (TekDT.day ==Votes.DBday)
                 {     //  ����  �����������
                        PrVoteDay=1;
                        Koib[1].VS = 9;
                        Say(11); //���� ����������� - �������
                        Ind(Out.Main[23]," "); //����������� �������� ������? ��/���
                        Say(14); //����������� �������� ������?
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
     switch ( PrVoteDay)
     {  case -1:  {  st2=Out.Main[20];    } break;
         case  0:  {  st2=Out.Main[19];   } break;
         case  1:  {   st2=""; } break;
     }
     Ind(st1,st2);
     //sleep(1);
    // ������� ��������� ����  �����������
    Say(12); //���� ����������� - �� �������


 /*     int ii,jj;
              stDig=IntToStr(Days);
              FirstDig=stDig[stDig.length()-1];
              ii=int(FirstDig);
              switch (ii)
              { case 49:  { st2=Out.Main[81]; jj=22;} break;  // ����
                 case 50:   case  51 : case  52 :  {st2=Out.Main[82];  jj=23;}break; // ���
                 case  48 :   case  53 : case  54 :case  55 :   case  56 : case  57 : {st2=Out.Main[83];jj=24;} break; //����
              }//switch
               st1=IntToStr(Days)+"  "+st2 +" "+Out.Main[28];
               Ind(st1," "); // DD  ���� �� ������ �����������
              Golos="";
	          Golos=CisloToStringOrVoice(Days,false) + OutV.MonthsPath[jj]+OutV.MainPath[19];
               SayN(Golos);
               */

   /* Golos="  ";
	Golos=OutV.MainPath[12]+" "+ CisloToStringOrVoice(Votes.DBday,false);
	Golos=Golos+ CisloToStringOrVoice(Votes.DBmonth,false)+CisloToStringOrVoice(Votes.DByear,false); //
	  switch ( PrVoteDay)
     {  case -1:  {  Golos=Golos+ OutV.MainPath[11];  } break;
         case  0:  {  Golos=Golos+ OutV.MainPath[9];  } break;
     }
    SayN(Golos);
    */
   //������ ������ � ������������� ������ �������?
    st1=Out.Main[22];       st2=Out.Main[118];       Ind(st1,st2);
    Say(13); //������ ������ � ������������� ������ �������?
}

void Sos_9(void)
{     // ����������
      Ind(Out.Main[23]," "); //����������� �������� ������? ��/���
      Say(14); //����������� �������� ������?
}
void Sos_10(void)
{
      Ind(Out.Main[24]," "); //����������� ����� �������� ������? ��/���
      Say(15); //����������� ����� �������� ������?
}
void Sos_11(void)
{
      Ind(Out.Main[25]," "); //�������� �������� ������? ��/���
      Say(16); //�������� �������� ������?
}
void Sos_12(void)
{
      Ind(Out.Main[26]," "); //�������� �������� ������ ��� ���? ��/���
      Say(17); //�������� �������� ������ ��� ���???
}
void Sos_13(void)
{
      Ind(Out.Main[27]," "); // �������� ������ �����? ��/���
      Say(18); //�������� ������ �����?
}
void Sos_14(void)
{
    std::string st1;
    std::string st2;
    std::string stDig;
    //st1=Out.Main[28]; //  �� ������ �����������
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
              switch (ii)
              { case 49:  { st2=Out.Main[81]; jj=22;} break;  // ����
                 case 50:   case  51 : case  52 :  {st2=Out.Main[82];  jj=23;}break; // ���
                 case  48 :   case  53 : case  54 :case  55 :   case  56 : case  57 : {st2=Out.Main[83];jj=24;} break; //����
              }//switch
               st1=IntToStr(Days)+"  "+st2 +" "+Out.Main[28];
               Ind(st1," "); // DD  ���� �� ������ �����������
              Golos="";
	          Golos=CisloToStringOrVoice(Days,false) + OutV.MonthsPath[jj]+OutV.MainPath[19];
               SayN(Golos);
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
    Say(21); //��� �������� � ����� ������������ ������� ��/ ��� ������ ������� ������� ���
}
void Sos_16(void)
{
    std::string st1;
    st1=Out.Main[34]+" "+IntToStr(Xblank);//+" "+Out.Main[35];
    // ������ �������  // ������� ������� ��� ������� N..? ��/���
    Ind(st1," ");
    Golos="";
	Golos=OutV.MainPath[22]+" "+CisloToStringOrVoice(Xblank,false) ; //����� �������
    SayN(Golos);
    //Say(22); // ������� ������� ��� ������� N..? ?!?!
}
void Sos_17(void)
{
    std::string st2;
      st2=Out.Main[36]+" "+IntToStr(Xcand);//+" "+Out.Main[37]+" "+IntToStr(Xblank)+" "+Out.Main[35];
      // ������ �������
      Ind("",st2); //������� ������� N...��� ������� N...? ��/���
      Golos="";
     Golos=OutV.MainPath[23]+" "+CisloToStringOrVoice(Xcand,false)+" "+OutV.MainPath[24]+" "+CisloToStringOrVoice(Xblank,false) ; // ����� �������
    SayN(Golos);

    //  Say(23); //������� ������� N... ?!?!
   //   Say(24); // ��� ������� N...? ?!?!
}
void Sos_18(void)
{
    std::string st2;
      st2=Out.Main[38]+" "+IntToStr(Xcand)+" "/*+Out.Main[37]+" "+IntToStr(Xblank)+" "*/+Out.Main[39];
      // ������ �������
      Ind("",st2); // � ##  �����!
      Golos=OutV.MainPath[25]+CisloToStringOrVoice(Xcand,false)+  // ����� �������
                            OutV.MainPath[24]+CisloToStringOrVoice(Xblank,false) +
                            OutV.MainPath[26]; // ����� �������
    SayN(Golos);
}
void Sos_19(void)
{
    std::string st1;
    std::string st2;
 	if (Snat[Xblank].Nsnat>0)
	{
        st1=Out.Main[37]+" "+IntToStr(Xblank); // ������
        st2=Out.Main[40]+" ";//+IntToStr(Xblank); // �����_���������_�������_��_�������_N_

        //  ������������ ����������  ��������� � ������ ��������
/*!!!*/       // Golos=

       for (int i = 1; i <=Snat[Xblank].Nsnat; i++)
	   {
		 st2=st2+IntToStr(Snat[Xblank].Snat[i])+", ";
       }
	   Ind(st1,st2); // ������ �## // �����: ##,##,##,...
    }
}
void Sos_20(void)
{
	  Ind(Out.Main[41]," "); //��������� �������� ������,
	  sleep(1);
	  Ind(Out.Main[23]," "); //�����������_��������_������?_��/���
      Say(59); //��������� �������� ������
      Say(60); //�����������_��������_������?
}
void Sos_21(void)
{
     Ind(Out.Main[43]," "); //����������� ������ �������? ��/���
/*!!!  */     // ������������ ���������� ��������� �� ������ ��������


     Say(27); //����������� ������ �������?
}
void Sos_22(void)
{
     Ind(Out.Main[44]," "); //������������� ��������� ?
     Say(61); //�������������_���������_�_��������_������?
}
void Sos_23(void)
{
        std::string st1;
        std::string st2;
        st1=Out.Main[45];// ��������� �������������!
        st2=Out.Main[48]+TimeS; // �����: ��:��
        Ind(st1,st2);
        sleep(2);
        st1=Out.Main[46] ; // ������������!
        st2=Out.Main[47] + " 0"; //���� 0
        Ind(st1,st2);
        Say(58); //��������� ������������ - ���
}

void Sos_24(void)
{
      Ind(Out.Main[49]," "); //��������� ����? ��/���
  	  Say(29); //��������� ����?
}

void Sos_50(void)
{
      std::string st1;
       //All=Koib[1].Rezult[XVote].All;
        st1=Out.Main[47];
      if (NumBullAll >= 0)
      {
         st1=st1+" "+IntToStr(NumBullAll); //����:...
      }
     Ind("",st1);
}
void ObrTestBull(void)
{  // ������������ ������ ������ ��� ��������� �������� ����������
        std::string st1="";         std::string st2="";         std::string st3="";
        Ind(Out.Main[127]," "); // ������������...
        Golosovanie();  // ����� � ������������� ���������
        st1=IntToStr(NumBullAll)+" : "+IntToStr(MasScan.MasRecOut[2]); // ���������� �������� ����������// ����� ���
        Golos="";    Golos=OutV.VotesPath[11]+" "+CisloToStringOrVoice(NumBullAll,false) ; //���������� �������� ����������
        SayN(Golos);
        if ((pr_rr==1)||(pr_rr==2))//���//��������� ��������������� �����!
        {   st1=st1+" "+Out.Votes[2];   Ind(st1,"");     SayVotes(2);   }
        if ((pr_rr==3))//��������� ����������������!//����� �������
        {    st1=st1+" "+Out.Votes[3];    Ind(st1,""); SayVotes(3);       }
        if ((pr_rr==4))//��������� ����������������!//��� �������
        {    st1=st1+" "+Out.Votes[4];    Ind(st1,"");    SayVotes(4);    }
        if ((pr_rr==5))
        {   //��������� ��������������!//��� � �������
            st1=st1+" "+Out.Votes[5]; // ��������������
            st3=Out.Votes[9]+" N : ";  // ������ �����
            SayVotes(5);
            int i=MasScan.MasRecOut[1]; // ����� ���� ������ ������������� ���������
            int NumV=MasScan.MasRecOut[3]; // ����� ���� ������ ������������� ���������
            for(int j=1;j <= NumV;j++) //  �� ���� ������� (��������) ��  ������
            if(Votes.Blank[i].Vopr[j].ID!="0")
            {   st2="";
                st2=st3+IntToStr(j)+" "+Out.Votes[10]+" N : ";
                Golos= "";
                Golos= OutV.VotesPath[9]+" "+CisloToStringOrVoice(j,false) ;
                for(int k=1; k <= Votes.Blank[i].Vopr[j].NCandidates;k++)
                {  //��  ���������� ��� ������ �������
                    if(MasScan.MasCandOut[j][k]!=0)
                    {  // ���� ����� - ����������, �� ��  ������� ������� ����������� ������ �������
                        if(k>1) // ���������
                        {   st2=st2+", "+IntToStr(k);
                            Golos= Golos+" "+OutV.VotesPath[13]+" "+CisloToStringOrVoice(k,false) ;
                        } else // ��������
                        { st2=st2 + IntToStr(k);
                            Golos= Golos+" "+OutV.VotesPath[10]+" "+CisloToStringOrVoice(k,false) ;
                        }
                    }
                }
                Ind(st1,st2);     SayN(Golos);
            }
        }
        SayVotes(8);
} //ObrTestBull

void Sos_25(void)
{
      Ind(Out.Main[50]," "); //����������� �������� ������������? ��/���
      Say(32);//����������� �������� ������������?
}
void Sos_26(void)
{
      Ind(Out.Main[24]," "); //����������� ����� ��������� ������������? ��/���
      Say(33); //����������� ����� ��������� ������������?
}
void Sos_27(void)
{
      Ind(Out.Main[52],Out.Main[53]); //�������� �������� ������������? ��/���
      Say(34); //�������� �������� ������������?
}
void Sos_28(void)
{
      Ind(Out.Main[52],Out.Main[53]); //�������� �������� ������������ ��� ���? ��/���
      Say(35);//�������� �������� ������������ ��� ���?
}
void Sos_29(void)
{
      Ind(Out.Main[54],Out.Main[55]);
      Say(36); //��� �������� � ������������ ����� ����������� ������� ��
      Say(37); //��� �������� � ��������� �������� ������ � ������������ ������� ���
}
void Sos_30(void)
{
      //std::string st;
     /* TekDateTime();
      int h=Votes.Bhour[1]-TekDT.hour;
      int m=Votes.Bmin[1]-TekDT.minit;
      st=IntToStr(h)+" "+Out.Main[100]+" "+IntToStr(m)+" "+Out.Main[101];*/
      Ind(Out.Main[56]," "); // �� ������������� ����������� �������� 10 �����:
      Say(38); // �� ������������� ����������� �������� 10 �����
}
void Sos_31(void)
{
      std::string st1;
      std::string st2;

      TekDateTime();
      int h =TekDT.hour;
      int m=TekDT.minit;
      //  ������������  ����������� �����: ** ����� ** �����
      st1=Out.Main[57]; // ������������ �����������
      st2= Out.Main[48]+IntToStr(h)+" : "+IntToStr(m); // ����� ��:��
      Ind(st1,st2);
      sleep(2);
      st2= Out.Main[58]+" 0"; // ������� : 0
      Ind("",st2);
      Say(39); //������ ������������� �����������
}

void Sos_60(void)
{
      //std::string st1;
      std::string st2;
     // st1=Out.Main[57]+TimeS; // ������������ �����������! ����� *.*
      if (NumBullAll >=0 )
      {
           st2=Out.Main[58]+IntToStr(NumBullAll); // �������:...
      }
      Ind("",st2);
}

void Sos_32(void)
{
      Ind(Out.Main[59],Out.Main[60]);
      // �����_�������������_�����������_�������! ���_�������������_�����_���������_�������_��
      Say(41); //�����_�������������_�����������_�������!
      Say(42); //���_�������������_�����_���������_�������_��
}
void Sos_33(void)
{
      Ind(Out.Main[61],Out.Main[64]); //��������� ������������ �����������? ��/���
 	  Say(43); //��������� ������������ �����������?
}
void Sos_34(void)
{
	  std::string st1;
	  std::string st2;
	  TekDateTime(); //������� ����, �����
	  st1=Out.Main[62]+" "+TimeS; //����������_�����������!_�����:_
      int All=Koib[1].Rezult[XVote].All;
	  st2=Out.Main[63]+" "+IntToStr(All); // ����������:_
	  Ind(st1,st2);
      Say(44); //����������_�����������!
}

void Sos_70(void)
{  // ������� ���������� ��� ���������� �����������
      std::string st1;
      //int All=Koib[1].Rezult[XVote].All;
       if (NumBullAll >=0 )
        {
            st1=Out.Main[63]+" "+IntToStr(NumBullAll); //����������:...
            Ind("",st1/*Out.Main[99]*/);
          //  Say(30); //������� ?!?!
        //    Say(31); //����������
        }
}

void Sos_35(void)
{
      Ind(Out.Main[61],Out.Main[65]);
      //��� ���������� ����������� ������������ ������� �� /��� �������� � ������ ���������� ������� ���
  	  Say(45); //��� ���������� ����������� ������������ ������� ��
  	  //Say(46); //��� �������� � ������ ���������� ������� ���
}
void Sos_36(void)
{
      Ind(Out.Main[66],Out.Main[67]);
      // ��� �������� � ���������� ������ ����������� ������� ��/ ��� �������� � ����������� ������� ���
      Say(48); // ��� �������� � ���������� ������ ����������� ������� ��
      usleep(200000);
      Say(49); // ��� �������� � ����������� ������� ���
}
void Sos_37(void)
{
      std::string st;
      st=Out.Main[68];//+IntToStr(XVote)+Out.Main[35];
      Ind(st," "); //����������� ����� ����������� ����������� �� ������� XVote? ��/���
      Say(50); //����������� ����� ����������� ����������� �� ������� ?!?!
}
void Sos_38(void)
{
  	  Ind(Out.Main[69]," "); //���������� �������� ����������
      Say(51); //���������� �������� ����������
}
void Sos_39(void)
{
  	  std::string st;
  	  //��������_����������_��_�������_N_XVote _?_��/���
  	  st=Out.Main[70]+" "+IntToStr(XVote)+" "+Out.Main[35];
	  Ind(st,Out.Main[104]); // ���������� ����������
	  //  ������� Speak
	  Golos="";
	Golos=OutV.MainPath[52]+" "+CisloToStringOrVoice(XVote,false) ; //����� ���
    SayN(Golos);
	 // Say(52); //��������_����������_��_������� ?!?!
}
void Sos_40(void)
{
  	  std::string st;
  	  st=Out.Main[71]+" "+IntToStr(XVote)+" "+Out.Main[72];
	  Ind(st," "); //�������� ���������� �� ������� Xvote ��� ���? ��/���
	  Golos="";
	  Golos=OutV.MainPath[52]+" "+CisloToStringOrVoice(XVote,false) +" "+OutV.MainPath[53]; //����� ���
      SayN(Golos);
	 // Say(52); //�������� ���������� �� ������� Xvote ?!?!
	  //Say(53); //��� ���?
}
void Sos_41(void)
{
      std::string st1;
      std::string st2;
      st1=Out.Main[73];
      st2=Out.Main[75]+" "+IntToStr(XVote)+". ";
      Ind(st1,st2);
      //Say(55); // ���� �������������� �������� �� ������� ?!?!
      //Say(56); // ��� ����������� ������ ������� ��
}
void Sos_42(void)
{
      std::string st;
      std::string st1;
      //if (NextXVopr())
      //{
         st=Out.Main[37]+" "+IntToStr(XVote)+" "+Out.Main[76]+"  "+IntToStr(Xv);
         // ������ N...XVote. ������ N...Xv
      //}
      st1=IntToStr(Votes.Blank[XVote].Vopr[1].Protocoles[1].Lines[Xv].Value);
      // ������  �������� ������
      Ind(st,st1);
      // �����_��_������_�����_��������������_��������-���
}


void Sos_44(void)
{
	  std::string st;
	  std::string st1;
	  st=Out.Main[78];//+" "+IntToStr(XVote)+" "; //������_�����������_�����������_��_�������_N_XVote
	  if (true) //�������� �� �� ������� N
	  {
		 //st=st+Out.Main[79]; //����������� ����������� ���������
		 st1=Out.Main[79];//+" "+IntToStr(XVote)+" "+Out.Main[82];
		 //������� � ��������� ��������� �� ������� XVote -��
	   } else {
		 //st=st+Out.Main[80]; //����������� ����������� �� ���������
		 Print(4); //���������� ������������� �� �� ������� XVote
		 st1=Out.Main[80];//+" "+IntToStr(XVote)+" "+Out.Main[84];
		 //������� � �����_�����_��������������_�������� �� ������� XVote -���
       }
	   Ind(st,st1); //������_�����������_�����������_��_�������_N_XVote
}
void Sos_45(void)
{
        std::string st;
        st=Out.Main[85];//+" "+IntToStr(XVote);
        Ind(st," ");
        sleep(3);
		//���������_�_���������_��������_��������_��_�������_N_XVote
		Ind(Out.Main[86]," "); //�������� ��������? ��/���
}
void Sos_46(void)
{
	    std::string st;
	    st=Out.Main[87]+" "+IntToStr(XVote)+" "+Out.Main[35];
        Ind(st," "); //�����������_��������������_�����_���������_���������_��_�������_N_
}
void Sos_47(void)
{
        std::string st;
        st=Out.Main[88]+" "+IntToStr(XVote)+" "+Out.Main[89];
		Ind(st," "); //�������_����������_�����_���������_���������_��_�������_N_XVote_._�������_ENTER
}
void Sos_49(void)
{
 	    std::string st;
 	    st=Out.Main[90]+" "+IntToStr(XVote)+" "+Out.Main[91];
		Ind(st," "); //����_������_�����������_�����������_��_�������_N_XVote_��_Flash
}

int Golosovanie(void)
{
	if (Koib[1].Opros)
    {    // ������ ������������
		GashBeforeScan();
        frontLED(5,0);   // ��������� ������� �������� ���� ������� ������������ � ���������
        ScanReady=true;
        VoteScan();
        ScanReady=false;
        pr_rr=WaitRR();
        if ((pr_rr==1)||(pr_rr==2)) //���
        {  //��������� ��������������� �����!
                //Page_stop();// ������� ��
                //usleep(100000);
                Page_back(); // �������� �� �����

                printf(" Defected Bulleten \n");      // ��������� ���������������  �����
        }
        if((pr_rr > 2)&&(pr_rr < 6))
        {  // ��������� �������������  �����
            NumBullAll= NumBullAll+1; // ��������� �������� ����������
            cout << " Recognized nVoteType = " << nVoteType << endl;
            XVote= MasScan.MasRecOut[3];  // ����� ���� ������������  ������
            cout << " Vote Type (XVote) = " << XVote << endl;
            cout << "  Blank Number  = " << Votes.Blank[MasScan.MasRecOut[1]].marker << endl;
            cout << " Marker Blank Vote Type  = " << Votes.Blank[MasScan.MasRecOut[3]].marker << endl;
            if ((pr_rr==3))
            {   //����� �������
                Koib[1].Rezult[XVote].All++;
                Koib[1].Rezult[XVote].ManyMarks++;
                Page_mark();
                printf(" Too  many Marks \n");      // ��������� ���������������  �����
            }
            if ((pr_rr==4))
            {   //��� �������
                Koib[1].Rezult[XVote].All++;
                Koib[1].Rezult[XVote].NoMarks++;
                Page_mark();
                printf("  No Marks \n");      // ��������� ���������������  �����
            }
            if ((pr_rr==5))
            {   //��� � �������
                Koib[1].Rezult[XVote].All++;
                Koib[1].Rezult[XVote].Valid++;
                Page_down();
                printf("  Bulleten has been recognized  One Mark  \n ");      // ��������� �������������  �����
            }
             Koib[1].Rezult[0].All=NumBullAll;
        }


        if(Koib[1].VS==50)  Sos_50();
        if((Koib[1].VS==60)||(Koib[1].VS==32))  Sos_60();
        if(Koib[1].VS==70)  Sos_70();
        //usleep(100000);

        frontLED(0,5);   // ��������� ������� �������� ���� ������� ������������ � ���������
    }
    return 0;
}


//---------------------------------------------------------------------------------------------------------------------
int Sostoanie(void)
{

    std::string st;  std::string st1; std::string st2;
	switch (Koib[1].VS)
	{

	case 1: //----------------------------------------------��������� ����+��������+���������������� ����
		{  //�������������
		  st1=Out.Main[1];  Ind(st1," ");   sleep(1);
		  //�������� �������
		  st1=Out.Main[2];  Ind(st1," ");
		  Zagruzka(); //  �������� ��������� ������ ���� (������������ �����)
		  st=Out.Main[3]+"  (2013)"; //��_����_���.
 		  st1=Out.Main[4]+" "+SerNumKoib;  // ������ N
		  Ind(st, st1);
          sleep(1);
		  st1=Out.Main[5];
		  Ind(st1," ");  //����������������...
          int S=0;
          //S=Samotestirovanie();
          if (S==0)
          {  // ����������������  ���������
              st=Out.Main[4]+" "+SerNumKoib; // ������ N
			 st1=Out.Main[6]; // �����
			 Ind(st,st1);
			 Say(1); //������ �����
			 Koib[1].VS = 2;
          }
          else
          {  st=Out.Main[4]+" "+SerNumKoib; // ������ N
             st1=Out.Main[7]; // ��  �����
			 Ind(st,st1); // ���������������� �� ��������
			 Say(2); //������ �� �����
          }
              PrNoBtnWait=1;
		}
	case 2: //------------------------------------------------�������� ������� ��������� ������
		{
          if (AvarFiles())
          {  // ���� ��������� ���� ������, �� ������ ��  ���������� ���������� ����� (��������� 3)
			 Koib[1].VS=3;
			 Sos_3(); //3
          } else {
             Koib[1].VS=4;
          }
          PrNoBtnWait=1;
		} break;
	case 3: //-------------------------------------------------��������� �� ���������� ����� ?
		{
              Koib[1].VS = 4;
              //  ���������  �  �������  �������� ������ �� ����������  �����
              //LoadFromAvar();

              NumBullAll=Koib[1].Rezult[0].All; // ��������� �������� ���������� 30_07_2013
              PrNoBtnWait=1;
		} break;
	case 4: //-------------------------------------------------���������� �� 2-� ����
		{
		    st1=Out.Main[8];     Ind(st1," "); // �����...
            // !!! �������� ���������� �� ������ ��������
            PrConnectOK=Connect();
		    // ����������� �������� ������� ( �� ��������)
			int KM=KoibMain();
			if (KM==1) //���� ���� �������, ������� ���������
			{   Koib[1].Avto=false;
			    Koib[1].Main=true;
				st1=Out.Main[9]; // ���������� �� ������
                // !!! �������� ������� ���������� �� ������ ��������
                if(PrConnectOK==1)
                {  // ���������� �� ������ // �������� �����������
                    Sos_5();   Say(6); // ���������� �� 2-� �������� �����������
                    // ������� �  ����������  ���������
                    Koib[1].VS = 6;
                    Sos_6(); // ������� �  �����  �������� ������
                } else
                { st2=Out.Main[10];   Ind("",st2);  // ��� - �������� �����
                   Koib[1].VS = 5;
                   PrNoBtnWait=1; // ���  �����������  ������  �������
                };
			}
			if (KM==2) //  ������ ��  ��������, ������� ��  ���������
			{
			    Koib[1].Avto=false;   Koib[1].Main=false;
			    // !!! �������� ������� ���������� �� ������ ��������
                if(PrConnectOK==1)
                {  // ���������� �� ������ // �������� �����������
                    Sos_5();     Say(6); // ���������� �� 2-� �������� �����������
                    Koib[1].VS = 6;
                    Sos_6();
                }
			}
		} break;
	case 5: //----------------------------------------�������� ����� ����������?
		{
           if(PrModeChange==1 )    {   Sos_5();   PrModeChange=0; 	}
           if(pr_btn==0) // No �������� �����, ������� �  ����������  �����
           {                          //������ �������� � ���������� ������
                    st1=Out.Main[12];      st2=Out.Main[13];       Ind(st1, st2);
                    Koib[1].Avto=true;
                    Say(5);
                    Koib[1].VS = 6;     PrNoBtnWait=1;
			}
			// ����� �� ������������ ������ �� ?
			if(pr_btn==1) // Yes  ���������� �����
            {       //  ����������� �  ������
                    Koib[1].VS = 4;        PrNoBtnWait=1;
			}
			if(pr_btn==-1)
			{  // ������� ��  �����������  ������ �������  ������� ���� ��  ����� ������ �����-���� ������
			    // ����  ������������ ������  ������ ���
			    //PrConnectOK=1;
			    Koib[1].VS = 4;   PrNoBtnWait=1;
			    sleep(2);
            } ;
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
                            {   st1=Out.Main[120];      st2=Out.Main[119];       Ind(st1, st2);   } break;
                        case -2: //  ����  ��  ������
                            {   st1=Out.Main[14];      st2=Out.Main[119];       Ind(st1, st2);      } break;
                        case -3: //  ������ �������� ������
                            {   st1=Out.Main[121];      st2=Out.Main[119];       Ind(st1, st2);      } break;
                    } // switch
                    PrNoBtnWait=1;
                }
                if(pr_btn==0) // No // ��������� ������
                {  Koib[1].VS = 500;     PrNoBtnWait=1;         }
                pr_btn=-1;
            } else
            {  // ������  ��������� "����������" ���  ������������  �������
                 //st1=Out.Main[114];    Ind(st1,"");
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
				Ind(Out.Main[122],Out.Main[119]); //������ ������. ��������� ������?
                Say(9); //������ ������. ��������� ������?
				if(pr_btn==1) //Yes  //  ������� �  ���������� ��������� ������� ����� �� ��  ����
                    {   Koib[1].VS = 6;				  Sos_6(); 				  PrNoBtnWait=1;			}
				if(pr_btn==0) //No  // ������� �  ��������� ������
                    {	  Koib[1].VS = 500;  PrNoBtnWait=1;    }//����� �� ��������!
				pr_btn=-1;
			}
			else
			{ // ������ � FLASH ���������!
				IDsave(); //�������� ������ �� HDD � Avar
				Ind(Out.Main[18]," "); //  ������  ��������
				Sos_8(); // ������� � 8 ���������
                Koib[1].VS = 8;
                PrVoteTimeStart=0;     PrVoteTimeEnd=0;
			}
            UMount("/mnt/usb");
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
			    if(CheckPrinter()!=-1)
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
            {
                Koib[1].VS = 11;
                Sos_11();
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
			    if(CheckPrinter()!=-1)
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
			{
				Koib[1].VS = 11;
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

                 // int pr1=SyncData( K, KB, VC);

		  } else
		  {  // ��������� "�������������..." ��  �����������  �������
		       st1=Out.Main[31];    Ind(st1," ");
		  }


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
                  Koib[1].VS = 23;         Sos_23();       PrNoBtnWait=1;
                }
                if(pr_btn==0) //No
                {   // ������� � ������ ���������� �  �����������
                    Xblank = 1;    for (int i = 1; i <=NumCandMax; i++)     Snat[i].Nsnat=0;
                    Koib[1].VS = 16;      Sos_16();        PrNoBtnWait=1;
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

                   if(CheckPrinter()!=-1)
			    {	    Ind(Out.Main[114]," "); //  ����������
                       // Print(1);
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
                    Say(62); //������ ������� �� ����������!
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

		  } else { //������ ����� ������, ���������� ������

		  }
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
			//st1=Out.Main[47]+" 0"; //����:...
           // Ind(Out.Main[102],st1); //����� �� ������ ������������ - ���

		} break;
	case 50: //----------------------------------------------------------��� ������ ������������
		{
	        if(pr_btn==0) //No
           {  // ���������  ���� �������� ����������
  	           Koib[1].VS = 24;    Sos_24();   pr_btn=-1;
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
                       //Page_back();    SayVotes(6); // ������� �� ������ ���������
                    }
                    if((pr_opt>1)&&(pr_opt < 6))
                    {       //Ind(Out.Votes[7],""); //��������  ���������  ���������
                        // ��������� ���������
                        SayVotes(7); // �������� ��������� ����� ������� ����!
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
                       //Page_back();    SayVotes(6); // ������� �� ������ ���������


                    }
                    if((pr_opt>1)&&(pr_opt < 6))
                    {       //Ind(Out.Votes[7],""); //��������  ���������  ���������
                        // ��������� ���������
                        SayVotes(7); // �������� ��������� ����� ������� ����!
                        //sleep(1);
                    }
                }
               //get_Optrons_Imm();
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

                   PrNoBtnWait=1;
               }
  	           if(pr_btn==0) //No
  	           {  // ������� �  �������� ������
  	               Koib[1].VS = 6;
  	               Sos_6();
  	               PrNoBtnWait=1;
			   }
			   pr_btn=-1;
		  } else
                {  // ������  ��������� "������������"/"����������" ���  ������������  �������
                    st1=Out.Main[46];  st2=Out.Main[114];    Ind(st1,st2);
                }
	    } break;

	case 30: //-------------------------------------------------------------------����� �������� ��
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
		}break;
	case 60: //--------------------------------------------------------------------��� ������������� �����������
		{
            if(pr_btn==2) //����� ��  ������ ������������� ����������� �������������� ��+���
  	        {
  	            if ((Koib[1].Main)||(Koib[1].Avto))
  	            {  // ���������� ����� ���� ������ ��  �������� ������� ��� �  ����������  ������
                    if((PrVoteTimeEnd==2)||(PrVoteDay!=1))
                    {  	Koib[1].VS = 33; //�������������� �����?
                            Sos_33();
                            //Ind(Out.Main[61],Out.Main[65]); //��������� ������������ �����������? ��/���
                            //Say(40); //��������� ������������ �����������?
                            PrNoBtnWait=1;
                    }
                    if((PrVoteTimeEnd!=2)&&(PrVoteDay==1))
                    {   // ����� ������������� ����������� ��� �� �������
                        // �������  ������������� ����� ��  ������ �� ���������  �������
                        // ����������
                        TekDateTime(); //������� ���� � �����
                        int Minuts=(Votes.Bhour[2]-TekDT.hour)*60- 60+TekDT.minit;
  /*!!!*/	        st2=Out.Main[106]+" "+IntToStr(Minuts)+" "+Out.Main[101];
                        Ind("",st2);
                        // ���������  ��������� �� ����
                        Golos="";
                        // Golos=OutV.Main[**]+  ** �����
                    }
  	            }
               pr_btn=-1;
 	        } else
 	        {  //  ��������� ��  ���������  ����������� ����� 10 �����
 	            if ((Koib[1].Main)||(Koib[1].Avto))
 	            {
 	                 if(PrVoteTimeEnd==1)
                    {   PrVoteTimeEnd=11; PrVoteTimeEnd=2;
                       st1=Out.Main[106]+" 10 "+Out.Main[101];
                      Ind(st1,"");
                     // ���������  ��������� � ���, � �� �� ����� ����������� �������� 10 �����

                    }
                    if(PrVoteTimeEnd==12)
                    {   PrVoteTimeEnd=13; PrVoteTimeEnd=2; Ind(Out.Main[106],"");
                        // ���������  ��������� � ���, � �� �� ����� ����������� �������� 5 �����
                        st1=Out.Main[106]+" 5 "+Out.Main[101];   Ind(st1,"");
                    }
                    // st=Out.Main[57]+TimeS; // ������������ �����������! ����� *.*
 	            }
                if((PrVoteTimeEnd==2)&&(PrVoteDay==1))
                {  // ����� ���� �����������   ������� ( �  ���� �����������)
                    st1=Out.Main[59];   Ind(st1,"");
                    if ((Koib[1].Main)||(Koib[1].Avto))
                    {
                            Koib[1].VS = 32;
                            Sos_32();
                            Say(41); usleep(200000); Say(42);
                            PrApprove=0;
                            cout << " Vote Time is over"<< endl;
                    }
                } else
                { // ���������� ����� �����������
                  //Ind(Out.Votes[1],""); // ������������
                  //�������� �� ������������ ������� ���������
				  // ���������  ���������, "������������!"
				  if(pr_opt==6)
				  {  // ��������� �������� ���������
                        //SayVotes(1);

                        Golosovanie();

                        // ���������  �������� " �� ��������!"
                        SayVotes(8);// �������
                        //usleep(200000);
                        //Ind(Out.Votes[6],""); //�� ��������!
                        //SayVotes(6);// �� ��������
				  }
                  if(pr_opt==100)
                  {   //-------------��������� �����, ��������� �������
                        SayVotes(2);
                        Page_back();
                                             // if(pr_rr==6)
                        //Ind(Out.Votes[2],""); //��������� ��������������� �����! 2 2 �����
                  }
                  if((pr_opt>1)&&(pr_opt < 6))
                  {  // Ind(Out.Votes[7],""); //��������  ���������  ���������
                      //usleep(200000);
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
                   //SayVotes(1); // ������������
  	               Golosovanie();
                   SayVotes(8);// �������
                   //usleep(200000);
                   //Ind(Out.Votes[6],""); //�� ��������!
                  // SayVotes(6);// �� ��������

                }
  	        }
  	         if(pr_opt==100)
             {   //-------------��������� �����, ��������� �������
                 SayVotes(2);
                 Page_back();
                 //Ind(Out.Votes[2],""); //��������� ��������������� �����! 2 2 �����
                // SayVotes(2);
              }
              if((pr_opt>1)&&(pr_opt < 6))
              {   //Ind(Out.Votes[7],""); //��������  ���������  ���������
                  //usleep(200000);
               }
                if (pr_btn==2) //No
                {  // ���������  ������������  �����������
                    //
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
/* !!!! */ // ������� ������������  ������� � ���������  ���������



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
			//if(PrModeChange==1 ){   Sos_34(); PrModeChange=0; 	}
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
	        if(pr_btn==2) //Yes+No ����� �� ������ �����������  �����������
  	        {
  	           Koib[1].VS = 35; // ��������� ����������  �����������
  	           Sos_35();
 	        } else
            {   if(pr_opt==6) //Yes
                {
                    Golosovanie();
                    SayVotes(8);// �������
                }
                if(pr_opt==100)
                {   //-------------��������� �����, ��������� �������
                    Page_back();
                     //Ind(Out.Votes[2],""); //��������� ��������������� �����! 2 2 �����
                     SayVotes(2);
                }
                if((pr_opt>1)&&(pr_opt < 6))
                {   //Ind(Out.Votes[7],""); //��������  ���������  ���������
                    //usleep(200000);
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
		  }
		} break;
	case 36: //---------------------������� � ���������� ������ �����������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if(PrModeChange==1 ) 	{  Sos_36(); PrModeChange=0; 	}
			if(pr_btn==1) //Yes
			{
                Print(3);
                XVote=1;
                Koib[1].VS = 37;
                Sos_37();
/* !!! */ // �������� �  �����������  ������ ��������� ����������  ������

			}
  	    	if(pr_btn==0) //No
  	        {   // ������� � �����������  �����������
  	            //Koib[1].VS = 70;
  	            Koib[1].VS = 34; PrModeChange=1; PrNoBtnWait=1;
			}
			pr_btn=-1;
		  } else {
		    //
		  }
		} break;
	case 37: //------------------���������� ����� ��������� ���������
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
             if(PrModeChange==1 )  {   Sos_37(); PrModeChange=0; 	}
			 if(pr_btn==1) //Yes
			 {
			    Print(3); //���������� ����� ��������� ����������� ����������� �� ������� XVote
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
			       	Koib[1].VS = 39;
			       	XVote=1;
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
               //Speak(4);
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
		     if (PrModeChange==1)   {   Sos_41(); PrModeChange=0; 	}
			 if(pr_btn==1) //Yes
			 {
                 Xv=0; //��������� �������� �������� ������� XVote � 0
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

               if((pr_btn==1)||(PrEnterStart==1))
                //if(pr_btn==1)
                {  // ���������� ���������� ��������
                    if((pr_btn!=0)&&(pr_btn!=3))
                    { get_KeyString(NUMBER);
                      zz=atof(NUMBER);
                      Votes.Blank[XVote].Vopr[1].Protocoles[1].Lines[Xv].Value=zz;
                    }
                    if(pr_btn==3)
                    { // ����������� ��  ���������� ������
                      Xv=Xv-1;
                      Sos_42();
                    } else
                    {
                        if (NextXVopr()) // ���������� ��������� �������������� ������ ���������, ���� ��� - ��������� ������
                        { // �������  �  ��������� ������
                            Sos_42();
                            PrEnterStart=1;
                        } else
                        {  // �������� ���������  ����� ������ �� ���� �������
                            if (XVote>=Votes.Nblank)
                            { //���  ������ �� ���� ������� �������, ����� �� ������ ����� �����
                            Koib[1].VS=44; //�� ������ ��
                            Sos_44();
                            PrEnterStart=0;
                            } else
                            { //��������� ������
                                Koib[1].VS=39; //������ �� ���������� ��������
                                PrModeChange=1; PrNoBtnWait=1;
                                //Sos_37();
                            }
                        }
                    }
                    pr_btn=-1;
                };
                if(pr_btn==0) //No
                {  // ����� �� ����� ���. �������� ��� ������ �������
                    Koib[1].VS = 44;
                    Sos_44();
                    PrEnterStart=0;
                }
                pr_btn=-1;
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

			if(pr_btn==1) //Yes
			{
                Koib[1].VS = 45;
                Sos_45();
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
		    Ind(Out.Main[84],""); //����������  ������
		  }
		} break;
	case 45: //-------------�������� �������� ����������� ��������?
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			Print(5);
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
                get_KeyString(NUMBER);
                int zz=atof(NUMBER);
                PrEnterStart=0;
                //���������� N ����� ��
                for(int i=1; i<=zz;i++)Print(5);
                Koib[1].VS = 48;
                st=Out.Main[93]+" "+IntToStr(XVote+1)+" "+Out.Main[94];
			    //���_����������_������_��_�������_N_XVote+1_�������_��
			    st1="";
			    Ind(st,st1);
			}
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		}// break;

	case 48: //------------------------
		{
		  if ((Koib[1].Main)||(Koib[1].Avto))
		  {
			if (XVote<Votes.Nblank)
			{  // �������  �  ����������  ����  �������
			    XVote++;
			    Koib[1].VS = 39;
			    PrNoBtnWait=1;
			    PrModeChange=1;
		    } else
		    {   // ��� ���� ������� ���������� ��� ���������� ������ �����������
		        st=Out.Main[95];  //��� �������� � ���������� ������ ������� ��
			    st1=Out.Main[96]; //���_����������_�����_�����������_�_������_������_�����_�������_�������_���
			    Ind(st,st1);
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
			{  // ������� � ���������� ������
			    Koib[1].VS = 35;
			   // Ind(Out.Main[66],Out.Main[67]);
                //��� �������� � ���������� ������ ����������� ������� ��, ��� �������� � ����������� ������� ���
			    PrNoBtnWait=1; PrModeChange=1;
			}
  	    	if(pr_btn==0) //No
  	        {  // ���������� ����������� ������� �������
  	            //if (SaveIG(XVote)) { Ind(Out.Main[92],""); }//����������_��������_��_Flash
  	            // ������� �  ������  �����  �������
  	            Koib[1].VS = 51;
                st1=Out.Main[109]; // ������� ���������� � ����
  	            st2=Out.Main[107] + ".  "+Out.Main[108]; // ����� ���� - ��, ������� � ���������� ������ - ���
			    Ind(st1, st2); //�������� Flash, ������� ��
 	        }
 	        pr_btn=-1;
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;
		case 51: //----------����� ���������� ���������� � ������������ ������
		{
          if ((Koib[1].Main)||(Koib[1].Avto))
		  { // ������ ����������� � ������� � ����� �������
            if(pr_btn==1) //Yes
			{  // ������� � ����������  ��������� ����� ��������� ����� �����������
			    Koib[1].VS = 1;
 	            PrNoBtnWait=1;
 /* !!!! */ // �������� �  �����������  ������  ��������� ��������� ��������


 	            UMount("/mnt/usb"); // ���������� ����-�����
			}
  	    	if(pr_btn==0) //No
  	        {  // ��� �������� � ���������� ������ ����������� ������� ��,
  	            // ��� �������� � ����������� ������� ���
  	            Koib[1].VS = 35;
			    Ind(Out.Main[66],Out.Main[67]);
			    PrNoBtnWait=1;
 	        }
 	        pr_btn=-1;
	      } else {
		    Ind(Out.Main[84]," "); //����������  ������
		  }
		} break;

  case 500: //------------------------
		{  Exit=1; Koib[1].VS =3;
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

    //�������� ������� ������ �������

    //Parser(FileName);
   // Speak(3);


    /*
    //�������� ������� ������ �������
    Votes.Nblank=1;
    Snat[1].Nsnat=1;
    Snat[1].Snat[1]=1;

    Speak(2);
    */



    Exit=false;
    Exit=Sostoanie();

   return Exit;
}



