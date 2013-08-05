// ---------------------------------------------------------------------------

//#pragma hdrstop
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "HeaderMain.h"
#include "Detect.h"

// ---------------------------------------------------------------------------
//#pragma package(smart_init)

// Global settings !!!!!!!!!!!!!!!!!!

int SymWidth = 18; // ??
int SymHeigt = 64; // ??
int SymHeigtT; // ??

int SymDist[3] = {36, 36, 36};
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
	//int s;
	double w, h;
	if (Richtung == 1) // в прямом направлении
	{
		w = 4.0 * NumDotX;
		h = 11.5 * NumDotY; // 11.0 * NumDotY;

		X0 = (a) + (int)((w * cos(alfa)) - (h * sin(alfa)) + 0.5); // (a+6)
		Y0 = (b - 2) - (int)(h * cos(alfa) + w * sin(alfa) + 0.5);
	}
	else // в обратном направлении
	{
		w = 4.0 * NumDotX;
		h = 3.0 * NumDotY; // 3.25 * NumDotY;

		X0 = (a) + (int)((w * cos(alfa)) + (h * sin(alfa)) + 0.5);
		Y0 = (b - 2) + (int)(h * cos(alfa) - w * sin(alfa) + 0.5);
	}
#ifdef AUSTESTEN
	Form2->Edit15->Text = IntToStr(X0);
	Form2->Edit16->Text = IntToStr(Y0);
#endif
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
		y = YT + k - (int)(d * sin(StAngle) + 0.5);
		/* ! */ //Color = Form1->Image2->Picture->Bitmap->Canvas->Pixels
		//	[x + (int)(k * sin(StAngle) + 0.5)][y];
	   //	urw_ch = (GetRValue(Color) * 0.3 + GetGValue(Color) * 0.59 +
		//	GetBValue(Color) * 0.11);
		//urw_ch=GetXYValue(x + (int)(k * sin(StAngle) + 0.5),y);
		urw_ch=GetXYValue(y,x + (int)(k * sin(StAngle) + 0.5));

		dd = urw_ch;
		// обращение  к функции  принятия решения о наличии  черного  в  точке
		g0 = RecBlack(alfa, beta, urw, surw, urbl, surbl, dd, &p00, &p11);
		// g0==1 черная!!!
		RowSym[i] = g0;
		// RowSym[i] = (dd<105 ? 1:0);
	}
}
// ------------------------------------------------------------------------------

// ++++++++++++++ rotating characters +++++++++++++++++++++++
// Уже не нужна !!!!!!!!!!!!!!!!!
#ifdef AUSTESTEN

void TurnSym(int a, int b) // a & b - coordinates of Symbol (LUC)
{
	int x, y, i, k, dd;
	double d;
	a = a;
	b = b; // b - 4;
	// for (x = 0; x < 80; x++)
	// for (y = 0; y < 40; y++)
	// Area[x][y] = 0;
	for (k = 0; k <= SymHeigt + 8; k++) {
		for (i = 0; i < 44; i++) // = SymWidth + 8
		{
			d = i;
			x = a + (int)(d * cos(StAngle));
			y = b + k - (int)(d * sin(StAngle));
			Color = Form1->Image2->Picture->Bitmap->Canvas->Pixels
				[x + (int)(k * sin(StAngle))][y];
			urw_ch = (GetRValue(Color) * 0.3 + GetGValue(Color) * 0.59 +
				GetBValue(Color) * 0.11);
			dd = urw_ch;
			// обращение  к функции  принятия решения о наличии  черного  в  точке
			g0 = RecBlack(alfa, beta, urw, surw, urbl, surbl, dd, &p00,
				&p11); // g0==1 черная!!!

			Form1->Image2->Picture->Bitmap->Canvas->Pixels[100 + i][200 + k] =
				(g0 == 1 ? 0 : 0xFFFFFF);
			// Area[k][i] = g0;
		}
	}
}
#endif
// ----------------------------------------------------------------------------

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ----------------------------------------------------------------------------

