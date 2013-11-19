
#include <cstdio>

//**********************************************************************
//****  Inter-Unit communication routine TCP (KOIB 2012)
//****   written by GAI
//**********************************************************************

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <algorithm>
#include <set>
#include <errno.h>
#include <semaphore.h>
#include "AlteraSocket.h"
#include "printer.h"
#include "HeaderMain.h"

int     sock_sr,sock_kl,listener;
int     sock,sl;
struct sockaddr_in addrK,addrS;
FILE    *logf;                  // InitSocket write LOF File "TCPlog.txt" !!!!

int     bytes_read;
int     have_Printer;
int     i_m_a_SRV;  // if ==1 then i'm a Server!!!
struct pollfd ufds[1],l_ufds[1];
char    IP_ADDR[]="127.00.00.11";  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int     isConnected =0;
int     ListOpened  =0;
int     flags;
int     rv;
int     sendFlag,recvFlag;

int     typB,lnB;
char    buf[1024];
char    bufR[1024],bufW[1024];
char*   bRptr;
char*   bWptr;
int     totR,totW;
int     FlagRC,FlagSD;  // Receive & Send Flaggs

char    msgK[] = "It's my";

struct  CRA
{
    char s0;
    char s1;
    uint16_t tp;
    uint32_t qty;
};
CRA*    ptCRA;
//==============================================================================================================
void CloseList()
{
    if(ListOpened) close(listener);
}
void CloseSrv()
{
    //if(logf!=NULL)fclose(logf);
    CloseList();
    close(sock_sr);
}
void CloseCl()
{
   // if(logf!=NULL)fclose(logf);
    close(sock_kl);
}
void CloseSocket()
{
    if(i_m_a_SRV) CloseSrv();
    else CloseCl();
}

// ****** PRINTER present - we build a  C L I E N T *******
 int InitClt()
   {
       sock_kl = socket(PF_INET, SOCK_STREAM, 0); // Anyway we build a Socket TCP/IP!
       if(logf!=NULL)fprintf(logf,"Client init result = %d\n",sock_kl);
            if(sock_kl < 0)
            {
                perror("Socket FAIL");
                if(logf!=NULL)fprintf(logf,"Socket FAIL\n");
                return(-1);
            }
        addrK.sin_family = AF_INET;
        addrK.sin_port = htons(1221); // .или любой другой порт
        if(inet_aton(Ip_slave/*IP_ADDR*/, (struct in_addr*)&addrK.sin_addr.s_addr)<0)
            {   if(logf!=NULL)fprintf(logf,"Ip_slave FAIL\n");
                CloseCl(); return(-2); }
            //  addrK.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            //  the first place. do non-blocking socket, then call and check the connection with poll()

        flags = fcntl(sock_kl, F_GETFL, 0);
        if(fcntl(sock_kl, F_SETFL, flags | O_NONBLOCK) == -1)
        { // не получилось...
          if(logf!=NULL)fprintf(logf,"Socket is Blocking\n");
          perror("Socket is Blocking");{CloseCl();return(-3);}
        }
        if(connect(sock_kl, (sockaddr*)&addrK, sizeof(addrK)) < 0)
        {
            if(errno == EINPROGRESS)
            { /*  Это не ошибка, это признак
          * того, что соединение все еще
          * не установлено*/

                ufds[0].fd = sock_kl;
                ufds[0].events = POLLIN | POLLOUT;
                rv = poll(ufds, 1, 3000);   //wait for 3 sec!
                if (rv==0) {CloseCl();return(-10);}  // Timeout by connection
                if(rv>0){
                    socklen_t err_len;
                    int error,gt;
                    err_len = sizeof(error);
                    gt = getsockopt(sock_kl, SOL_SOCKET, SO_ERROR, &error, &err_len);
                    if( gt < 0 || error != 0)
                    {  /* рТПЙЪПЫМБ ПЫЙВЛБ УПЕДЙОЕОЙС.*/
                        if(logf!=NULL)fprintf(logf,"getsockopt > error - %d > %d\n",gt,error);
                        printf("getsockopt > error - %d > %d\n",gt,error);
                        CloseCl();return(-10);
                    }
                    else
                    { /* чУЕ ОПТНБМШОП. */
                        isConnected = 1;
                    }
                }

            }
            else
            { /* рТПЙЪПЫМБ ПЫЙВЛБ УТБЪХ ЦЕ  РТЙ УПЕДЙОЕОЙЙ. */
                 if(logf!=NULL)fprintf(logf,"Socket Connection fail\n");
                 perror("Socket Connecting fail");CloseCl();
                 return(-10);
            }
        }
        else
        { /*
       * Соединение было установлено за время
       * системного вызова, работа продолжается
       * традиционным способом.
       */
             isConnected = 1;
        }
        if(isConnected) send(sock_kl,msgK,sizeof(msgK),0);
        return 0;
   }
 //******************************************************************************************
 //  ****** PRINTER absent - we build a  S E R V E R *******
 int InitSrv()
   {
      sock_sr = socket(PF_INET, SOCK_STREAM, 0); // Anyway we build a Socket TCP/IP!
      if(logf!=NULL)fprintf(logf,"Server init result = %d\n",sock_sr);
         if(sock_sr < 0)
            {
                if(logf!=NULL)fprintf(logf,"Socket fail\n");
                perror("Socket FAIL");
                return(-1);
            }
        i_m_a_SRV = i_m_a_SRV | 1;
        addrS.sin_family = AF_INET;
        addrS.sin_port = htons(1221);
        addrS.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(sock_sr, (struct sockaddr *)&addrS, sizeof(addrS)) < 0)
        {
            if(logf!=NULL)fprintf(logf,"Socket bind fail\n");
            perror("bind FAIL ");CloseSrv();
            return(-2);
        }
        listen(sock_sr, 1);
        flags = fcntl(sock_sr, F_GETFL, 0);
        if(fcntl(sock_sr, F_SETFL, flags | O_NONBLOCK) == -1)
        { // оЕ РПМХЮЙМПУШ...
          perror("Socket is Blocking");{CloseSrv();return(-3);}
        } // Socket is in NON_BLOCK Mode now!!
 //      isConnected = 1;//??????????????????????????
       printf("SRV listen is active!!!\n");
       if(logf!=NULL)fprintf(logf,"SRV listen is active\n");
         return 0;
   }

