#ifndef TIMER
#define TIMER

#include "status.h"

//Külső változó behívása
extern StatusStore status;

//Időzítő újraindító
void timerReset();

//Időzítő szüneteltető
void timerPause();

//Időzítő folytatása
void timerContinue();

//Időzítő újraindítása
void timerStart();

//Időzítő frissítése
void updateTimer();

#endif