//*****************************************************************************
// ���� ����������  �������  ������������  ���������
//
//                     �.�������      15.01.2013
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

//-----------------������  ��  �������� �� �������� ������---------------------
   int PrKoef;
   int NstrFrControl; //  ������� ����� ��� ������������ �������� ������
   float dyLine1M;
   int dyLine1;

   char NUMBER[32]; //  �����, ���  �����������   ����� �  ����������


   int PrUprOpt; // ������� ���������  ��������
   int PrNoBtnWait; // ������� �������� ��������  ������
   int Sost_Optrons[4];

   int	NumV;   // ���� �������  �����
   int MaxMarks;   // ���� �������  �����
  // ��������� ����
   int Lang;  // ���� ����������
   int VolumeUr; // ������� ���������
   int VolumeUrMax; // ����������� ������� ���������
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

   // ���������  ����������
   int PrCheckBr; // ������� ������������� �������� ������ �����
   int PrCheckSh; // ������� ������������� ��������  ������
   int PrCheckEx;// ������� ������������� ���������� ���������������� ����������

   int MaxLightR; // ������������ ������� ��������� ��� ����������
   int MaxLightG; // ������������ ������� ��������� ��� ����������
   int MaxLightB; // ������������ ������� ��������� ��� ����������
   float KoefY,KoefX; // ������������  ������ ��� ���������� �����
   int PrKoefY,NumDotKoefY;
   int PrIdOk; // ������� ����, ��� �������� ������ ��� ������������ ���������� � ������� ������
   int PagePar[NumVoteType+1][NumParPage]; // ������ ���������� ��������
   int PrVoteTypeId[NumVoteType+1]; // �������� ���� ���������� �  ������� �� ������ �������� ������



   int NumDotX; // ����������  ����� ��  ��  �� ����������� �����
   int NumDotY; // ����������  ����� ��  ��  �� ���������   �����

   float Max_Bull_Height; // ������������ ����� ���������
   float Max_Bull_Width; // ������������ ����� ���������
   float Dx_Max; // ���������� ���������� ������ �����
   float Dy_Max; //  ���������� ���������� ������  �����
   float MaxAngle;// ������������ ���� �������  ������
   float Dig_Height; //  ������  ���� �  ���������
   float Dig_Width; //  ������  ���� �  ���������
   float Dig_Interv; //  ���������� �����  ������� ������
   float Max_ShAngle; // ������������ ���� ������� ������

   float PageWM; // ������  �������� �  ��
   float PageHM; // ������  �������� �  ��
   float dxVoteTypeM; // ������ ���� ���� ������� �� ����������� �  ����������
   float dyVoteTypeM;    // ������ ���� ���� ������� �� ��������� �  ����������
   float LineWM;    // �������  ����� ���� ���� ������� �  ����������
   float LMarSizeM; // ������ �����  ��������� �  ����������� �� �����������
   float TMarSizeM; // ������ �����  ��������� �  �����������  �� ���������
   float dxStampAreaM; // ������  ����  ������ �  ��
   float dyStampAreaM; // ������  ����  ������ �  ��
   float dxStampM; // ������  ������ (������) �  ��
   float dyStampM; // ������  ������ (������) �  ��
   float dyNumZoneM;// ������  ���� ������  � ��
//-----------------���������� �� ��������������� �������� �  ������------------
	int PageW; // ������  ��������  �  ������
	int PageH; // ������  ��������  �  ������
	int LineWx;			//  ������� ����� �  ������ �� �����������
	int LineWy;			//  ������� ����� �  ������ �� ���������
	int LMarSize; // ������ �����  ��������� �  ������ �� �����������
	int TMarSize; // ������ �����  ��������� �  ������  �� ���������
	int dxStampArea; // ������  ����  ������ �  ������
	int dyStampArea; // ������  ����  ������ �  ������
	int nsShArStart,nbShAr,ShArXo,ShArYo,ShArPr,colShAr;
	// �������������� � ���������� ��� ������ ������
	int dxStamp; // ������  ������ �  ������
	int dyStamp; // ������  ������ �  ������
	float dyNumZone;// ������  ���� ������  � ������
	int nstrStampStartL; // ������ ������ ������ ������ �����
	int nstrStampEndL; // ������ ��������� ������ ������ �����
	int nstrStampStartR; // ������ ������ ������ ������  ������
	int nstrStampEndR; // ������ ��������� ������ ������ ������
	int PrFirstL,PrFirstLy; // ������� ���������� ������ ����� ����� ��� ������ ������, ����
					// ���������� ���� �������� �����
	int PrFirstR,PrFirstRy; // ������� ���������� ������ ����� ������ ��� ������ ������, ����
					// ���������� ���� �������� �����
	int PrXMin,PrXMax; // �������� ���������� �������� � ��������� X
			// ��� ������ �����, ���� ���������� ���� �������� �����
	int XoShR1,YoShR1,PrAlfaShR,XoMax,YoMax,nbXoMax;
	int dynstr; // ��������, �� ������� ������������� ���� ������� ������
	int colStampL; // ������� ������� ������ ������ ������ �����
	int colStampR; // ������� ������� ������ ������ ������ ������
	int colStampLo; // ��������� ������� ������ ������ ������ �����
	int colStampRo; // ��������� ������� ������ ������ ������ ������
	int dxStFindL; // ���������� ����� ���  ������ ������ ���� ������
	int dxStFindR; // ���������� ����� ���  ������ ������� ���� ������
	int dyStampNum; // ������  ����  ���������� ������ �������
	int masXoR[6],masXoL[6]; // ������� �������� ������ ���� �������� ������ � �����
						// ������ ������ ��� ���������� ������
	int XoShL,XoShR,YoShL,YoShR; // ���������� ������ � ������� ������ ����� ������
	int PrShL,PrShR; // �������� ����, ��� ���������� ����� � ������  ����� ������ �������
	int PrShFound; // ������� ����, ��� ���������� ������ �������
	int nbShL,nbShR; // ��������  ���������� ������ ������ ������� �����
	int nbyShL,nbyShR; // �������� �������� ����� � ������
	int PrNaklShL,PrNaklShR; // ������� �������  ������ "1" - �����, "0" - ����� ��� ������
	int NumUIKRec=0;// ����� ���������� �������������  �������� ������������,
					// �������� � �������� ������
	char NumUIKStr[5]; // ���������  �������� ������ �������������  ��������
	int UIKNum[MaxNumUIK];      //������ �������, ���������� ��� �������
	int dyMF; // ������ ���� ������ + ���� ���� �������
	int dyHeader; // ������ ���������  � ������
 // ---------------������� ���  ����������� ���� �����������
	int PrVoteType[NumVoteType+1]; // ������  ���������  ����  �����������
	int PrVoteTypeTop[NumVoteType+1]; // ������  ���������  ����  �����������
			//  �  ����� �����
			//  ������ [0] - ������ �����������  �������� ��������� ( BulHead)
	int nbVoteT[NumVoteType+1]; // ������� ����� �  ����� ������ ��� ������
			// ����������� ��������� �  ���� �������
	int nstrVoteType[NumVoteType+1]; // �����  ������, �  ������� ����������
// ����� ��������  ��� ������� ���� ������� ( �  ������ ������� �����)
    int ncVoteType[NumVoteType+1]; // ������� �  �������� ������������� �������
	int dxVoteType; // ������ ���� �������� ����  �������    � ������
	int dyVoteType; // ������ ���� �� ��������� ��� ����������� ���� ������� � ������
	  // ������ ����� � ����  ����  ������� (�������� ������ (�����) ���������
	int PrVoteTypeStart[NumVoteType+1]; // �������� ������ ������ ���� ������� ��  ������� �����
// ������ � ������������  ��������� � ��  ���������	��� ������� ���� �������
// ���  ������� � ���������  �����������
	int NumSQMax[NumVoteType+1];  // ������������  ���������� ���������
					//��� ������� ���� �����������  (�� �������� ������)
	//int NumSQMaxVopr[NumVoteType+1][NumVoprMax+1][2];  // ������ �� ��������� ���
			// ��� ������� �������
	int**** MasSQ;// ���������� ��������� ��� ������� (�������� �� ��������
					// ������) � ��������� (�����������) ������� ���������
	int** SearchSQ; // ������ ���  ������ ������ ��������, � ����� ��� ���������
					// �����  ������ ��������
	int **NumBlP,**NumPMax; // ���������� ������ � ����� ����� �  ������� ��������
	int **RezRec; // ����������  ������������� ��������� ( ������� �������)
	// ��������� �������� ����� ������������  �����
	TScanOut MasScan; //TScanOut
	// ------------------����������  ���  �����������  �����  ����� (������-�����)
	int UrWhite0; // ������ ������ ������
	int UrBlack0; // ������ ������ �������

  float p00,   // ����������� ����,  ���  �����  �����  ������  ����
 		p11,    // ����������� ����,  ���  �����  �����  �����  ����
		alfa,   // ����������� ������  �������  ���� ( ��������  �������)
		beta,// ����������� ������  ������� ���� (��������� ������ ��  ������)
		g0,     // �������� "������" "1" - ����  ������
		g1,     // �������� "�����"  "1" - ����  �����
		surw,   // ������ ��� ���. ��������  ������  ������ (������� ��������)
		surbl,  // ������ ��� ���. ��������  ������  ������� (������� ��������)
		urw, urw1,UrWtSh,   // ������ ���. ��������  ������  ������ (������� ��������)
		urbl,urbl1,UrBlSh;   // ������ ���. ��������  ������  ������� (������� ��������)
	double	AlfaPage;   // ���� �������  �����
	int dyALfa,dxALfa;   // ������� ��������� Y  ����� � ������ �����

//------ ���������� ��� ������ �������  ����� ���  ������  ����� ����������������
  int ncL;  // �����  ��������� ������� (�������) ��� ������ ����� �������
  int ncTL; // �����  ��������� ������� (�������) ��� ������ �������
			// ������� �����
  int dxT;  // ����������  ����� ������ ���  ������ �������� �� ����� ����
			// ��� ������  ������� �������
  int dxL;  // ����������  ����� ������ ���  ������ �������� �� ����� ����
			// ��� ������  ����� �������
  int nbTL; // ������� ����� �  ����� ������ ��� ������ �����  �����
  int nbLL; // �������  �����, ���������� ���� ��� ������ ������ �����
  int PgXoL;// ���������� ����� ���������  �����  �� ���  �� (���������)
  int PgYoL;// ���������� ����� ���������  �����  �� ���  �Y (���������)
  int ncR;  // �����  ��������� ������� (�������) ��� ������ ������ �������
  int ncTR; // �����  ��������� ������� (�������) ��� ������ �������
			// ������� ������
  int nbTR; // ������� ����� �  ����� ������ ��� ������ ����� ������
  int nbRR; // �������  �����, ���������� ���� ��� ������ ������ ������
  int PgXoR;  // ���������� ������ ���������  �����  �� ���  �� (���������)
  int PgYoR;  // ���������� ������ ���������  �����  �� ���  �Y (���������)
  int PrLeft;  // ������� ����, ���  �����  ������� ����������
  int PrTopL; //  ������� ����,  ��� ����  �����  ������
  int PrTopR; //  ������� ����,  ��� ����  ������  ������
  int PrRight;// ������� ����, ���  ������  ������� ����������
  int nstr;   // �����  �������  ������
  int nstrL;  //  �����  ������, �  �������  ������  ������
			// ���������� �������� �����
  int nstrR;  //  �����  ������, �  �������  ������  ������
			// ���������� �������� ������
  int nstrBegVoteType; // ����� ������, ��  �������  ���������� �������� �����
			// ��� ��������� ���� �������
  int nstrHeaderEnd; //  ������, ��  �������  ������������� �������� ���������
				// ���������
 int nstrLsEnd; 	//  ������, �� ������� ������������� �������� ����� ����� ���������
 int nstrRsEnd; 	//  ������, �� ������� ������������� �������� ����� ����� ���������

 int nstrLsStart; 	//  ������, �� ������� ������������� �������� ����� ����� ���������
 int nstrRsStart; 	//  ������, �� ������� ������������� �������� ����� ����� ���������

 int colTestPageL; 	// �������, �� �������  ����������� ���������  ���������   �����
 int colTestPageR; 	// �������, �� �������  ����������� ���������  ���������   ������
 int dxTestPage;  	// �����  ������ ���  �������� ��������� ���������
 int nstrPageEnd; 		//  ������, �� ������� ������������� ��������
 int PrPageEnd;

 int Step;   //  ���, ������������ ����������� ������: "1" - ����� �������;
				// "-1" ������ ������

  int nstrFrYmax; // ����������sq ����� ������ �� ������ �����
