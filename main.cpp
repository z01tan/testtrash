#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

using namespace std;
int Pr_Event=0;
int main()
{
    Start(true);
    int ch=0, PrBklKoib=0;
//**********************************************
    PrNoBtnWait=0; // признак пропуска ожидания  кнопки

   // system("cat ./Wav/BULL_NUF.wav > /dev/dsp"); //dsp

    Init_Altera();           // инициализация  железа
    Create_Threads(); //  инициализация потоков
     Koib[1].VS = 1; Exit=false;
    PrKlav=0;  //  отладочный  режим до подключения  клавиатуры
    //Lang=1;     // установка английского языка ( для  отладки)
    Version=0; // Номер версии сообщений!

    LoadInd(); // загрузка сообщений из  файлов в  массивы

    //  ввод кодов доступа из файла
    FILE *fw;
    if ((fw = fopen("./SetUpFiles/CodesOpFile.txt", "r"))!= NULL)
    {  // ввод кодов  операторов  из  файла
            fscanf(fw,"%d",&CodesOp[0]);
            fscanf(fw,"%d",&CodesOp[1]);
            fscanf(fw,"%d",&CodesOp[2]);
             fclose(fw);
    } else cout << "Не найден файл CodesOpFile.txt" << endl;

     ReadSetUpFiles();

    Exit=0; //PrBklKoib=1;
    PrVoteTimeStart=0;
    PrVoteTimeEnd=0;
    PrVoteDay=0;
     PrConnectOK=0;// признак установки  связи со  вторым  сканером
    do
    {
        ch=0;
        if(KeyisReady)PrKlav=1; else PrKlav=0;

/* !!!!*/        PrKlav=1;

  /*      if(((PrKlav==1)&&(PrMenuBtn < 0))||(Exit))
        {  // выдача сообщения о выборе режима, если подключена клавиатура
                    PrMenuBtn =0; //PrBklKoib=1;
                    InfSTablo1=1; SendInfToTablo1(); usleep(500000);
                    InfSTablo2=0; SendInfToTablo2();
        }
        */
        Exit=0;
        if(PrBklKoib==0) goto LabRun; // определение момента  включения КОИБ

//******************НАЧАЛО  СУПЕРВИЗОРА ОБРАБОТКИ СОБЫТИЙ**************
            // гашение кнопок
            for(i=0;i<6;i++) Buttons[i]=0;
            BtnPushed=-1; pr_btn=-1;
            pr_opt=-1; PrUprOpt=-1;
            // ожидание нажатия кнопок
                 //for(int i=0;i<4;i++)     Optron[i]=0;
           //get_Optrons_Imm();

           if (PrNoBtnWait==0) Pr_Event= Any_Wait() ; // ожидание каког-либо события; возврат 1 -оптроны, 2- кнопки, 3- клава           PrNoBtnWait=0;
           else
           {  // проверка нажатия  кнопок
               //get_Buttons_Imm();  Pr_Event=2;
               //if(PrConnectOK==1)Buttons[1]=1;
           }
           PrNoBtnWait=0;
           switch (Pr_Event)
           { case 1:
              {  // событие  изменения  состояния  оптронов
                 PrUprOpt=0;
                 //get_Optrons_Imm();
                 for(int i=0;i<4;i++)
                 { if(Optron[i] < ONE_PAPER) { PrUprOpt=100; goto MetEnd;}
                   else
                   {  // формирование признаков состояния каждого  оптрона
                      if( Optron[i] < NO_PAPER) Sost_Optrons[i]=1; else Sost_Optrons[i]=0;
                    cout << " Optron  N =      "<< i << "     Value =   "<<  Optron[i] <<  "     Sostoyanie =    " << Sost_Optrons[i]<< endl;
                   }
                 }
                 // формирование признака управления PrUprOpt///
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=0; //  тракт пустой
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=1; // ошибка оптрона
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=2; // Неправильно вставлен лист ( криво)
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=3; // Возможно - Неправильно вставлен лист ( криво) при ошибке среднего оптрона
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=4;    // Сообщение "Вставьте  лист ровно
/*как3*/ if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=5;    // Сообщение "Вставьте  лист ровно
// 6 - лист на первых дыух оптронах, можно начинать сканировать бюллетень
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=6; // Начать сканирование
// 7 - лист  в  тракте, идет сканирование
                 if((Sost_Optrons[0] ==0)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=7;    // на оптронах 1-2-3  определен  листИдет сканирование
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=8;    // Конец сканирвоания, иаркировка бюллетеня
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=9;    // Конец сканирвоания, остановка двигателя при  сканирвании
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=10;    // на оптронах 0  и 2  определен  лист,попытка вставить лист, когда не закончена работа с предыдущем
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==0)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=11;    // на оптронах 0  и 2 и 3 определен  лист,попытка вставить лист, когда не закончена работа с предыдущим
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==0))PrUprOpt=12;    //на оптронах 0  и 1 определен  лист, попытка вставить лист, когда не закончена работа с предыдущим
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==0)&&(Sost_Optrons[3] ==1))PrUprOpt=13;    // на оптронах 0  и 1 и  определен  лист,попытка вставить лист, когда не закончена работа с предыдущим
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==0))PrUprOpt=14;    //на оптронах 0  и 1 и 2 определен  лист, попытка вставить лист, когда не закончена работа с предыдущим
                 if((Sost_Optrons[0] ==1)&&(Sost_Optrons[1] ==1)&&(Sost_Optrons[2] ==1)&&(Sost_Optrons[3] ==1))PrUprOpt=15;    //на оптронах 0  и 1 и 2 И 3 определен  лист, лист  в  тракте
                 //  для основного режима
                 pr_opt=PrUprOpt;
                //             for(int i=0;i<4;i++)     Optron[i]=0;
                 //printf(" Optrons sost  Pr_UprOpt =  "); cout << pr_opt<< endl;
                 MetEnd:;
              }    break;
             case 2:
              {  // есть  событие нажатия  кнопки
                 //if(PrNoBtnWait==0) get_Buttons_Wait();   PrNoBtnWait=0;
                 if(Buttons[0]>0)BtnPushed=1;    if(Buttons[1]>0)BtnPushed=0;
                 if(Buttons[2]>0)BtnPushed=3;    if(Buttons[3]>0)BtnPushed=3;
                 if(Buttons[4]>0)BtnPushed=3;    if(Buttons[5]>0)BtnPushed=5;
                 if((Buttons[0]>0)&&(Buttons[1]>0))BtnPushed=2;
                 if((PrKlav==1)&&(PrMenuBtn < 1))
                 {  // нажата  кн. Меню
                    if(BtnPushed==5) //  нажата кн. МЕНЮ
                    {
                        PrMenuBtn=1;  KodOp=-1; KodDost=-1;   BtnPushed=-1;
                    }
                 }
                 pr_btn=BtnPushed;
              }    break;
             case 3:
              { // события связанные с информацией от клавиатуры

              }    break;
           }

//        **************КОНЕЦ  СУПЕРВИЗОРА ОБРАБОТКИ СОБЫТИЙ**********

        LabRun:; //  метка , обеспечивающая запуск основной программы после включения

            if(PrMenuBtn<1)
            {  calibrate_mode= MODE_NORMAL;
                Exit=RUN();
                if(PrNoBtnWait==0) PrModeChange=0;
            } else { ch=ServMenu(); }

           PrBklKoib=1;

           //usleep(200000);

    } while ((ch!=27)||(!Exit));

    Start(false);
    cout << "  KOIB Work has been finished!" << endl;

    return 0;
}// main
