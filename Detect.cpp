// -------------------written by GAI--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "HeaderMain.h"
#include "Detect.h"

// ---------------------------------------------------------------------------
// Global settings !!!!!!!!!!!!!!!!!!

int SymWidth ;//= 24; // ??
int SymHeigt ;//= 64; // ??
int SymHeigtT; // ??

int SymDist[3] ;//{36,36,36};

int SymDIST0 ;  // !!!!!!!!!!!!!!!!!!NEW CONST FROM INPUT DATA !!!!!!!!!

int Richtung = 1; // if richtung==0 then "end befor begin"
int StZ[4]; // "0","9","8","9";
// ---------------------------------------------------------------------------
int X0, Y0;
int XT, YT;
int XR, XR_B;

int Ziff;

int SgBs[5][2];
int SgEs[5][2];
int LnWd[5][2];
int LnCnt[5];
int Sign[5];

int RowSym[44]; // Row of Symbol for analyses
int Row1[44];

int Gewicht[4] = {1000, 100, 10, 1};
int Ziffer[10][5] = { {7, 3, 3, 3, 7}, // 0
	{1, 5, 3, 1, 1}, // 1
	{7, 1, 1, 4, 7}, // 2
	{7, 4, 7, 4, 2}, // 3
	{3, 3, 7, 1, 1}, // 4
	{7, 2, 7, 1, 7}, // 5
	{1, 4, 7, 3, 7}, // 6
	{7, 4, 2, 2, 2}, // 7
	{7, 3, 7, 3, 7}, // 8
	{7, 3, 7, 4, 2}}; // 9

int Biffer[10][5] = { {7, 3, 3, 3, 7}, // 0
	{2, 2, 3, 6, 2}, // 1
	{7, 4, 2, 2, 7}, // 2
	{1, 4, 7, 4, 7}, // 3
	{2, 2, 7, 3, 3}, // 4
	{7, 2, 7, 1, 7}, // 5
	{7, 3, 7, 4, 2}, // 6
	{1, 1, 1, 4, 7}, // 7
	{7, 3, 7, 3, 7}, // 8
	{1, 4, 7, 3, 7}}; // 9

double StAngle;

//
// +++++++++++++ S T A M P  D E T E R M I N A T I O N +++++++++++++++++++++++++
//
// -------------------------------------------------------------------------

// ++ по координатам нижнего угла рамки и наклону штампа вычислим X0,Y0 ++++++
void  Transform(int a, int b, double alfa)
{
	double w, h;
	if (Richtung == 1) // в прямом направлении
	{
		w = 4.0 * NumDotX;
		h = 11.5 * NumDotY * KoefY; // 11.0 * NumDotY;

		X0 = (a) + (int)((w * cos(alfa)) - (h * sin(alfa)) + 0.5); // (a+6)
		Y0 = (b - 2) - (int)(h * cos(alfa) + w * sin(alfa) + 0.5);
	}
	else // в обратном направлении
	{
		w = 4.0 * NumDotX;
		h = 3.0 * NumDotY * KoefY; // 3.25 * NumDotY;

		X0 = (a) + (int)((w * cos(alfa)) + (h * sin(alfa)) + 0.5);
		Y0 = (b - 2) + (int)(h * cos(alfa) - w * sin(alfa) + 0.5);
	}
}
// ----------------------------------------------------------------------------