// ============================================================================
int FindFirst() {
	int i, k, sm, pr;
	// int si[4];
	k = 0;
	pr = 0;
	while (k < 20) {
		getRowSym(k); // Запрашиваем строку с учетом наклона
		sm = 0;
		for (i = 0; i < 44; i++) {
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
			}
		}
		k++;
		pr = 1;
	}
	return (-15);
}

// ------------------------------------------------------------------------------
int FindLast() {
	int i, k, sm, pr;
	// int si[4];
	k = 0;
	pr = 0;
	while (k < 10) {
		getRowSym(SymHeigt - k); // Запрашиваем строку с учетом наклона
		sm = 0;
		for (i = 0; i < 44; i++) {
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
			}
		}
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
#ifdef AUSTESTEN
	Form2->Label14->Caption =
		"# " + IntToStr((Richtung == 1 ? next : 3 - next));
	Form2->Label14->Color = clBtnFace;
#endif

	if (next == 0) {
		XT = X0; // -2
		YT = Y0;
#ifdef AUSTESTEN
		Form2->Memo1->Lines->Add("XTn=" + IntToStr(XT) + " YTn=" +
			IntToStr(YT));
#endif
		k = FindFirst();
#ifdef AUSTESTEN
		Form2->Memo1->Lines->Add("FFirst k=" + IntToStr(k) + " YTn=" +
			IntToStr(YT));
#endif
		if (k > -15)
			YT = YT + k;
		i = FindLast();
		if (i > -15)
			SymHeigtT = SymHeigt - i;

#ifdef AUSTESTEN
		TurnSym(XT, YT); // for Debug only
#endif
	}
	else {
		x = X0;
		y = Y0;
		for (i = 0; i < next; i++) {
			x = x + (SymDist[i] * cos(StAngle)); // es war 36.0  !!
			y = y - (SymDist[i] * sin(StAngle));
		}
		XT = (int)(x + 0.5); // -2
		YT = (int)y;
#ifdef AUSTESTEN
		Form2->Memo1->Lines->Add("XTn=" + IntToStr(XT) + " YTn=" +
			IntToStr(YT));
#endif
		k = FindFirst();
		if (k > -15)
			YT = YT + k;
#ifdef AUSTESTEN
		TurnSym(XT, YT); // for Debug only
#endif
	}
}

// -----------------------------------------------------------------------------
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++ 					DETECT FUNCTION! 						  +++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int DetectSign(int next) {
	int i, n, k, sm, pr, ptB, ptE;
	int BegH;//, nn,Nsgn;
	int Begin[3], End[3], cnt[3];
	//System::UnicodeString ss, mm;
    //AnsiString ss, mm;
#ifdef AUSTESTEN
	Form2->Memo1->Lines->Clear();
	Form2->Memo1->Lines->Add("++++++++++++++++++++");
	Form2->Memo1->Lines->Add("XT=" + IntToStr(XT) + " YT=" + IntToStr(YT));
	Form2->Memo1->Lines->Add("SymHeigt=" + IntToStr(SymHeigtT));
#endif

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
#ifdef AUSTESTEN
			Form2->Memo1->Lines->Add("BegH+k=" + IntToStr(BegH + k));
#endif

			getRowSym(BegH + k);
			//ss = "";
			for (i = 0; i < 44; i++) // i=4
			{
#ifdef AUSTESTEN
				if (RowSym[i] == 0)
					ss = ss + "_";
				else
					ss = ss + "1";
#endif
				if (RowSym[i] == 1)
					Row1[i] = 1;
			} // for 44 1. mal

#ifdef AUSTESTEN
			Form2->Memo1->Lines->Add("symb=" + ss);
#endif
		} // for  строки на анализ

		sm = 0;
		pr = 0;
		ptB = ptE = 0;
		//ss = "";
		for (i = 0; i < 44; i++) // i=4
		{
#ifdef AUSTESTEN
			if (Row1[i] == 0)
				ss = ss + "_";
			else
				ss = ss + "1";
#endif
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

#ifdef AUSTESTEN
		Form2->Memo1->Lines->Add("Row1=" + ss);
#endif
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
#ifdef AUSTESTEN
				Form2->Memo1->Lines->Add("*Beg=" + IntToStr(Begin[i]) +
					"  *End=" + IntToStr(End[i]));
#endif
			}

		} // цикл по отрезкам
