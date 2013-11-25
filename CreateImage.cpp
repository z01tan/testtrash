//*****************************************************************************
// Создание потоков обработки изображения и внешних устройсв
//                     А.Галкин      07.05.2013
//******************************************************************************
#include <iostream>
#include "CreateImage.h"
#include "InitAltera.h"
#include "AlteraSocket.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/poll.h>
//#include <sched.h>
#include <errno.h>
#include "HeaderMain.h"
#include "AlteraSocket.h"

#define OUTPUT_FILE "scan.gr"

bool ScanReady; // Готовность к обработке скана

int Scptr_s,Scptr_r;//указатели на сканированные и обработанные строки

int ScanInProg;   // идет сканирование и третий оптрон сработал!!!
bool MotorRqst;   // запрос на изменение состояния мотора

bool CalibrRqst; // требование установить калибровку сканера

bool KeyStrPocess; // идет ввод строки с клавиатуры

int  Starting;    // задержка передачи инфы в буфер после затемнения 3-го оптрона
int  Braking;     // задержка выключения двигла после освобождения 3-го оптрона

int max_gelesen;    //кол-во записанных в массив строк скана
int MAXLINES = SCAN_MAXL; //600*8;
int MAXWIDTH = SCAN_LINEW; //216*8;
int Errno;
int image_fd=-1;
char line_buffer[LINE_LENGTH];
int first_block_found = 0;


int output_fd;
int ChangeO;
int altopt[4];

char KEYB[KEYBUF];
int  Key_B;
int  Key_E;
struct i_e {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
}input_event;

char TTY_c[5];
short l_sensor;

pthread_mutex_t  mutx;
pthread_t  PID_Image;
pthread_t  PID_But;
sem_t semBut;
sem_t semOpt;
sem_t semRC;
sem_t semSD;


char* PixelBMP;
char** PixCol;

//------------------------------------------------------------------------------
int get_line_number(char * packet)
{
        int line;

        if(packet[4] != 0x47)
                return -1;
        line = packet[5];
        line |= ((packet[6])<<8);
        line |= (((packet[7]&0xF0)>>4)<<16);

        return line;
}

int get_block_index(char * packet)
{
        int block_index;

        if(packet[4] != 0x47)
                return -1;
        block_index = packet[7]&0x0F;

        return block_index;
}

int process_line(char * line_buffer, int line_number)
{

       if((ScanInProg>0) && (image_fd>=0))
       {
   //      write(output_fd, line_buffer, LINE_LENGTH);
 //        Errno = pthread_mutex_lock(&mutx);

         memcpy(PixCol[Scptr_s],line_buffer,LINE_LENGTH);
         Scptr_s++;
 //        Errno = pthread_mutex_unlock(&mutx);
         if((Scptr_s >= MAXLINES)||(Scptr_s>SCAN_MAXL-1)){ScanReady=false;
                return 1;}
       }

       return 0;
}

int process_block(char * data)
{
        int ret=0;
        int i;
        int line;
        int block_index;
        char * packet;

        for (i=0; i<PCKT_AGGR; i++)
        {
                packet = data+i*BLOCK_SIZE;
                line = get_line_number(packet);
                block_index = get_block_index(packet);
                if(line >= 0){
                        if(block_index == 1)
                                first_block_found = 1;
                        memcpy(line_buffer+PIXELS_PER_BLOCK*(block_index-1),
                                packet+PIXEL_DATA_OFFSET, PIXELS_PER_BLOCK);

                        if(first_block_found&&(block_index==8)){
                                ret = process_line(line_buffer, line);// в буфер!!!
                        }
                }
        }
        return ret;
}