int ListenerCrt(int timewait) //try read listener for 'timewait' msec
{ int n_bt;
         l_ufds[0].fd = sock_sr;
         l_ufds[0].events = POLLIN ;//| POLLOUT;
         rv = poll(l_ufds, 1, timewait);   //wait for 3 sec!
         if (rv==0) {CloseSrv();return(-10);}  // Timeout by connection tel O.E.450 93 28
         if(rv>0){
              listener = accept(sock_sr, NULL, NULL);
              if(listener < 0)
                {
                    if(logf!=NULL)fprintf(logf,"accept fail\n");
                    perror("accept FAIL ");CloseSrv();
                    return(-2);
                }
              ListOpened = 1; // LISTENER is Active & Client is connected!!!!!
              isConnected = 1;//?????????????????????????
              n_bt = recv(listener,bufR,1024,0);
              send(listener,bufR,n_bt,0);
              if(logf!=NULL)fprintf(logf,"received: %d buf>%s\n",n_bt,bufR);
              printf("received: %d buf>%s\n",n_bt,bufR);
             // close(listener);
             }
           return (n_bt);
}

int InitSockets()
{
   int res;
   FlagRC =0;
   FlagSD =0;
   if((logf = fopen("./Log/TCPlog.log","w"))==NULL)   printf("LogFile open eroor!!");
   have_Printer = CheckPrinter(); // 1 -   ЕУФШ РТЙОФЕТ
   i_m_a_SRV = 0;
    // 1 - Srv Sock is opend;
    // 0 - CLnt Sock is opened
   isConnected = 0;
   printf("Printer %d\n",have_Printer);

   // if have_Printer==1 we try to connect as a Client, otherwise as a Server!!!

    switch (have_Printer){

    //  ****** PRINTER absent - we build a SERVER *******
   case 0: {
            res = InitSrv();
            if (res==0) res=ListenerCrt(timeout);
            break;
    }
     // ****** PRINTER present - we build CLIENT *******
   case 1:{
            res=InitClt();
            break;
    }
    default:;} //switch (have_Printer)
/**********************************************************************************
res = 0  -  УПЕДЙОЕОЙЕ  ХУФБОПЧМЕОП
res = -1    Socket FAIL!!!! Fatal
res = -2    Socket bind or accept error!! Fatal
res = -3    Socket left in Blocking Mode!! Fatal
res = -10   Socket Timeout. Repeat
*/
if(logf!=NULL)
{ fprintf(logf,"Return from InitSocket = %d\n",res);
                fclose(logf);}
if(isConnected)recvFlag = 1; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
return res;
}

