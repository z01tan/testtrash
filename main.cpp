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
    // гашение управляющих переменных
    std::string st1;     std::string st2;
//char *sst1;
    //char sst1[128],sst2[128];
    PrNoBtnWait=0; // признак пропуска ожидания  кнопки
    Exit=false; // выход
    PrVoteTimeStart=0;      // признак наступления  времени голосования ( в день голосования)
    PrVoteTimeEnd=0;        // признак окончания  времени  голосования ( в день голосования)
    PrVoteDay=0;                    // признак "День голосования"
    Koib[1].VS = 1;                  // установка начального  состояния КОИБ
    PrKlav=0;                             //  признак наличия клавиатуры

    Version=0;                           // Номер версии сообщений!
    PrConnectOK=0;             // признак установки  связи со  вторым  сканером
    Init_Altera();                    // инициализация  железа
    ReadSetUpFiles();  // считывание установочных файлов (начальных данных)
    Create_Threads();          //  инициализация потоков
    //Lang=1;// установка английского языка ( для  отладки)
    LoadInd();// загрузка сообщений  и путей голосовых файлов в  массивы

    FILE *fw;
    if ((fw = fopen("./SetUpFiles/CodesOpFile.txt", "r"))!= NULL)
    {  // ввод кодов  операторов  из  файла
            fscanf(fw,"%d",&CodesOp[0]);            fscanf(fw,"%d",&CodesOp[1]);            fscanf(fw,"%d",&CodesOp[2]);
            fclose(fw);
    } else cout << "Не найден файл CodesOpFile.txt" << endl;
     ReadSetUpFiles(); // считывание  начальных  данных
    do
    {   // основной  цикл программы управления
            ch=0;
            //if(KeyisReady)PrKlav=1; else PrKlav=0; // перенесено в AnyWait 15.10.2013
            /* !!!!*/        //PrKlav=1;
            Exit=0;
            if(PrBklKoib==0) goto LabRun; // определение момента  включения КОИБ
//******************НАЧАЛО  СУПЕРВИЗОРА ОБРАБОТКИ СОБЫТИЙ**************
            // гашение кнопок
        StartCycle:
            for(i=0;i<6;i++) Buttons[i]=0;
            BtnPushed=-1; pr_btn=-1;   pr_opt=-1; PrUprOpt=-1;Pr_Event=0;
           if (PrNoBtnWait==0) Pr_Event= Any_Wait() ; // ожидание каког-либо события; возврат 1 -оптроны, 2- кнопки, 3- клава           PrNoBtnWait=0;
           PrNoBtnWait=0;
           switch (Pr_Event)
           { case 1:
              {  // событие  изменения  состояния  оптронов
                 PrUprOpt=0;
                 //get_Optrons_Imm();
                 for(int i=0;i<4;i++)
                 { if(Optron[i] < ONE_PAPERO[i]) { PrUprOpt=100; goto MetEnd;}
                   else
                   {  // формирование признаков состояния каждого  оптрона
                        if( Optron[i] < NO_PAPERO[i]) Sost_Optrons[i]=1; else Sost_Optrons[i]=0;
                   }
                    //cout << " Optron  N =      "<< i << "     Value =   "<<  Optron[i] <<  "     Sostoyanie =    " << Sost_Optrons[i]<< endl;
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
                 if((PrUprOpt==2)||(PrUprOpt==4))
                 {   //  если засветился только один оптрон, то ожидание засветки обоих оптронов
                     usleep(25000); goto StartCycle;
                 }
                 //  для основного режима
                 pr_opt=PrUprOpt;
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
            {  // рабочий  режим  КОИБ
                calibrate_mode= MODE_NORMAL;
                Exit=RUN();
                if(PrNoBtnWait==0) PrModeChange=0;
            } else
            {  // работа  в  сервисном  меню
                ch=ServMenu();
            }
           PrBklKoib=1;
    } while ((ch!=27)||(!Exit));
    Start(false);
    cout << "  KOIB Work has been finished!" << endl;
    return 0;
}// main
