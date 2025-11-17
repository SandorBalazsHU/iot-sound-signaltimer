#ifndef EVENTS
#define EVENTS

#include "status.h"
#include "timer.h"

//Külső változó behívása
extern StatusStore status;

//Hang kezelő
void beep(int pin, int freq, int duration, int repeat);

//Hang esemény
void soundEvent();

//Indító esemény
void startEvent();

//Félidő esemény
void middleEvent();

//Befejező esemény
void endEvent();

//Eseménykezelő
void updateTimeEvents();

#endif