// -------------------���������� ����������  ����� ���������,  ---------------
//--------------------������������ �� ����� ������������  -------------------

 int FrXoL;
 int FrYoL; // ���������� ������ ���� ����� ���������
 int FrXoR;
 int FrYoR; // ���������� ������� ���� ����� ���������
 int EndYoL,EndYoR,EndXoR,EndXoL,PrEndXoL; // ���������� ������ ����� ����� �����
 int BulHead=0;// ������� ����������� ���������:
				// "0" � ������ �����������;
				// "1" - � �������� �����������
 // ���������� ���  �������� ����� ���������
 int LsCol,Lsdx,nbLs,LsXo,LsPr,LsNs;
 int RsCol,Rsdx,nbRs,RsXo,RsPr,RsNs;

//----------------------����������  ��� ����������  ��������������  ����������
//--------------------- �  ������  ����� (�����  ����������� ������ �����)
 int nr1;// ����� ������,  � �������  ����������  ���������  ������� ������� (������)
 int nr2;// ����� ������, �� ������� �������������  ���������  ������� ������� (������)
 int nc1;// ����� �������, � ��������  ����������  ���������  ������� ������� (������)
 int nc2;// ����� �������,  �� ������� �������������  ���������  ������� ������� (������)
 int nn; //  ���  ����������
//---------------- ���������� ��� ���������� �������������  --------------
 int PrPage;  // ������� ����, ��� ������ ����� ����������
 int PrFrame; // ������� �����������  �����  �����
				 // "0" ����� �����  �� �������,
				 // "1" - ������ �����  �������,
//----------------- ������������� ���������� ------------------------------
	unsigned urw_ch; // �������� ������������� �����
	float dd, gam, sigma,u1,u2,xx,yy,aa,bb,AlfaSh,AlfaShR;
	float SinAlfaPage,CosAlfaPage,TanAlfaPage;
	int i,j,k,prizn,ii,nVoteType,XoSh1,YoSh1,PrAlfaSh,XoMin,YoMin,nbXoMin,XiL,XiR;
	int zz,dyy,DeltaX,DeltaY,dxAl,dyAl;
	int dxSQ,dySQ,NumPLines,NumWtP,SPL,SPM,NumPV,PrAlfa,PrPageSize=0,summa=0;
	int dxFindSQ; // ���������� �����  �  ������  ���  ������ ������ ��������
	int nstrUrWhite; //  ������, ��  �������  �����������  ����� ����
	char Ch;
	int XoLSh,PrXoLSh, XoRSh,PrXoRSh; // ���������� ����� ������ �� �


int ReadSetUpFiles(void)
{ // ����������  ������ ��� ������������� �������
  int i,/*sum0=0,sum1=0,sum2=0,*/ pr=0;
  std::string sst;
  //  ����������  ���������� ����
  pr=SetUpKoibRead();
  if(pr==1)   {  sst=Out.Menu1[40];   setSecline(&sst[0],1);   };
  // ���������� ���������� ����������
  if(pr==0) pr=SetUpUprRead();
    if(pr==1)   {  sst=Out.Menu1[41];   setSecline(&sst[0],1);   };
    // ���������� ����������  ����������
  if(pr==0) pr=SetUpBullRead();
    if(pr==1)   {  sst=Out.Menu1[42];   setSecline(&sst[0],1);   };
  // ���������� ����������  �������������
  if(pr==0) pr=SetUpRecRead();
    if(pr==1)   {  sst=Out.Menu1[43];   setSecline(&sst[0],1);   };
  // ���������� ����������  ��������
  if(pr==0) pr=ReadKoeffSens();
    if(pr==1)   {  sst=Out.Menu1[44];   setSecline(&sst[0],1);   };
    // ���������  ������ ��������� ���������
    setVoiceVolume(VolumeUr);
    setVoiceVolume(3);
  // ����������  ��������� �������� �� ������
  //MaxLightR=160; MaxLightG=160;  MaxLightB=160;
  altera_backlight(MaxLightR,MaxLightG,MaxLightB);

 NO_PAPER  =MasSenValue[0][0];
 ONE_PAPER=MasSenValue[0][2];
   for(i=1;i<NumSensors;i++)  if(NO_PAPER > MasSenValue[i][0]) NO_PAPER=MasSenValue[i][0];
            NO_PAPER=NO_PAPER-20;
    for(i=1;i<NumSensors;i++)  if(ONE_PAPER  < MasSenValue[i][2]) ONE_PAPER=MasSenValue[i][2];
            ONE_PAPER=ONE_PAPER+20;

	 //  ������������ ���������� ���������  �  ������� ����������

  PageWM=Max_Bull_Width;// ������  �������� �  ��  (��� �������� ( ������������)
  PageHM=Max_Bull_Height;// ������  �������� � ��� �������� (������������) ��
  PageW=int(PageWM*float(NumDotX)); // ������  ��������  �  ������
  PageH=int(PageHM*float(NumDotY)); // ������  ��������  �  ������

  LMarSize= int(LMarSizeM*float(NumDotX));
  TMarSize= int(TMarSizeM*float(NumDotY));// ������ �����  ��������� �  ������  �� ���������
  LineWx=int(LineWM*float(NumDotX));	//  ������� ����� �  ������ �� �����������
  LineWy=int(LineWM*float(NumDotY));		//  ������� ����� �  ������ �� ���������
  LMarSize=int(LMarSizeM*float(NumDotX)); // ������ �����  ��������� �  ������ �� �����������
  TMarSize=int(TMarSizeM*float(NumDotY)); // ������ �����  ��������� �  ������  �� ���������
  dxStamp=int(dxStampM*float(NumDotX)); // ������  ����  ������ �  ������
  dyStamp=int(dyStampM*float(NumDotY)); // ������  ����  ������ �  ������
  dyNumZone=int(dyNumZoneM*float(NumDotY)); // ������  ����  ������ �  ������
  dyStampNum=int(dyNumZoneM*NumDotY); // ������  ����  ���������� ������ �������
  dxVoteType=int(dxVoteTypeM*float(NumDotX)); // ������ ���� ���� �������
  dyVoteType=int(dyVoteTypeM*float(NumDotY)); // ������ ���� ���� �������
  dyMF=TMarSize+2*LineWy+dyVoteType; // ������ ���� �� ������ ����� �� ������  ����  ���������
  nstrStampStartL=LineWy+dyStamp-dyStampNum; // ������ ������ ������ ������
  colStampR=PageW-(LMarSize+LineWx)-2;  // ������� ������ ������ ������ ������
  surw=5;   // ������ ��� ���. ��������  ������  ������ (������� ��������)
  surbl=5;  // ������ ��� ���. ��������  ������  ������� (������� ��������)

    //MAXLINES=PageH+5*TMarSize;
    MAXLINES=int(Max_Bull_Height*float(NumDotY))+5*TMarSize;

    //printf(" MAXLINES  =   %d\n",MAXLINES);

  return pr;
} //ReadSetUpFiles

unsigned GetXYValue(int xi,int yi) // ���������  ��������  ������  ������� �  �����
{ //xi  ����� ������,  yi - ����� �������
   urw_ch=PixCol[xi][yi];
   return urw_ch;
}

void FormIdScan(void)
{ // ������������ ������ ��� ������������ �� �������� �������� ������
  // ����������� �����  ��������� � ����������� �������� ������ � �����������
  // ��  ���������� ���� ����� ���������  �����������
	int dxR,dyR,dySh;
   // �������� ���������� ������ �� ����� �����������
   for(i=0;i <= NumVoteType;i++)
   {  NumSQMax[i]=0;
	  for(j=0;j < NumParPage ;j++)PagePar[i][j]=0;
   }
   // ���������� ������ ������� ��� �������  �������
   // �� �������� ������
   for(i=0;i < MaxNumUIK;i++)UIKNum[i]=0;

   UIKNum[0]=Votes.Nuik;         //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   // ����� ��������� ������ ������������� ��������
   // ����������  ��������� ���������
   dySh=TMarSizeM+dyVoteTypeM+2*LineWM; // ���������� �� ������ ����� �� ������ ���� ������
    for(i=1;i <= Votes.Nblank;i++)
   if(Votes.Blank[i].ID!="0")
   { // ����� ������ ����������
		nVoteType=Votes.Blank[i].marker;
		PagePar[nVoteType][1]=Votes.Blank[i].Height; //297;������ ��������� � ��
		PagePar[nVoteType][2]=40;  //  ������  ����  ������ �  ��
		PagePar[nVoteType][3]=int(float(Votes.Blank[i].ShAreaHeight)/10)-dySh; //  ������  ����  ������ �  ��
		PagePar[nVoteType][4]=Votes.Blank[i].Width;
        PagePar[nVoteType][5]=Votes.Blank[i].LH[1].y/10;
		PrVoteTypeId[nVoteType]=1;
		for(j=1;j <= Votes.Blank[i].NVopr;j++)
		if(Votes.Blank[i].Vopr[j].ID!="0")
		{  // ������� ���������� ��������� � ��������� ����� (��� ���� ��������)
		   NumSQMax[nVoteType]=NumSQMax[nVoteType]+Votes.Blank[i].Vopr[j].NCandidates;
		}
	}
	// ��������� ������������ ������
	try
	{  // ������ ��������� ������� �����������
	   MasSQ =new int***[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) 	   for(j=0;j < NumSQMax[i]+1;j++)
	   MasSQ[i]=new int**[NumSQMax[i]+1];
	   for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
	   MasSQ[i][j]=new int*[2];
	   for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
	   { MasSQ[i][j][0]=new int[6]; MasSQ[i][j][1]=new int[6];	   }
	   // ������� ��� ��������  ����� ����� ��� �������  ���������
	   NumBlP=new int *[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) NumBlP[i]=new int [NumSQMax[i]+1];
	   // ������ �������� �������������  ���������� �����
	   NumPMax=new int *[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) NumPMax[i]=new int [NumSQMax[i]+1];
	   //  ������ �����������  ������ ������� � ���������
	   RezRec=new int *[NumVoteType+1];
	   for(i=0;i < NumVoteType+1;i++) RezRec[i]=new int [NumSQMax[i]+1];
	}
	catch (std::bad_alloc)
   {  cout << "���  ������. ������ zamXY ..."; }
   //  �������  ������� MasSQ
   for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
   for(k=0;k < 5;k++) { MasSQ[i][j][0][k]=0; MasSQ[i][j][1][k]=0;	   }
   //  ������������� ������ �� �������� �  ������  MasSQ
   // ��� ������� � ���������  ���� ���������
   for(i=1;i <= Votes.Nblank;i++)
   if(Votes.Blank[i].ID!="0")
   { // ����� ������ ����������
		nVoteType=Votes.Blank[i].marker;  // ��� ��������� ( �� ������� �� ���������)
		PageHM=PagePar[nVoteType][1];    // ������  �������� �  ��
		PageH=int(PageHM*float(NumDotY));//+0.5); // ������  ��������  �  ������
		PageWM=PagePar[nVoteType][4]; // ������  �������� �  ��  (��� �������� ( ������������)
		PageW=int(PageWM*float(NumDotX)); // ������  ��������  �  ������
		PrVoteTypeId[nVoteType]=1;	dxR=PageW-2*LMarSize+LineWx;   dyR=PageH-2*TMarSize+LineWy;
		// ����������� ������������� ���������� ����� �� ����� ����������
		if(MAXLINES < 1.2*PageH)MAXLINES = 1.2*PageH;
		ii=0;
		for(j=1;j <= Votes.Blank[i].NVopr;j++)
		if(Votes.Blank[i].Vopr[j].ID!="0")

		{ // NVopr - ���������� ������ � ��������� ( ���������� ��������)
		  for(k=1; k <= Votes.Blank[i].Vopr[j].NCandidates;k++)
		  if((Votes.Blank[i].Vopr[j].Candidates[k].N!=0)&&(Votes.Blank[i].Vopr[j].Candidates[k].disabled==false))
		  {   // ���� ��  ���� � �����������. ��������� �� ����������� ������������� ��������� - ?
			  // ��������� ��������
			  ii=ii+1;
			  MasSQ[nVoteType][ii][0][0]=1; // ���� �������� ( ��  ����)
			  dxSQ= int((float(Votes.Blank[i].Vopr[j].Candidates[k].dx)*NumDotX/10)/2);
			  dySQ =int((float(Votes.Blank[i].Vopr[j].Candidates[k].dy)*NumDotY/10)/2);
			  MasSQ[nVoteType][ii][0][3]=2*dxSQ; MasSQ[nVoteType][ii][0][4]=2*dySQ;
			  // ���������� ����� ������ ������ �������� ������������  ����� ���������
			  MasSQ[nVoteType][ii][0][1]=int(float(Votes.Blank[i].Vopr[j].Candidates[k].SQx*NumDotY)/10)-LMarSize+int(0.5*float(LineWx))+dxSQ;
			  MasSQ[nVoteType][ii][0][2]=int(float(Votes.Blank[i].Vopr[j].Candidates[k].SQy*NumDotY)/10)-TMarSize-int(1*float(dySQ))+int(0.5*float(LineWy));
			  // ����������  ��������� ���� �������� ��� ����������� �������������  �  �������� ������������
			  // ����������  ������  ����������  �� ��, �� �� ����������� �����  �������
			  // �� ���������� ����� ������������� ��������� ����� (������� �������� �����)
			  MasSQ[nVoteType][ii][0][5]=int(float(Votes.Blank[i].Vopr[j].Candidates[k].SQy*NumDotY)/10)-TMarSize+int(0.5*float(LineWy));
			  nn=j;
		  }
		}
		// ������������ ���������� ��������� ��� ���������  ���� ���������
		if(NumSQMax[i] > 0)
		{	for(j=1;j < NumSQMax[nVoteType]+1;j++)
			{ MasSQ[nVoteType][j][1][1]=dxR-MasSQ[nVoteType][j][0][1];
			  MasSQ[nVoteType][j][1][2]=dyR-MasSQ[nVoteType][j][0][2]- int(4*float(dySQ));
			  MasSQ[nVoteType][j][1][3]=MasSQ[nVoteType][j][0][3];
			  MasSQ[nVoteType][j][1][4]=MasSQ[nVoteType][j][0][4];
			  if(j==18)
			  nn=j;
			  // ���  �������  ������������  ������ �� ���������
			  MasSQ[nVoteType][j][1][5]=dyR -MasSQ[nVoteType][j][0][5]-2*float(dySQ);
			  // ��� ������� ���� ���������� �� ������ ������� �����
			  MasSQ[nVoteType][j][0][1]=dxR-MasSQ[nVoteType][j][0][1];
			}
		}
	}
	// �������� ������  ���  ������  ���  ������������


	PrIdOk=1; // ������� ����, ��� �������� ������ ��� ������������ ���������� � ������� ������
} //FormIdScan