int CheckList()
{   int ret=0;
    l_ufds[0].fd = listener;
    l_ufds[0].events = POLLIN | POLLOUT;
    rv = poll(l_ufds, 1, 0);   //wait for 0 msec!
         if (rv==0) return(-10);  // Timeout by connection
         if(rv>0){
                if(l_ufds[0].revents & POLLOUT)ret|= 1;// Outbuf is free!Ready for output!
                if(l_ufds[0].revents & POLLIN) ret|= 2;// input ready!
                return ret;
         }
    return -1;
}
int WriteList(char* msg,int ln)  // Atom servers write. Used from CreateImage/GetButt Thread!!!!!!!!!!!
{ if(ListOpened==1){
     int r = (send(listener, msg, ln, 0));
     if (r==0) return -2;
     if (r<0) return -1;
     return r;

  }
  else return -3;   // -3 -> Listener is NOT OPENED!!!!
}   // return qty of bites to send or  -1 by connection error
    //                                 -2 connection is lost
    //                                 -3 by not opened listener/connection

int ReadList(char* msg,int ln)  // Atom servers read. Used from CreateImage/GetButt Thread!!!!!!!!!!!
{ if(ListOpened==1){

    int r =recv(listener,msg,ln,0);
    if (r==0) return -2;
    if (r<0) return -1;
    return r;
}
  else return -3;   // -3 -> Listener is NOT OPENED!!!!
}   // return qty of bites to send or  -1 by connection error
    //                                 -2 connection is lost
    //                                 -3 by not opened listener/connection

//
//
int CheckClt()
{ int ret=0;
  ufds[0].fd = sock_kl;
  ufds[0].events = POLLIN | POLLOUT;
  rv = poll(ufds, 1, 0);   //wait for 0 msec!
        if (rv==0) return(-10);  // Timeout by connection
        if (rv>0) {
            if(ufds[0].revents & POLLOUT) ret = ret | 1;// Outbuf is free! Ready for output!
            if(ufds[0].revents & POLLIN) ret = ret | 2;// input ready!
            return ret;
         }
   return -1;
}

int WriteClt    (char* msg,int ln)  // Atom clients write. Used from CreateImage/GetButt Thread!!!!!!!!!!!
{if(isConnected==1)
    {
   int r = send(sock_kl,msg,ln,0);
   if (r==0) return -2;
   if (r<0) return -1;
   return r;
    }
 return -3;
}   // return qty of bites to send or  -1 by connection error
    //                                 -3 by not opened listener/connection
    //                                 -2 connection is lost

int ReadClt (char* msg,int ln)  // Atom clients read. Used from CreateImage/GetButt Thread!!!!!!!!!!!
{
    if(isConnected==1)
    {

        int r =recv(sock_kl,msg,ln,0);
        if (r==0) return -2;
        if (r<0) return -1;
        return r;

    }
    return -3;   // -3 -> Connect is NOT OPENED!!!!
}   // return qty of bites to send or  -1 by connection error
    //                                 -2 connection is lost
    //                                 -3 by not opened listener/connection
//****************************************************************************************

int Clientsend(char* buff,int length)
{
   if(!isConnected)return -3;
   int ln = length;
   bWptr = buff;
   totW =   ln;

   while(totW>0){
        sendFlag = 1;
        while(sendFlag==1) usleep(30000);
        if(totW<0)return totW;
        bWptr+=totW;
        ln-=totW;
        totW = ln;
   }
    return 0;// All sended!!
}

