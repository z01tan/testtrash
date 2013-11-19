//*****************************************************************************
// Общение с Altera: программы "физического" уровня
//                     А.Галкин      07.05.2013
//******************************************************************************
#include "InitAltera.h"
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>  
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <getopt.h>
//#include <linux/i2c-dev.h>
#include "i2c-dev_user.h"
#include <sys/time.h>
#include "mv_tsu_ioctl.h"
#include <termios.h>

//#include <sys/poll.h>


int     altera_fd;
static  uint32_t  speed = 2000000;
static  uint8_t mode = 0;


#define ALTERA_SPI_DEVICE "/dev/spidev0.0"
#define ALTERA_ADC_DEVICE "/dev/spidev0.1"
#define EN_LED "/sys/class/gpio/EN_LED/value"
#define ADC_I2C_ADDR 0x48
#define I2C_DEV "/dev/i2c-0"

#define Kn1_f   "/sys/class/gpio/KNOPKA1/value"
#define Kn2_f   "/sys/class/gpio/KNOPKA2/value"



#define Keyb_f "/dev/input/event0"

#define TTY     "/dev/ttyS1"

#define DEFAULT_BACKLIGHT  400;   ///511

mdirect motor_direction;
sol     solenoid;
int     backlight;
tesm    test_mode;
clbr    calibrate_mode;

uint8_t  calibration_line_skip = 10;
uint16_t calibration_data[LINE_LENGTH];



 int    NO_PAPER ;
 int    ONE_PAPER ;
 int    TWO_PAPER ;

 int    Optron[4] ;// 0самый нижний,1и2 правый и левый,3 -перед скан-линейкой!!!
 int    Buttons[5];// Массив кнопок(задних и терминальных) НУЖНО нулить после испоьзования!
 int    BackBut[5];// Рабочие массив кнопок.Нулить после использования
 int    but1,but2;  
 
 bool   KeyisReady;   // наличие клавиатуры!!
 bool   MotorFuS;   // запущен мотор вперед со сканированием
 
 int    kfd;
 

struct termios tty, savetty;
int    fdTTY;

int     max_brightness = 0x00;


//------------------------------------------------------------------------------
void pabort(const char *s)
{
	perror(s); 
   //     std::cout <<"";
	abort();
}
//------------------------------------------------------------------------------
//******************utils******************************************************
uint16_t float_to_8_8(double x)
{
        uint16_t ret;
        uint8_t p1;
        uint8_t p2;
        
        p1 = x;
        p2 = (x-p1)*255;
        ret = (p1<<8)|p2;
        return ret;
}
uint16_t calculate_mult(uint8_t black_level)
{
        double mult;
        uint16_t ret;
        
        mult = 255/(255-(double)black_level);
        ret = float_to_8_8(mult);
        return ret;
}
//*****************************************************************************
void save_calibration_data(char * line_buffer)
{
        int fd;
        
        if(calibrate_mode == MODE_CALIBRATE_INIT)
        {
                if(calibration_line_skip>=0)
                        calibration_line_skip--;
                if(calibration_line_skip==0){
                        fd = creat(CALIBRATION_FILE, O_WRONLY);
                        if(fd < 0)
                                pabort("Error: cannot write calibration data\n");
                        write(fd, line_buffer, LINE_LENGTH);
                        close(fd);
                }
        }
        
}
//-----------------------------------------------------------------------------
int load_calibration_data(void)
{
        int fd;
        fd = open(CALIBRATION_FILE, O_RDONLY);
        if(fd < 0){
                memset(calibration_data, 0x00, LINE_LENGTH);
                printf("\nCALIBRATION_FILE NOT Found\n");
                return -1;
        }
        read(fd, calibration_data, LINE_LENGTH);
        close(fd);
        return 0;
}
void set_calibration_data(void)
{ int i;
        for(i=0; i<LINE_LENGTH; i++){
              altera_write(altera_fd, 0x800+i, calibration_data[i]<<8); //?????
                }
        for(i=0; i<LINE_LENGTH; i++){
              altera_write(altera_fd, 0x1000+i, calculate_mult(calibration_data[i])); }       
}
//*****************************************************************************
//-----------------------------------------------------------------------------
void int_ioctl(int dev_fd)
{
        struct tsu_buff_info g_buff_info;
        unsigned int g_frequency = 10000000;

        if(ioctl(dev_fd,MVTSU_IOCBUFFPARAMGET,&g_buff_info) < 0){
                fprintf(stderr,"Error reading device buffer information.\n");
                return;
        }

        if(ioctl(dev_fd,MVTSU_IOCCLEARSTAT,0) < 0) {
                fprintf(stderr,"Error Clearing statistics.\n");
                return;
        }

        if(ioctl(dev_fd, MVTSU_IOCFREQSET,&g_frequency) < 0) {
                fprintf(stderr,"Error configuring port frequency.\n");
                return;
        }
}
//-------------------------ALtera-----------------------------------------------
int open_device(const char *device)
{
   int ret;
   int fd;
        
        fd = open(device, O_RDWR);
        ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
        ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
        if(ret<0) pabort("Speed WR");
        ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
        if(ret<0) pabort("Speed RX");
        return fd;
}

