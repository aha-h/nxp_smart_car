#ifndef _FUZZY_STEER_H
#define _FUZZY_STEER_H

#include "include.h"

float Fuzzy_DirP(float e, float ec);
float Fuzzy_DirD(float e, float ec);
void E_Membership(float e);
void EC_Membership(float ec);
void LinearQuantization(float e, float ec);

#endif

//#include "common.h"
//
//float Fuzzy(float E, float EC);
//float Fuzzy_D(float E, float EC);
//float Fuzzy_island(float E, float EC);




