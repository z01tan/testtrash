/*
 * File:   InitAltera.h
 * Author: andr-u
 *
 * Created on 29 Апрель 2013 г., 12:07
 */

#ifndef INITALTERA_H
#define	INITALTERA_H

#include <stdint.h>


#define LINE_LENGTH 1728
#define CALIBRATION_FILE "./SetUpFiles/calibration_data.bin"
#define CALIBRATION_KOEF "./SetUpFiles/calibration_koef.bin"
#define CALIBRATION_FILEK "./SetUpFiles/calibration_dataK.bin"

uint16_t float_to_8_8(double x);

extern int      Optron[4] ;// 0-255
extern int      Buttons[6];// 2 задних и 3 передних кнопки
extern int      BackBut[5];// 2 задних кнопки и 3 передних кнопки(рабочие)

extern bool     KeyisReady;
extern int      kfd;

extern enum mdirect{MOTOR_STOP, MOTOR_FORWARD, MOTOR_FORWARD_NO_SCAN, MOTOR_BACKWARD, MOTOR_SIZE} motor_direction;
extern enum sol{SOL_OFF, SOL_ON, SOL_SIZE} solenoid;
extern enum tesm{TEST_NONE, TEST_SAW, TEST_MEDIAN, TEST_SIZE}test_mode;
extern enum clbr{MODE_NORMAL, MODE_CALIBRATE_INIT, MODE_CALIBRATE_BRIGHT}calibrate_mode;

extern int     motorWatch;

extern bool MotorFuS;   // запущен мотор вперед со сканированием
extern uint8_t calibration_data[LINE_LENGTH];

extern int  NO_PAPER ;
extern int ONE_PAPER ;
extern int TWO_PAPER ;

extern int fdTTY;


 void     adc_read(void);
 int      Init_Altera();
 void     pabort(const char *s);
 void     int_ioctl(int dev_fd);

 void     altera_motor(mdirect MD) ;
 void     altera_testMode(tesm TMD);
 void     altera_backlight(int r,int g, int b);
 void     set_calibration_data(void);
 void     save_calibration_data(uint8_t * line_buffer);
 int      load_calibration_data(void);

 int      butt_open(void);
 void     butt_close(void);
 int      butt_get(int* b1, int* b2);

 int      open_Key(void);


 int      openTTY(void);
 void     closeTTY();
 int      readTTY(char* a,int size);
 void     resetTTY();
 int      writeTTY(char* cmd,int size);
 uint16_t altera_read(int fd, uint16_t addr);
 uint16_t altera_write(int fd, uint16_t addr, uint16_t data);
 void     _clear();

#endif	/* INITALTERA_H */