int open_image_device(const char *device)
{
        int ret;
        ret = open(device, O_RDONLY);
        if(ret < 0)
                pabort("ts open error\n");
        int_ioctl(ret);
        return ret;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void configure_line(int line_select, int speed, int led_r, int led_g, int scroll_1, int scroll_2,
        int led_div, int mode)
{
        char tmp[20];

        sprintf(tmp, "\x1B%d%d%d%d%d%d%d%d", line_select, speed, led_r, led_g, scroll_1, scroll_2, led_div, mode);
//        writeTTY(tmp, strlen(tmp));
         writeTTY(tmp, 9);
}

void print_line(char* line)
{
    char tmp[128];
    int len,i;
    for(i=0;i<128;i++)tmp[i] = 0x00;
    memmove(tmp,"\r",1);//strcpy
    len = strlen(line);
    if (len>124) len = 124;
    strncat(tmp+1,line,len);
    strcat(tmp+1+len,"\n");
    writeTTY(tmp, strlen(tmp));
}
//..............................................................................
int read_Key()
{
    struct pollfd ufds[1];
        int rv=1;
        int ret;
   ufds[0].fd = kfd;
   ufds[0].events = POLLIN;
   if((rv = poll(ufds, 1, 10))>0)
   {
        if(ufds[0].revents&POLLIN)
        {
           ret = read(kfd, &input_event, sizeof(input_event));
           if(ret<0) {printf("Read fail!!\n");
           return -1;}
           if(input_event.type == 1 && input_event.value ==0) // key is released
           {
            ret = input_event.code;
            if(ret!=69)return ret;
           }
        }
   }
   return 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int parceTTY(int fd)
{
        struct pollfd ufds[1];
        char in_buf[5];
        int i=0;
        int rv;
        int len;
        int msg=-1;
        int data,res;

        memset(in_buf,0,5);
        for(i=2;i<5;i++)BackBut[i]=0;
        ufds[0].fd = fd;
        ufds[0].events = POLLIN;
        rv = poll(ufds, 1, 0);
        i = 0;
        res = 0;
        if(rv>0){
                readTTY(in_buf+i, 1);
                if(in_buf[i]!=0x1B)
                        return 0;
                i ++;
                readTTY(in_buf+i, 1);
                switch(in_buf[i]){
                case '0':
                        // датчик освещенности
                        len = 2;
                        msg = 0;
                       break;
                case '1':
                        // клавиатура
                        len = 2;
                        msg = 1;
                     //   res = 1;
                        break;
                }
                i++;
                readTTY(in_buf+i, len); // resetTTY();
        //        printf("BUF_TTY %s\n",in_buf);
                switch(msg){
                case 0:
                        in_buf[4] = 0;
                        if(sscanf(in_buf+2, "%x", &data)==1)
                                l_sensor = data;
                        break;
                case 1:
                        switch(in_buf[2]){
                        case '1':
                                BackBut[2] = in_buf[3]-'0';
                                break;
                        case '2':
                                BackBut[3] = in_buf[3]-'0';
                                break;
                        case '3':
                                BackBut[4] = in_buf[3]-'0';
                                break;
                        }
                        break;
                default:
                        printf("TTY:Unknown message %s\n",in_buf);
                }
                  for(i=2;i<5;i++){
                      if (BackBut[i]==1 && BackBut[i]>Buttons[i]){
                          Buttons[i] = BackBut[i]; res = 1;
                      }
                  }
//                printf("\rLight: %3d, keys [%d] [%d] [%d]", l_sensor, Buttons[2],
//                        Buttons[3], Buttons[4]);
//                fflush(0);
        }
        return res;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int ScanRead()
{
     char buf[BLOCK_SIZE*PCKT_AGGR];
     int ret = 0;
//     int refr = 3;
//     while(refr>0){
         ret = read(image_fd, buf, sizeof(buf));
         if(ret<0){printf("read scan error\n");return 1;}
//       if(ret<0){refr--;usleep(1);if(refr==0) pabort("read scan error\n");}
//          else break;
//     }
     ret = process_block(buf);
     return (ret);
}
//==============================================================================
//-----------------------Thread ScanRead----------------------------------------
void* GetImage(void* arg) // Thread for getting Image & Optrones from Altera
{ //int loc_id;
  int tmp,Rest,Strt;


 // loc_id = * (int *) arg;
pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
// . . .Крутимся все время!!!!
while(true) // основной бесконечный цикл
{
    if (CalibrRqst) usleep(10000); // "останов" нити для записи регистров АЛЬТЕРЫ
    else{
           if (MotorFuS && (image_fd <0)) {
            image_fd = open_image_device(IMAGE_DEVICE);
            Scptr_s =0;
                    printf("\nimage_fd opened = %d\n",image_fd);fflush(0);
            Rest =-1;
            Strt = Starting;

           } //MotorFuS && (image_fd ==0)

           if(image_fd>=0){
               if (Rest>0) Rest--;
               tmp = ScanRead();


               if((tmp>0)||(!ScanReady))
               {
              // ***** ЗАКРЫТИЕ СКАН-ФАЙЛА *******************

                        motor_direction = MOTOR_STOP;
              //          altera_motor(motor_direction);
                        MotorRqst=true;

                   // if(ScanRead()>0) printf("ReadError after stop\n");

                        ScanReady = false;
                        ScanInProg = 0;
                        MotorFuS = false;
                        printf("\n image_fd closed!=%d\n",close(image_fd));fflush(0);
                    //    close(image_fd);
                        image_fd = -1;
                        max_gelesen = Scptr_s;
                     //   printf("\n image_fd closed!\n");fflush(0);

                } //if((tmp>0)||(!ScanReady)!!!!!!!!
           }else /* image_fd>=0 */
                {usleep(1000);} // если не сканируем, то ждем 1000 мкс


        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      if(calibrate_mode==MODE_NORMAL){
        if(MotorFuS && (ScanInProg==0) && (Optron[3] < NO_PAPERO[3])) //затемнился средний оптрон
          { Strt--; if(Strt==0)ScanInProg = 1; //!!!!!!!!!!!! начали передавать инфу!!!!!!!!!!!!
             //std::cout << " Scanning  begins !!!!! 20130913" << std::endl;
          }
        if((ScanInProg==1) && (Optron[3]>NO_PAPERO[3])) // освободился средний оптрон
           {
               std::cout << "  Image  Scan  The 3 optron  is free  nstr=  " << nstr<< std::endl;
               for(int i=0;i<4;i++)
                {std::cout << " End  Scanning  Optron  N =     "<< i << "   Value =   "<<Optron[i] <<  "   Porog = "<< NO_PAPERO[i] <<std::endl;}

       		if(Rest<0) Rest = Braking;  }

       // if((Rest==0) && (ScanInProg ==1))
        if(Rest==0)
           {Rest =-1;ScanInProg = 0;ScanReady = false;} /// !!!!!!!!!!!!!!!!!!!!!!!!!!!
       } // if(calibrate_mode==MODE_NORMAL)

      if(MotorFuS && (calibrate_mode==MODE_CALIBRATE_INIT))ScanInProg =1;
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

            //  if(MotorRqst){altera_motor(motor_direction);MotorRqst=false;}

     } //if (!CalibrRqst)
 } //while
 return 0;
}
//==============================================================================
//-----------------------Thread Buttons&Co--------------------------------------
void* GetButt(void* arg) // Thread for getting SPI & other
{ //int loc_id;
int bu1,bu2;
//loc_id = * (int *) arg;
int i,tmp;
//int impl1,impl2;
int semV,changeV,changeF;

char K_buff;

pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
bu1 = 1; bu2 = 1;// impl1=0;impl2=0;
while (true){

 // нет ли требования записать регистры Альтеры
 if (CalibrRqst) usleep(15000); // "останов" нити для записи регистров АЛЬТЕРЫ
 else{

    if(MotorRqst) { altera_motor(motor_direction); MotorRqst=false; }

    if (!KeyisReady) usleep(15000);      //      wait for 15 msec!
    else usleep(5000);

  /*  if(motorWatch > 0)motorWatch--;
    if(motorWatch == 0){motor_direction = MOTOR_STOP;motorWatch=-1;
                        max_gelesen=0;MotorRqst = true;}
*/

     // Опросим оптроны:
        for(i=0;i<4;i++) altopt[i] = Optron[i];
        adc_read(); // опросили!!
        for(i=0;i<4;i++){if (abs(altopt[i] - Optron[i])>20) ChangeO = i+1;}
        sem_getvalue(&semOpt,&i);
        if(ChangeO>0 && i==0) {sem_post(&semOpt); ChangeO = 0;} // сообщаем об изменении оптронов*****
    //-------------------------------------------------------------
// если сканируем, то кнопки не спрашиваем ?!
  if (ScanInProg==0){  //**********************************************

   // опросим изменения кропок задней панели...
    BackBut[0] =1 - bu1;
    BackBut[1] =1 - bu2;
    changeV= 0;
    if(butt_get(&bu1,&bu2)) pabort("Buttons Error");
    if (bu1==0 && BackBut[0]==0){ //impl1 =1;
        if (Buttons[0]==0){changeV =1; Buttons[0]=1;}} // нажали и не была нажата
    if (bu2==0 && BackBut[1]==0){ //impl2 =1;
        if (Buttons[1]==0){changeV =1; Buttons[1]=1;}}
   // if(BackBut[0]==1 && bu1==1)impl1 =0;
   //if(BackBut[1]==1 && bu2==1)impl2 =0;
    // printf("BackButs %d, %d\n",bu1,bu2);//!!!!!!!!!!!!!!!!!!!!
    //... и кнопок дисплея
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    changeF = parceTTY(fdTTY);  // опрос "мониторчика"
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// опросим наличие клавы ...
    tmp = 0;
    if (!KeyisReady) {
        if(open_Key()) {KeyisReady = true; }}
    if (KeyisReady) {
        tmp = read_Key(); // in mode non_block
        if(tmp<0){close_Key();KeyisReady = false;} //???????????????????????????????????????????????????????????
        if (tmp>0) {
          // printf("KeyPressed %d\n",tmp);
           if(!KeyStrPocess) if(tmp==KEYYES){Buttons[0] = 1;changeV=1;tmp=0;}
           if(tmp==KEYNO){Buttons[1] = 1;changeV=1;tmp=0;}
           if(tmp==KEYMENU){Buttons[5] = 1;changeV=1;tmp=0;}
           if(tmp==KEYHELP){Buttons[2] = 1;changeV=1;tmp=0;}
//     //      if((tmp==KEYBSPCE) && (Key_E>Key_B)){Key_E--;tmp=0;}
           if(tmp>0){
                 K_buff = tmp;
                 KEYB[Key_E]=K_buff;
                 Key_E++;
                 if(Key_E>KEYBUF-1)Key_E = 0;
           }
        }
    }
    sem_getvalue(&semBut,&semV);
    if((changeV>0 || changeF>0) && semV==0 ) sem_post(&semBut); // сообщаем об изменении кнопок***
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // теперь опросим сетку
  if(isConnected==1){ //
    if (have_Printer==1) // Client
    {
        i = CheckClt();

        switch(i){
        case -10:{totR=-10;break;}
        case 1: {if(sendFlag){tmp = WriteClt(bWptr,totW);
                totW = tmp;sendFlag=0;}break;}
        case 2: {if(!recvFlag){tmp = ReadClt(bRptr,totR);
                 totR = tmp;recvFlag = 1;}break;}
        case 3:{
          //  if(sendFlag){tmp = WriteClt(bWptr,totW);totW = tmp;sendFlag=0;}
            if(!recvFlag){tmp = ReadClt(bRptr,totR);totR = tmp;recvFlag = 1;}break;
        }
        default:totR = i;  }

    }
    if (have_Printer==0)    // Server
    {
        i = CheckList();

        switch(i){
        case -10:{totR=-10;break;}
        case 1: {if(sendFlag){tmp = WriteList(bWptr,totW);
                totW = tmp;sendFlag=0;}break;}
        case 2: {if(!recvFlag){tmp = ReadList(bRptr,totR);
                 totR = tmp;recvFlag = 1;}break;}
        case 3:{
            if(!recvFlag){tmp = ReadList(bRptr,totR);totR = tmp;recvFlag = 1;}break;
        }
        default:totR = i;   }

    }
  } //PrConnectOK==1
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

 }  // if (ScanInProg==0)******************************************************

 }  //if (!CalibrRqst)  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}   //while (true)
  return 0;
}
//------------------------------------------------------------------------------
//==============================================================================

int Create_Threads()
{
//============================ Main funftion =================================
 int i,j;
 // Erstens, get memory for BULLETINs BitMap
 int L,C;

 pthread_attr_t attr;
 struct sched_param param;

 MAXLINES = SCAN_MAXL;
 MAXWIDTH = LINE_LENGTH;
 L=MAXLINES;
 C=MAXWIDTH;
 PixelBMP = (char*)malloc(L*C); //выделяем память для массива размером L*C байт
 if (PixelBMP==0) {return(-1);}
 printf("PixelBMP OK allocated lines=%d\n",L);
 PixCol	  = (char**)malloc(L*sizeof(char*)); //массив указателей на строки матрицы
 if (PixCol==0) {free(PixelBMP); return(-1);}
 printf("PixCol OK\n");
 for (i = 0; i < L; i++)
	PixCol[i] = PixelBMP + i*C; //инициализируем строки
 // нулим массив
 for(i=0;i<L;i++)
	{
		for(j=0;j<C;j++)   PixCol[i][j]=0;
	} // Fertig!

// подготовка для чтения скана
CalibrRqst = false;             //!!!!!!!!!!!!!!!!
ScanInProg = 0;
MotorFuS = 0;
ScanReady = false;
calibrate_mode = MODE_NORMAL;

Braking = BremsDelay;
Starting= StartDelay;

Scptr_s = 0;
Scptr_r = 0;// Нулим указатели на сканированные и обработанные строки

// creating MUTEX for Scan routine

  Errno = pthread_mutex_init(&mutx,NULL);
// For use: pthread_mutex_lock(), pthread_mutex_trylock(),pthread_mutex_timedloc,
// pthread_mutex_unlock() and pthread_mutex_destroy();
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!мьютексом будем защищать Scptr_s - указатель на сосканир.строку!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  printf("create mutex OK\n");
  adc_read();
  for(i=0;i<4;i++) printf("OPTRON%d,=%d\n",i,Optron[i]);

// creating Semaphor for OPTRONS

  Errno = sem_init(&semOpt,0,0);
  if(Errno!=0) {printf("SemOptrons error!!!\n");return(-1);}
// For use: sem_post(), sem_wait(), sem_trywai(), sem_destroy() & sem_getvalue().
  printf("SemOpt OK\n");
// creating Thread for Image
  ChangeO = 0;
  int idS = 1;
  pthread_attr_init( &attr );
//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED );
//pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
  pthread_attr_getschedparam( &attr, &param );
  param.sched_priority =  9;
  pthread_attr_setschedparam( &attr, &param );
  Errno = pthread_create(&PID_Image,&attr,GetImage,&idS);
  if(Errno!=0)  {printf("ThreadImage error!!!!\n");return(-1);}
// Запустили!!!
// creating Semaphor for LAN
    Errno = sem_init(&semSD,0,0);
    if(Errno!=0) {printf("SemSEND error!!!\n");return(-1);}
// For use: sem_post(), sem_wait(), sem_trywait(), sem_destroy() & sem_getvalue().
    Errno = sem_init(&semRC,0,0);
    if(Errno!=0) {printf("SemRECEIVE error!!!\n");return(-1);}
///////////////////////////////////////////////////////////
// подготовка для кнопок и Co
  KeyisReady = false;
  for(i=0;i<5;i++) {Buttons[i] = 0;BackBut[i]=0;}
  if(butt_open()<0) pabort("BackButtons fail!!!\n");
  Key_B = 0;Key_E = 0;

// creating Semaphor for Buttons&Co

  Errno = sem_init(&semBut,0,0);
  if(Errno!=0) {printf("SemButtons error!!!\n");return(-1);}
// For use: sem_post(), sem_wait(), sem_trywai(), sem_destroy() & sem_getvalue().
  printf("SemButt OK\n");
// creating Thread for Buttons&Co

  idS = 2;
  Errno = pthread_create(&PID_But,NULL,GetButt,&idS);
  if(Errno!=0) {printf("ThreadButtons error!!!!\n");return(-1);}
// Запустили!!!
  printf("Thread Button OK\n");


  return 0;

}
//.............................................................................
void Close_Threads()
{
        closeTTY();
        close_device();
        pthread_mutex_destroy(&mutx);
        pthread_cancel(PID_Image);
        pthread_cancel(PID_But);
        sleep(1);
        close_Key();

        free(PixCol);
        free(PixelBMP);
        sleep (1);
        sem_destroy(&semBut);
        sem_destroy(&semOpt);
        sem_destroy(&semRC);
        sem_destroy(&semSD);


}
//==============================================================================
