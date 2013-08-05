/*
 * File:   HardProc.h
 * Author: andr-u
 *
 * Created on 16 Май 2013 г., 10:29
 */

#ifndef HARDPROC_H
#define	HARDPROC_H


 void Init_COIB();      // инициализация Альтеры и потоков
 void ClearKoeff();     // очистка регистров уровня черного и коеффициентов

 int get_Optrons_Imm(void);// немедленный опрос оптронов. возврат 1 - было изменение
 int get_Optrons_Wait(void);// опрос с ожиданием. возврат 0 -норм. -1 -ошибка.
void waitAllOpenedOpt();    // ожидание, пока все оптроны засветятся
 int Wait_beiden();       // ожидание двух оптронов в течение 0,5 сек. -1, если не дождались!
void waitBothOpt();       // ожидание двух оптронов Для старта двигателя

 int get_Key_Imm(void); // немедленный опрос клавиатуры. возврат 0 -не было нажатия
 int get_Key_Wait(void);// опрос с ожиданием. возврат код клавиши
 int get_KeyString(char* Input); // ввод строки с клавиатуры

 int get_Buttons_Imm(); // если возврат=1, то в Buttons[] обновленные значения;
 int get_Buttons_Wait(); // ожидание нажатия кнопок. в Buttons[] - дейсвительная инфа

 void frontLED(int red,int green); // "0" - off,"9" - automat, "1"-"8" - brightness
 void setOneline(char* txt,int shift);   //установка Одной  строки и вывод текста. shift=1 > бегущая
 void setFirstline(char* txt,int shift); //установка Первой строки и вывод текста. shift=1 > бегущая
 void setSecline(char* txt,int shift);   //установка Второй строки и вывод текста. shift=1 > бегущая
 int getNextScanLine(void);      //возвращает номер готовой для обработки строки в массиве скана
 int Any_Wait();          // ожидание каког-либо события; возврат 1 -оптроны, 2- кнопки, 3- клава


 void Page_scan();      // начать сканирование бланка
 void Page_back();      // вернуть бланк назад
 void Page_backTest();
 void Page_down();      // сбросить бланк в корзину
 void Page_mark();      // отметить бланк перфоратором
 void Page_stop();      // остановить мотор(сканирование)

 void alteraRegCalibr_write(void); // записать калибровку в регисты Альтеры

 int say_aloud(char*a,char*b,char*c,char*d,char*e); // произнести звуки в файлах a,b,c,d,e или NULL
 void setVoiceVolume(int Vol); //установить уровень громкости

 int MakeBinFile(char* nameF,int W, int H);    //создание бинарного файла образа

 void Prov_Line(int ln);// !***************

#endif	/* HARDPROC_H */