void close_device(void)
{
        close(altera_fd);
}

uint16_t altera_read(int fd, uint16_t addr)
{
        char tx[4] = {0xFF, 0xFF, 0xFF, 0xFF};
        char rx[sizeof(tx)];
        int ret;
        
        struct spi_ioc_transfer tr = {
			tr.tx_buf = (unsigned long)tx,
			tr.rx_buf = (unsigned long)rx,
			tr.cs_change=1,
			tr.len = sizeof(tx),
        };
        tx[0] = (1<<7)| ((addr>>8)&0x7F);
        tx[1] = addr & 0xFF;
        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if(ret<0)
                pabort("spi REG read error\n");
        return (rx[2]<<8)|(rx[3]);
}

uint16_t altera_write(int fd, uint16_t addr, uint16_t data)
{
        char tx[4] = {0xFF, 0xFF, 0xFF, 0xFF};
        char rx[sizeof(tx)];
        int ret;
        
        struct spi_ioc_transfer tr = {
			tr.tx_buf = (unsigned long)tx,
			tr.rx_buf = (unsigned long)rx,
			tr.cs_change=1,
			tr.len = sizeof(tx),
        };
        tx[0] = (0<<7)| ((addr>>8)&0x7F);
        tx[1] = addr&0xFF;
        tx[2] = data>>8;
        tx[3] = data&0xFF;
        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if(ret<0)
  //              pabort("spi REG write error\n");
        {  printf("spi REG write error\n");return -1;}
        return (rx[2]<<8)|(rx[3]);
}
//------------------------------------------------------------------------------
void altera_motor(mdirect MD)     /* Регистр 1 */
{uint16_t tmp;
int refr;
        tmp = 0;
        refr=3;
        switch(MD){
        case MOTOR_FORWARD:
                tmp|=(1<<0);
                MotorFuS = true;
                printf("FORWARD SCAN\n");
                break;
        case MOTOR_FORWARD_NO_SCAN:
                tmp|=(1<<1);
                printf("FORWARD\n");
                break;
        case MOTOR_BACKWARD:
                tmp|=(1<<2);
                printf("BACKWARD\n");
                break;
        case MOTOR_STOP:
                MotorFuS = false;
                printf("STOP\n");
                break;
         default:;   
        }
        if(solenoid == SOL_ON)
                tmp|=(1<<3);
        while(refr>0){
         if( altera_write(altera_fd, 0x01, tmp)<0){refr--;usleep(10);}
         else return;
        }
   //    altera_write(altera_fd, 0x01, tmp); 
}

