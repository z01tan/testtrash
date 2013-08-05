// ---------------------------------------------------------------------------

#ifndef DetectH
#define DetectH

//#define   AUSTESTEN


typedef int Signum[10][5];
extern int X0, Y0;

extern int Ziff;
//extern int StZ[4];   // All Symbols of Stamp
extern int Richtung; // 1 - direct, 0 - back
// ---------------------------------------------------------------------------

//int tryConfirmStamp(int trend);
void GetNext(int next);
int DetectOne();
void DetCalibr();

void SetStamp();


#endif