int FormRezScanBull(void)
{ // ����������� ����������� ������������ ���������
   // 999 int NumV, MaxMarks;
   int pr1=0;

  for(i=1;i <= Votes.Nblank;i++)
   if(Votes.Blank[i].ID!="0")
   {  if(nVoteType==Votes.Blank[i].marker)
	  { // ������������ ��������� ��������� � ������
		// ����� ������ ������������� ���������
		pr1=111;
		NumV=Votes.Blank[i].NVopr;
		MasScan.MasRecOut[0]=pr1; //  ������� ��������� ������������ ���������
		MasScan.MasRecOut[1]=i;   // ����� ���� ������ ������������� ���������
		MasScan.MasRecOut[2]=NumUIKRec; // ����� ������ �� ���������
		MasScan.MasRecOut[3]=NumV;
		ii=0;  MaxMarks=0;
		for(j=1;j <= NumV;j++)
		if(Votes.Blank[i].Vopr[j].ID!="0")
		{ //  j - ����� ������� �  ���������
		  MaxMarks=Votes.Blank[i].Vopr[j].maxMarks;//NumSQMaxVopr[nVoteType][j][0];
		  MasScan.MasVoprOut[j][0]=MaxMarks;
		  summa=0;
		  MasScan.MasCandOut[j+1][0]=Votes.Blank[i].Vopr[j].NCandidates;//NumSQMaxVopr[nVoteType][j][1]; // ���������� ���������� � �������
		  for(k=1; k <= Votes.Blank[i].Vopr[j].NCandidates;k++)
		  if((Votes.Blank[i].Vopr[j].Candidates[k].N!=0)&&(Votes.Blank[i].Vopr[j].Candidates[k].disabled==false))
		  { // ������������ �������� ������ �� ����������
			ii=ii+1;
			// ������� ���������� ������� �� �������
			if(RezRec[nVoteType][ii]==1)summa=summa+1;
			MasScan.MasVoprOut[j][2]=summa;
			// �������� � ����������
			MasScan.MasCandOut[j][k]=RezRec[nVoteType][ii];
		  }
		  // ����������� ���������������� ����������, ��������� �� ���� �������� (�������)
		  // ������� ���������. ���� ������ ��������� ���� ������� � ������� ������ ��������� "0"
		  // �� ��������� ��������������
		  if(summa==0) MasScan.MasVoprOut[j][1]=-1;//����������������, ��� �������
		  else
		  {  if(summa > MaxMarks) MasScan.MasVoprOut[j][1]=-2;  //����������������, ����� �������
			 else MasScan.MasVoprOut[j][1]= i;  //   ��������� ��������������
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
  NumDotKoefY=0;  // �������  �����  �������� ������������ ������

  for(j=0;j < 4 ;j++) MasScan.MasRecOut[j]=0;
  for(j=0;j <= NumVoprMax;j++)
  { MasScan.MasVoprOut[j][0]=0; MasScan.MasVoprOut[j][1]=0;
	for(k=0; k <= NumCandMax; k++)  MasScan.MasCandOut[j][k]=0;
  }
  // ������� �������� ����������� ���� ���������
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
  ncL=0;// ����� ������� ���  ������ ����� ������� �����
  ncR=NumStrScan-1;   // ��������� ����� ������ ��� ������ ������ �������
  ncTL=130; // ����� ������� ��� ������ ����� ����� �����
  ncTR=NumStrScan-ncTL; // ����� ������� ��� ������ ������ ������� ����� �����
  dxT=10; dxL=2*LMarSize; PrLeft=0; PrTopL=0;PrTopR=0;PrRight=0;
  nbTL=0;nbLL=0;nbRR=0; nbTR=0;  BulHead=0; AlfaPage=0; PrPage=0; PrFrame=0;
  PrFirstL=0; PrFirstR=0; PrXMin=0; PrXMax=0; XoShR1=0; YoShR1=0; PrAlfaShR=0;
  XoMax=0; YoMax=0; nbXoMax=0; PrPageEnd=0;
  PrAlfa=0;
  MAXLINES=int(Max_Bull_Height*float(NumDotY))+5*TMarSize;
} //GashBeforeScan

 double erf(double z)
{ // ��������  ������������
  const float pi = 3.14159;
  int pr=0; double x;
  if (z> 20.0) z=20; else if (z < -20)z=-20;
  if (z > 0.0) pr=1;  if (z < 0.0) pr=-1;
  z=-2.0*z*z/pi; z=1-exp(z); if(z >= 0)x=sqrt(z);
  if (pr<0) x=-x;
  return x;
} // erf

 double erf1(double z)
{ // �������� ������� �������
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
{ // ����������� ����,  ��� ������ ����� ������ ����
  float sigma,gam,u1,u2,aa,bb,dd;//,p00,p11,;
  int g0,g1;
  dd=1-sqrt(alfa);   gam=erf1(dd);
  sigma=sqrt(surw*surw+surbl*surbl);
  gam=erf1(dd)*sigma;   gam=0;
  aa=(1-beta)/alfa;  bb=beta/(1-alfa);  dd=urbl;
  u1=1;  u2=(urs-gam-urbl)/sigma/1.4;
  *p00=0.5*(1-erf(u2));  		// ����������� ����,  ��� ��� ������ �����
  u1=(urs+gam-urw)/sigma/1.4;   // u2=(urs-gam-urw)/sigma/1.4;
  u2=(gam+dd)/sigma/1.4;
  *p11=0.5*(1+erf(u1));  		// ����������� ����,  ��� ��� ����� �����
  // ��������  ������� � ���, ��� �����  ������
  g0=0; g1=0;
  if  (*p00 >= *p11*bb)  { g0=1;  }else
  if (*p00 >= *p11*aa)g1=1;
  if (g0==0 && g1 == 0) { if (*p00 >= *p11*(aa+bb)/2)g0=1;else g1=1;   }
  return g0;
}//RecBlack

//---------------------------------------------------------------------------
void BLev(int nn,unsigned urw_ch,float* ur,float* sur)
{ // ������� �����������  ��������  ������ ������� � ���
  //  ����� ����������������� ����������
	int urw1;  float dd;
	urw1=urw_ch; dd=*ur;
	if(nn>1)
	{ *ur=((*ur)*(nn-1)+urw1)/nn;
	  *sur=sqrt((*sur)*(*sur)+((dd-(*ur))*(dd-(*ur)))/nn);
	} else { *ur=urw1;  *sur=1;}
}// ������� �����������  ��������  ������ ������� � ���

void EvChangeColCol(int BlWt,int nstr,int step,int ncT,int dxT,int* nbT,int* yo,int* prT)
  // BlWt - ������� ����������� ����� �����:
  //         "0"  - �  �������  ��  �����
  //         "1"  - �  ������ ��  ������
  // nstr - ������� ����� ������
  // step - ��������  ����  ��� �����������  �������� �������� ��� ��� ������
  //       ����� (1) ��� ������ (-1)  ������� �����
  // ncT - ����� ������� (�������) �� ��������  ������������ ������� ���� ��������
  // dxT - ����������  ����� �  ������, �� ������� ������������
  //      �������  �������� �������  �  ������  ������
  // nbT - ���������� ����� ������, ����������� ��� �����
  // yo - ����� ������, �  �������  ����������  ��������� (������� �������)
  // prT - ������� ����,  ��� �����  �������  ��������� �������

  //       ����� (1) ��� ������ (-1)  ������� �����
{ // �����������  ������  ������� (������) ����� ��������
  int i; float dd;
  if (*prT==0) // ����� ����� �����, ���� ��  ���  ��  ������
  {  *yo=0;
	 dd=0;
	 if(step <0)dxT=-dxT;
	 if(step>0)
	 {  // ����� ����� �������
		for(i=ncT;i < ncT+dxT;i=i+step)
		{ // �����������  �������� �������� ������������� ������� �� ������ ������ (dxT)
//---------------���������  �  ������� �����-----------------------------------
		  urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
		  dd=dd+urw_ch;
		 }
		 dd=dd/dxT; // ������� ��������  ������  �������
		 // ���������  � �������  �������� ������� � �������  �������  �  �����
		 g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
		 if(BlWt==0) { if(g0==0) *nbT=*nbT+1; else *nbT=*nbT-1; };
		 if(BlWt==1) { if(g0==1) *nbT=*nbT+1; else *nbT=*nbT-1; }
		 if(*nbT <0) *nbT=0;
		 if(*nbT >2)
		 { // ��� ������ ������ ������  ��������  �����, ����� �����������
			// ������� �  ���,  ��� ������ �����  �������
			*yo=nstr- *nbT;  *prT=1;
		}
	 }
	 if(step < 0)
	 {  // ����� ����� �������
		for(i=ncT;i > ncT+dxT;i=i+step)
		{ // �����������  �������� �������� ������������� ������� �� ������ ������ (dxT)
//---------------���������  �  ������� �����-----------------------------------

		  urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
		  dd=dd+urw_ch;
		 }
		 dd=dd/dxT; // ������� ��������  ������  �������
		 // ���������  � �������  �������� ������� � �������  �������  �  �����
		 g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);

		 if(BlWt==0) {if(g0==0) *nbT=*nbT+1; else *nbT=*nbT-1;}
		 if(BlWt==1) {if(g0==1) *nbT=*nbT+1; else *nbT=*nbT-1; }
		if(*nbT <0) *nbT=0;
		 if(*nbT >2)
		 { // ��� ������ ������ ������  ��������  �����, ����� �����������
			// ������� �  ���,  ��� ������ �����  �������
			*yo=nstr- *nbT;  *prT=1;
		}
	 }
  }// ����� ������ �����  �����
} // PageTop

void EvChangeColRow(int BlWt,int nstr,int step,int ncL,int dxL,int* nbL,int* xo,int* prL)
  // BlWt - ������� ����������� ����� �����:
  //         "0"  - �  �������  ��  �����
  //         "1"  - �  ������ ��  ������
  // nstr - ������� ����� ������
  // step - ��������  ����  ��� �����������  �������� �������� ��� ��� ������
  //       ����� (1) ��� ������ (-1)  ������� �����
  // ncL - ����� ������� (�������) �� ������� ����������  ����� ����� ������� �����
  // dxL - ����������  ����� �  ������, ������� ���������� ����� ��� ������ �����
  //      ��� ������  ������� �����
  // nbL - ���������� ����� ������, ����������� ��� �����
  // xo - ����� �������, �  ��������  ����������  ����� ������� ���������
  // prL - ������� ����,  ��� �����  �������  ��������� �������
  // step - ��������  ����  ��� �����������  �������� �������� ��� ��� ������
  //       ����� (1) ��� ������ (-1)  ������� �����
{ // �����������  ������  ������� (������) ����� ��������
  int i,ii; float dd;
  if (*prL==0) // ����� �����  �������  �����, ���� ��� ��� ��  �������
  {  *xo=0;
	 dd=0;
	 *nbL=0;
	 if((step) > (int(0)))ii=dxL; else ii=-dxL;
	 if(step>0)
	 {  // ����� ����� �������
		for(i=ncL;i < ncL+ii;i=i+step)
	   {//---------------���������  �  ������� �����---------

			urw_ch=GetXYValue(nstr,i);

		//---------------------------------------------------
			dd=urw_ch;
			// ���������  � �������  �������� ������� � �������  �������  �  �����
			g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
			if(BlWt==0) { if(g0==0) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(BlWt==1) { if(g0==1) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(*nbL <0) *nbL=0;
			if(*nbL >2)
			{ // ��� ������ ������ ����� ������  ��������  ����� (������), ����� �����������
			  // ������� �  ���,  ��� ����� ������� �������
			  *xo=i-(*nbL);   *prL=1; goto fin1;
			}
		};
	 }
	 if(step < 0)
	 {  //  ����� ������ ������
		for(i=ncL;i > ncL+ii;i=i+step)
		{
//---------------���������  �  ������� �����-----------------------------------

			urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
			dd=urw_ch;
			// ���������  � �������  �������� ������� � �������  �������  �  �����
			g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);

			if(BlWt==0) { if(g0==0) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(BlWt==1) { if(g0==1) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(*nbL <0) *nbL=0;
			if(*nbL >2)
			{ // ��� ������ ������ ����� ������  ��������  ����� (������), ����� �����������
			  // ������� �  ���,  ��� ����� ������� �������
			*xo=i+(*nbL);  *prL=1; goto fin1;
			}
		}
	 }
	 fin1:;
  }// ����� ������ ������ ���� �����
} // PageLeft

void EvChangeColRowSh(int BlWt,int nstr,int step,int ncL,int dxL,int* nbL,int* xo,int* prL)
  // BlWt - ������� ����������� ����� �����:
  //         "0"  - �  �������  ��  �����
  //         "1"  - �  ������ ��  ������
  // nstr - ������� ����� ������
  // step - ��������  ����  ��� �����������  �������� �������� ��� ��� ������
  //       ����� (1) ��� ������ (-1)  ������� �����
  // ncL - ����� ������� (�������) �� ������� ����������  ����� ����� ������� �����
  // dxL - ����������  ����� �  ������, ������� ���������� ����� ��� ������ �����
  //      ��� ������  ������� �����
  // nbL - ���������� ����� ������, ����������� ��� �����
  // xo - ����� �������, �  ��������  ����������  ����� ������� ���������
  // prL - ������� ����,  ��� �����  �������  ��������� �������
  // step - ��������  ����  ��� �����������  �������� �������� ��� ��� ������
  //       ����� (1) ��� ������ (-1)  ������� �����
{ // �����������  ������  ������� (������) ����� ��������
  int i,ii; float dd;
  if (*prL==0) // ����� �����  �������  �����, ���� ��� ��� ��  �������
  {  *xo=0;
	 dd=0;
	 *nbL=0;
	 //ur2bl=urbl; ur2w=urw;
	 if((step) > (int(0)))ii=dxL; else ii=-dxL;
	 if(step>0)
	 {  // ����� ����� �������
		//ur1w=0; ur1bl=0; nbl=0; nw=0;
		for(i=ncL;i < ncL+ii;i=i+step)
	   {//---------------���������  �  ������� �����---------

			urw_ch=GetXYValue(nstr,i);

		//---------------------------------------------------
			dd=urw_ch;
			// ���������  � �������  �������� ������� � �������  �������  �  �����
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
			{ // ��� ������ ������ ����� ������  ��������  ����� (������), ����� �����������
			  // ������� �  ���,  ��� ����� ������� �������
			  *xo=i-(*nbL);   *prL=1; goto fin1;
			}
		};
	 }
	 if(step < 0)
	 {  //  ����� ������ ������
		for(i=ncL;i > ncL+ii;i=i+step)
		{
//---------------���������  �  ������� �����-----------------------------------

			urw_ch=GetXYValue(nstr,i);

//---------------------------------------------------
			dd=urw_ch;
			// ���������  � �������  �������� ������� � �������  �������  �  �����
			g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
					   //	if(dd > 85) g0=0;
			if(BlWt==0) { if(g0==0) *nbL=*nbL+1; else *nbL=*nbL-1;}
			if(BlWt==1) { if(g0==1) *nbL=*nbL+1; else *nbL=*nbL-1;}

			if(*nbL <0) *nbL=0;
			if(*nbL >2)
			{ // ��� ������ ������ ����� ������  ��������  ����� (������), ����� �����������
			  // ������� �  ���,  ��� ����� ������� �������
			*xo=i+(*nbL);  *prL=1; goto fin1;
			}
		}
	 }
	 fin1:;
	 //urbl=ur2bl; urw=ur2w;
  }// ����� ������ ������ ���� �����
} // ColRowSh

int FindPageStart(int* PrPage)
{  //  �������  ������ ������  ����� ���������
	 int PrErr=0;
	 if(nstr > 10*TMarSize)
	 PrErr=1;
	 if(PrTopL==0)
	 {  //����������� ������� ������� �����
		//Step=int(1);
		EvChangeColCol(0,nstr,1,ncTL,dxT,&nbTL,&PgYoL,&PrTopL);
		if (PrTopL==1)
		{   // nr1 - �����  ������  ��� ��������  ������  �������
			// nr2 - �����  ������  ���  ���������  ������ ������
			nstrL=PgYoL+5;//+20;
			nr1=0;  nr2=nstrL;
			if(PgYoL > 10) nr1=5; else nr1=0; nr2=nstrL;
			if(PgYoL > 10)
			// ��������� ������ ������  �������
			{ 	nn=1;  urbl=0; nr1=5; nc1=100; nc2=500;
				for (j = nr1; j <= nr1+3; j++)
				{  for (i = nc1; i <= nc2; i++)
				  {
//-------------------����� ����� ����������  �  �������  ����� ������-------

					urw_ch=GetXYValue(nr1,i);

   //---------------------------------------------------
					 // �����������  �������� ������ �������
					 BLev(nn,urw_ch,&urbl,&surbl);
					 // ������� �  ���������  �����
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
	 {  // ����������� ����� �������
		 EvChangeColRow(0,nstr,1,ncL,dxL,&nbLL,&PgXoL,&PrLeft);
		   if(PrLeft==1)  // ��������� ������� ��  40 ������ �����
		   {  nc1=PgXoL+20; nc2=PgXoL+60; nr2=nstr;
			  //  ������  ������  ������
			  nn=1;  urw=0;
			  {  for (i = nc1; i <= nc2; i++)
				 {  // �����������  ������  ������������� �������
//-----------------------����� ����� ����������  �  �������  ����� ������-------

					urw_ch=GetXYValue(nstr,i);

					//---------------------------------------------------
					// �����������  �������� ������ �������
					BLev(nn,urw_ch,&urw,&surw);
					//	 ������� �  ���������  �����
					nn=nn+1;
				 }
			  };
				if(surw <15) surw=15;
		   }
	 }
	 // ����������� ��������� ������� ������  �����  �����
	 if(PrTopR==0)
	 {  //����������� ������� ������� ������
		EvChangeColCol(0,nstr,1,ncTR,dxT,&nbTR,&PgYoR,&PrTopR);
		if (PrTopR==1)
		{ nstrR=PgYoR+5;  goto fin11;	}
	 }
	 fin11:;
	 if ((nstr > nstrR)&&(PrTopR==1)&&(PrRight==0))
	 {  // ����������� ������ �������
		   EvChangeColRow(0,nstr,-1,ncR,dxL,&nbRR,&PgXoR,&PrRight);
	 }
	  // ������������ ��������  ���������  ������   ������  ����� PrPage=1
	  if((PrTopR==1)&&(PrRight==1)&&(PrTopL==1)&&(PrLeft==1)) *PrPage=1;
	 if(*PrPage==1)
	 { // ������ ����� ����������, �����  ����  ��������  �  ����  �����
		 // ����� �����������  ��������� ��������  ������� ����� � ��������  ���
		 //  ������  ���������  �����  �����, �������  ������������  ��  �����
		 //  ���������  �����  �����, ���������  �  ����������  �����  ���  �����
		 //  �������� ������������  �����
		 nstrHeaderEnd=0;
		 dxT=10; dxL=LMarSize;   nbTL=0;nbLL=0;nbRR=0; nbTR=0;
		 PrLeft=0; PrTopL=0;PrTopR=0;PrRight=0;
		 for(i=0;i < NumVoteType+1; i++)PrVoteType[i]=0;
         //  ����������  ����  �������  ����� AlfaPage
		 dyALfa=PgYoR-PgYoL; dxALfa=PgXoR-PgXoL;
		 AlfaPage=(float(dyALfa)*float(NumDotY))/(float(dxALfa)*float(NumDotX));
		 SinAlfaPage=sin(AlfaPage); CosAlfaPage=cos(AlfaPage);
		 TanAlfaPage=SinAlfaPage/CosAlfaPage;
		 // ����������� ������ ��� ������ ������ ����� ���������
		 ncL=PgXoL+int(0.5*float(LMarSize)); // ����� ������� ���  ������ ����� ������� ����� �  ������
		 ncR=PgXoR-int(0.5*float(LMarSize)); // ��������� ����� ������ ��� ������ ������ ������� �����
		 ncTL=PgXoL+int(1.5*float(LMarSize)); // ����� ������� ��� ������ ����� ����� �����
		 ncTR=PgXoR-int(1.5*float(LMarSize));// ����� ������� ��� ������ ������ ������� ����� �����
		 nstrL=PgYoL +int(0.5*float(TMarSize));
		 nstrR=PgYoR +int(0.5*float(TMarSize));

		 if(nstrR > nstrL)nstrUrWhite=nstrR+1; else nstrUrWhite=nstrL+1;//int((float(nstrL)+ float(nstrR))/2);
		 //  ������������ ������� � ����� ��� ����������� ���� ������� � ������ ���� �������

		 // ��������� ������ ������ ��� ������ ����������� ���������
		 // �������� ���� ���������  �  ������ ���� ������� �����
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
		 nstrFrYmax=nstrFrYmax+LineWy*2+dyVoteType+4; // 4 ����� ��  ������  ������
		 nstrBegVoteType= ii-2;
		 dyy=dyy + fabs(PgYoR-PgYoL);
		 // ������������ ������ ��� �������� ����� �����
		 nstrPageEnd=PgYoL;
		 if(PgYoL > PgYoR)nstrPageEnd=PgYoL; else nstrPageEnd=PgYoR;

		 nstrPageEnd=nstrPageEnd+int(297*float(NumDotY))-TMarSize;
		 nstrLsStart=nstrVoteType[0]+dyVoteType+2*NumDotY;
		 nstrRsStart=nstrVoteType[NumVoteType]+dyVoteType+2*NumDotY;

	 } // PrPage=1
   return PrErr;
}// FindPageStart;

int FindFrame(int* PrFrame)
{ // ����� ���������  ������  �����
	 int PrErr=0;
	 if ((nstr>(nstrL+2*TMarSize))&&(nstr>(nstrR+2*TMarSize)))
	 { PrErr=1; goto fin;}
	 if((PrTopL==0)&&(nstr > nstrL))
	 {  //����������� ������� ������� ����� �����
		EvChangeColCol(1,nstr,1,ncTL,dxT,&nbTL,&PgYoL,&PrTopL);
		if (PrTopL==1){	nstrL=PgYoL+6;	goto fin1f;	}
	 }
	 fin1f:;
	 if((nstr > nstrL)&&(PrTopL==1)&&(PrLeft==0))
	 { //XoL=PgXoL;
	   EvChangeColRow(1,nstr,1,ncL,dxL,&nbLL,&PgXoL,&PrLeft);
	 }
		// ����������� ��������� ������� ������  �����  �����
	 if((PrTopR==0)&&(nstr > nstrR))
	 {  //����������� ������� ������� ������
		EvChangeColCol(1,nstr,1,ncTR,dxT,&nbTR,&PgYoR,&PrTopR);
		if (PrTopR==1) { 	nstrR=PgYoR+6;	goto fin11f;	}
	 }
	 fin11f:;
	 if ((nstr > nstrR)&&(PrTopR==1)&&(PrRight==0))
	 {  // ����������� ������ ������� �����
		   EvChangeColRow(1,nstr,-1,ncR,dxL,&nbRR,&PgXoR,&PrRight);
	 }
		// ������������ ��������  ���������  ������   ������  ����� PrPage=1
	  if((PrTopR==1)&&(PrRight==1)&&(PrTopL==1)&&(PrLeft==1))
	  { *PrFrame=1;
		// ���������  ���� �������  �����
		dyALfa=PgYoR-PgYoL; dxALfa=PgXoR-PgXoL;
		AlfaPage=(float(dyALfa)*float(NumDotY))/(float(dxALfa)*float(NumDotX));
	  }
	  fin:;
	return PrErr;
} //FindFrame

int FrameControl(void)
{ // �������� �������  �����  ����� ��� ������ � ��������  ������������
	 int PrErr=0,nbK,XoK,PrK;
	 // ������������� ������� �����
	 LsPr=0; RsPr=0;
	 if(nstr==1081)
	 RsPr=0;
	 if (EndYoL==0)
	 {  EvChangeColRow(1,nstr,1,LsCol,Lsdx,&nbLs,&LsXo,&LsPr);
		if(LsPr==0)
		{  //  �������  ����� ������ ������ �����, ��� ��  �������  ������� �����
			LsNs=LsNs+1;
		} else
		{ LsNs=0;
		  LsCol=LsXo-2*NumDotX; //EndXoL=LsXo;
		  if(PrEndXoL==0){ EndXoL=LsXo; PrEndXoL=1;}
		  //if(fabs(EndXoL-LsXo) < LineWy)EndXoL=LsXo;
		  //if ((LsXo-EndXoL) > LineWy) PrErr=11;
		  EndXoL=LsXo;
		  Lsdx=5*NumDotX;
		  // ����� ������ ����� ��� ������� ������������ ������ ��� ������ ����
		  if((BulHead==0)&&(PrKoef==0)&&(nstr > PgYoL+2*dyVoteType))
		  { nbK=0;XoK=0;PrK=0;
			EvChangeColRow(0,nstr,1,LsXo+3,2*NumDotX,&nbK,&XoK,&PrK);
			if(PrK==0)
			{  // ����� ������� ������ ������������
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
	 //  �������� ������  �������  �����
	 if(EndYoR==0)
	 {  EvChangeColRow(1,nstr,-1,RsCol,Rsdx,&nbRs,&RsXo,&RsPr);
		if(RsPr==0)
		{  //  �������  ����� ������ ������ �����, ��� ��  �������  ������� �����
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
	 { // ��������� ������������ ������ �� ��� ��
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
{ // ����� ����� ������ ������ ���� ������ �� ���������
  //��� �������� ���� ���������
	 int PrErr=0,dx=0,dy=0;
	 // ����� ������ ���� ������ �� ����������
	 if(nstr ==4070)
	 dx=0;
	 if(nstr > nsShArStart*KoefY)
	 {  //����������� ������� ������� ����� �����
		colShAr=LsXo+2*NumDotX;
		EvChangeColCol(1,nstr,1,colShAr,10,&nbShAr,&ShArYo,&ShArPr);
		if (ShArPr==1)
		{ 	ShArXo= LsXo;
			//	��������� ���� ������� �����
			dyALfa=ShArYo-PgYoL; dxALfa=ShArXo-PgXoL;
			AlfaPage=-float(dxALfa)/float(dyALfa);
			SinAlfaPage=sin(AlfaPage); CosAlfaPage=cos(AlfaPage);
			TanAlfaPage=SinAlfaPage/CosAlfaPage;
			// ������������ ����� ������ ������ ������
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
{ //  ����� ����������� ��������� � ���� �������
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
{ //  ����� ���� ������� �  ����� �����
  int Err=0,ii=0,jj=0,j=0,dx=0;
  if((nstr > nstrBegVoteType)&&(*PrPg==3))
  { dxT=20;
	dx=fabs(nstrVoteType[0]-nstrVoteType[NumVoteType]);
	 for(j=nstr;j > nstr-2*TMarSize-dx; j--)
	 { // ����� ������ ����� ����� �����
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
		if(ii !=(NumVoteType+1))Err=22; // ��  ������� ������  �������  ����� �����
	 finVote:;
  } //nstr > nstrBegVoteType
  return Err;
}   // FindVoteTypeBot

int TestPageEnd(int* PrPg)
{ //  ������� ������ �����  ����� �����
	int PrErr=0;
	int yy=0,pr=0;
	 if(nstr > nstrPageEnd)
	 {  //����������� ��������� ��������
		EvChangeColCol(1,nstr,1,colTestPageL,dxTestPage,&nbTL,&yy,&pr);
		if (pr==1)
		{  *PrPg=1;		}
	 }
	// �������� ����, ��� ������� � ���, ��� ���� ������ ���� �������
	if(nstr > nstrPageEnd+ 2*TMarSize) PrErr=8;
	return PrErr;
} // TestPageEnd1


int FindNumYchTop(int* PrSt)
// PrSt - ������� ���������� ��������� ������
{ // ������� �����������  ��������� ������� ������ ���
  // ���� ������  ��  �����  ������� ������
   int Err=0,PrL=0,PrR=0;
   int XoL,XoR,YoL,YoR,j,ij,xmin,xmax,sumx;
   float dx;
   if ((nstr > nstrStampEndL+30)&&(*PrSt==0))
   { if((PrShL==0)&&(PrShR==0)) {  Err=4; goto fin; }
   }
   if(nstr > nstrStampStartL)
   { // � ����� ���� ������ ��� ������ � ������� ���� �  ������
	 // ���� ������� �����
	 dx=float(nstr-nstrStampStartL)*TanAlfaPage;
	 colStampL=colStampLo-dx;
	 colStampL=RsXo-dxStampArea+int(1.5*float(NumDotX));
	 colStampR=colStampRo-dx;
	 colStampR=RsXo-int(3.5*float(NumDotX));
	 dxStFindL=int(0.4*float(dxStampArea)); // ���������� ����� ���  ������ ������ ������
	 dxStFindR=dxStFindL;

	 dynstr=nstr-nstrStampStartL;
	 if(PrShL==0)
	 {  // ����������� ��������� ����� ������� ������
		if(PrFirstL!=0)  // ��������� ������ ������ �����
		{ dx=XoSh1 - colStampL;  dxStFindL=dx + 3*NumDotX; 	}

		EvChangeColRowSh(1,nstr,1,colStampL,dxStFindL,&nbShL,&XoL,&PrL);
		PrXoLSh=PrL;
		if(PrL==1)
		{  XoLSh=XoL;
			if(colStampL>=XoL) // ������� ������ ����� ��������� � ������ ���������
			{   cout << " Left side  Error nstr =  "<<nstr<<endl;
			     PrShL=-1; goto finShL; }
			//dxStFindL=XoL-colStampL+2*NumDotX;

			if(PrFirstL==0)
			{ // �����������  ���������  �����  ������ ���� ������
			  // � ����  ���  �������
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
					 { // ����������� ���� �������
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
				   { // ���� ������� ���������, ������ ������ ���� ������
					 if (PrNaklShL==1)   // ������ ������
					 {  XoSh1=XoL;    YoSh1=nstr;
					   /* if(nbXoMin==0)
						{ XoSh1=XoL;    YoSh1=nstr; nbXoMin=1;}
						if((fabs(XoL-XoSh1) <= LineWx)&&(nbXoMin==1))
						{ XoSh1=XoL;    YoSh1=nstr;}
						*/
					 }
					 if (PrNaklShL==-1)  // ������ �����
					 { // ������ �����
					   if(XoL <= XoMin)
					   { XoSh1=XoL;  YoSh1=nstr; XoMin=XoL; nbXoMin=0;} else
					   { if((XoL-XoMin)>3) nbXoMin=nbXoMin+1; 		  }
					   if (nbXoMin > 6)
					   { PrShL=1; XoShL=XoSh1;  YoShL=YoSh1;}
					 }
					 if (PrNaklShL==0)  // ��� �������
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
	 { 	// ����������� ��������� ������ ������� ������
		if(PrFirstR!=0)
		{ dx=colStampR-XoRSh; dxStFindR=dx+3*NumDotX;  }

		EvChangeColRowSh(1,nstr,-1,colStampR,dxStFindR,&nbShR,&XoR,&PrR);
		PrXoRSh=PrR;
		if(PrR==1)
		{   XoRSh=XoR;
			// �������� �� ��, ��� ������� ������ ����� ��������� � ������ ���������
			if(colStampR<=XoR)
			{ cout << " Right side  Error nstr =  "<<nstr<<"XoR=  "<<XoR<< "colStampR =  "<< colStampR<< endl;
			    PrShR=-1; goto finShR;	}
			//dxStFindR=colStampR-XoR - 2*NumDotX;
		  if(PrFirstR==0)
		  { // �����������  ���������  �����  ������ ���� ������
			// � ����  ���  �������
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
			{  // ���������� ���� ������� ������
			   if ((masXoR[0]<6)&&(PrAlfaShR==0))
			   { masXoR[0]=masXoR[0]+1; ii= masXoR[0]; masXoR[ii]=XoR; }
			   else
			   {    // ���������� ������, ��������� ����� ������
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
				{ // ���� ������� ���������, ������ ������ ���� ������
					if (PrNaklShR==1)
					{ // ������ ������
					  if(XoR >=  XoMax)
					  { //XoShR1=XoR;
						YoShR1=nstr; XoMax=XoR;XoShR1=XoMax; nbXoMax=0;
					  }	else
					  { if((XoMax-XoR)>3) nbXoMax=nbXoMax+1;	}
					  if (nbXoMax > LineWy)
					  { PrShR=1; XoShR1=XoMax;  XoShR=XoShR1;  YoShR=YoShR1; }
					}
					if (PrNaklShR==-1)
					{ // ������ �����  ��� �����

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
	  { // ����������� ��������� ������ ����� ������ ������
		PrFirstR=XoShR1; PrFirstL=XoSh1; PrFirstRy=YoShR1; PrFirstLy=YoSh1;
	  }
	}

	if((PrShL==-1)||(PrShR==-1))
	{  Err=41; goto fin; }

	if((PrShL==1)&&(PrShR==1))
	{ 	// ������� ���������� ����� �����, �����������
		// ������������ ������
		XoShR=XoShR1;  YoShR=YoShR1;
		XoShL=XoSh1;   YoShL=YoSh1;
		AlfaShR=float(XoShR-PrFirstR)/float(YoShR-PrFirstRy);
		AlfaSh=float(XoShL-PrFirstL)/float(YoShL-PrFirstLy);
		AlfaSh=-(AlfaSh+AlfaShR)/2;
		//AlfaSh=float(YoShR-YoShL)/float(XoShR-XoShL);
		// �������� ���� ��� ������� �������� ������� �  ������ ������
		dx=sqrt((XoShR-XoShL)*(XoShR-XoShL)+(YoShR-YoShL)*(YoShR-YoShL));
		if (fabs(dxStamp-dx) > int(2.5*float(NumDotX))) 	{ Err=42; goto finPrSh;  }
		//  �������� ������ �� ������� ������� ����� ������
		XoL=XoShL+dyStamp*sin(AlfaSh); 	YoL=YoShL-dyStamp*cos(AlfaSh);
		XoR=XoShR+dyStamp*sin(AlfaSh); 	YoR=YoShR-dyStamp*cos(AlfaSh);
		if (((XoR-NumDotX) > PgXoR)||((XoL+NumDotX) < PgXoR-dxStampArea)||
		   ((YoR-NumDotY) < PgYoR+dyVoteType + 2*LineWy)||
		   ((YoL-NumDotY) < PgYoR+dyVoteType + 2*LineWy))
		   { Err=41; goto finPrSh;}
		// ������������ �������� ������� ������
		*PrSt=1;
		finPrSh:;
	}
   } //if(nstr > nstrStampStartL)
  fin:;
  return Err;
} //int FindNumYchT0p()


int FindNumYchBot(int* PrSt)
// PrSt - ������� ���������� ��������� ������
{ // ������� �����������  ��������� ������� ������ ���
  // ���� ������  ��  �����  ������� ������
   int Err=0,PrL=0,PrR=0;
   int XoL,XoR,YoL,YoR;
   float dx;
   if ((nstr > nstrStampEndL)&&(*PrSt==0))
   {   if((PrShL==0)&&(PrShR==0)) {  Err=4; goto fin; }     }
	 //  ������  ��������� �����������  ��������� ����� ���� ������ ( ��� �������)
	 // ��������� � ��������� ����� �� ������ ���, ���  ��  ��� � ����������
	 // ������������ �������
	 // ���������  ��� ������ ����� ������
	 //  ����� �����  �������� ������ ������� ��� ������ ������
	 // � ����� ���� ������ ��� ������ � ������� ���� �  ������
	 // ���� ������� �����

   if(nstr > nstrStampStartL)
   { //  ����� �����  �������� ������ ������� ��� ������ ������
	 // � ����� ���� ������ ��� ������ � ������� ���� �  ������
	 // ���� ������� �����
	 dx=float(nstr-nstrStampStartL)*TanAlfaPage;
	 colStampL=colStampLo-int(dx)+1;
	 colStampL=LsXo+int(1.5*float(NumDotX));
	 dynstr=nstr-nstrStampStartL;
	 if((AlfaPage > 0)&&(fabs(AlfaPage) > 0.005)&&(dynstr < 8))
	 { // ��������� ����� ���� ������� �  ������ �������
	   dx=2*dynstr/sin(2*fabs(AlfaPage));
	 } else dx= int(float(dxStampArea)/2);
	 if(dx < float(dxStampArea)/2)dxStFindL=dx; else dxStFindL=int(float(dxStampArea)/2);
	 if(PrShL==0)  //nbShL >=0)// ����������  �������� 1
	 {  // ����������� ��������� ����� ������� ������
		//EvChangeColRow(1,nstr,1,colStampL,dxStFindL,&nbShL,&XoL,&PrL);
		EvChangeColRowSh(1,nstr,1,colStampL,dxStFindL,&nbShL,&XoL,&PrL);
		if(PrL==1) // ���� ������ �����
		{ if(colStampL>=XoL) // ������� ������ ����� ��������� � ������ ���������
		  { cout << " Left side  Error nstr =  "<<nstr<<endl;
		      PrShL=-1; goto finShL; }
		  if(PrFirstL==0)
		  { // �����������  ���������  �����  ������ ���� ������
			// � ����  ���  �������
			if((nstr-1)==nstrStampStartL) { Err=41; goto fin;}
			PrFirstL=nstr; // ������ ����� �������
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
		  { // ����� ��������   ������������  ����� �������
			if(PrNaklShL==-1)
			{ if(XoL <  XoMin)
			  {  if(( XoMin - XoL)> 3) { /*YoMin=nstr; */ XoMin=XoL;}
				YoMin=nstr; PrXMin=1;
				XoMin=XoL;
			  } else
			  {  nbXoMin=nbXoMin+1;
				if (nbXoMin > 3)
				{ if (PrXMin==1)
				   { //����� ����� �������
					 PrShL=1;
					PrXMin=2;  XoSh1=XoMin;  YoSh1=YoMin;
				   }
				}
			  }
			}
			if(PrNaklShL==1)
			{ nbXoMin=nbXoMin+1;
			   if (nbXoMin > 0)
			  { //����� ����� �������
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
		{ // PrL==0   ��� ������  �����
		  if (PrFirstL!=0)
		  { // ���� ���� �������� ����� ����, ��� ������� ������ �����
			// � ����� �����  ���� ������
			nbyShL=nbyShL+1;  nbXoMin=0;
			if(nbyShL > 2*NumDotY)
			{ // ����������� �����  ������� ������
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
	  { // ��������� ����� ���� ������� �  ������ �������
		dx=2*dynstr/sin(2*fabs(AlfaPage));
	  } else dx= int(float(dxStampArea)/2);
	  if(dx < float(dxStampArea)/2)dxStFindR=dx; else
			dxStFindR=int(float(dxStampArea)/2);
	 if(PrShR==0)
	 {  // ����������� ��������� ������ ������ ������� ������
		// ����������� ��������� ����� ������� ������
		EvChangeColRowSh(1,nstr,-1,colStampR,dxStFindR,&nbShR,&XoR,&PrR);
		if(PrR==1) // ���� ������ �����
		{  if(colStampR<=XoR) // ������� ������ ����� ��������� � ������ ���������
			{ cout << " Right side  Error nstr =  "<<nstr<<endl;
			    PrShR=-1; goto finShR; }
			nbyShR=0; //  ������� �������� �������� ����� � ������ ������
		  if(PrFirstR==0)
		  { // �����������  ���������  �����  ������ ���� ������
			// � ����  ���  �������
			if((nstr-1)==nstrStampStartR) { Err=41; goto fin;}
			PrFirstR=nstr; // ������ ����� �������
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
		  { // ����� ���������   ������������  ������ �������
			if(PrNaklShR ==1)
			{ if(XoR > XoMax)
			  { if(( XoR - XoMax)>3) {/*YoMax=nstr;*/  XoMax=XoR;};YoMax=nstr; PrXMax=1;

				XoMax=XoR; YoMax=nstr; PrXMax=1;

			  }
			  else
			  {  nbXoMax=nbXoMax+1;
				if (nbXoMax > 3)
				{ PrShR=1; PrXMax=2;  XoShR1=XoMax;  YoShR1=YoMax;
				  if ((PrXMax==1)&&(XoR < XoMax)) //������ ����� �������
				  { PrShR=1; PrXMax=2;  XoShR1=XoMax;  YoShR1=YoMax; }
				}
			  }
			}
			if(PrNaklShR==-1)
			{  nbXoMax=nbXoMax+1;
			   if (nbXoMax > 0)
			  { //������ ����� �������
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
		{ // PrL==0   ��� ������  �����
		  if (PrFirstR!=0)
		  { // ���� ���� �������� ����� ����, ��� ������� ������ �����
			// � ������ �����  ���� ������
			nbyShR=nbyShR+1;
			if(nbyShR > 2*NumDotY) // ����������� ������  ������� ������
			{ Err=44; goto fin; }
		  }
		} // PrR
		finShR: ;
	 } // if(PrShR==0)
   } //if(nstr > nstrStampStartR)
   // ������ ������� ������  ��� ����������� ������  �����
   if(((PrFirstL!=0)||(PrFirstR!=0))&&((PrNaklShL==0)&&(PrNaklShR==0)))
   { if((PrFirstL!=0)&&(PrFirstR!=0)) { PrNaklShL=2; PrNaklShR=2; }
	 else
	 { if(PrFirstL!=0)
	   { PrNaklShL=1;
			/*PrShL=1; ���������������, ����� �� ��������� ������ �������� X*/
			XoSh1=XoMin;  YoSh1=YoMin;
	   }
	   if(PrFirstR!=0)
	   { PrNaklShR=-1;
			/*PrShR=1;���������������, ����� �� ��������� ������ �������� X */
			XoShR1=XoMax;  YoShR1=YoMax;
	   }

	 }
   }
   if((PrShL==-1)||(PrShR==-1)){  Err=41; goto fin; }
	// ������������ ��������  ���������  ������   ������  ����� PrPage=1
	if((PrShL==1)&&(PrShR==1)&&(*PrSt==0))
	{ 	*PrSt=1;
		// ���������� ���� �������
		XoShR=XoShR1;  YoShR=YoShR1;
		XoShL=XoSh1;  YoShL=YoSh1;
		AlfaSh=float(YoShR-YoShL)/float(XoShR-XoShL);
		//  �������� ���� ��� ������� �������� ������� �  ������ ������
		dx=sqrt((XoShR-XoShL)*(XoShR-XoShL)+(YoShR-YoShL)*(YoShR-YoShL));
		if (((dx-dxStamp) > int(2.2/*2*/*float(NumDotX)))||((dx-dxStamp) < -NumDotX))
			{ Err=42; goto finPrSh;  }

		//if (fabs(dxStamp-dx) > 2*NumDotX) 	{ Err=42; goto finPrSh;  }
		//  �������� ������ �� ������� ������� ����� ������
		// �������� ����� ������� �����
		dx=AlfaPage*(YoShR+YoShL)/2;
		XoL=XoShL-dyStamp*sin(AlfaSh); 	YoL=YoShL+dyStamp*cos(AlfaSh);
		XoR=XoShR-dyStamp*sin(AlfaSh); 	YoR=YoShR+dyStamp*cos(AlfaSh);
		nstrStampEndR=nstrStampStartR+dyStampArea-2*NumDotY;
		nstrStampEndL=nstrStampStartL+dyStampArea-2*NumDotY;
		if (((XoR-NumDotX) > colStampRo)||((XoL+NumDotX) < colStampLo)||
		   ((YoR-NumDotY) > nstrStampEndR)||((YoL-NumDotY) > nstrStampEndL))
		   { Err=41; goto finPrSh;}
		// ������������ �������� ������� ������
		*PrSt=1;
		finPrSh:;
	}
  fin:;
  return Err;
} //int FindNumYchBot()


void FindVoteMarks(void)
{ // ����������� �������  �������  �����������  �  ���������
   int ii,XTop,YTop,XLe,PrL,PrYTop,ncL,dxL,ns,Napr=0;
//   float dyy,dxy;
   if(NumSQMax[nVoteType]>0)
   for (j=1; j < NumSQMax[nVoteType]+1; j++)
   {  if (BulHead==0)Napr=1; else Napr=-1;
	  ns=int(float(SearchSQ[j][2])*KoefY);
   if((nstr > ns/*SearchSQ[j][2]*/)&&(SearchSQ[j][0]!=3))
   {   if(SearchSQ[j][0] == 0)
	   { // ����� ������� ����� ��������
			YTop=0;PrYTop=0; //jj=0;
			dxFindSQ=10;
			if(Napr==1)XTop=RsXo-SearchSQ[j][1]; else XTop=SearchSQ[j][1]+LsXo;
			XTop=XTop/CosAlfaPage;
			EvChangeColCol(1,nstr,1,XTop,10,&SearchSQ[j][5],&YTop,&PrYTop);
			if(PrYTop==1) // ���� ������ �����
			{ SearchSQ[j][5]=0; // ��������� �������� ����������� ������ ���������
			  SearchSQ[j][0]=1; // ������� ������� �����, ����� ����� �������
			  // ���������� ������������  ������
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

			  // ��������� �������, � ��������  ����������  ����� ����� �������
			  // SearchSQ[j][1]=SearchSQ[j][1]-Napr*SearchSQ[j][3];
			  SearchSQ[j][1]=SearchSQ[j][1]+SearchSQ[j][3];
			  SearchSQ[j][2]=YTop+NumDotY+int(0.5*float(NumDotY)); // ����������� ������ ������ ������ ������� �����
			  goto finNumSQ;
			}
	   }
	   if(SearchSQ[j][0] == 1)
	   {  // ����� ����� ( ������ ��� ��������� ����) ������� ��������
		  PrL=0;
		  if(Napr==1)ncL=RsXo-SearchSQ[j][1]; else ncL=SearchSQ[j][1]+LsXo;
		  // ��������  ������� ������ ������

		  dxL=SearchSQ[j][3];
		  EvChangeColRow(1,nstr,Napr,ncL,dxL,&SearchSQ[j][5],&XLe,&PrL);
		  if(PrL==1)
		  { // ������������ ������ ��� �������� ����� � ��������
			 SearchSQ[j][0]=2; // ������� ����� �������
			 SearchSQ[j][5]=0; // ��������� �������� ����������� ������ ���������
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
			{  // ������� ����� ��� �������  ������
			   for (i=nc1; i <= nc2; i++)
			   { // ������� ����� �����
				 //=====================================
				 //Color=Form1->Image2->Picture->Bitmap->Canvas->Pixels[i][nstr];
				 //urw_ch=(GetRValue(Color)*0.3+ GetGValue(Color)*0.59+ GetBValue(Color)*0.11);
                // urw_ch=GetXYValue(i,nstr);
                 urw_ch=GetXYValue(nstr,i);
				 //if(urw_ch > 170)urw_ch=170;
				 //=====================================
				 dd=urw_ch;
				 // ���������  � �������  �������� ������� � �������  �������  �  �����
				 //g0=0;
				 g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,dd,&p00,&p11);
						   //	if(dd > 85) g0=0;/
				 if(g0==1) NumBlP[nVoteType][j]=NumBlP[nVoteType][j]+1;
			   }
			} else
			{  // ����������� �������� ������� �������
			   SearchSQ[j][0]=3; // ������� ��������� �������� ����� � ��������
			   // ������� �����������  10% ����������� �������
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
{ //  ������� ����������  ������������� ��������� ��� �����������  ���������  ������
  int ErrScan=0;

// ======================���������  ������ ===============================
/////////////////////////////////////////////////////////////////////////////
//- PrPage=0 - � � � � � � � � � �   � � � � � �    � � � � �  -------------
   int dxS,dyS;
   if (PrPage==0)
   { // �����������  ������  �����
	 ErrScan=FindPageStart(&PrPage);
	 if(PrPage==1)
	 { FrXoL=PgXoL;   FrYoL=PgYoL;  FrXoR=PgXoR;   FrYoR=PgYoR;	}
	 if (ErrScan >0){
	 goto FinEvRow;  }
   }
   ////////////////////////////////////////////////////////////////////////////
   //- PrPage=1  � � � � � � � � � � � � �   � � � � �   �  � � � �  � � � � � � � � �  -------------
  if((PrPage==1)&&((nstr > nstrL)||(nstr > nstrR)))
   { if (nstr==nstrUrWhite)  //  ��������� ������  ������ �� ����  ������
	 { nn=1;  urw=0;
	   for (i = PgXoL+100; i <= PgXoR-100; i++)
	   {  // �����������  ������  ������������� �������
		  //-----------------------����� ����� ����������  �  �������  ����� ������-------
		  //Color=Form1->Image2->Picture->Bitmap->Canvas->Pixels[i][nstr];
		  //urw_ch=(GetRValue(Color)*0.3+ GetGValue(Color)*0.59+ GetBValue(Color)*0.11);
		  //urw_ch=GetXYValue(i,nstr);
		  urw_ch=GetXYValue(nstr,i);
		  //---------------------------------------------------
		  // �����������  �������� ������ ������
			BLev(nn,urw_ch,&urw,&surw);
			//	 ������� �  ���������  �����
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
	 // �����������  ����  ���������
	 urw1=urw;
	 urw=150;
	 if(PrPage==1)ErrScan=FindVoteTypeTop(&PrPage);

	 // ������  ������ ��� ������ ����� � ���� ���������
	 if((ErrScan==2)&&(PrFrame==0))ErrScan=1;
	 urw=urw1;
	 // �������� ����,  ��� ��� ��������� ������������� �������� ������
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
	 {  // ����� ����� �������, ��� ��������� ���������
		// ����������  ������ ���  ������ ������ � ��������� �����������
		// ���������� ������������� ���� ��������� � ������ nVoteType
		EndYoL=0;EndYoR=0;EndXoL=0; EndXoR=0; PrEndXoL=0; // ���������� ����� ����� �����
		 for(i=1;i < NumVoteType+1;i++) if(PrVoteTypeTop[i]==1)nVoteType=i;
		//  ��������� ���������� �������� � ����������� �� ���� ���������
		PageHM=PagePar[nVoteType][1];  // float PageHM=148.5; // ������  �������� �  ��
		PageH=int(PageHM*float(NumDotY)); // ������  ��������  �  ������
		PageHM=PagePar[nVoteType][1];    // ������  �������� �  ��
		PageH=int(PageHM*float(NumDotY));//+0.5); // ������  ��������  �  ������
		PageWM=PagePar[nVoteType][4]; // ������  �������� �  ��  (��� �������� ( ������������)
		PageW=int(PageWM*float(NumDotX)); // ������  ��������  �  ������
		dxStampAreaM=PagePar[nVoteType][2];  // ������  ����  ������ �  ��
		dxStampArea=int(dxStampAreaM*float(NumDotX)); // ������  ����  ������ �  ������
		dyStampAreaM=PagePar[nVoteType][3];
		dyStampArea=int(float(dyStampAreaM)*float(NumDotY)); // ������  ����  ������ �  ������
		nstrStampStartL=LineWy+dyStamp-dyStampNum; // ������ ������ ������ ������
		nstrStampEndL=LineWy+dyStampArea;    // ������ ��������� ������ ������
		colStampL=PageW-(LMarSize+LineWx+dxStampArea)+2;  // ������� ������ ������ ������ �����
		colStampR=PageW-(LMarSize+LineWx)-2;  // ������� ������ ������ ������ ������
		dxStFindL=int(0.4*float(dxStampArea)/2); // ���������� ����� ���  ������ ������ ������
		dxStFindR=int(0.4*float(dxStampArea)/2); // ���������� ����� ���  ������ ������ ������
		dyStampNum=int(dyStampNum*float(float(dxStampArea)/float(dyStamp)));
		nstrStampEndR=nstrStampStartR+dyStampArea-2*NumDotY;
		nstrStampEndL=nstrStampStartL+dyStampArea-2*NumDotY;
		nstrPageEnd=PgYoL;
		if(PgYoL > PgYoR)nstrPageEnd=PgYoL; else nstrPageEnd=PgYoR;
		nstrPageEnd=nstrPageEnd+PageH-2*TMarSize;
		//  ���������  ���� ������� �����
		dyALfa=PgYoR-PgYoL; dxALfa=PgXoR-PgXoL;
		AlfaPage=(float(dyALfa)*float(NumDotY))/(float(dxALfa)*float(NumDotX));
		SinAlfaPage=sin(AlfaPage); 	CosAlfaPage=cos(AlfaPage);
		TanAlfaPage=SinAlfaPage/CosAlfaPage;
		PrShL=0;PrShR=0; // ������� ���������
		PrAlfaSh=0; PrShFound=0;  nbShL=0; nbShR=0;
		nbyShL=0;  	nbyShR=0; 	// �������� �������� ����� � ������
		PrShL=0; 	PrShR=0; 		XoShL=0; 	XoSh1=0;  YoShL=0;	YoSh1=0;
		PrNaklShL=0;PrNaklShR=0; 	PrFirstL=0; PrFirstR=0;
		PrXMin=0; PrXMax=0;   NumWtP=0;
		XoShR1=0; YoShR1=0; PrAlfaShR=0; XoMax=0; YoMax=0; nbXoMax=0;
		PrFirstR=0; PrFirstL=0; PrFirstRy=0; PrFirstLy=0;
		for(j=0;j < 6;j++){ masXoR[j]=0,masXoL[j]=0;}
		// ��������� ������ ��� ������  ������ ������ ����� �����
		nstrBegVoteType =nstrVoteType[0];
		if(nstrBegVoteType > nstrVoteType[NumVoteType])
					nstrBegVoteType =nstrVoteType[NumVoteType];
		if (BulHead==0)
		{ // ��� ������ ���� ���������
		   zz=int(dyStampArea*AlfaPage);
		   colStampL=PgXoR-dxStampArea-zz+int(0.7*float(LineWx));
		   colStampR=PgXoR-LineWx-zz-int(0.7*float(LineWx));
		   zz=int(dyStamp*float(float(dxStampArea)/float(dyStamp)));
		   nstrStampStartL=PgYoL+dyVoteType+ dyStamp - dyStampNum;
		   nstrStampStartR =nstrStampStartL;
			// ������������ ����� ������ ������ ������
		   dyS=nstrStampStartR-PgYoR;
		   colStampRo=PgXoR-int(2.0*float(NumDotX))-int(float(dyS)*SinAlfaPage+0.5);
		   dxS=int(float(dxStampArea-int(1.0*float(NumDotX)))*CosAlfaPage);
		   colStampLo=PgXoR-dxS;
		   dyS=dyStampArea +dyVoteType;//08_03-int(2.0*float(NumDotX));
		   nstrStampEndR=PgYoR+dyS*CosAlfaPage;
		   dyS=int(float(dxStampArea-int(1.8*float(LineWx)))*SinAlfaPage);
		   nstrStampEndL=nstrStampEndR+dyS;
		 } else
		 { //��� �������� ���� ���������
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
		 // ��������� ��������� ��� �������� ����� ���������
		 Lsdx=LMarSize;//4*NumDotX;
		 LsCol=PgXoL-2*NumDotX;
		 Rsdx=LMarSize;//4*NumDotX;
		 RsCol=PgXoR+2*NumDotX;
		 nstrLsEnd=PageH; nstrRsEnd=PageH;
		 // ��������������� ������������ ���� ��� ������ ���� �������
		 dynstr= dyStampArea;
		 // ������� �������� �������� ����������   �����
		 for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
		 {  NumBlP[i][j]=0;	 NumPMax[i][j]=0; RezRec[i][j]=0; }
		 //  ������������  ������� ��� ������  � ������ ���������
		 try
		 { SearchSQ=new int*[NumSQMax[nVoteType]+1];
		   for(j=0;j < NumSQMax[nVoteType]+1;j++) SearchSQ[j]=new int [7];
		 }
		 catch (std::bad_alloc)
		 {  cout << "���  ������. ������ zamXY ..."; exit(-1); }
         //  ���������� ������  ���  �������  ���� ��������� � ������� ������
		 // ��������� ���������  ���������  ���������  �  ������ �������������
		 // ����� ���� ����� ����� � ��� �������
		 for(j=1;j < NumSQMax[nVoteType]+1;j++)
		 {  // ����� �������� ����������� ������ � ������� � ������ �������
			dxS= MasSQ[nVoteType][j][BulHead][1];
			dyS= MasSQ[nVoteType][j][BulHead][2];
			DeltaX=dxS; DeltaY=dyS;
			if(fabs(AlfaPage) > 1/57.3)
			{  DeltaX=int(float(dxS)/CosAlfaPage);
				if(BulHead==0)
				{ // ��� ������ ����
					DeltaY=dyS+int(float(dxS)*SinAlfaPage);
				} else
				{ // ��� ��������  ����
					DeltaY=dyS-int(float(dxS)*SinAlfaPage);
				}
			}
			// ������������ ������� ������ �  ���������� ��� ������� ���������
			// ���������� ������ �� ������������� ���� ������� �  ������� ������ ������������
			SearchSQ[j][0]=0;
			//  ��� � ���������� �� ����� ����� �� �����������, ���������
			// ����� ���������  �������� �������� �� ������� ��������
			// �����  ������� �����, ��� ����� �����
			SearchSQ[j][1]=DeltaX;
			SearchSQ[j][2]=PgYoL+DeltaY;
			SearchSQ[j][3]=MasSQ[nVoteType][j][BulHead][3];
			SearchSQ[j][4]=MasSQ[nVoteType][j][BulHead][4];
			SearchSQ[j][5]=0;
			SearchSQ[j][6]=MasSQ[nVoteType][j][BulHead][5];;
		 }

        cout << " The End of Frame Kod =    " << ErrScan    << endl;
	  } // Page==2 // ������� �  ���������� �����
   } // PrPage==1

	/////////////////////////////////////////////////////////////////////////////
	//-  PrPage=2  --- � � � � � � � � � � � � �   � � � � � � � � �  -------------
   if(PrPage==2)
   {  // ����������� � ��� ������� ����������, ������������� ���������
	  // �������� ������ �����  ����� � ������
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
	  { // ����� ����� �����

        cout << " The End of Frame Kod =    " << ErrScan    << endl;

		if(ErrScan==10)
		{ // ������ ����� ����, ������������ ����� � ������ �������� ����� �����
		  EndYoL=nstr-LsNs; EndYoR=nstr-RsNs;
		} else
		{ if(ErrScan==11) EndYoL=nstr-LsNs; if(ErrScan==12) EndYoR=nstr-RsNs;
		}
		ErrScan=0;
		if((EndYoL > 0)&&(EndYoR > 0))
		{ 	// ������������ ������ ���  ������  ������ ������ ���� �������
			//  �  ����� ��������


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
	  //  �������� ���  ���������  ����������� ���������
	  if (BulHead==0)
	  {  // ������������� �  ������ �����������
		 //----- ����������� ������� ������ � ���  ���������
		 if(PrCheckSh==1)
		 { //  ���� �������  �������� ������
			if (PrShFound==0)
		 	{  urbl1=urbl;
				//urbl=int((urw-urbl)/2);//65;
                //urbl=100;
                urbl=urbl+(urw-urbl)/3;
				ErrScan=FindNumYchTop(&PrShFound);
				if(PrShFound==1)
				{ // ����� ������ ������, ��������� ��� ������������� ������
				  //  ��������� � �������  ������������� ������ ��

				//while (urbl < 80)
				//{
					NumUIKRec=tryConfirmStamp(1);

					cout << " Number  UIK   ID =    " << UIKNum[0]    << endl;
					cout << " Number  UIK   =    " << NumUIKRec << endl;


					if(NumUIKRec > 0)
					{  // �������������� ������ ������� �  ������
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
		 	} // if(PrShFound==0) ������ ������ ������ ���
		 	if (ErrScan >0) goto FinEvRow;
		 }//  �����  ��������  �������� ������
		 // ������������� �������  �����������
         urw1=urw;  urw=150;
				FindVoteMarks();
         urw=urw1;
		 // ���������� ������ ��� ������ ����� ��������
		 if((ShArPr==0)&&(nstr > nsShArStart))
		 {  //  ���������� ��  ���������� ���� ������ � ������
			// �����������
			ShArPr=1;
			ShArYo=nstr;	ShArXo= LsXo;
			//	��������� ���� ������� �����
			dyALfa=ShArYo-PgYoL; dxALfa=ShArXo-PgXoL;
			AlfaPage=-float(dxALfa)/float(dyALfa);
			SinAlfaPage=sin(AlfaPage); CosAlfaPage=cos(AlfaPage);
			TanAlfaPage=SinAlfaPage/CosAlfaPage;
			//  ���������� �  ������ �����  ��������
			nbTL=0;nbLL=0;nbRR=0; nbTR=0;
			DeltaY=int(float(dyStampArea)*CosAlfaPage);
			DeltaX= RsXo-LsXo;
			nstrLsEnd= ShArYo+DeltaY;//nstrVoteType[0]-int(1.2*float(TMarSize));
			nstrRsEnd= nstrLsEnd + int(DeltaX*SinAlfaPage);

		 }
		 // ��������  ��������� ������������� � �������� �  ������
		 //  ������  ����� ����� ��� ������ ������� ���������
		 //if((nstr > nstrLsEnd)&&(nstr > nstrRsEnd)) PrPage=3;
		 if((EndYoL > 0)&&(EndYoR > 0)) PrPage=3;

	  } //BulHead==0
	  else
	  { //  �������������  �  �������� �����������
		//  ����� ������ ���� ������
		if(ShArPr==0)
		{ ErrScan=FindShAreaStart();
		  if(ShArPr==1)
		  { //  ���������� �  ������ �����  ��������
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
		// ������������� �������  �����������
		         urw1=urw;  urw=150;
				   FindVoteMarks();
				            urw=urw1;
		// �����  ���������  ������ ���
		// ����������� ������� ������ � ���  ���������
		if(PrCheckSh==1)
		{ //  ���� �������  �������� ������
			if((PrShFound==0)&&(ShArPr==1))
			{ urbl1=urbl;  //urbl=55;
			  //urw1=urw;    urw=urw-(65-urbl1);
			  //urbl=int((urw-urbl)/2);//65;
			  urbl=urbl+(urw-urbl)/3;
			  ErrScan=FindNumYchBot(&PrShFound);
			  if(PrShFound==1)
			  { //����� ������ ������, ��������� ��� ������������� ������
				//  ��������� � �������  ������������� ������ ��
				//while (urbl < 80)
				//{
					NumUIKRec=tryConfirmStamp(0);

                    cout << " Number  UIK   ID =    " << UIKNum[0]    << endl;
					cout << " Number  UIK   =    " << NumUIKRec << endl;

					if(NumUIKRec > 0)
					{  // �������������� ������ ������� �  ������
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
			}// if(PrShFound==0) ������ ������ ������ ���
			if (ErrScan >0) goto FinEvRow;
		} // �����  �������� �������� ������
		// ��� �������� ���� ���������  ������� �  �������� ���������  �����
		// �������������� ��� ������� ������, � ��������� ������ - ������
		if((EndYoL > 0)&&(EndYoR > 0)) PrPage=3;
	  } //BulHead
	  /////////////////////////////////////////////////////////////////////////
	  //  ����� ��������  ���������� ��  ����������� ���������
	  if(PrPage==3)
	  { //  ���������� �  ������ �����  ��������
		// ������� ���������� ������� ������ � ����������
		for(j=0;j < NumSQMax[nVoteType]+1;j++) delete [] SearchSQ[j]; delete [] SearchSQ;
		PrPageEnd=0; // ������� �������� ���������  �������� ��� ������ ��������
		//  ���������� ������ ���  ������  ����� �����
		nstrPageEnd =nstrVoteType[0];  colTestPageL= ncVoteType[0];
		if(nstrPageEnd < nstrVoteType[NumVoteType])
		{ nstrPageEnd =nstrVoteType[9];  colTestPageL= ncVoteType[9]; }
		nstrPageEnd=nstrPageEnd+dyVoteType;
		dxTestPage=10;
		nbTL=0;
	  }
   } //if(PrPage==2)
/////////////////////////////////////////////////////////////////////////////
//-  PrPage=3 -� � � � � � � � � � �    � � � � �  � � � � �  �  � � � �  � � � � � � � � �
   if(PrPage==3)
   {  // ��������� ������, ���������, ����������� ����� ���������
	  if(PrPage==3)
	  { urw1=urw;
		urw=150;
		ErrScan=FindVoteTypeBot(&PrPage);
		if(PrPage==4)
		{ //  ���������  ������ ����  ������� ����� � ������
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
//-  PrPage=4  ---� � � � � � � � � � �   � � � � �  � � � � �-----------------
   if(PrPage==4)
   {  //  ����������  ����������  �������������  �������� � ������
	  //  �����������  �����������
	  ////////////////////////////
	  // RezRec
//	  if(MasScan.MasRecOut[0]==0)
//	  { MasScan.MasRecOut[0]=FormRezScanBull();  }
	  /////////////////////
	  // �������� ����������   ����� �����  �����������  ����� ����� ���������
	  // ���� ����� �������������  �����  ���������  �������������
	  // ��  �����������, �� ��� ������� �  ���,  ��� �����  ���� ������ ���������
	  // ��� ������, �������  ��������� ���� ����������� ������ �  ����
	  //  �������������  ���������, �  �����  �������� ��������� ��  ������
	  // �� ������������  ���������  �����
	  if(PrPageEnd==0)
	  { ErrScan=TestPageEnd(&PrPageEnd);
		if (ErrScan >0) goto FinEvRow;
	  }
   } // if(PrPage==4)
   FinEvRow:; // ����� ��������� ������
   return ErrScan;
}//ScanBul


void GashMasVote(void)
{ // ������� ������������ ��������

 // delete MasSQ;
   try
   { // ������� ������ �  �  ������� ����������� �����������

	for(i=0;i < NumVoteType+1;i++) for(j=0;j < NumSQMax[i]+1;j++)
	{ 	delete  [] MasSQ[i][j][0]; delete  [] MasSQ[i][j][1]; }
	for(i=0;i < NumVoteType+1;i++)
			for(j=0;j < NumSQMax[i]+1;j++) delete [] MasSQ[i][j];
	for(i=0;i < NumVoteType+1;i++)  delete [] MasSQ[i];
	delete [] MasSQ;
		// ������� ����� ����� �  ��������� �����������
	for(i=0;i < NumVoteType+1;i++) { delete [] NumBlP[i]; delete [] NumPMax[i];}
	delete [] NumBlP; delete [] NumPMax;
	   // ������� ��������������  ������� �����������
	for(i=0;i < NumVoteType+1;i++) delete [] RezRec[i]; delete [] RezRec;
	}
   catch (std::bad_alloc)
   {
	 //ShowMessage(" ������ ������������ ������������ ������");
	 cout << "������ ������������ ������. MasSQ ...";
	}

}


