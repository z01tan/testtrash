/*
 * File:   HardProc.cpp
 * Author: andr-u
 *
 * Created on 16 05 2013 г., 10:15
 */


#include <cstdlib>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include "CreateImage.h"
#include "InitAltera.h"
#include "HardProc.h"
//#include "HeaderMain.h"      // UWAGA!!!! ERASE COMMENT SIGN!!!  

//#define OUTPUT_FILE "scan.gr"

//******************************************************************************
// int ScanInProg;     // идет сканирование и третий оптрон сработал!!!
// bool MotorFuS;      // запущен мотор вперед со сканированием
// bool ScanReady;     // Готовность к обработке скана
//int getNextScanLine(void) //возвращает номер готовой для обработки строки в массиве скана
//                           или -1, если читать нечего!
//******************************************************************************
int LedR =0;
int LedG =0;
int Schift1 = 0;
int Schift2 = 0;
//******************************************************************************
void Init_COIB()
{
    if (Init_Altera()) abort();     // Обязательно!
   //*********************************************************
    system("amixer -c0 sset 'Playback DAC' 90% ");      // Звук!!
   //**********************************************************
    if (Create_Threads()) abort();  // Обязательно!
}
//******************************************************************************
 int say_aloud(char*a, char*b, char*c, char*d, char*e)
{  // произнести звуки в файлах a,b,c,d,e или NULL
    char say[100];
    int i;
    for(i=0;i<100;i++)say[i]=0x00;
    strcpy(say,"aplay   ");
    if(a==NULL)return -1;
    // формирование сообщения из кусочков
    strcat(say," ");  strcat(say,a);
    if(b!=NULL) { strcat(say," ");  strcat(say,b);    }
    if(c!=NULL) { strcat(say," ");  strcat(say,c);    }
    if(d!=NULL) { strcat(say," ");  strcat(say,d);    }
    if(e!=NULL) { strcat(say," ");  strcat(say,e);    }
    //  воспроизведение набранного  текста
    system(say);
    return 0;
}


//******************************************************************************
void setVoiceVolume(int Vol)
{
   char msg[36] = "amixer -c0 sset 'Playback DAC' ";
   char percent[5];
   for(int i=0;i<5;i++)percent[i] = 0x00;
   switch(Vol){
       case 0:memmove(percent,"0% ",3); break;
       case 1:memmove(percent,"50% ",4);break;
       case 2:memmove(percent,"55% ",4);break;
       case 3:memmove(percent,"60% ",4);break;
       case 4:memmove(percent,"65% ",4);break;
       case 5:memmove(percent,"72% ",4);break;
       case 6:memmove(percent,"76% ",4);break;
       case 7:memmove(percent,"80% ",4);break;
       case 8:memmove(percent,"85% ",4);break;
       case 9:memmove(percent,"90% ",4);break;

       default:memmove(percent,"100% ",5);}
   memcpy(&msg[31],percent,5);
   system(&msg[0]);
}
//******************************************************************************
void ClearKoeff()
{
    _clear();
}

//------------------------------------------------------------------------------
int MakeBinFile(char* nameF,int W, int H)    //создание бинарного файла образа
{
    int output_fd,i;
  output_fd = creat(nameF, O_WRONLY);
  if(output_fd < 0){ printf("output bin file error\n"); return -1;}
  for(i=0;i<H;i++) write(output_fd, PixCol[i], LINE_LENGTH);
  close(output_fd);
  return 0;
}
//------------------------------------------------------------------------------

int get_Optrons_Imm(void) // немедленный опрос оптронов. возврат 1 - было изменение
{
   int s;
   sem_getvalue(&semOpt,&s);
   if (s>0){sem_wait(&semOpt);return(1);}
   return(0);
}

int get_Optrons_Wait(void) // опрос с ожиданием. возврат 0 -норм. -1 -ошибка.
{
    int s;
    s = sem_wait(&semOpt);
    return(s);
}
int Wait_beiden() // ожидание двух оптронов в течение 0,05 сек. -1, если не дождались!
{
    usleep(10000); // 10 msec!
    if(get_Optrons_Imm()==0)return -1;
    if((Optron[1]<NO_PAPER) && (Optron[2]<NO_PAPER)) return 0;
    return -1;
}

void waitBothOpt()
{
    int d =-1;
    while(d<0){
        // get_Optrons_Wait();
        get_Optrons_Imm();
         if ((Optron[1]<NO_PAPER) || (Optron[2]<NO_PAPER)) d = Wait_beiden();
    }
}

