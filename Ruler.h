// ---------------------------------------------------------------------------

#ifndef RulerH
#define RulerH

#include "MyX.h"
#include "HeaderMain.h"

// ----------------------------����������� ������--------------------------------
// ------------------------------------------------------------------------------
extern int NumBullAll; // ���������  ���������� �������� ����������
extern int pr_btn;
extern int pr_opt;
extern int pr_rr;
/* 10.06.13 !!!!!! */ extern  char OutS1[128];
/* 10.06.13 !!!!!! */ extern  char OutS2[128];
/* 10.06.13 !!!!!! */ extern  char OutS[128];  // �������������� ���������


//------------------------------------------------------------------------------
//-----------------------------------�������------------------------------------
//------------------------------------------------------------------------------
std::string dtx1(int m1 , int m2, int m3, int flag, int Lang);
std::string CisloToStringOrVoice(double Cislo, bool String);
std::string DateToStringOrVoice(int year,int month,int day, bool String);
std::string TimeToStringOrVoice(int Chas,bool C,int minit,bool M,int sec,bool S,bool String);

//void LoadInd(void); // �������� ��������� �� ����� ��������
int WaitOptrons(int Vektor);// �������� ��������� ��������� ��������
int ImmOptrons(void);// ����� ��������� ��������� �������� �� ����
int WaitYesNo(void); // �������� ������� ������ �� ���
int ImmYesNo(void); // ������������ ����� ������� ������ �� ���
bool DriveGO(int Vektor, bool Solenoid); // ���������� ��
int WaitRR(void); //�������� ����������� ������������� ���������

bool SaveIG(int XVote); //������ ����������� �� Flash
bool LoadSetup(void); // �������� �������������� ������
bool AvarFiles(void);  // �������� �� ����� ���������� ����������
//bool CheckPrinter(void); //����������� - ���� �� �������?
bool SyncData(TKoib K, bool KB,TVotes V, bool VC); //����� ������� �� ����
int KoibMain(void); // ����������, ������� �� ����
void Ind(std::string st1, std::string st2, int t); // ����� ������ �� ���������
void Zagruzka(void);      // �������� ��������� ����������
int Expression(std::string st); //
void Delay(int Second);   // ��������
int Samotestirovanie(void);    // ���������������� ������������
bool Catch(void); // �������� ���������� ������

//void TekDateTime(void);

void IDsave(void); //������ �� � ��������� ����������
void IDload(void); //�������� �� �� ��������� ����������
bool Protokol(std::string Prot, int Xbl, int Xv, int j); //���������� ������ ���������
//bool Parser(std::string Path); // ������ XML-�����
int MakeFile(char* NameF);
bool Print(int Type); // ������ �� ��������

void SayN(std::string st1); //�������� ������� �������
void Say(int Type); //�������� ������� �������

//void Say5(std::string str1, std::string str2, std::string str3,std::string str4,std::string str5); //�������� 5 ������� �������
bool Speak(int Type); // ��������� ���� ��� ������� �������
void KoibZero(void); // ��������� ������������ ���������� �����

bool NextXVopr(void); //���������� ��������� �������������� ������ ���������, ���� ��� - ��������� ������
int Sostoanie(void); // ��������� ������� ��������� ����
int Golosovanie(void); // ���������� ������� ��������� ��� �����������
bool Timer(bool GoSG, int day, int chas, int min); // �� �������
void DeShifr(void);

void SetMainTimer(void); //��������� �������� ������� MainTimer
void Start(bool S);
bool RUN(void); // ����� ����� ���������


//------------------------------------------------------------------------------
//----------------------------------����������----------------------------------
//------------------------------------------------------------------------------


//----end �������� ��������������� �����������




//------------------------------------���������---------------------------------
 const unsigned int MainTimer=1;
 const unsigned int SocketTimer=2;
 const std::string newstr("\n");

 extern bool Exit;
 extern int Xblank; // ���������
 extern int Xcand; // ���������
 extern int Xbl; // ���������
 extern int Xv; // ���������
 extern int XVote; // ���������



#endif
