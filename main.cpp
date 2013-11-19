#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>


#include <sys/soundcard.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


#include "HeaderMain.h"
#include "UnitKoibScan.h"
#include "UnitServiceMenu.h"
#include "Ruler.h"

#include "InitAltera.h"
#include "CreateImage.h"
#include "HardProc.h"
#include "printer.h"
#include "AlteraSocket.h"

using namespace std;
int Pr_Event=0;
int main()
{
    Start(true);
    int ch=0, PrBklKoib=0;
//**********************************************
    // ������� ����������� ����������
    std::string st1;     std::string st2;
//char *sst1;
    //char sst1[128],sst2[128];
    PrNoBtnWait=0; // ������� �������� ��������  ������
    Exit=false; // �����
    PrVoteTimeStart=0;      // ������� �����������  ������� ����������� ( � ���� �����������)
    PrVoteTimeEnd=0;        // ������� ���������  �������  ����������� ( � ���� �����������)
    PrVoteDay=0;                    // ������� "���� �����������"
    Koib[1].VS = 1;                  // ��������� ����������  ��������� ����
    PrKlav=0;                             //  ������� ������� ����������

    Version=0;                           // ����� ������ ���������!
    PrConnectOK=0;             // ������� ���������  ����� ��  ������  ��������
    Init_Altera();                    // �������������  ������
    ReadSetUpFiles();  // ���������� ������������ ������ (��������� ������)
    Create_Threads();          //  ������������� �������
    //Lang=1;// ��������� ����������� ����� ( ���  �������)
    LoadInd();// �������� ���������  � ����� ��������� ������ �  �������

    FILE *fw;
    if ((fw = fopen("./SetUpFiles/CodesOpFile.txt", "r"))!= NULL)
    {  // ���� �����  ����������  ��  �����
            fscanf(fw,"%d",&CodesOp[0]);            fscanf(fw,"%d",&CodesOp[1]);            fscanf(fw,"%d",&CodesOp[2]);
            fclose(fw);
    } else cout << "�� ������ ���� CodesOpFile.txt" << endl;
     ReadSetUpFiles(); // ����������  ���������  ������
    do
    {   // ��������  ���� ��������� ����������
            ch=0;
            //if(KeyisReady)PrKlav=1; else PrKlav=0; // ���������� � AnyWait 15.10.2013
            /* !!!!*/        //PrKlav=1;
            Exit=0;
            if(PrBklKoib==0) goto LabRun; // ����������� �������  ��������� ����
//******************������  ����������� ��������� �������**************
            // ������� ������
        StartCycle:
            for(i=0;i<6;i++) Buttons[i]=0;
            BtnPushed=-1; pr_btn=-1;   pr_opt=-1; PrUprOpt=-1;Pr_Event=0;
           if (PrNoBtnWait==0) Pr_Event= Any_Wait() ; // �������� �����-���� �������; ������� 1 -�������, 2- ������, 3- �����           PrNoBtnWait=0;
           PrNoBtnWait=0;
           switch (Pr_Event)
           { case 1:
              {  // �������  ���������  ���������  ��������
                 PrUprOpt=0;
                 //get_Optrons_Imm();
                 for(int i=0;i<4;i++)
                 { if(Optron[i] < ONE_PAPERO[i]) { PrUprOpt=100; goto MetEnd;}
                   else
                   {  // ������������ ��������� ��������� �������  �������
                        if( Optron[i] < NO_PAPERO[i]) Sost_Optrons[i]=1; else Sost_Optrons[i]=0;
                   }
                    //cout << " Optron  N =      "<< i << "     Value =   "<<  Optron[i] <<  "     Sostoyanie =    " << Sost_Optrons[i]<< endl;
                 }
                 // ������������ �������� ���������� PrUprOpt///
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=0; //  ����� ������
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=1; // ������ �������
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=2; // ����������� �������� ���� ( �����)
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=3; // �������� - ����������� �������� ���� ( �����) ��� ������ �������� �������
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=4;    // ��������� "��������  ���� �����
/*���3*/ if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=5;    // ��������� "��������  ���� �����
// 6 - ���� �� ������ ���� ��������, ����� �������� ����������� ���������
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=6; // ������ ������������
// 7 - ����  �  ������, ���� ������������
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=7;    // �� �������� 1-2-3  ���������  �������� ������������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=8;    // ����� ������������, ���������� ���������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=9;    // ����� ������������, ��������� ��������� ���  �����������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=10;    // �� �������� 0  � 2  ���������  ����,������� �������� ����, ����� �� ��������� ������ � ����������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=11;    // �� �������� 0  � 2 � 3 ���������  ����,������� �������� ����, ����� �� ��������� ������ � ����������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=12;    //�� �������� 0  � 1 ���������  ����, ������� �������� ����, ����� �� ��������� ������ � ����������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=13;    // �� �������� 0  � 1 �  ���������  ����,������� �������� ����, ����� �� ��������� ������ � ����������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=14;    //�� �������� 0  � 1 � 2 ���������  ����, ������� �������� ����, ����� �� ��������� ������ � ����������
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=15;    //�� �������� 0  � 1 � 2 � 3 ���������  ����, ����  �  ������
                 if((PrUprOpt==2)||(PrUprOpt==4))
                 {   //  ���� ���������� ������ ���� ������, �� �������� �������� ����� ��������
                     usleep(25000); goto StartCycle;
                 }
                 //  ��� ��������� ������
                 pr_opt=PrUprOpt;
                 MetEnd:;
              }    break;
             case 2:
              {  // ����  ������� �������  ������
                 //if(PrNoBtnWait==0) get_Buttons_Wait();   PrNoBtnWait=0;
                 if(Buttons[0]>0)BtnPushed=1;    if(Buttons[1]>0)BtnPushed=0;
                 if(Buttons[2]>0)BtnPushed=3;    if(Buttons[3]>0)BtnPushed=3;
                 if(Buttons[4]>0)BtnPushed=3;    if(Buttons[5]>0)BtnPushed=5;
                 if((Buttons[0]>0)&&(Buttons[1]>0))BtnPushed=2;
                 if((PrKlav==1)&&(PrMenuBtn < 1))
                 {  // ������  ��. ����
                    if(BtnPushed==5) //  ������ ��. ����
                    {
                       PrMenuBtn=1;  KodOp=-1; KodDost=-1;   BtnPushed=-1;
                    }
                 }
                 pr_btn=BtnPushed;
              }    break;
             case 3:
              { // ������� ��������� � ����������� �� ����������

              }    break;
           }
//        **************�����  ����������� ��������� �������**********
            LabRun:; //  ����� , �������������� ������ �������� ��������� ����� ���������

            if(PrMenuBtn<1)
            {  // �������  �����  ����
                calibrate_mode= MODE_NORMAL;
                Exit=RUN();
                if(PrNoBtnWait==0) PrModeChange=0;
            } else
            {  // ������  �  ���������  ����
                ch=ServMenu();
            }
           PrBklKoib=1;
    } while ((ch!=27)||(!Exit));
    Start(false);
    cout << "  KOIB Work has been finished!" << endl;
    return 0;
}// main