void waitAllOpenedOpt()
{
    int i,e;
    while(true){
         usleep(50000); // wait for 50 msec;
         get_Optrons_Imm(); e=0;
         for(i=0;i<4;i++) if(Optron[i]<NO_PAPER)e++;
         if(!e)return;

    }
}
//==============================================================================
int get_Key_Imm(void)  // немедленный опрос клавиатуры. возврат 0 -не было нажатия
{
   int res;
   if(Key_B==Key_E){return(0);}
   res = (int)KEYB[Key_B];Key_B++;
   if(Key_B>KEYBUF-1)Key_B = 0;
   return(res);

}
int get_Key_Wait(void)  // опрос с ожиданием. возврат код клавиши
{
   int res=0;
   while(res==0){
        if(Key_B==Key_E) usleep(20000);// ждем 20 мсек
        else res =1;
   }
   return(get_Key_Imm());
}
int get_KeyString(char* Input)
{
    int i,p;
    char c;
    frontLED(5,0);
    for(i=0;i<32;i++)Input[i] = 0x00;
    KeyStrPocess = true;
    p=0;
    while(p<32){
        i = get_Key_Wait();
                if(i==KEYBSPCE){if(p>0)p--;
                        Input[p] = 0x00;
                      }
                if(i==KEYHELP){
                        KeyStrPocess=false; frontLED(0,5);c=0x00;setSecline(&c,0);return p;}
        switch(i){
                case KEY0: c='0';break;
                case KEY1: c='1';break;
                case KEY2: c='2';break;
                case KEY3: c='3';break;
                case KEY4: c='4';break;
                case KEY5: c='5';break;
                case KEY6: c='6';break;
                case KEY7: c='7';break;
                case KEY8: c='8';break;
                case KEY9: c='9';break;
            case KEYPUNKT: c='.';break;
                default:c='x';}
       if(c!='x'){
        memmove(Input+p,&c,1);
        p++;}
        setSecline(Input,0);
    }
    KeyStrPocess=false;
    frontLED(0,5);
    return 32;
}
//==============================================================================
int get_Buttons_Imm()  // если возврат=1, то в Buttons[] обновленные значения;
{
    int s,res;
   sem_getvalue(&semBut,&s);
   if (s>0){
//       for (res=0;res<5; res++)
//            {
//                Buttons[res] = BackBut[res];
//                BackBut[res]=0;
//            }
       sem_wait(&semBut);return 1;}
   return 0;
 }

int get_Buttons_Wait()
{
    int res;
    sem_wait(&semBut);
//    for (res=0;res<5; res++)
//    {
//        Buttons[res] = BackBut[res];
//        BackBut[res]=0;
//    }
    return 0;
}
//------------------------------------------------------------------------------
int Any_Wait() // ожидание каког-либо события; возврат 1 -оптроны, 2- кнопки, 3- клава
{ bool a = true; int pr1=0;
         while(a){
        //if(Wait_beiden() == 0) return 1;
// UWAGA!!!! ERASE COMMENT SIGN!!!             
 /*       if(PrVoteDay==1)
        {   TekDateTime();

             if(PrVoteTimeStart==0)
             {  // контроль времени начала голосования за 10 минут до начала
                int h=Votes.Bhour[1]-TekDT.hour;  int m=Votes.Bmin[1]-TekDT.minit-wngtime;
                if ((h<=0)&&(m<=0))
                {  PrVoteTimeStart=1; a=false;   }
            }
            if(PrVoteTimeStart==11)
            {  // контроль времени начала голосования
                int h=Votes.Bhour[1]-TekDT.hour;  int m=Votes.Bmin[1]-TekDT.minit-5;
                if ((h<=0)&&(m<=0))
                {  PrVoteTimeStart=12; a=false;   }
            }
            if(PrVoteTimeStart==13)
            {  // контроль времени начала голосования
                int h=Votes.Bhour[1]-TekDT.hour;  int m=Votes.Bmin[1]-TekDT.minit;
                if ((h<=0)&&(m<=0))
                {  PrVoteTimeStart=2; a=false;   }
            }
            //  контроль окончания  стационарного  голосования
            if(PrVoteTimeEnd==0)
            {  // контроль времени окончания голосования  за 10 минут до окончания
                int h=Votes.Bhour[2]-TekDT.hour;  int m=Votes.Bmin[2]-TekDT.minit-wngtime;
                if ((h<=0)&&(m<=0))
                {  PrVoteTimeEnd=1; a=false;   }
            }
            if(PrVoteTimeEnd==11)
            {  // контроль времени окончания голосования
                int h=Votes.Bhour[2]-TekDT.hour;  int m=Votes.Bmin[2]-TekDT.minit-5;
                if ((h<=0)&&(m<=0))
                {  PrVoteTimeEnd=12; a=false;   }
            }
            if(PrVoteTimeEnd==13)
            {  // контроль времени окончания голосования
                int h=Votes.Bhour[2]-TekDT.hour;  int m=Votes.Bmin[2]-TekDT.minit;
                if ((h<=0)&&(m<=0))
                {  PrVoteTimeEnd=2; a=false;   }
            }
        }// if(PrVoteDay==1)
  */
// UWAGA!!!! ERASE COMMENT SIGN!!!             
        if (get_Optrons_Imm() == 1) { a=false; pr1=1;}
        if (get_Buttons_Imm() == 1) { a=false; pr1=2;}
        if ((Key_B!=Key_E))         { a=false; pr1=3;}
        usleep(25000);  //25msec!!
     }
return pr1;
}


