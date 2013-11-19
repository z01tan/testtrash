//**************************
#ifndef A_SOCKET_H
#define A_SOCKET_H
/* ***************************************************************************************
Каждая посылка состоит из 4-х фаз:
1. приказ-команда всегда длиной 8 байт
2. ответ READY 8 байт
3. инфоданные длиной, указанной в теле команды
4. ожидание ответа-подтверждения приема длиной 8 байт.
Формат команды = &C TT LLLL , где ТТ тип данных, LLLL -  длина структуры данных
Формат Ready   = &R TT LLLL
Формат Ответа  = &A TT LLLL

****************************************************************************************** */
extern char buf[1024];// длина буфера должна быть не меньше САМОЙ ДЛИННОЙ СТРУКТУРЫ!!!!

extern int  isConnected;

extern char    bufR[1024],bufW[1024];
extern int     typB;
extern int     lnB; //!!!!!!!!!!!!!!!!!!!
extern char*   bRptr;
extern char*   bWptr;


extern int     totR;
extern int     totW;
extern int     have_Printer;
extern int     sendFlag,recvFlag;
extern int     i_m_a_SRV;  // if ==1 then i'm a Server!!!

int InitSockets (void);  //
void CloseSocket();
int ReadClt     (char* dtg,int ln);
int WriteClt    (char* msg,int ln);
int WriteList   (char* msg,int ln);
int ReadList    (char* msg,int ln);

int CheckList();
int CheckClt();
int Clientsend(char* buff,int length);
int Clientrecv(char* buff,int length);
int Servsend(char* buff,int length);
int Servrecv (char* buff,int length);
int ReceivefromAnother();
int SendtoAnother(int typ,char* buf,size_t ln);

#endif