#ifdef AUSTESTEN
		Form2->Memo1->Lines->Add("****LnCnt=" + IntToStr(LnCnt[n]));
#endif

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
#ifdef AUSTESTEN
		Form2->Memo1->Lines->Add("****LnCnt_K=" + IntToStr(LnCnt[n]));
#endif
	} // for n
	// -----------------------------------------------------------------
	if (next < 3 && ptE > SymDist[next])
		SymDist[next] = ptE + 2; // корректировка смещения по Х
	//

#ifdef AUSTESTEN
	Form2->Memo1->Lines->Add("****sm=" + IntToStr(sm));
#endif
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
#ifdef AUSTESTEN
		Form2->Memo1->Lines->Add("Sign=" + IntToStr(Sign[n]));
#endif
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

// ----------------------------------------------------------------------------
void StOut() {
#ifdef AUSTESTEN
	int oo;
	oo = (Richtung == 1 ? Ziff : 3 - Ziff);
	switch (oo) {
	case 0: {
			Form2->Edit8->Text = IntToStr(StZ[0]);
			break;
		}
	case 1: {
			Form2->Edit9->Text = IntToStr(StZ[1]);
			break;
		}
	case 2: {
			Form2->Edit10->Text = IntToStr(StZ[2]);
			break;
		}
	case 3: {
			Form2->Edit11->Text = IntToStr(StZ[3]);
			break;
		}

	default: ;
	}
#endif
}

// ++++++++++++++++ Подтвердить очередную цифру    ++++++++++++++++++++++++++++
// Выход: 		1 - подтвердили!!!, 0 - ОШИБКИ!
int DetectOne() {
	int Num;
	if (Richtung == 1) {
		Num = DefineSym(Ziff, Ziffer);
		if (Num >= 0) {
			StZ[Ziff] = Num;
#ifdef AUSTESTEN
			StOut();
			Form2->Label14->Caption = "Det!!";
			Form2->Label14->Color = clLime;
#endif
			return (Ziff+1);
		}
		else {
#ifdef AUSTESTEN
			Form2->Label14->Caption = "No D";
			Form2->Label14->Color = clRed;
#endif
			return (0);
		}
	}
	if (Richtung == 0) {
		Num = DefineSym(Ziff, Biffer);
		if (Num >= 0) {
			StZ[3 - Ziff] = Num;
#ifdef AUSTESTEN
			StOut();
			Form2->Label14->Caption = "Det!!";
			Form2->Label14->Color = clLime;
#endif
			return (Ziff+1);
		}
		else {
#ifdef AUSTESTEN
			Form2->Label14->Caption = "No D";
			Form2->Label14->Color = clRed;
#endif
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
	for (i = 0; i < 3; i++)
		SymDist[i] = 36;
#ifdef AUSTESTEN
	// !!!!!!!!!Пока !!!!
	Ziff = -1;

	Form2->Edit8->Text = "-";
	Form2->Edit9->Text = "-";
	Form2->Edit10->Text = "-";
	Form2->Edit11->Text = "-";
	Form2->Visible = true;
	return (0);
#endif
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
#ifndef AUSTESTEN
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
		//Form1->Edit4->Text = IntToStr(Ret);

		return (Ret);
	}
	else {
		//Form1->Edit4->Text = IntToStr(DetS - 10); //-4!!
		return (DetS - 10);  //-4
	}
#endif

}
// --------------------------------------------------------------------------

// +++++++++++++++++++ устанавливаем образцы и умолчания в соотв. со штампом
void SetStamp() {
	Ziff = -1;
	// Form2->CheckBox3->Checked = true;
	// Form2->Label14->Color = clBtnFace;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// *******************************************************************************