int Clientrecv(char* buff,int length)
{
   if(!isConnected)return -3;
   int ln = length;
   bRptr = buff;
   totR =   ln;

   while(totR>0){
        usleep(30000);
        if(totR<0)return totR;
        if (recvFlag)
        {bRptr+=totR;
        ln-=totR;
        totR = ln;recvFlag=0;}
   }
    return 0;// All recieved!!  if ret==-10 then NO DATA (timeout)!
}
//...........................................................................
int Servsend(char* buff,int length)
{
   if(!ListOpened)return -3;
   int ln = length;
   bWptr = buff;
   totW =   ln;

   while(totW>0){
        sendFlag = 1;
        while(sendFlag==1) usleep(30000);
        if(totW<0)return totW;
        bWptr+=totW;
        ln-=totW;
        totW = ln;
   }
    return 0;// All sended!!
}
int Servrecv (char* buff,int length)
{
   if(!ListOpened )return -3;
   int ln = length;
   bRptr = buff;
   totR =   ln;
// recvFlag ==1 if thread GetButt in CreateImage routine receives the packet of information !!!!
// otherwise recvFlag==0!!!!
   while(totR>0){
        usleep(30000);
        if(totR<0)return totR;
        if (recvFlag)
        {bRptr+=totR;
         ln-=totR;
         totR = ln;recvFlag=0;}
   }
    return 0;// All recieved!! if ret==-10 then NO DATA (timeout)!
}
//=======================USER AREA========================================
int ReceivefromAnother()    // typB - Data TYP, bufR - Data, lnB - Data size
{
  int rr,TP,LN;
  if(FlagRC)return 0; // Если данные обработаны, FlagRC СБРОСИТЬ В 0!!!!!!
// FlagRC ==1 Data not saved in stuctures!!!!!!!!!!!!!!!
  recvFlag = 0;
  if(i_m_a_SRV) rr = Servrecv(buf,8);
  else rr = Clientrecv(buf,8);
  if (rr==-10) return 0;    // nothing to read!
  if (rr<0) return rr;
  // Data in buf!!
  if((buf[0]=='&')&&(buf[1]=='C')) // Command comes!!   1.Phase
  { ptCRA =(CRA*) &buf[0];
    TP = ntohs(ptCRA->tp);
    LN = ntohl(ptCRA->qty);
    buf[1] = 'R';
    if(i_m_a_SRV) rr = Servsend(buf,8);  // send "READY" 2.Phase
    else rr = Clientsend(buf,8);
    if(i_m_a_SRV) rr = Servrecv(buf,LN); //              3.Phase
    else rr = Clientrecv(buf,LN);
    if(rr<0)return rr;      // ERROR BY READING DATA
    memcpy(bufR,buf,LN);
    typB = TP;
    lnB  = LN;
    buf[0]='&';buf[1]='A';
    if(i_m_a_SRV) rr = Servsend(buf,8);  // send "ANSWER" 4.Phase
    else rr = Clientsend(buf,8);
    FlagRC = 1;return 1;
  }
  return 0;
}

int SendtoAnother(int typ,char* buf,size_t ln)
{
 int rr;
 sendFlag = 0;
 memset(bufW,0,1024);
 bufW[0] = '&';bufW[1] = 'C';
 ptCRA = (CRA*)&bufW[0];
 ptCRA->tp = htons(typ);
 ptCRA->qty= htonl(ln);
 if(i_m_a_SRV) rr = Servsend(bufW,8);    // send Command   1.Phase
 else rr = Clientsend(bufW,8);
 if(rr<0)return rr;
 if(i_m_a_SRV) rr = Servrecv(bufW,8); //    wait for READY 2.Phase
    else rr = Clientrecv(bufW,8);
 if(rr<0)return rr;
 if((bufW[0]=='&')&&(bufW[1]=='R'))
 {
    if(i_m_a_SRV) rr = Servsend(buf,ln);    // send Data   3.Phase
    else rr = Clientsend(buf,ln);
    if(rr<0)return rr;      // ERROR BY send DATA
    if(i_m_a_SRV) rr = Servrecv(bufW,8);  // wait "ANSWER" 4.Phase
    else rr = Clientrecv(bufW,8);
    if(rr<0)return rr;
    if((bufW[0]=='&')&&(bufW[1]=='A')){FlagSD=1;return 1;}
 }
 return 0;
}

// *** END ***
