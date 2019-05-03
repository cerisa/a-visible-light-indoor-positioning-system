#ifndef __STEPER_H
#define __STEPER_H
#include "sys.h"

void SteperIoInit(void);
void SteperSetAngle(float ag);
float  SteperGetAngle(void);
void SteperSetSpeed(int sp);
int  SteperGetSpeed(void);
void SteperRun(void);
void SteperWriteData(int data);
void SteperFrontTurn(void);
void SteperBackTurn(void);






#endif