void altera_testMode(tesm TMD)      /* Регистр 2 */
{
  int tmp;
  tmp = 0;
  switch (TMD){
        case TEST_NONE:
                printf(" NONE\n");
                break;
        case TEST_SAW:
                printf(" SAWTOOTH\n");
                tmp|=(1<<0);
                break;
        case TEST_MEDIAN:
                printf(" MEDIAN\n");
                tmp|=(1<<1);
                break;
         default:;  
        }
        altera_write(altera_fd, 0x02, tmp);
}

void altera_backlight(int r,int g, int b)
{
     altera_write(altera_fd, 0x04, r);
     altera_write(altera_fd, 0x05, g);
     altera_write(altera_fd, 0x06, b);
}
//-----------------------------i2c----------------------------------------------
int i2c_open(void)
{
   int fd;

        fd = open( I2C_DEV, O_RDWR );
        if(fd<0){
                printf("Can not open i2c bus %s\n", I2C_DEV);
                abort();
        }
        return fd;
}

void i2c_close(int fd)
{
        close(fd);
}

int i2c_set_addr(int fd, int addr)
{
        if( ioctl( fd, I2C_SLAVE, addr ) < 0 )
        {
                fprintf( stderr, "Failed to set slave address: %m\n");
                return 2;
        }
        return 0;
}

uint8_t i2c_read(int fd, uint8_t reg_addr)
{
   int ret;

        ret = i2c_smbus_read_byte_data(fd, reg_addr);
        if(ret == -1){
                printf("i2c read error\n");
                abort();
        }
        return ret;
}
//---------------------------ADC------------------------------------------------

void adc_write(int fd, char * data)
{
        char rx[2];
        int ret;
        
        struct spi_ioc_transfer tr = {
			tr.tx_buf = (unsigned long)data,
			tr.rx_buf = (unsigned long)rx,
			tr.cs_change=1,
			tr.len = 2,
        };
        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if(ret<0)
                pabort("spi adc read error\n");
}

void init_adc(void)
{
        int adc_fd;
        char settings[2]={0, 0x60};
	char mux[2]={0x10, 0x70};
        
        printf("initializing adc\n");
        adc_fd = open_device(ALTERA_ADC_DEVICE);
        adc_write(adc_fd, settings);
        adc_write(adc_fd, mux);
        close(adc_fd);
}
//------------------------end of ADC--------------------------------------------
void adc_read(void)     // Чтение состояния Оптронов
{
   int fd;
   uint8_t ret;
   int i;
        fd = i2c_open();
        i2c_set_addr(fd, ADC_I2C_ADDR);
        for(i=0; i<4; i++){
            ret = i2c_read(fd, (uint8_t)i);
            Optron[i] = ret;
            }
        i2c_close(fd);
}
int Led_opt(char* ON)   // Вкл/Выкл светодиодов оптронов
{
  int fd;
  fd = open(EN_LED,O_WRONLY);
   if (fd<0)return(-1);
   write(fd,ON,1);
  close(fd);
  return(0);
}
//-----------------------------------------------------------------------------
//------------------------Back Buttons-----------------------------------------
int butt_open(void)
{
   but1 = open(Kn1_f,O_RDONLY | O_NONBLOCK);
   if(but1<0)return (-1);
   but2 = open(Kn2_f,O_RDONLY | O_NONBLOCK);
   if(but2<0)return (-1);
   return 0;
}

void butt_close(void)
{
    close(but1);
    close(but2);
}