//==============================================================================
void frontLED(int red,int green) // "0" - off,"9" - automat, "1"-"8" - brightness
{
    configure_line( 1, 2, red, green, Schift1, Schift2, 0, 0);
    LedR = red;
    LedG = green;
    usleep(120000);
}

void setOneline(char* txt,int shift)
{
     Schift1 = shift;
     configure_line(1, 2, LedR, LedG, Schift1, 0, 0, 1);
     print_line(txt);
     usleep(120000);
}

void setFirstline(char* txt,int shift)
{
     Schift1 = shift;
     configure_line( 1, 2, LedR, LedG, Schift1, Schift1, 0, 0);
     print_line(txt);
     usleep(120000);
}

void setSecline(char* txt,int shift)
{
     Schift2 = shift;
     configure_line( 2, 2, LedR, LedG, Schift1, Schift2, 0, 0);
     print_line(txt);
     usleep(120000);
}
//==============================================================================
int getSptr()
{int ptr,Errno;
 //   Errno = pthread_mutex_lock(&mutx);
      ptr = Scptr_s;
            //    printf("\rSptr_S = %d",ptr);
 //   Errno = pthread_mutex_unlock(&mutx);
    return ptr;
}

int getNextScanLine(void) //возвращает номер готовой для обработки строки в массиве скана
{                         // или -1, если читать нечего!
  while (true)
  {

      if(Scptr_r >= max_gelesen)return -1;
      if(getSptr() > Scptr_r){Scptr_r++; return Scptr_r;}
    usleep(1000);// wait 0.1 millisec!
  }
  return -1;
}
//==============================================================================
void Page_scan()
{
    ScanReady = true;
    ScanInProg = 0;
    motor_direction = MOTOR_FORWARD;
    max_gelesen = MAXLINES;
    MotorRqst = true;
 //   altera_motor(motor_direction);  // start scan Blatt!
    while(MotorRqst)usleep(103);
    return;
}

void Page_back()
{
    motor_direction = MOTOR_BACKWARD;
    MotorRqst = true;
    while(MotorRqst)usleep(103);
  //  altera_motor(motor_direction);
    //sleep(3);
    waitAllOpenedOpt();    get_Optrons_Imm();
    motor_direction = MOTOR_STOP;
     MotorRqst = true;
  //  altera_motor(motor_direction);
      while(MotorRqst)usleep(103);
    return;
}
void Page_backTest()
{
    motor_direction = MOTOR_BACKWARD;
    MotorRqst = true;
    while(MotorRqst)usleep(103);
  //  altera_motor(motor_direction);
    sleep(2);
    motor_direction = MOTOR_STOP;
     MotorRqst = true;
  //  altera_motor(motor_direction);
      while(MotorRqst)usleep(103);
    return;
}
void Page_stop()
{
    ScanReady = false;
    ScanInProg = 0;
    MotorFuS = false;
  //  if(image_fd>0) {close(image_fd);image_fd = 0; max_gelesen = Scptr_s;}
    motor_direction = MOTOR_STOP;
    MotorRqst = true;
    //altera_motor(motor_direction);
    while(MotorRqst)usleep(103);
    return;
}

void Page_down()
{
   motor_direction = MOTOR_FORWARD_NO_SCAN;
   MotorRqst = true;
   //altera_motor(motor_direction);
   sleep(1);
   motor_direction = MOTOR_STOP;
   MotorRqst = true;
   //altera_motor(motor_direction);
    while(MotorRqst) usleep(103);
    return;
}

void Page_mark()
{   solenoid = SOL_ON;
   /*  motor_direction = MOTOR_FORWARD_NO_SCAN;
     MotorRqst = true;
     //altera_motor(motor_direction);
     sleep(1);
     solenoid = SOL_OFF;
     motor_direction = MOTOR_STOP;
     MotorRqst = true;
     //altera_motor(motor_direction);
     while(MotorRqst)usleep(100);*/

    Page_down();
    solenoid = SOL_OFF;
    usleep(500003);
    Page_down();
    return;
}
//==============================================================================
void alteraRegCalibr_write(void) // записать калибровку в регисты Альтеры
{
    CalibrRqst = true;
     while(CalibrRqst)usleep(100);
    return;
}
//==============================================================================
void Prov_Line(int ln)
{
    ;
}