// ++++++++++++ дать строку текущего символа с переводом в ч/б +++++++++++++++++
void getRowSym(int k) {
	int i, x, y, dd;
	double d;
	for (i = 0; i < 40; i++) // (i = 4; i < 44; i++)
	{
		d = i;
		x = (XT) + (int)(d * cos(StAngle) + 0.5);
		y = YT + k - (int)(d * sin(StAngle) * KoefY + 0.5);
		urw_ch=GetXYValue(y,x + (int)(k * sin(StAngle) + 0.5));
		dd = urw_ch;
		// обращение  к функции  принятия решения о наличии  черного  в  точке
		g0 = RecBlack(alfa, beta, urw, surw, urbl, surbl, dd, &p00, &p11);
		// g0 == 1  - черная!!!
		RowSym[i] = g0;
		// RowSym[i] = (dd<105 ? 1:0);
	}
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ----------------------------------------------------------------------------

// ============================================================================
int FindFirst() {
	int i, k, sm, pr;
	k = 0;
	pr = 0;
	while (k < 20) {
		getRowSym(k); // Запрашиваем строку с учетом наклона
		sm = 0;
		for (i = 0; i < SymWidth+20; i++) {      //вместо 44 SymWidth=24 + 16+4 ??????
			if (RowSym[i] == 1)
				sm++;
			if (i > 1 && (RowSym[i - 2] == 1))
				sm--;
			if (sm > 1) {
				if (pr == 0) {
					k = k - 8;
					break;
				}
				else
					return (k);
			} // if (sm > 1)
		} //for (i...
		k++;
		pr = 1;
	}
	return (-15);
}

// ------------------------------------------------------------------------------
int FindLast() {
	int i, k, sm, pr;
	k = 0;
	pr = 0;
	while (k < 10) {
		getRowSym(SymHeigt - k); // Запрашиваем строку с учетом наклона
		sm = 0;
		for (i = 0; i < SymWidth+20; i++) {  //вместо 44 SymWidth=24 + 16+4 ??????
			if (RowSym[i] == 1)
				sm++;
			if (i > 3 && (RowSym[i - 4] == 1))
				sm--;
			if (sm > 1) {
				if (k > 0)
					return (k);
				if (pr == 0) {
					k = k - 8;
					break;
				}
			} // if (sm > 1)
		} // for (i = 0;
		k++;
		pr = 1;
	}
	return (-15);
}

// ------------------------------------------------------------------------------
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++		 Устанавливаем коорд ЛевВерхУгла (LUC) след.Символа  ++++++++++++++
void GetNext(int next) {
	int i, k;
	double x, y;

	if (next == 0) {
		XT = X0; // -2
		YT = Y0;
		k = FindFirst();
		if (k > -15)
			YT = YT + k;
		i = FindLast();
		if (i > -15)
			SymHeigtT = SymHeigt - i;
	} // if (next == 0)
	else {
		x = X0;
		y = Y0;
		for (i = 0; i < next; i++) {
			x = x + (SymDist[i] * cos(StAngle)); // es war 36.0  !!
			y = y - (SymDist[i] * sin(StAngle));
		} // for (i = 0;
		XT = (int)(x + 0.5); // -2
		YT = (int)y;
		k = FindFirst();
		if (k > -15)
			YT = YT + k;

	} // else
}

// -----------------------------------------------------------------------------
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++ 					DETECT FUNCTION! 						  ++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int DetectSign(int next) {
	int i, n, k, sm, pr, ptB, ptE;
	int BegH;//, nn,Nsgn;
	int Begin[3], End[3], cnt[3];

	for (n = 0; n < 5; n++) // 5 сигнатур
	{
		LnCnt[n] = 0;
		BegH = ((SymHeigtT+2)/4 * n);

		if (n > 0)
			BegH = BegH - 1;
		if (n == 4)
			BegH = SymHeigtT - 3;

		for (k = 0; k < 3; k++) {
			Begin[k] = 0;
			End[k] = 0;
			cnt[k] = 0;
		} // почистили!
		for (i = 0; i < 44; i++)
			Row1[i] = 0; // почистили!

		for (k = 0; k < 4; k++) // по 3(4?) строки на анализ
		{
			getRowSym(BegH + k);
			for (i = 0; i < 44; i++) // i=4
			{
				if (RowSym[i] == 1) Row1[i] = 1;
			} // for 44 1. mal

		} // for  строки на анализ

		sm = 0;
		pr = 0;
		ptB = ptE = 0;
		for (i = 0; i < 44; i++) // i=4
		{
			if (Row1[i] == 1)
				sm++;
			if (i > 3 && (Row1[i - 4] == 1))
				sm--;
			if (sm >= 2 && pr == 0) // начало пачки
			{
				Begin[ptB] = (i - 2);
				pr = 1;
				if (ptB < 2)
					ptB++;
			}
			if (pr == 1 && sm < 1) // конец пачки
			{
				End[ptE] = (i - 4);
				cnt[ptE]++;
				pr = 0;
				if (ptE < 2)
					ptE++;
			}
		} // for 44 2. mal


		if (ptB > ptE)
			ptB = ptE;

		for (i = 0; i < 2; i++) {
			if (End[i] > 0)
			{
				if (Begin[i] >= 36) break;
				if (i==1 && (Begin[1]-Begin[0])>=30) break;
				SgBs[n][LnCnt[n]] = Begin[i];
				if (End[i] - Begin[0] > 30)
					End[i] = Begin[0] + 30;
				SgEs[n][LnCnt[n]] = End[i];
				LnCnt[n]++;

			}

		} // цикл по отрезкам

	} // for n    по сигнатурам!
	ptB = 32;
	ptE = 0;
	for (n = 0; n < 5; n++) {
		for (i = 0; i < LnCnt[n]; i++) {
			if (ptB > SgBs[n][i])
				ptB = SgBs[n][i];
			if (ptE < SgEs[n][i])
				ptE = SgEs[n][i];
			LnWd[n][i] = SgEs[n][i] - SgBs[n][i]; // длины линий!!
		}
	} // ------------------------------------------
	sm = ptE - ptB; // ШИРИНА СИМВОЛА
	for (n = 0; n < 5; n++) {
		if ((LnCnt[n] == 2) && ((SgBs[n][1] - ptB) > 34)) {
			LnCnt[n] = 1;
		} // !!!!!!!!!!!!!
	} // for n
	// -----------------------------------------------------------------
	if (next < 3 && ptE > SymDist[next])
		SymDist[next] = ptE + 2; // корректировка смещения по Х
	//
	for (n = 0; n < 5; n++) {
		pr = LnCnt[n];
		switch (pr) {
		case 1: {
				if (LnWd[n][0] >= (sm * 0.8)) {
					Sign[n] = 7;
					break;
				}
				if (LnWd[n][0] >= (sm / 2 + 2) && LnWd[n][0] <
					(sm * 0.8) && SgEs[n][0] > ptE - 3) {
					Sign[n] = 5;
					break;
				}
				if (LnWd[n][0] >= (sm / 2 + 2) && LnWd[n][0] <
					(sm * 0.8) && SgBs[n][0] > ptB - 3) {
					Sign[n] = 6;
					break;
				}
				if (LnWd[n][0] < sm / 2 && SgBs[n][0] > (ptB + sm/2)) {
					Sign[n] = 1;
					break;
				}
				if (LnWd[n][0] < sm / 2 + 3 && SgEs[n][0] > (ptB + sm/2)) {
					Sign[n] = 4;
					break;
				}
				if (LnWd[n][0] < sm / 2 && SgBs[n][0] > (ptB - 3)) {
					Sign[n] = 2;
					break;
				}
			}
		case 2: {
				if ((LnWd[n][0] <= sm / 2 && SgBs[n][0] > ptB - 3) &&
					(LnWd[n][1] <= sm / 2 && SgBs[n][1] > sm / 2 + 2)) {
					Sign[n] = 3;
					break;
				}
				if ((LnWd[n][0] <= sm / 2 && SgBs[n][0] > ptB - 3) &&
					(LnWd[n][1] <= sm / 2 + 3 && SgEs[n][1] > ptB + sm / 2)) {
					Sign[n] = 6;
					break;
				}
				if ((LnWd[n][0] <= sm / 2 && SgEs[n][0] > ptB + sm / 2) &&
					(LnWd[n][1] <= sm / 2 && SgBs[n][1] > ptB + sm / 2 + 2)) {
					Sign[n] = 5;
					break;
				}
			}
		default:
			return (-15);
		}

	}
	return (0);
}

// ------------------------------------------------------------------------------
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ****               Define Symbol                             ********
// nz - номер цифры для обработки
// Arr- pointer to signature array
// Выход: 		n - цифра!!!, -15 - ОШИБКИ!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int DefineSym(int nz, Signum Arr) {
	int i, k;
	int pr;
	if (DetectSign(nz) < 0)
		return (-15);
	pr = 0;
	for (k = 0; k < 10; k++) {
		for (i = 0; i < 5; i++) {
			if (Arr[k][i] != Sign[i]) {
				pr = 1;
				break;
			}
		}
		if (pr == 0)
			return (k);
		pr = 0;
	}
	return (-15);
}
// ------------------------------------------------------------------------------