int butt_get(int* b1, int* b2)
{
    int res;
    char a;
    long offset = 0L;
    lseek(but1, offset, SEEK_SET);
    res = read(but1,&a,1);
    if(res<0)return -1;
    *b1 = a-'0';
    lseek(but2, offset, SEEK_SET);
    res = read(but2,&a,1);
    if(res<0)return -1;
    *b2 = a-'0';
    return 0;
}
//----------------------End of Buttons------------------------------------------
//---------------------Keyboard-------------------------------------------------
int      open_Key(void) // 
{
    kfd = open(Keyb_f,O_RDONLY | O_NONBLOCK);
    if (kfd<0)return 0;
    return 1;
}
//---------------------End of KeyBoard Open-------------------------------------
//------------------------TERMINAL----------------------------------------------
int openTTY(void)
{
  char port[11];
  char* pport = &port[0];
  
   memmove(pport,"/dev/ttyS1",11);
   fdTTY = open ( pport, O_RDWR | O_NONBLOCK);  
   if (fdTTY == -1) { printf("open_portTTY: Unable to open %s. \n", port);return(-1);}
                
   fcntl(fdTTY, F_SETFL, 0);
   printf("TTY-port is opened!!\n");
   tcgetattr(fdTTY, &savetty); // получили структуру termios
 //  savetty = tty; // сохранили
  // tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	// ISIG - when any of the characters  INTR,  QUIT,  SUSP,  or DSUSP are received, generate the corresponding signal.
	// ICANON - enable canonical mode.  This  enables  the  special characters  EOF,  EOL,  EOL2, ERASE, KILL, REPRINT, STATUS, and WERASE, and buffers by lines.
   fcntl(fdTTY, F_SETFL, 0);
   cfsetispeed(&tty, B115200);
   cfsetospeed(&tty, B115200);
   tty.c_cflag &= ~PARENB;
   tty.c_cflag &= ~CSTOPB;
   tty.c_cflag &= ~CSIZE;
   tty.c_cflag |= CREAD;
   tty.c_cflag |= CS8;
   
  // tty.c_ospeed = B115200;
  // tty.c_ispeed = B115200;
 //  tty.c_cc[VMIN] = 0;
 //  tty.c_cc[VTIME]= 0;     // Unblocked mode ohne WAIT!!!!!!
   tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
   cfmakeraw(&tty);
   tcsetattr(fdTTY, TCSANOW, &tty);
   return 1;
}

void closeTTY()
{
   tcsetattr(fdTTY, TCSANOW, &savetty);
   close(fdTTY); 
}
void resetTTY()
{
      long offset = 0L;
      lseek(fdTTY, offset, SEEK_SET);
}

int readTTY(char * a,int size)
{ 
  int r;
  r = read(fdTTY, a, size);
  if (r == 0)*a = 0x00;
  return(r);  
}   

int writeTTY(char* cmd, int size)
{ int r;
r = write(fdTTY,cmd,size);
if (r<0) return(-1);
return(1);
}
//----------------------End of TTY----------------------------------------------
//==============================================================================
int Init_Altera()
{ 
  int i; 
  char led;
  
 motor_direction = MOTOR_STOP;
 solenoid = SOL_OFF;
 backlight = DEFAULT_BACKLIGHT;
 test_mode = TEST_NONE;
 calibrate_mode = MODE_NORMAL;
 
 NO_PAPER  = 240;
 ONE_PAPER  = 120;
 TWO_PAPER  =  80;
 for (i=0;i<4;i++)Optron[i] = 0;
 for (i=0;i<5;i++)Buttons[i]= 0;
    
  //  ScanInProg = 0;
    adc_read();
    printf("adc_data OK\n");
    init_adc();
    printf("init_adc OK\n");
    altera_fd = open_device(ALTERA_SPI_DEVICE);// for close call: close_device();
 // Initial calibration data BlackLevel & Koeff
    if (load_calibration_data()==0) set_calibration_data();
    else{
         for(i=0; i<LINE_LENGTH; i++)
          { altera_write(altera_fd, 0x800+i, 0x0);}
         for(i=0; i<LINE_LENGTH; i++)
          { altera_write(altera_fd, 0x1000+i, 0x100); }
    }
   
    led = '1';
    if(Led_opt(&led))pabort("Led Error!!!!\n");// зажгли опторары!!!!!
     altera_write(altera_fd, 0x01, 0);  // stop motor!
     altera_write(altera_fd, 0x02, 0);   // Test zero!
     altera_write(altera_fd, 0x04, backlight);
     altera_write(altera_fd, 0x05, backlight);
     altera_write(altera_fd, 0x06, backlight);  
      printf("BlackLevel write OK\n");
     if(openTTY() < 0) printf("NO Terminal!!!!!\n");
     return 0;
}