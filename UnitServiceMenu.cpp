//*****************************************************************************
// Файл реализации  функций  сервисного  меню КОИБ
//
//                     О.Чудаков      23.05.2013
//******************************************************************************
//******************************************************************************

#include <getopt.h>

#include <cstdlib>
#include <string>



#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
//---------------------------------------------------------------------------
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "cr_BMP.h"
#include "InitAltera.h"
#include "CreateImage.h"
#include "HardProc.h"
#include "HeaderMain.h"
#include "printer.h"
#include "usb.h"
//#include "UnitServiceMenu.h"

using std::cout;
using std::endl;

int nbxL=0,nbxR=0,XoL=0,XoR=0,PrL=0,PrR=0;

int MaxLight;
  int PrEnterStart;
  int PrModeChange=0;
    //char FBMP[]        = "/www/pages/Scan.bmp";
  time_t timer;
  struct tm *TekTime;
  char SerNumKoib[124];
  char SerNumKoib1[124];
  float ScanerWorkTime;
 //  переменные  для  сервисного  меню
  int PrExKeyB=0;
  int PrMenuBtn=-1;
  int KodDost=-1;
  int KodOp=-1;
  int BtnPushed=-1;
  int PrUsb1=0,PrUsb2=0,PrUsb3=0,PrSDCard=0,PrPrinter=0,PrKlav=0;
  int MenuItems0[NumMenuItems+1];
  int ItemMenu0=0,ItemMenu1=0,MenuUr=0;
  char InfS1[124];
  char InfS2[124];
  char InfS[124];
  int InfSTablo1,InfSTablo2;// переменные с номерами сообщений на табло 1,2
  int  CodesOp[3]; //  сохраненные коды  доступа операторов
  float MasSenValue[NumSensors][3];// массив значений порогов датчиков для листов
  unsigned MasBlUr[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
  unsigned  MasWtUr[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
  float MasBlUrSig[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
  float MasWtUrSig[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
  int MasNumPoints[NumStrScan]; //  массив  количества  точек для подсчитанных для  осреднения
  double MasKoefPzs[NumStrScan]; // массив среднего  уровня  каждой  точки по  уровню черного
  int PrPageCal,PgXoLCal,PgYoLCal,PgXoRCal,PgYoRCal;
  struct tm MyTime;

  float Optr0,Optr1,Optr2,Optr3;

void TestScan(void)
{  //********Тест сканирвоания - сканирование и запоминание образа ****************
    Scptr_r = 0;
    Scptr_s = 0;
    int nstr2;
    MotorFuS = false;
    waitBothOpt();
    nstr =0;
            for(int i=0;i<4;i++)      cout << " Start Image Scan Optron  N =      "<< i << "     Value =   "<<  Optron[i] << endl;
    Page_scan();
    while(nstr>=0)
    { //  передача  очередной  строки из буфера
        nstr2=nstr;
        nstr = getNextScanLine();
    }
    cout << "  The  end  of  Image  Scan   nstr=  " << nstr<<  "nstr2 =  " << nstr2 <<endl;
    get_Optrons_Imm();
        for(int i=0;i<4;i++)
          {cout << " End  Scanning  Optron  N =     "<< i << "   Value =   "<<Optron[i] <<  "   Porog = "<< NO_PAPERO[i] <<endl;}

    sleep(1);
        Page_backTest();
    //Page_back();// до  освобождения  всех оптронов ( внутри функции  проверка)
    // сохранение образа  в  файле  .bmp
    if(MakeBMPFile(&FBMP[0],SCAN_LINEW,max_gelesen)==0)  printf("\n BMP_File is written\n");
    else printf("\n BMP_File open error\n");
    return;
}


void TestRecogn(void)
{  //********Сканирование и распознавание в режиме Сервисное меню************
    //int d;
    Scptr_r = 0;
    Scptr_s = 0;
    MotorFuS = false;
    waitBothOpt();
    int pr1=0;
    int NumV=0;
    int rez=0;
    nstr =0;
    MasScan.MasRecOut[0]=0;
    Page_scan();
    while(nstr>=0)
    { //  ппередача  очередной  строки из буфера
      nstr = getNextScanLine();
      // функция построчного  сканирования
      if(nstr>=0)
      {
         pr1=ScanBulString();
         rez=MasScan.MasRecOut[0];
      }
      if((pr1!=0)||(rez==111)) nstr=-1;
    }
    cout <<  " PrError =   %d    "<<  pr1 << endl;
    cout <<  " Result =   %d    "  <<  rez << endl;
     cout << "  The  end  of  Image  Scan   nstr=  " << nstr<< endl;
                     for(int i=0;i<4;i++)          cout << " Optron  N =      "<< i << "     Value =   "<<  Optron[i] <<  endl;
    if((pr1<100)&&(pr1>0))
    {  // бюллетень неустановленной  формы
       InfSTablo2=36;
       // формировать Log-file
    } else
    { rez=MasScan.MasRecOut[0];
      if(rez==111)
      {  // результаты  сформированы
        NumV=MasScan.MasRecOut[3];
        int ii= MasScan.MasVoprOut[NumV][1];
        cout <<"Marks  =   "<<  ii  << endl;

        switch (ii)
        {   case -1: InfSTablo2=37;break;
            case -2: InfSTablo2=38;break;
            default: InfSTablo2=39;break;
        }
      }
    }
    sleep(1);
    Page_back();// до  освобождения  всех оптронов ( внутри функции  проверка)
    // сохранение образа  в  файле  .bmp
    if(MakeBMPFile(&FBMP[0],SCAN_LINEW,max_gelesen)==0)  printf("\n BMP_File is written\n");
    else printf("\n BMP_File open error\n");
    return;
}// TestRecogn
 //  запись и считывание  файлов начальной  загрузки
 //***********************************
void SetUpKoibWrite(void)
{ // сохранение параметров КОИБ
  FILE *out;
  if ((out = fopen("./SetUpFiles/SetUpKoibPar.dat", "wt"))!= NULL)
  { // запись в  файл
	fprintf(out,"%10d\n",Lang);
	fprintf(out,"%s",SourceDir1);   fprintf(out,"\n");
	fprintf(out,"%s",SaveDir);      fprintf(out,"\n");
	fprintf(out,"%s",Ip_master);    fprintf(out,"\n");
//	fprintf(out,"%s",Ip_slave);     fprintf(out,"\n");
	fprintf(out,"%s",iface);        fprintf(out,"\n");
	fprintf(out,"%s",udp_port);     fprintf(out,"\n");
	fprintf(out,"%s",SerNumKoib);   fprintf(out,"\n");
	fprintf(out,"%f",ScanerWorkTime);
	fclose(out);
  }
}// SetUpKoibWrite

int SetUpKoibRead(void)
{ // считывание параметров КОИБ при инициализации  системы
    int pr=0;
   FILE *fw;
   if ((fw = fopen("./SetUpFiles/SetUpKoibPar.dat", "r"))!= NULL)
   {  // ввод парамеров бюллетеней  из  файла исходных данных
		fscanf(fw,"%d",&Lang);
		fscanf(fw,"%s",SourceDir1);
		fscanf(fw,"%s",SaveDir);
		fscanf(fw,"%s",Ip_master);
//		fscanf(fw,"%s",Ip_slave);
		fscanf(fw,"%s",iface);
		fscanf(fw,"%s",udp_port);
		fscanf(fw,"%s",SerNumKoib);
		fscanf(fw,"%f",&ScanerWorkTime);
	  fclose(fw);
	} else
	{   pr=1;
	     cout << "Не найден файл SetUpKoibPar.dat" << endl;
	}
	return pr;
}// SetUpKoibRead

void SetUpUprWrite(void)
{ // сохранение параметров КОИБ
  FILE *out;
  if ((out = fopen("./SetUpFiles/SetUpUprPar.dat", "wt"))!= NULL)
  { // запись в  файл
	fprintf(out,"%10d", VolumeUr);
	fprintf(out,"%10d", VolumeUrMax);
	fprintf(out,"%10d", PrPrint);
	fprintf(out,"%10d", PrSlave);
	fprintf(out,"%10d", wngtime);
	fprintf(out,"%10d", PrSaveLog);
	fprintf(out,"%10d", timeout);
	fclose(out);
  }
}// SetUpUprWrite

int SetUpUprRead(void)
{ // считывание параметров КОИБ при инициализации  системы
    int pr=0;
	FILE *fw;
   if ((fw = fopen("./SetUpFiles/SetUpUprPar.dat", "r"))!= NULL)
   {  // ввод парамеров бюллетеней  из  файла исходных данных
		fscanf(fw,"%d",&VolumeUr); fscanf(fw,"%d",&VolumeUrMax);
		fscanf(fw,"%d",&PrPrint);  fscanf(fw,"%d",&PrSlave);
		fscanf(fw,"%d",&wngtime);  fscanf(fw,"%d",&PrSaveLog);
		fscanf(fw,"%d",&timeout);
	  fclose(fw);
	} else
	{  pr=1;
	   cout << "Не найден файл SetUpUprPar.dat" << endl;
	}
	return pr;
}// SetUpUprRead

//--------------------------------
void SetUpRecWrite(void)//SetUpRecWrite(void)
{  // сохранение параметров распознавания
   // общие данные  по  распознаванию SetUpRecPar.txt
  FILE *out;
  if ((out = fopen("./SetUpFiles/SetUpRecPar.dat", "wt"))!= NULL)
  { // запись в  файл
	fprintf(out,"%10d",UrWhite0);	    fprintf(out,"%10d",UrBlack0);
	fprintf(out,"%12.6f", urw);         fprintf(out,"%12.6f", urbl);
	fprintf(out,"%10d", NumDotX*100);   fprintf(out,"%10d",NumDotY*100);
	fprintf(out,"%10d",MaxLightR);       fprintf(out,"%10d",MaxLightG);    fprintf(out,"%10d",MaxLightB);
	fprintf(out,"%10d",PrCheckBr);
	fprintf(out,"%10d",PrCheckSh);      fprintf(out,"%10d",PrCheckEx);
	fprintf(out,"%12.6f",alfa);         fprintf(out,"%12.6f",beta);
	fprintf(out,"%12.6f", UrBlSh); 	    fprintf(out,"%12.6f", UrWtSh);
	fclose(out);
  }
}// SetUpRecWrite

int SetUpRecRead(void)
{ // считывание параметров КОИБ при инициализации  системы
   FILE *fw; int z1,z2, pr=0;
   if ((fw = fopen("./SetUpFiles/SetUpRecPar.dat", "r"))!= NULL)
   {  // ввод парамеров бюллетеней  из  файла исходных данных
		fscanf(fw,"%d",&UrWhite0);
		fscanf(fw,"%d",&UrBlack0);
		fscanf(fw,"%f",&urw); fscanf(fw,"%f",&urbl);
		fscanf(fw,"%d",&z1); fscanf(fw,"%d",&z2);
		fscanf(fw,"%d",&MaxLightR); fscanf(fw,"%d",&MaxLightG); fscanf(fw,"%d",&MaxLightB);
		fscanf(fw,"%d",&PrCheckBr);
		fscanf(fw,"%d",&PrCheckSh); fscanf(fw,"%d",&PrCheckEx);
		fscanf(fw,"%f",&alfa); fscanf(fw,"%f",&beta);
		fscanf(fw,"%f",&UrBlSh); 	fscanf(fw,"%f",&UrWtSh);
		NumDotX=int(float(z1)/100);
		NumDotY=int(float(z2)/100);
	  fclose(fw);
	} else
	{  pr=1;
	     cout << "Не найден файл SetUpRecPar.dat" << endl;
	}
		return pr;
}// SetUpRecRead
// --------------------------------
void SetUpBullWrite(void)
{ // сохранение параметров бюллетеней
  FILE *out;
  if ((out = fopen("./SetUpFiles/SetUpBullPar.dat", "wt"))!= NULL)
  { // запись в  файл
	fprintf(out,"%12.6f",Max_Bull_Height); fprintf(out,"%12.6f",Max_Bull_Width);
	fprintf(out,"%12.6f",LineWM); fprintf(out,"%12.6f", TMarSizeM);
	fprintf(out,"%12.6f",LMarSizeM); fprintf(out,"%12.6f",dxStampAreaM);
	fprintf(out,"%12.6f",dyStampAreaM); fprintf(out,"%12.6f",dxVoteTypeM);
	fprintf(out,"%12.6f",dyVoteTypeM); fprintf(out,"%12.6f",Dx_Max);
	fprintf(out,"%12.6f",Dy_Max); fprintf(out,"%12.6f",MaxAngle*57.3);
	fprintf(out,"%12.6f", dxStampM); 	fprintf(out,"%12.6f",dyStampM);
	fprintf(out,"%12.6f",Dig_Height); 	fprintf(out,"%12.6f",Dig_Width);
	fprintf(out,"%12.6f",Dig_Interv);
	fprintf(out,"%12.6f",dyNumZoneM);
	fprintf(out,"%12.6f",Max_ShAngle*57.3);
	fclose(out);
  }
}// SetUpBullWrite

int SetUpBullRead(void)
{ // считывание параметров КОИБ при инициализации  системы
   FILE *fw; int pr=0;;
   if ((fw = fopen("./SetUpFiles/SetUpBullPar.dat", "r"))!= NULL)
   {  // ввод парамеров бюллетеней  из  файла исходных данных
		fscanf(fw,"%f",&Max_Bull_Height);   fscanf(fw,"%f",&Max_Bull_Width);
		fscanf(fw,"%f",&LineWM); 			fscanf(fw,"%f",&TMarSizeM);
		fscanf(fw,"%f",&LMarSizeM); 		fscanf(fw,"%f",&dxStampAreaM);
		fscanf(fw,"%f",&dyStampAreaM); 		fscanf(fw,"%f",&dxVoteTypeM);
		fscanf(fw,"%f",&dyVoteTypeM); 		fscanf(fw,"%f",&Dx_Max);
		fscanf(fw,"%f",&Dy_Max); 			fscanf(fw,"%f",&MaxAngle);
		fscanf(fw,"%f",&dxStampM); 			fscanf(fw,"%f",&dyStampM);
		fscanf(fw,"%f",&Dig_Height); 		fscanf(fw,"%f",&Dig_Width);
		fscanf(fw,"%f",&Dig_Interv);
		fscanf(fw,"%f",&dyNumZoneM);
		fscanf(fw,"%f",&Max_ShAngle);
		MaxAngle=MaxAngle/57.3;    Max_ShAngle=Max_ShAngle/57.3;
	  fclose(fw);
	} else
	{ pr=1;
	    cout << "Не найден файл SetUpBullPar.dat" << endl;
	}
		return pr;
}// SetUpBullRead

//***********************************
//  для  сервисного  меню

void GashId(void)
{  // гашение  массивов  исходных  данных


}//IdClear

float ReadParFromTablo2(void)  // считывание  параметра  с  табло
{ float zz=-1;
  if(strlen(NUMBER)!=0)   zz=atof(NUMBER);
   return zz;
}

void SendInfToTablo1(void)
{  // выдача параметров распознавания на  табло
   std::string sst;
   strcpy(InfS,"\0");  strcpy(InfS1,"");
   if(InfSTablo1>0) sst=Out.Menu[InfSTablo1];else sst="";
   for(int i=0;i<124;i++)InfS[i]=sst[i];
   cout << InfS << endl;
   setFirstline(&sst[0],1);
   usleep(200000);
   InfSTablo1=0;strcpy(InfS,"\0"); strcpy(InfS1,"");
}

void  SaveKoeffPzs(void)
{ // сохранение калибровочных коэффициентов линейки ПЗС

    // удаление старого файла уровня  черного
    system("rm -f ./SetUpFiles/calibration_data.bin");
    // сохранение уровня  черного  в файл при  калибровке
    calibrate_mode= MODE_CALIBRATE_INIT;
    for(int i=0; i < LINE_LENGTH; i++) calibration_data[i]=MasBlUr[i];
    save_calibration_data(calibration_data);
    // удаление старого файла коэффициентов
    system("rm -f ./SetUpFiles/calibration_dataK.bin");
    // сохранение коэффициентов
    for(int i=0; i < LINE_LENGTH; i++) calibration_dataK[i]=MasKoefPzs[i];
    save_calibration_dataK(calibration_dataK);
} // SaveKoeffPzs

void  SaveKoeffSens(void)
{ // сохранение калибровочных датчиков
  FILE *out;
  if ((out = fopen("./SetUpFiles/KoefSenTek.dat", "wt"))!= NULL)
  { // запись в  файл
                          cout << " Saving Sensors data " << endl;
    for(int i=0; i < NumSensors; i++)
    for(int j=0; j < 3;j++ )fprintf(out,"%12.6f", MasSenValue[i][j]);
    int fclSen=-20;
    fclSen=fclose(out);
    cout <<"    fclose = " << fclSen << endl;
  }
  // расчет пороговых значений определения 1, 2 листов
  NO_PAPER  =MasSenValue[0][0];
  ONE_PAPER=MasSenValue[0][2];
  for(int i=1;i<NumSensors;i++)  if(NO_PAPER > MasSenValue[i][0]) NO_PAPER=MasSenValue[i][0];
  NO_PAPER=NO_PAPER-20;
  for(int i=1;i<NumSensors;i++)  if(ONE_PAPER  < MasSenValue[i][2]) ONE_PAPER=MasSenValue[i][2];
  ONE_PAPER=ONE_PAPER+20;
  for(int i=0;i<NumSensors;i++) NO_PAPERO[i]=MasSenValue[i][0]-20;
  for(int i=0;i<NumSensors;i++) ONE_PAPERO[i]=MasSenValue[i][2]+20;

cout << " MasSenValue =   " << MasSenValue[1][0] << endl;
cout << " NO_PAPERO =   " ;   for(int i=0;i<NumSensors;i++)   cout << "   i=  " << i<< "  " << NO_PAPERO[i];  cout << endl;

} // SaveKoeffSens

int  ReadKoeffSens(void)
{ // сохранение калибровочных датчиков
  FILE *fw; int pr=0;
  if ((fw = fopen("./SetUpFiles/KoefSenTek.dat", "r"))!= NULL)
  { // запись в  файл
	for(int i=0; i < NumSensors; i++)
	for(int j=0; j < 3;j++ )fscanf(fw,"%f",&MasSenValue[i][j]);
	fclose(fw);
  } else
  { pr=1;
        cout << "Не найден файл KoefSenTek.dat " << endl;
  }
  	return pr;
} // SaveKoeffSens

void BLev1(int nn,unsigned urw_ch,unsigned* ur,float* sur)
{ // функция определения  среднего  уровня сигнала и СКО
  //  путем последовательного накопления
	int urw1;  float dd;
	urw1=urw_ch; dd=*ur;
/*		if(nn>1)
	{ *ur=((*ur)*(nn-1)+urw1)/nn;
	  *sur=sqrt((*sur)*(*sur)+((dd-(*ur))*(dd-(*ur)))/nn);
	} else { *ur=urw1;  *sur=1;}
	*/
	if(nn>1)
	{ *ur=((*ur)*(nn)+urw1)/(nn+1);
	  *sur=sqrt((*sur)*(*sur)+((dd-(*ur))*(dd-(*ur)))/(nn+1));
	} else { *ur=urw1;  *sur=5;}

}// функция определения  среднего  уровня сигнала и СКО

int  FindPgStartCalibr(int nstr)
{  //  функция  поиска начала  листа бюллетеня
	int Pr=0;
    // определение левой границы
    EvChangeColRow(0,nstr,1,0,200,&nbxL,&XoL,&PrL);
	// определение правой границы
	EvChangeColRow(0,nstr,-1,NumStrScan-1,200,&nbxR,&XoR,&PrR);
	// формирование признака  окончания  поиска   начала  листа PrPage=1
	if((PrR==1)&&(PrL==1)) Pr=1;
   return Pr;
}// FindPgStartCalibr;


int EvalMedWhite(int nstr1)
{ // вычисление  среднего  значения  уровня  белого для каждого датчика линейки
  int nn=0,i1=-1,i2=-1;//, pr1=0;//,sur1=0
  i1=200;//TMarSize*2;
  for(nstr=i1;nstr < i1+nstr1; nstr++)
  { //  цикл по  строкам
     //nbxL=0;nbxR=0;PrL=0;PrR=0;
     XoL=100;//LMarSize;
     XoR=NumStrScan-100;//LMarSize;
     //pr1=FindPgStartCalibr(nstr);
     //if(pr1==1)
     //{
        for(int i= XoL; i <=XoR; i++)
	    { // по точкам  строки
            urw_ch=GetXYValue(nstr,i);
            nn=MasNumPoints[i];
            //	g0=RecBlack(alfa,beta,urw,surw,urbl,surbl,urw_ch,&p00,&p11);
            //	if (g0==0)
            //	{  // точка определена  как белая
            BLev1(nn,urw_ch,&MasWtUr[i],&MasWtUrSig[i]);
            nn=nn+1;
            MasNumPoints[i]=nn;
            //	}
        }
     //}
  }
    // определение  уровня  белого в  начале и в конце строки
  for(int i=XoL+1; i >= 0; i--) if(MasNumPoints[i]==0)   { i1=i; goto fini1; };
  fini1:;
  for(int i=NumStrScan-XoR-1; i < NumStrScan; i++) if(MasNumPoints[i]==0) { i2=i; goto fini2;};
  fini2:;
  if(i1 >=0)
  {	for(int i=0; i<= i1; i++)
	{ 	MasWtUr[i]=MasWtUr[i1+1]; MasWtUrSig[i]=MasWtUrSig[i1+1];
		MasNumPoints[i]=MasNumPoints[i1+1];
	}
  }
  if(i2 >=0)
  { for(int i=i2; i< NumStrScan; i++)
	{ MasWtUr[i]=MasWtUr[i2-1]; MasWtUrSig[i]=MasWtUrSig[i2-1];
      MasNumPoints[i]=MasNumPoints[i2-1];
	}
  }
  return 1;
} // EvalMedWhite

int EvalMedBlack(int nstr1)
{ // вычисление  среднего  значения  уровня  белого для каждого датчика линейки
  int nn=0;//i1=-1,i2=-1;//sur1=0
  for(nstr=0;nstr < nstr1; nstr++)
  { //  цикл по  строкам
  //nstr=10;
	for(int i=0; i < NumStrScan; i++)
	  { // по точкам  строки
		urw_ch=GetXYValue(nstr,i);
		nn=MasNumPoints[i];
		BLev1(nn,urw_ch,&MasBlUr[i],&MasBlUrSig[i]);
		nn=nn+1;
		MasNumPoints[i]=nn;
	  }
  }
  return 1;
} // EvalMedBlack

void GetPzsScan(int ur)
{ // сканирование линейки ПЗС
  if(ur==0) // сканирование  уровня  черного
  { // сканирование без листа
      max_gelesen=1000;
      calibrate_mode= MODE_CALIBRATE_INIT;
      Page_scan();
      sleep(1); // для  снятия  отсчета
      Page_stop();
      calibrate_mode= MODE_NORMAL;
      EvalMedBlack(200);
  }
  else
  {	//int i=-1;
    if(ur==1) // сканирование  чистого  листа
	{
        calibrate_mode= MODE_NORMAL;
        TestScan();
        cout << " The Last Optron Has been reached. Stop. Back " << endl;
        EvalMedWhite(300);

    }
  }
}

void GetSensorsValue(int n)
{ // получение  значений  датчиков
  // n  -  количество листов
  if(n==0)
  {  get_Optrons_Imm();
     if(Optron[0]>10)MasSenValue[0][n]=Optron[0];     if(Optron[1]>10)MasSenValue[1][n]=Optron[1];
     if(Optron[2]>10)MasSenValue[2][n]=Optron[2];     if(Optron[3]>10)MasSenValue[3][n]=Optron[3];
  } else
  {  //  при наличии 1 или 2 листов
    Optr0=0;Optr1=0;Optr2=0;Optr3=0;
    get_Optrons_Imm(); //waitBothOpt();
    cout << "Start Cal Optrons" <<endl;

 //   int dd=Optron[0] ;
    Page_scan();
    sleep(1);
       cout << "Stop  Page" <<endl;
   // usleep(800000);
  // while (dd-Optron[0] >  30) get_Optrons_Wait();
    Page_stop();
    get_Optrons_Imm();
    if(Optron[0]>10)MasSenValue[0][n]=Optron[0];    if(Optron[1]>10)MasSenValue[1][n]=Optron[1];
    if(Optron[2]>10)MasSenValue[2][n]=Optron[2];    if(Optron[3]>10)MasSenValue[3][n]=Optron[3];
    Optr0=MasSenValue[0][n];Optr1=MasSenValue[1][n];Optr2=MasSenValue[2][n];Optr3=MasSenValue[3][n];
            //  сдвиг листа на  новую точку ( точка 2)
    usleep(500000); Page_scan();  usleep(100000); Page_stop();
            //  снятие  второй  точки значений оптронов
    get_Optrons_Imm();
    if(Optron[0]>10)MasSenValue[0][n]=Optron[0];    if(Optron[1]>10)MasSenValue[1][n]=Optron[1];
    if(Optron[2]>10)MasSenValue[2][n]=Optron[2];    if(Optron[3]>10)MasSenValue[3][n]=Optron[3];
    Optr0=Optr0+MasSenValue[0][n];Optr1=Optr1+MasSenValue[1][n];
    Optr2=Optr2+MasSenValue[2][n];Optr3=Optr3+MasSenValue[3][n];
        //  сдвиг листа на  новую точку
    usleep(500000);Page_scan();  usleep(100000); Page_stop();
        //  снятие третьей  точки значений оптронов
    get_Optrons_Imm();
    if(Optron[0]>10)MasSenValue[0][n]=Optron[0];    if(Optron[1]>10)MasSenValue[1][n]=Optron[1];
    if(Optron[2]>10)MasSenValue[2][n]=Optron[2];    if(Optron[3]>10)MasSenValue[3][n]=Optron[3];
    usleep(500000);  //Page_back();
    Optr0=Optr0+MasSenValue[0][n];Optr1=Optr1+MasSenValue[1][n];
    Optr2=Optr2+MasSenValue[2][n];Optr3=Optr3+MasSenValue[3][n];
    Page_backTest();
        // осреднение значений оптронов по 3 точкам
    MasSenValue[0][n]=Optr0/3; MasSenValue[1][n]=Optr1/3; MasSenValue[2][n]=Optr2/3; MasSenValue[3][n]=Optr3/3;
    MasSenValue[0][n]=Optr0/4; MasSenValue[1][n]=Optr1/4; MasSenValue[2][n]=Optr2/4; MasSenValue[3][n]=Optr3/4;
  }
};

int KodDostAn(int kod)
{ // проверка правильности кода доступа
  int i,pp=-1;
  for (i = 0; i < 3; i++)
  if(kod==CodesOp[i]) pp=i;
  return pp;
}

void SendInfToTablo2()
{ // формирование информации для  выдачи во  вторую строку табло
  std::string sst;
  char chs[41];  strcpy(chs,"");
  strcpy(InfS,""); strcpy(InfS1,""); strcpy(InfS2,"");

  if(InfSTablo2>0)sst=Out.Menu1[InfSTablo2];
//  InfS=sst;

  switch (InfSTablo2)
   {   case 0: sst=""; break;
       case 1:
        { strcpy(InfS1,"Level 0 :  ");  //strcpy(InfS,"Уровень 0 :  ");

          sprintf(InfS2,"%03.0f : %03.0f : %03.0f : %03.0f ",MasSenValue[0][0],MasSenValue[1][0],MasSenValue[2][0],MasSenValue[3][0]);
          sst=sst+InfS2;//strcat(InfS,InfS2);
          strcat(InfS1,InfS2);
        } break;
     case 2:
        { strcpy(InfS1,"Level 1 :  ");//strcpy(InfS,"Уровень 1 :  ");
          sprintf(InfS2,"%03.0f : %03.0f : %03.0f : %03.0f ",MasSenValue[0][1],MasSenValue[1][1],MasSenValue[2][1],MasSenValue[3][1]);
          sst=sst+InfS2;//strcat(InfS,InfS2);
          strcat(InfS1,InfS2);
        } break;
     case 3:
        { strcpy(InfS1,"Level 2 :  ");//strcpy(InfS,"Уровень 2 :  ");
          sprintf(InfS2,"%03.0f : %03.0f : %03.0f : %03.0f ",MasSenValue[0][2],MasSenValue[1][2],MasSenValue[2][2],MasSenValue[3][2]);
          sst=sst+InfS2;//strcat(InfS,InfS2);
          strcpy(InfS1,InfS);
        } break;
     case 4:  {	sprintf(InfS2,"%03.0f",urbl); sst=sst+InfS2;strcpy(InfS1,InfS); strcpy(NUMBER,InfS2); strcat(NUMBER,"\0");   } break;
     case 5:  { sprintf(InfS2,"%03.0f",urw);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0");  } break;
     case 6:  { sprintf(InfS2,"%03.0f",UrBlSh);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 7:  { sprintf(InfS2,"%03.0f",UrWtSh);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0"); } break;
     case 8:  { sprintf(InfS2,"%03d",MaxLight);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 9:  { sprintf(InfS2,"%03.0f",MaxAngle*57.3);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 10: { sprintf(InfS2,"%02d",PrCheckSh);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 11: { sprintf(InfS2,"%02d",PrCheckBr);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 12: { sprintf(InfS2,"%02d",PrCheckEx);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 13: { sprintf(InfS2,"%02d:%02d:%04d  %02d.%02d.%02d ",MyTime.tm_mday,MyTime.tm_mon,MyTime.tm_year,MyTime.tm_hour,MyTime.tm_min,MyTime.tm_sec);
                sst=sst+InfS2;strcpy(InfS1,InfS);} break;
     case 14: { sprintf(InfS2,"%02d",MyTime.tm_mday); sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 15: { sprintf(InfS2,"%02d",MyTime.tm_mon);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0");  } break;
     case 16: { sprintf(InfS2,"%02d",MyTime.tm_year);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 17: { sprintf(InfS2,"%02d",MyTime.tm_hour); sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 18: { sprintf(InfS2,"%02d",MyTime.tm_min);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 19: { sprintf(InfS2,"%s",SerNumKoib);sst=sst+InfS2;strcpy(InfS1,InfS); strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); /*cерийный номер КОИБ*/} break;
     case 20: { sprintf(InfS2,"%s",Ip_master);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0");  } break;
     case 21: { sprintf(InfS2,"%s",Ip_slave);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); } break;
     case 22: { sprintf(InfS2,"%02d",VolumeUr); sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0");  } break;
     case 23: { sprintf(InfS2,"%s",SerNumKoib);sst=sst+InfS2; strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); /* данные по cканеру - серийный номер  */} break;
     case 24: { sprintf(InfS2,"%s",SerNumKoib1);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2); strcat(NUMBER,"\0"); /*данные по сканеру - соседу - серийный номер*/} break;
     case 25: { sprintf(InfS2,"%10.1f",ScanerWorkTime);sst=sst+InfS2;strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0"); /*  время работы  сканера*/} break;
    case 46: { sprintf(InfS2,"%d",CodesOp[0]);sst=sst+InfS2;   strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0");/*  код администратора*/} break;
    case 47: { sprintf(InfS2,"%df",CodesOp[1]);sst=sst+InfS2;  strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0");/*  код техн. специалиста*/} break;
    case 48: { sprintf(InfS2,"%d",CodesOp[2]);sst=sst+InfS2;   strcpy(InfS1,InfS);strcpy(NUMBER,InfS2);strcat(NUMBER,"\0");/* код оператора УИК*/} break;
    case 49: { sprintf(InfS2,"%02d",Lang);sst=sst+InfS2;strcpy(InfS1,InfS);} break;
   }
   for(int i=0;i<124;i++)InfS[i]=sst[i];
   cout << InfS1 << endl;
   setSecline(&sst[0],1);
   usleep(200000);
   InfSTablo2=0; strcpy(InfS,"\0"); strcpy(InfS1,"\0");strcpy(InfS2,"\0");
}// SendInfToTablo2
int ServMenu()
{ //   управляющая программа  сервисного  меню
   char *ss=new char[10]; int i;   int pr_out=0;

   // перезапись значений  кнопок  в  мой  массив
   if(PrMenuBtn > 0)
   { //  нажата  кн. Меню
       PrNoBtnWait=0;
      if((KodDost < 1)&&(PrMenuBtn==1))
      {  InfSTablo1=2; SendInfToTablo1();
         strcpy(NUMBER,"*\0");   setSecline(NUMBER,0);
         get_KeyString(NUMBER);
         KodDost=int(ReadParFromTablo2());
         BtnPushed=1;// чтобы не подтверждать еще в одном цикле опроса кнопкой ДА
      }
	  if((BtnPushed==0)&&(KodOp < 0)){  PrMenuBtn=-1;   goto finMenu;  }
	  // проверка значения  кода доступа
	  if(KodDost < 1)
	  { if(PrMenuBtn==1){ InfSTablo1=2; SendInfToTablo1();}
		// формирование признаков  полного  меню
		for(i=1;i< NumMenuItems+1;i++) MenuItems0[i]=i;
		BtnPushed =-1;
	  } else
	  { // код доступа введен, проверка правильности кода  доступа
		 if((BtnPushed==0)&&(KodOp < 0)){  PrMenuBtn=-1;   goto finMenu1;  }
		 // проверка введенного  кода  доступа и формирование меню
		 if((BtnPushed==1)&&(KodOp==-1))
		 { // действия при  вводе  кода  оператора, пока он еще не введен
			KodOp=KodDostAn(KodDost);
			if(KodOp==-1)
			{ InfSTablo1=3; SendInfToTablo1();
                KodDost=-1;                sleep(1);                PrNoBtnWait=1;
			}
			else
			{  PrMenuBtn=2;
			    for(int i=0; i< NumMenuItems+1; i++)MenuItems0[i]=0;
			   switch (KodOp)
				{ case 0:
				  {  for(int i=1; i< NumMenuItems+1; i++)MenuItems0[i]=i;
					 ItemMenu0=0;InfSTablo1=4; SendInfToTablo1();
				  }
				  break;
				case 1:
				  {   InfSTablo1=5; SendInfToTablo1();
					  for(int i=1; i< NumMenuItems+1; i++)MenuItems0[i]=i;
					  MenuItems0[15]=0;  ItemMenu0=0;
				  } break;
				case 2:
				  {   MenuItems0[5]=5; MenuItems0[7]=7;MenuItems0[10]=10;
					  MenuItems0[NumMenuItems]=NumMenuItems;
					  ItemMenu0=0; InfSTablo1=6; SendInfToTablo1();
				  }  break;
				} // switch
			 }
			 //гашение признака нажатой  кн
			 BtnPushed=-1;
		 }
		 else
		 { // код оператора введен работа  в сервисном  меню
		   if(BtnPushed!=-1)
		   {   if(ItemMenu0==0)
			   { switch (KodOp)
					{ case 0: { ItemMenu0=1; InfSTablo1=7;  } break;
					  case 1: { ItemMenu0=1; InfSTablo1=7;  } break;
					  case 2: { ItemMenu0=5; InfSTablo1=11; } break;
					} // switch
					SendInfToTablo1();
			   }
			   else
			   { if((BtnPushed==0)&&(MenuUr==0))
				 {  // уменьшение  номера пункта меню
					BtnPushed=-1;
                    if(ItemMenu0 ==1)
                    {  for(int i=NumMenuItems;i >0; i--)if(MenuItems0[i]!=0) { ItemMenu0=i; goto met11;}
                        met11:;
					} else
					{ 	  for(int i=ItemMenu0-1;i >0; i--)if(MenuItems0[i]!=0) { ItemMenu0=i; goto met1;}     met1:;	}
                }
				if((BtnPushed==1)&&(MenuUr==0))
				{   // увеличение  номера пункта меню
					BtnPushed=-1;
					if(ItemMenu0 == NumMenuItems)
					{  for(int i=1;i < NumMenuItems+1; i++ )   if(MenuItems0[i]!=0) { ItemMenu0=i;goto met222;  } met222:;
					} else
					{  for(int i=ItemMenu0+1;i < NumMenuItems+1; i++ )   if(MenuItems0[i]!=0) { ItemMenu0=i;goto met2;  } met2:;    }
                }
                if(MenuUr==0)
                { // Формирование сообщения о пункте меню
                    switch (ItemMenu0)
                    {  case 1: InfSTablo1=7;  break; 		  case 2: InfSTablo1=8;  break;		  case 3: InfSTablo1=9;  break;
                        case 4: InfSTablo1=10; break;		  case 5: InfSTablo1=11; break;	  case 6: InfSTablo1=12; break;
                        case 7: InfSTablo1=13; break;		  case 8: InfSTablo1=14; break;	  case 9: InfSTablo1=15; break;
                        case 10: InfSTablo1=16;break;		  case 11: InfSTablo1=17;break;	  case 12: InfSTablo1=18;break;
                        case 13: InfSTablo1=19;break;		  case 14: InfSTablo1=20;break;     case 15: InfSTablo1=21;break;
                        case 16: InfSTablo1=22;break;
                    } // switch
                    SendInfToTablo1();
                }
            }
        }
//***************** начало  работы  по  пунктам  сервисного  меню  *********
		switch (ItemMenu0)
/*******/
        { case 1:
            { // подменю КАЛИБРОВКА датчиков  и ПЗС
			  if(BtnPushed==3) //нажата кн. ВЫБОР
	          { InfSTablo1=23; SendInfToTablo1();
				ItemMenu1=1; MenuUr=1;//MenuUr+1;
				for(int i=0; i < NumStrScan; i++)
				{ // гашение  массивов калибровки
				  MasBlUr[i]=0;	MasWtUr[i]=0;	MasBlUrSig[i]=0; MasWtUrSig[i]=0;
				  MasNumPoints[i]=0;
				 }
              };
              switch (ItemMenu1)
				{ case 1: // калибровка датчиков без листа
					{   if(BtnPushed==0)
						{  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения  о  следующем параметре
							 InfSTablo1=25; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                             BtnPushed=-1;
						}
					  //Buttons
                      if(BtnPushed==1)
					  { //strcpy(InfS,"Калибровка датчиков на уровне 0");
                        frontLED(2,0);// включение красной лампочкиидет процесс сканирования и обработки
					    InfSTablo1=24; SendInfToTablo1();   InfSTablo2=0; SendInfToTablo2();
						GetSensorsValue(0);
						InfSTablo2=1; SendInfToTablo2();    InfSTablo1=25; SendInfToTablo1();
						ItemMenu1=ItemMenu1+1; BtnPushed=-1;
						frontLED(0,2);// увеличение интенсивности зеленой  лампочки и отключение красной
					  }
					}; break;
				  case 2: // калибровка с  одним листом
					{  if(BtnPushed==0)
						{  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения  о  следующем параметре
							 InfSTablo1=27; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                             BtnPushed=-1;
						}
					    if(BtnPushed==1)
					  {  frontLED(2,0);// включение красной лампочкиидет процесс сканирования и обработки
					     InfSTablo1=26; SendInfToTablo1();   InfSTablo2=0; SendInfToTablo2();
						 GetSensorsValue(1);
						 usleep(50000);
						 InfSTablo2=2; SendInfToTablo2();    InfSTablo1=27; SendInfToTablo1();
						 ItemMenu1=ItemMenu1+1;
						 BtnPushed=-1;
						 frontLED(0,2);// включение зеленой  лампочки и отключение красной
					  }
					}; break;
                  case 3: // калибровка  датчиков с двумя листами
                    {
                         if(BtnPushed==0)
						{  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения  о  следующем параметре
							// InfSTablo1=29; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                           //  BtnPushed=-1;
                           // выдача сообщения о необходимости сохранения параметров датчиков
                            InfSTablo1=33; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                             BtnPushed=1;
						}
						if(BtnPushed==1)
					  {     frontLED(2,0);// включение красной лампочкиидет процесс сканирования и обработки
                            InfSTablo1=28; SendInfToTablo1();
                            GetSensorsValue(2);
                            InfSTablo2=3; SendInfToTablo2();  InfSTablo1=33; SendInfToTablo1();
                            ItemMenu1=ItemMenu1+1;  	BtnPushed=-1;
						//  подготовка массива  к  сканированию уровня  черного
						for(i=0;i< NumStrScan ;i++) MasBlUr[i]=0;
						frontLED(0,2);// включение зеленой  лампочки и отключение красной
					  }
					}; break;
                }//switch (ItemMenu1)
                if((ItemMenu1==6)&&(BtnPushed==1))
				{ // возвращение  в  меню предыдущего уровня
				  ItemMenu1=0; MenuUr=0;//MenuUr-1; strcpy(InfS,"");
                  InfSTablo1=7; SendInfToTablo1();
                  InfSTablo2=0; SendInfToTablo2();
                  BtnPushed=-1; if(MenuUr < 0)MenuUr=0;
                }
				if((ItemMenu1==5)&&(BtnPushed==1))
				{ // возвращение  в  меню предыдущего уровня
				  // расчет и сохранение  коэффициентов ПЗС
                    for(i=0;i< NumStrScan;i++)
                    {   double dd=double(MasWtUr[i])-double(MasBlUr[i]);
                        if(dd>1)  MasKoefPzs[i]=double(UrWhite0)/dd; else MasKoefPzs[i]=1;
                        //if ((i<10)||((i>300)&&(i<311)))
                        //{ cout << "  UrBlack = "<<MasBlUr[i] << "  UrWhite = " << MasWtUr[i]<< "  Koeff= "<<MasKoefPzs[i]<<endl;}
                    }
                    // сохранить  коэффициенты  в файл

                    cout << " Before Saving Sensors data " << endl;
                    // сохранение уровней  датчиков  бумаги

                    SaveKoeffSens();
                    ItemMenu1=6; //Калибровка завершена, нажмите кн. ДА
                    InfSTablo1=34; SendInfToTablo1();
                    InfSTablo2=0; SendInfToTablo2();
                    BtnPushed=-1;
				}
				if((ItemMenu1==4)&&(BtnPushed==1))ItemMenu1=5;
            }; break;
/*******/
        case 2:
            { // подменю КАЛИБРОВКА  ПЗС
              if(BtnPushed==3) //нажата кн. ВЫБОР
              { InfSTablo1=29; SendInfToTablo1();
				ItemMenu1=1; MenuUr=1;//MenuUr+1;
				//  подготовка массива  к  сканированию уровня  черного
				for(i=0;i< NumStrScan ;i++) MasBlUr[i]=0;
				for(int i=0; i < NumStrScan; i++)
				{  // гашение  массивов калибровки
				   MasBlUr[i]=0;	MasWtUr[i]=0;	MasBlUrSig[i]=0; MasWtUrSig[i]=0; MasNumPoints[i]=0;
                }
				BtnPushed=-1;
              }
			  switch (ItemMenu1)
				{ case 1: // калибровка ПЗС уровень черного
					{  if(BtnPushed==0)
						{  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения  о  следующем параметре
							 InfSTablo1=31; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                             BtnPushed=-1;
						}
					    if(BtnPushed==1)
					  { InfSTablo1=30; SendInfToTablo1();   InfSTablo2=0; SendInfToTablo2();
                            frontLED(2,0); // включение красной лампочки идет процесс сканирования и обработки
                        CalibrRqst=true; usleep(200000);
                        _clear();
                        usleep(200000);
                                    altera_backlight(20,20,20);
                        CalibrRqst=false;  // очистка регистров
                         //sleep(1);

						GetPzsScan(0);
                        CalibrRqst=true; usleep(200000);
                                        altera_backlight(MaxLightR,MaxLightG,MaxLightB);
                        CalibrRqst=false;  // очистка регистров
                        frontLED(0,2); // включение зеленойлампочки идет процесс сканирования и обработки
						//  подготовка массива  к  сканированию уровня  белого
						InfSTablo1=31; SendInfToTablo1();
						for(i=0;i< NumStrScan ;i++)
						{ MasWtUr[i]=0; MasNumPoints[i]=0; MasWtUrSig[i]=0;  }
						BtnPushed=-1; 	ItemMenu1=ItemMenu1+1;
					  }
					}; break;
                  case 2: // калибровка ПЗС уровень белого
					{   if(BtnPushed==0)
						{  ItemMenu1=ItemMenu1+1;
                            // выдача сообщения  о  следующем параметре
                            InfSTablo1=33; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                            BtnPushed=1;
						}else
					    {   if(BtnPushed==1)
                            {   InfSTablo1=32; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                            frontLED(2,0); // включение красной лампочки идет процесс сканирования и обработки
                                GetPzsScan(1);
                                frontLED(0,2); // включение зеленой лампочки идет процесс сканирования и обработки
                                InfSTablo1=33; SendInfToTablo1(); ItemMenu1=ItemMenu1+1;
                            }
                        }
					}; break;
			    }//switch (ItemMenu1)
				if((ItemMenu1==5)&&(BtnPushed==1))
				{ // возвращение  в  меню предыдущего уровня
				  ItemMenu1=0; MenuUr=0;
				  InfSTablo1=8; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
   			      BtnPushed=-1; if(MenuUr < 0)MenuUr=0;
				 }
				 if((ItemMenu1==4)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=5; InfSTablo1=34; SendInfToTablo1();  InfSTablo2=0; SendInfToTablo2();
				   // расчет и сохранение  коэффициентов ПЗС
				   frontLED(2,0); // включение красной лампочки идет процесс сканирования и обработки
                    for(i=0;i< NumStrScan;i++)
                    {    double dd=double(MasWtUr[i])-double(MasBlUr[i]);
                       //if(dd>1)  MasKoefPzs[i]=double(UrWhite0)/dd; else MasKoefPzs[i]=1;
                        if(dd>1)  MasKoefPzs[i]=double(MasWtUr[i])/dd; else MasKoefPzs[i]=1;
                        if ((i<10)||((i>800)&&(i<831)))
                        { cout << "  UrBlack = "<<MasBlUr[i] << "  UrWhite = " << MasWtUr[i]<< "  Koeff= "<<MasKoefPzs[i]<<endl;}
                    }
                    // сохранить  коэффициенты  в файл
                    SaveKoeffPzs();
                    CalibrRqst=true; usleep(200000); /*set_calibration_data();  */set_calibration_dataK();  CalibrRqst=false;
                    frontLED(0,2); // включение зеленой лампочки идет процесс сканирования и обработки
                    ItemMenu1=5;
                    BtnPushed=-1;
				 }
				 if((ItemMenu1==3)&&(BtnPushed==1))ItemMenu1=4;
			   } break;
/* ==================================================================================******/
        case 3: { // подменю РАСПОЗНАВАНИЕ
				if(BtnPushed==3) //нажата кн. ВЫБОР
				{ InfSTablo1=35; SendInfToTablo1(); InfSTablo2=4; SendInfToTablo2();
				  ItemMenu1=1; MenuUr=1; BtnPushed=-1; PrNoBtnWait=1; //PrEnterStart=0;
				}
				switch (ItemMenu1)
                {  case 1: // Начальный уровень черного
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;  //ParOut=0;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=36; SendInfToTablo1(); InfSTablo2=5; SendInfToTablo2();
                             BtnPushed=-1;
						  } else
						  { if((BtnPushed==1))//||(PrEnterStart==1))
							{   // считывание параметра
							    get_KeyString(NUMBER);
								urbl=ReadParFromTablo2(); //PrEnterStart=0;
								InfSTablo2=4; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 2: // начальный  уровень  белого
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения  о  следующем параметре
							 InfSTablo1=37; SendInfToTablo1(); InfSTablo2=6; SendInfToTablo2();
                             BtnPushed=-1;
						  } else
						  { if((BtnPushed==1))//||(PrEnterStart==1))
                            {   get_KeyString(NUMBER);
                                urw=ReadParFromTablo2(); //PrEnterStart=0;
                                InfSTablo2=5; SendInfToTablo2();
                            }
                          }
						  BtnPushed=-1;
						}; break;
						 case 3: // калибровка уровень черного  для штампа
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;BtnPushed=-1;
							 // выдача сообщения  о  следующем параметре
							 InfSTablo1=38; SendInfToTablo1(); InfSTablo2=7; SendInfToTablo2();
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{ // считывание параметра
							  get_KeyString(NUMBER);
							  UrBlSh=ReadParFromTablo2(); //PrEnterStart=0;
							   InfSTablo2=6; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 4: // начальный уровень белого  для  штампа
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;   //ParOut=0;
							 // выдача сообщения  о  следующем параметре
							 MaxLight=MaxLightR;
							 InfSTablo1=39; SendInfToTablo1();  InfSTablo2=8; SendInfToTablo2();
                             BtnPushed=-1;
						  } else
						  { if((BtnPushed==1))//||(PrEnterStart==1))
							{  // считывание параметра
							   get_KeyString(NUMBER);
							   UrWtSh=ReadParFromTablo2(); //PrEnterStart=0;
							   InfSTablo2=7; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 5: // калибровка максимальный уровень подсветки красным
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1; // для выхода из меню
							  InfSTablo1=73; SendInfToTablo1();
							  MaxLight=MaxLightG;InfSTablo2=8; SendInfToTablo2();
							    BtnPushed=-1;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{ // сохранение  параметров в файл текущих параметров
							  get_KeyString(NUMBER);
							  MaxLightR=ReadParFromTablo2();
							   MaxLight=MaxLightR; 	  InfSTablo2=8; SendInfToTablo2();
							  //PrEnterStart=0;
							}
						  }
						  BtnPushed=-1;
						}; break;
                        case 6: // калибровка максимальный уровень подсветки зеленым
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1; // для выхода из меню
						      MaxLight=MaxLightB;
							  InfSTablo1=74; SendInfToTablo1();  InfSTablo2=8; SendInfToTablo2();
							   BtnPushed=-1;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{ // сохранение  параметров в файл текущих параметров
							  get_KeyString(NUMBER);
							  MaxLightG=ReadParFromTablo2();
							  MaxLight=MaxLightG; 	InfSTablo2=8; SendInfToTablo2();
							  //PrEnterStart=0;
							}
						  }
						  BtnPushed=-1;
						}; break;
                        case 7: // калибровка максимальный уровень подсветки синим
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1; // для выхода из меню
                             InfSTablo1=40; SendInfToTablo1();
                             InfSTablo2=0; SendInfToTablo2();
							  //PrEnterStart=0;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{ // ввод уровня  подсветки синего
							   get_KeyString(NUMBER);
							  MaxLightB=ReadParFromTablo2();
							  MaxLight=MaxLightB; 	InfSTablo2=8; SendInfToTablo2();
							  //PrEnterStart=0;
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 8: //
						{ if(BtnPushed==0)
						  {   // для выхода из без сохранения в файл меню
						        ItemMenu1=ItemMenu1+1; BtnPushed=1;
						        frontLED(2,0); // включение красной лампочки идет процесс записи в регистр
                                CalibrRqst=true; usleep(200000);
                                        altera_backlight(MaxLightR,MaxLightG,MaxLightB);
                                CalibrRqst=false;
                                frontLED(0,2); // включение зеленой лампочки  запись в регистр закончилась
						  }
						  if(BtnPushed==1)
                         { // сохранение введенных параметров в  файл
                             ItemMenu1=ItemMenu1+1; BtnPushed=1;
                             frontLED(2,0); // включение красной лампочки идет процесс записи в регистр
                                SetUpRecWrite();
                                CalibrRqst=true; usleep(200000);
                                        altera_backlight(MaxLightR,MaxLightG,MaxLightB);
                                CalibrRqst=false;
                                frontLED(0,2); // включение зеленой лампочки запись в регистр закончилась
							}

						}; break;
				   }//switch (ItemMenu1)
				   if((ItemMenu1==9)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;
				   InfSTablo1=9; SendInfToTablo1(); InfSTablo2=0; SendInfToTablo2();
                   BtnPushed=-1; if(MenuUr < 0)MenuUr=0;
				 }
			   } break;
/* ================================================================================= */
        case 4: { // подменю УПРАВЛЕНИЕ
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ InfSTablo1=41; SendInfToTablo1();
				  ItemMenu1=1; MenuUr=1;
                  InfSTablo2=9; SendInfToTablo2(); // максимальный угол наклона
                  BtnPushed=-1; PrNoBtnWait=1; //PrEnterStart=1;
                }
				switch (ItemMenu1)
					{  case 1: // максимальный угол наклона бланка MaxAngle
					   { if(BtnPushed==0)
						 {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=79; SendInfToTablo1();
                            InfSTablo2=49; SendInfToTablo2();
                            BtnPushed=-1; //PrNoBtnWait=1; PrEnterStart=1;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{  // считывание параметра
							   get_KeyString(NUMBER);
							   MaxAngle=ReadParFromTablo2()/57.3;
							   InfSTablo2=9; SendInfToTablo2(); // максимальный угол наклона
							   //PrEnterStart=0;
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 2: // изменение языка
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=42; SendInfToTablo1();
                             InfSTablo2=10; SendInfToTablo2();

						  } else
						  { if(BtnPushed==1)
							{  // считывание параметра
							   if(Lang==0)Lang=1; else Lang=0;
                               InfSTablo2=49; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;

						case 3: // максимальный угол наклона бланка MaxAngle
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=43; SendInfToTablo1();
                             InfSTablo2=11; SendInfToTablo2();

						  } else
						  { if(BtnPushed==1)
							{  // считывание параметра
							   if(PrCheckSh==0)PrCheckSh=1; else PrCheckSh=0;
                               InfSTablo2=10; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 4: // максимальный угол наклона бланка MaxAngle
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=44; SendInfToTablo1();
                             InfSTablo2=12; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{  // считывание параметра
							   if(PrCheckBr==0)PrCheckBr=1; else PrCheckBr=0;
                               InfSTablo2=11; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 5: // максимальный угол наклона бланка MaxAngle
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=40; SendInfToTablo1();
						  } else
						  { if(BtnPushed==1)
							{  // считывание параметра
							   if(PrCheckEx==0)PrCheckEx=1; else PrCheckEx=0;
                               InfSTablo2=12; SendInfToTablo2();							}
						  }
						  BtnPushed=-1;
						}; break;
						case 6: // сохранение  параметров
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1; BtnPushed=1;// для выхода из меню
							 // выдача сообщения о следующем параметре
							 InfSTablo1=40; SendInfToTablo1();
						  } else
						  { if(BtnPushed==1)
							{  // Сброс параметров (считывание из файла начальных параметров)
							   SetUpUprWrite();
							   SetUpRecWrite();
							   ItemMenu1=ItemMenu1+1; BtnPushed=1;// для выхода из меню
							 // выдача сообщения о следующем параметре
							 InfSTablo1=40; SendInfToTablo1();
							}
						  }
						}; break;
					}//switch (ItemMenu1)
				   if((ItemMenu1==7)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;//MenuUr-1;
				   InfSTablo1=10; SendInfToTablo1();
				   InfSTablo2=0; SendInfToTablo2();
				   BtnPushed=-1; if(MenuUr < 0)MenuUr=0;
    LoadInd();

				 }
			   } break;
/* ================================================================================= */
        case 5: { // подменю ПОВТОР ПЕЧАТИ
                    // сделать выдачк у на  печать
                    // 1. Исходных данных
                    // 2. Результатов  тестирования
                    // 3. Результатов  голосования по всем  выборам


			   } break;
/* ================================================================================= */
        case 6: { // подменю СБРОС ПАРАМЕТРОВ
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ InfSTablo1=45; SendInfToTablo1();
                  // ввод  начальных  значений из  файлов инициализации
                  SetUpKoibRead();
                  SetUpUprRead();
                  SetUpRecRead();
				 // возврат  в  меню
				 ItemMenu1=0; InfSTablo1=12; SendInfToTablo1();
				 BtnPushed=-1;
				}
			   } break;
/* ================================================================================= */
        case 7: { // подменю Сброс ИД
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ InfSTablo1=46; SendInfToTablo1();
				  ItemMenu1=1; MenuUr=1;
				}
				if(ItemMenu1==1)
				{ // если меню выбрано
                  if(BtnPushed==0)
				  {  ItemMenu1=ItemMenu1+1; BtnPushed=1;// для выхода из меню
				  } else
				  { if(BtnPushed==1)
					{  // Удаление исходных данных
					  InfSTablo1=47; SendInfToTablo1();
//!!! функция                 // GashId();

                      InfSTablo1=48; SendInfToTablo1();// сброшены
					  ItemMenu1=ItemMenu1+1;
					}
				  }
				  if((ItemMenu1==2)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;//MenuUr-1;
				   InfSTablo1=13; SendInfToTablo1();
                    InfSTablo2=0; SendInfToTablo2();
                    BtnPushed=-1; if(MenuUr < 0)MenuUr=0;
				 }
				}
			   } break;
/* ================================================================================= */
        case 8: { // подменю ВВОД ДАТЫ
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ // определение  текущей даты и времени
                        time(&timer);   TekTime=localtime(&timer);
                        MyTime.tm_year=TekTime->tm_year+1900; MyTime.tm_mon=TekTime->tm_mon+1; MyTime.tm_mday=TekTime->tm_mday;
                        MyTime.tm_hour=TekTime->tm_hour; MyTime.tm_min=TekTime->tm_min; MyTime.tm_sec=TekTime->tm_sec;
                        InfSTablo1=49; SendInfToTablo1();
                        ItemMenu1=1;  MenuUr=1;
                        //cout <<  " Menu Level for  date enter" << endl;
                        InfSTablo2=13; SendInfToTablo2(); //  выдача даты  и времени
                        BtnPushed=-1;
				}
				switch (ItemMenu1)
					{  case 1: // запрос  на  изменение даты
					   { if(BtnPushed==0)
						 {      //  выход из установки даты даты и времени
                                ItemMenu1=9; BtnPushed=1;
                                //BtnPushed=-1; //if(MenuUr < 0)MenuUr=0;
						 } else
						 { if(BtnPushed==1)
						   {    ItemMenu1=ItemMenu1+1;
                                // формирование сообщения об изменении даты
                                InfSTablo1=50; SendInfToTablo1();
                                //usleep(300000);
                                InfSTablo2=14; SendInfToTablo2(); // выдача дня
                                BtnPushed=-1; PrNoBtnWait=1; PrEnterStart=0;
						   }
                         }
						}; break;
						case 2: // выход из установки даты
						{ if(BtnPushed==0)
						  {     ItemMenu1=ItemMenu1+1;
                                // выдача сообщения о следующем параметре
                                InfSTablo1=51; SendInfToTablo1();
                                InfSTablo2=15; SendInfToTablo2(); //  выдача месяца
                                PrEnterStart=0;
						  } else
						  { if((BtnPushed==1))//||(PrEnterStart==1))
						    {   InfSTablo2=0; SendInfToTablo2(); // гашение табло 2 для набора данных
                                get_KeyString(NUMBER);
                                MyTime.tm_mday=int(ReadParFromTablo2());
                                InfSTablo2=14; SendInfToTablo2(); // выдача дня
                                PrEnterStart=0;
						    }
                          }
						}; break;
						case 3: // установка месяца
						{ if(BtnPushed==0)
						  {     ItemMenu1=ItemMenu1+1;
                                // выдача сообщения о следующем параметре
                                InfSTablo1=52; SendInfToTablo1();
                                InfSTablo2=16; SendInfToTablo2(); // выдача года
                                BtnPushed=-1; //PrNoBtnWait=1; PrEnterStart=1;
                                PrEnterStart=0;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{  // считывание месяца
							    InfSTablo2=0; SendInfToTablo2(); // гашение табло 2 для набора данных
                                get_KeyString(NUMBER);
                                MyTime.tm_mon=ReadParFromTablo2();
                                InfSTablo2=15; SendInfToTablo2(); //  выдача месяца
                                PrEnterStart=0;
							}
						  }
						}; break;
						case 4: // установка года
						{ if(BtnPushed==0)
						  {     ItemMenu1=ItemMenu1+1;
                                // выдача сообщения о следующем параметре
                                InfSTablo1=53; SendInfToTablo1();
                                InfSTablo2=17; SendInfToTablo2();// выдача часов
                                BtnPushed=-1; //PrNoBtnWait=1;
                                PrEnterStart=0;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{  // считывание года
							     InfSTablo2=0; SendInfToTablo2(); // гашение табло 2 для набора данных
							    get_KeyString(NUMBER);
                                MyTime.tm_year=ReadParFromTablo2();
                                InfSTablo2=16; SendInfToTablo2(); // выдача года
                                PrEnterStart=0;
							}
						  }
						}; break;
						case 5: //  время :  часы
						{ if(BtnPushed==0)
						  {     ItemMenu1=ItemMenu1+1;
                                // выдача сообщения о следующем параметре
                                InfSTablo1=54; SendInfToTablo1();
                                InfSTablo2=18; SendInfToTablo2(); // выдача минут
                                BtnPushed=-1; //PrNoBtnWait=1;
                                PrEnterStart=0;
						  } else
						  {
						     if((BtnPushed==1))//||(PrEnterStart==1))
							{  // считывание часа
							     InfSTablo2=0; SendInfToTablo2(); // гашение табло 2 для набора данных
							    get_KeyString(NUMBER);
                                MyTime.tm_hour=ReadParFromTablo2();
                                InfSTablo2=17; SendInfToTablo2(); // выдача года
                                PrEnterStart=0;
							}
						  }
						}; break;
						case 6: // ввод  минут
						{ if(BtnPushed==0)
						  {     ItemMenu1=ItemMenu1+1;
                                InfSTablo1=55; SendInfToTablo1();
                                InfSTablo2=13; SendInfToTablo2();
                                BtnPushed=-1; //PrNoBtnWait=1;
                                PrEnterStart=0;
						  } else
						  {  if((BtnPushed==1))//||(PrEnterStart==1))
							{  // установка  введенного значения  времени
							    InfSTablo2=0; SendInfToTablo2(); // гашение табло 2 для набора данных
							    get_KeyString(NUMBER);
/*  считывание минут  */	    MyTime.tm_min=ReadParFromTablo2();
                                InfSTablo2=18; SendInfToTablo2(); // выдача минут
                                PrEnterStart=0;
							}
						  }
						}; break;
						case 7: // сохранение
						{ if(BtnPushed==0)
						  {     ItemMenu1=ItemMenu1+1;// для проверки  введенного времени
                                BtnPushed=1; // для перехода к  меню
                                InfSTablo1=56; SendInfToTablo1();
                                InfSTablo2=13; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{  // сохранение новой даты и времени
                                    ItemMenu1=ItemMenu1+1;
                                    InfSTablo1=57; SendInfToTablo1();
                                    char sst[40],ss[128];
                                    sprintf(sst,"\"%04d%02d%02d %02d:%02d\"",MyTime.tm_year,MyTime.tm_mon,MyTime.tm_mday,MyTime.tm_hour,MyTime.tm_min);
                                    strcpy(ss,"date --set ");
                                    strcat(ss,sst);
                                    // установка  системного  времени
                                    system(ss);
                                    // установка  времени на  аппаратуре
                                    strcpy(ss,"hwclock -w");     system(ss);
                                    InfSTablo2=13; SendInfToTablo2(); // выдача даты и времени
							}
						  }
						}; break;
					}//switch (ItemMenu1)
                    if((ItemMenu1==9)&&(BtnPushed==1))
                    { // возвращение  в  меню предыдущего уровня
                        ItemMenu1=0; MenuUr=0;//MenuUr-1;
                        InfSTablo1=14; SendInfToTablo1();
                        InfSTablo2=0; SendInfToTablo2();
                        BtnPushed=-1;
                    }
                    if((ItemMenu1==8)&&(BtnPushed==1)) ItemMenu1=9;
			   } break;
/* ================================================================================= */
        case 9: { // подменю ИЗГОТОВИТЕЛЬ
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ // заводские настройки
                    InfSTablo1=58; SendInfToTablo1();
                    ItemMenu1=1;  MenuUr=1;//MenuUr+1;
                    BtnPushed=-1;
                }
				switch (ItemMenu1)
					{  case 1: // серийный номер
					   { if(BtnPushed==0)
						 {  //  выход из  данного  пункта меню
							ItemMenu1=5; BtnPushed=1;
						 } else
						 { if(BtnPushed==1)
						   {    ItemMenu1=ItemMenu1+1;
                                // выдача серийного  номера
                                InfSTablo1=59; SendInfToTablo1();
                                InfSTablo2=19; SendInfToTablo2();// серийный номер ведущего
                                BtnPushed=-1; //PrNoBtnWait=1; PrEnterStart=1;
						   }
						 }
						 //BtnPushed=-1;
						}; break;
						case 2: // IP   ведущий
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=60; SendInfToTablo1();
                             InfSTablo2=20; SendInfToTablo2(); // IP  ведущего
                             BtnPushed=-1; //PrNoBtnWait=1; PrEnterStart=1;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{ // ввод серийного номера ведущего
							  get_KeyString(NUMBER);
							  *SerNumKoib=*NUMBER;
                                InfSTablo2=19; SendInfToTablo2();// серийный номер ведущего
							  PrEnterStart=0;
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 3: // IP   ведомый
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=61; SendInfToTablo1();
							 //usleep(300000);
                             InfSTablo2=21; SendInfToTablo2(); // IP  ведомого
                             BtnPushed=-1; //PrNoBtnWait=1; PrEnterStart=1;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{  //  ввод IP - ведущего сканера
							    get_KeyString(NUMBER);
							    *Ip_master=*NUMBER;
                                InfSTablo2=20; SendInfToTablo2(); // IP  ведущего
                                PrEnterStart=0;
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 4: //
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 BtnPushed=1; // для перехода к  меню
							 //PrEnterStart=1;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{  // ввод IP - ведомого сканера
							  get_KeyString(NUMBER);
							   *Ip_slave=*NUMBER;
                                    InfSTablo2=21; SendInfToTablo2(); // IP  ведомого
							   PrEnterStart=0;
							}
						  }
						}; break;
				}//switch (ItemMenu1)
				if((ItemMenu1==5)&&(BtnPushed==1))
				 {  // возвращение  в  меню предыдущего уровня
                        ItemMenu1=0; MenuUr=0;
                        InfSTablo1=15; SendInfToTablo1();   InfSTablo2=0; SendInfToTablo2();
                        BtnPushed=-1;
				 }
			   } break;
/* ================================================================================= */
        case 10: {// подменю ГРОМКОСТЬ
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ // выдача параметров уровня громкости
				  if(ItemMenu1==0)
				  {    ItemMenu1=1;   MenuUr=1;  InfSTablo1=62;  SendInfToTablo1();
                        InfSTablo2=22;  SendInfToTablo2();   //  выдача уровня  громкости
                        BtnPushed=-1;
				  } else
				  {     ItemMenu1=2;
                        InfSTablo1=66; SendInfToTablo1();   InfSTablo2=0; SendInfToTablo2();    //  выдача уровня  громкости
                        BtnPushed=1;
				  }
				}
				if(BtnPushed==2) //нажата кн. ДА+НЕТ
				{ // выдача параметров уровня громкости
                        ItemMenu1=2;
                        InfSTablo1=66;  SendInfToTablo1();
                        InfSTablo2=0;    SendInfToTablo2();//  гашение второй строки
                        BtnPushed=1;
				}
				switch (ItemMenu1)
					{  case 1: //
					    { if(BtnPushed==0)
						 {  //  уменьшение  уровня  громкости
							if(VolumeUr > 0)
							{ VolumeUr=VolumeUr-1;
                              InfSTablo2=22; SendInfToTablo2(); // выдача уровня  громкости на  табло
                              setVoiceVolume(VolumeUr);  SayVotes(1);  // здравствуйте
                            }
						 } else
						 { if(BtnPushed==1)
						   { // увеличение  уровня  громкости
                             if(VolumeUr < VolumeUrMax)
							 { VolumeUr=VolumeUr+1;
                               InfSTablo2=22; SendInfToTablo2(); // выдача уровня  громкости на  табло
                                setVoiceVolume(VolumeUr); SayVotes(1);  // здравствуйте
                             }
						   }
						}
						}; break;
				}//switch (ItemMenu1)
				if((ItemMenu1==2)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;
				   InfSTablo1=16; SendInfToTablo1();
				   // установка  уровня громкости динамиков
                    setVoiceVolume(VolumeUr);
				   InfSTablo2=0; SendInfToTablo2();
				   BtnPushed=-1; //if(MenuUr < 0)MenuUr=0;
				 }
			   } break;
/* ================================================================================= */
        case 11: {// подменю Log- файлы и образы
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ InfSTablo1=63; SendInfToTablo1();
				  ItemMenu1=1; MenuUr=1;//MenuUr+1;
				}
				switch (ItemMenu1)
					{  case 1: // печать Log - файлов
					   { if(BtnPushed==0)
						 {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=64; SendInfToTablo1();
                            InfSTablo2=0; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{  // печать Log - файлов
/*!!! функция */               // PrintLogFiles();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 2: // запись Log - файлов  на  флэш
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
                           InfSTablo2=0; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{  // запись Log - файлов  на  флэш
                                frontLED(2,0); // включение красной  лампочки
                                system("mkdir /mnt/usb/Log"); // создание папки на  флэшкарте
/*!!! функция */                system("cp -a Log /mnt/usb/Log"); // копирование log - файлов из  папки Log на флэшку
                                frontLED(0,2); // включение  зеленой лампочки
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 3: // запись  образов  на  флэш
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=66; SendInfToTablo1();
                            InfSTablo2=0; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{  // запись образов на  флэш
							    frontLED(2,0); // включение красной  лампочки
                                system("mkdir  /mnt/usb/Image"); // создание папки на  флэшкарте
/*!!! функция */                system("cp -a /www/pages  /mnt/usb/Image");
                                frontLED(0,2); // включение  зеленой лампочки
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 4: // выход
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;  BtnPushed=1;

						  } else
						  { if(BtnPushed==1)
							{  // переход дальше
							   ItemMenu1=ItemMenu1+1;
							}
						  }
						}; break;
					}//switch (ItemMenu1)
				   if((ItemMenu1==5)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;//MenuUr-1;
				   InfSTablo1=17; SendInfToTablo1();
                   InfSTablo2=0; SendInfToTablo2();
				   BtnPushed=-1; //if(MenuUr < 0)MenuUr=0;
				 }
			   } break;
/* ================================================================================= */
        case 12: { // подменю Информация о сканере
				if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{ InfSTablo1=67; SendInfToTablo1();// данные по ведущему
                  InfSTablo2=23; SendInfToTablo2();
				  ItemMenu1=1; MenuUr=1;//MenuUr+1;
				}
				switch (ItemMenu1)
					{  case 1: //  Главный сканер
					   { if((BtnPushed==0)||(BtnPushed==1))
						 {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=68; SendInfToTablo1();// данные по ведомому, серийный номер
						     InfSTablo2=24; SendInfToTablo2(); //серийный номер сканера ведомого
						  }
						  BtnPushed=-1;
						}; break;
						case 2: // Ведомый сканер
						{ if((BtnPushed==0)||(BtnPushed==1))
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=69; SendInfToTablo1();// время работы сканера
                             InfSTablo2=25; SendInfToTablo2();
						  }
						  BtnPushed=-1;
						}; break;
						case 3: // Время работы
						{ if((BtnPushed==0)||(BtnPushed==1))
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=66; SendInfToTablo1();
                            InfSTablo2=0; SendInfToTablo2();
						  }
						  BtnPushed=-1;
						}; break;
						case 4: // выход
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;  BtnPushed=1; } else
						  { if(BtnPushed==1)// считывание параметра
							{ ItemMenu1=ItemMenu1+1;}
						  }
						}; break;
					}//switch (ItemMenu1)
				   if((ItemMenu1==5)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;//MenuUr-1;
				   InfSTablo1=18; SendInfToTablo1();
				   InfSTablo2=0; SendInfToTablo2();
				   BtnPushed=-1;
				 }
			   } break;
/* ================================================================================= */
        case 13: { // подменю ТЕСТЫ
				if((BtnPushed==3)&&(ItemMenu1==0)) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
				{
                  MenuUr=1;
				  // формирование пути к  файлу
                  FindSourceFile();
                  if (! Parser(FileName))
                  { // нет файла  исходных данных
                    ItemMenu1=2;
                    InfSTablo1=71; SendInfToTablo1();
					InfSTablo2=27; SendInfToTablo2();
                  } else
                  {  // файл  данных найден
                     ItemMenu1=1;
                     InfSTablo2=26; SendInfToTablo2(); // вставьте  лист
                     // формирвоание исходных данных по выборам
                     FormIdScan();
                  }
                  UMount("/mnt/usb");
                  BtnPushed=-1;
                }
				switch (ItemMenu1)
					{  case 1: // тестирование  сканера
					   { if(BtnPushed==0)
						 {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							  InfSTablo1=71; SendInfToTablo1();
							  InfSTablo2=27; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{   // тестирование  сканера
							    // гашение  массивов сканирования  бюллетеня
                                GashBeforeScan();
                                calibrate_mode= MODE_NORMAL;
                                frontLED(2,0);   // включение красной лампочкиидет процесс сканирования и обработки
                                TestRecogn(); // сканирование с  распознаванием
								InfSTablo2=26; SendInfToTablo2();// вставьте лист
								frontLED(0,2);// увеличение интенсивности зеленой  лампочки и отключение красной
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 2: // тестирование  кнопок
						{ if(BtnPushed==3) // выход
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
                             InfSTablo2=31;  SendInfToTablo2();
							 InfSTablo1=72; SendInfToTablo1();
							  sleep(1);
							  InfSTablo2=0; SendInfToTablo2();// гашение
						  } else
						  {    if(BtnPushed==0) InfSTablo2=29; if(BtnPushed==1) InfSTablo2=30;
							   if(BtnPushed==3) InfSTablo2=31; if(BtnPushed==4) InfSTablo2=32;
							   if(BtnPushed==5) InfSTablo2=33;
                                SendInfToTablo2();
						  }
						  BtnPushed=-1;
						}; break;
						case 3: // тестирование флэш-карты
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=66; SendInfToTablo1();
							 InfSTablo2=0; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{  // тестирование флэш-карты
                                 int PrIdError=FindSourceFile();
                                 if(PrIdError==-1)
                                 {   InfSTablo2=35; SendInfToTablo2();// неисправна
                                 } else
                                 {   InfSTablo2=34; SendInfToTablo2();// исправна
                                 }
                                 UMount("/mnt/usb");
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 4: // выход
						{ if((BtnPushed==0)||(BtnPushed==1))	  {  ItemMenu1=ItemMenu1+1;  BtnPushed=1; }
						}; break;
					}//switch (ItemMenu1)
				   if((ItemMenu1==5)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;//MenuUr-1;
				   InfSTablo1=19; SendInfToTablo1();
				   InfSTablo2=0; SendInfToTablo2();
				   BtnPushed=-1; //if(MenuUr < 0)MenuUr=0;
				 }
			   } break;
/* ================================================================================= */
        case 14: { // подменю Сканирование  образа и его сохранение
				if(BtnPushed==3) //нажата кн. ВЫБОР
				{ InfSTablo1=75; SendInfToTablo1();// данные по ведущему
                  InfSTablo2=45; SendInfToTablo2();// нажмите ДА, вставьте бюллетень
				  ItemMenu1=1; MenuUr=1;//MenuUr+1;
                  BtnPushed=-1;
                }
				switch (ItemMenu1)
					{  case 1: // тестирование  сканера
					   { if(BtnPushed==0)
						 {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							  InfSTablo1=66; SendInfToTablo1();
							  InfSTablo2=0; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{   // сканирование образа
                                calibrate_mode= MODE_NORMAL;
                                frontLED(2,0);   // включение красной лампочкиидет процесс сканирования и обработки
                                TestScan();
								InfSTablo2=45; SendInfToTablo2();// вставьте лист
								frontLED(0,2);// увеличение интенсивности зеленой  лампочки и отключение красной
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 2: // выход
						{ if((BtnPushed==0)||(BtnPushed==1)){  ItemMenu1=ItemMenu1+1;  BtnPushed=1; }
						}; break;
					}//switch (ItemMenu1)
				   if((ItemMenu1==3)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;//MenuUr-1;
				   InfSTablo1=20; SendInfToTablo1();  InfSTablo2=0; SendInfToTablo2();   BtnPushed=-1;
				 }
			   } break;
/* ================================================================================= */
        case 15: { // подменю КОДЫ  ДОСТУПА
                    if(BtnPushed==3) //нажата кн. ВЫБОР (кнопка 1 на КОИБ)
                    {   InfSTablo1=76; SendInfToTablo1(); // Код администратора
                        ItemMenu1=1; MenuUr=1;
                        InfSTablo2=46; SendInfToTablo2(); // текущее значение  кода  администратора
                        BtnPushed=-1; PrNoBtnWait=1; //PrEnterStart=1;
                    }
                    switch (ItemMenu1)
					{  case 1: // код доступа  администратора
					   { if(BtnPushed==0)
						 {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=77; SendInfToTablo1();
                            InfSTablo2=47; SendInfToTablo2();
                            BtnPushed=-1; //PrNoBtnWait=1; PrEnterStart=1;
						  } else
						  {
						    if((BtnPushed==1))//||(PrEnterStart==1))
							{  // считывание параметра
							   get_KeyString(NUMBER);
							   CodesOp[0]=ReadParFromTablo2();
                                InfSTablo2=46; SendInfToTablo2();
							   PrEnterStart=0;
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 2: // код  доступа  технического  специалиста
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // выдача сообщения о следующем параметре
							 InfSTablo1=78; SendInfToTablo1();
                             InfSTablo2=48; SendInfToTablo2();

						  } else
						  { if(BtnPushed==1)
							{  // считывание параметра
							   get_KeyString(NUMBER);
							   CodesOp[1]=ReadParFromTablo2();
                                InfSTablo2=47; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 3: // код доступа  оператора УИК
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1;
							 // запрос на сохранение  изменений кодов  доступа
							 InfSTablo1=40; SendInfToTablo1();
                             InfSTablo2=0; SendInfToTablo2();
						  } else
						  { if(BtnPushed==1)
							{  // считывание параметра
							   get_KeyString(NUMBER);
							   CodesOp[2]=ReadParFromTablo2();
                                InfSTablo2=48; SendInfToTablo2();
							}
						  }
						  BtnPushed=-1;
						}; break;
						case 4: // сохранение  параметров
						{ if(BtnPushed==0)
						  {  ItemMenu1=ItemMenu1+1; BtnPushed=1;// для выхода из меню
							 // выдача сообщения о следующем параметре
							 InfSTablo1=40; SendInfToTablo1();
						  } else
						  { if(BtnPushed==1)
							{  // Сохранение  новых  кодов  в  файл
                                FILE *out;
                                if ((out = fopen("./SetUpFiles/CodesOpFile.txt", "wt"))!= NULL)
                                {  // ввод кодов  операторов  из  файла
                                   	fprintf(out,"%d\n", CodesOp[0]);    fprintf(out,"%d\n", CodesOp[1]);        fprintf(out,"%d\n", CodesOp[2]);
                                    fclose(out);
                                } else cout << "Ошибка открытия файла  для  записи" << endl;
                              ItemMenu1=ItemMenu1+1; BtnPushed=1;// для выхода из меню
							}
						  }
						}; break;
					}//switch (ItemMenu1)
				   if((ItemMenu1==5)&&(BtnPushed==1))
				 { // возвращение  в  меню предыдущего уровня
				   ItemMenu1=0; MenuUr=0;//MenuUr-1;
				   InfSTablo1=10; SendInfToTablo1();
				   InfSTablo2=0; SendInfToTablo2();
				   BtnPushed=-1; if(MenuUr < 0)MenuUr=0;
				 }
//  пока  не  ясно, нужно  это  или  нет
//  позже, если понадобится, доделаем

			   } break;
/* ================================================================================= */
        case 16:
			   { // подменю ВЫХОД
				 if(BtnPushed==3)
				 {  ItemMenu0=0; PrMenuBtn=-1;KodOp=-1;  KodDost=-1;
				    MenuUr=0; PrNoBtnWait=1; PrModeChange=1;
					//if(MenuUr>0)MenuUr=MenuUr-1;
					strcpy(InfS,"");
					pr_out=27;
				 }
			   } break;
		   } // switch
/* ================================================================================= */
		   //finMenu2:;
		 }
		 finMenu1:;
	  }
	  finMenu:;
   }
   // гашение признака нажатой  кнопки
   BtnPushed=-1;
   // освобождение  динамической  переменной
   free(ss);
   return pr_out;
}
