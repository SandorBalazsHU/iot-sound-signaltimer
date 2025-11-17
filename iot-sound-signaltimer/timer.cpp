#include "timer.h"
#include "events.h"

//Időzítő újraindító
void timerReset(){
  status.timerRunning = false;
  status.currentTime = 0;
  status.currentHalfTime = 0;
  status.eventRunning = false;
  status.middleEventTriggered = false;
  status.startEventTriggered = false;
  status.endEventTriggered = false;
  status.lastSoundTime = -1;
}

//Időzítő szüneteltető
void timerPause(){
  status.timerRunning = false;
}

//Időzítő folytatása
void timerContinue(){
  status.timerRunning = true;
  status.lastMinuteTime = millis();
  status.lastSecondTime = millis();
}

//Időzítő újraindítása
void timerStart(){
  status.currentTime =  status.cfg.fullLength +  status.cfg.startDelay;
  status.currentHalfTime = status.cfg.fullLength / 2;
  status.lastSecondTime = millis();
  status.lastMinuteTime = millis();
  status.timerRunning = true;
  status.eventRunning = false;
}

//Időzítő frissítése
void updateTimer() {
    if (!status.timerRunning) return;

    unsigned long now = millis();

    // Másodperc marker váltása
    if (now - status.lastSecondTime >= 1000) {
        status.lastSecondTime += 1000;
        status.secondMarker = !status.secondMarker;
    }

    // Percenként csökkentjük az időt
    if (now - status.lastMinuteTime >= 10000) {
        status.lastMinuteTime += 10000;
        status.currentTime--;
        if (status.eventRunning && status.currentHalfTime > 0) status.currentHalfTime--;
    }
    updateTimeEvents();
}