// ++++++++++++++++ Подтвердить очередную цифру    ++++++++++++++++++++++++++++
// Выход: 		1 - подтвердили!!!, 0 - ОШИБКИ!
int DetectOne() {
	int Num;
	if (Richtung == 1) {
		Num = DefineSym(Ziff, Ziffer);
		if (Num >= 0) {
			StZ[Ziff] = Num;
			return (Ziff+1);
		}
		else {
			return (0);
		}
	}
	if (Richtung == 0) {
		Num = DefineSym(Ziff, Biffer);
		if (Num >= 0) {
			StZ[3 - Ziff] = Num;
			return (Ziff+1);
		}
		else {
			return (0);
		}
	}
	return (0);
}
// ---------------------------------------------------------------------------

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// подтверждаем штамп. 	Параметр: 	1-прямое движение, 0 - обратное
// Выход: 		0 - подтвердили!!!, >0 - ОШИБКИ!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int tryConfirmStamp(int trend) {
	int DetS, Ret, i;
	Richtung = trend; // !!!!!!!!!!!!!!!
	StAngle = -AlfaSh;
	Transform(XoShL, YoShL, StAngle);
	for (i = 0; i < 4; i++)
		StZ[i] = 0;
	for (i=40; i<44; i++)
            RowSym[i] = 0;
	for (i = 0; i < 3; i++) SymDist[i] = SymDIST0;
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1

	DetS = 0;
	for (i = 0; i < 4; i++) {
		Ziff = i; // Ziff - порядковый номер анализир.цифры
		GetNext(Ziff); // ВСЕГДА слева направо!
		DetS = DetS + DetectOne();
	}
	if  (DetS==10)   //(DetS == 4)
	 {
		Ret = 0;
		for (i = 0; i < 4; i++)
			Ret = Ret + StZ[i] * Gewicht[i];
			return (Ret);
	}
	else {
		return (DetS - 10);  //-4
	}


}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// *******************************************************************************
