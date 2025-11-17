#include "events.h"

//Hang kezelő
void beep(int pin, int freq, int duration, int repeat) {
  if(freq == 0 || duration == 0 || repeat == 0) return;
  for (int i = 0; i < repeat; i++) {
    tone(pin, freq);           // hang indítása adott frekvencián
    delay(duration);           // hang ideje
    noTone(pin);               // leállítás
    delay(200);                // kis szünet az ismétlések között
  }
}

//Hang esemény
void soundEvent(){
  beep(speakerPin, status.cfg.soundFreq, status.cfg.soundLength, 1);
}

//Indító esemény
void startEvent(){
  beep(speakerPin, status.cfg.startSoundFreq, status.cfg.startSoundLength, status.cfg.startSoundNumber);
}

//Félidő esemény
void middleEvent(){
  beep(speakerPin, status.cfg.middleSoundFreq, status.cfg.middleSoundLenght, status.cfg.middleSoundNumber);
}

//Befejező esemény
void endEvent(){
  beep(speakerPin, status.cfg.endSoundFreq, status.cfg.endSoundLength, status.cfg.endSoundNumber);
}

//Eseménykezelő
void updateTimeEvents() {
    // Ellenőrizzük, hogy elindulhatnak-e az események (start delay lejárt)
    if (!status.eventRunning) {
        if (status.currentTime <= status.cfg.fullLength) {
            status.eventRunning = true;
            startEvent();              // Start esemény a start delay lejártakor
            status.startEventTriggered = true;
            status.lastSoundTime = status.currentTime; // Első hang azonnal lehet, ha soundInterval > 0
        } else {
            return; // Start delay még tart
        }
    }

    // Félidő esemény
    if (!status.middleEventTriggered && status.currentHalfTime <= 0) {
      middleEvent();
      status.middleEventTriggered = true;
      return;
    }

    // Vége esemény
    if (!status.endEventTriggered && status.currentTime <= 0) {
      endEvent();
      status.eventRunning = false; // Leállítjuk az események futását
      status.currentScreen = 5;
      status.endEventTriggered = true;
      timerReset();
      return;
    }

    // Hang események (soundInterval percenként), de félidőnél ne fusson
    if (status.cfg.soundInterval > 0) {
        if ((status.lastSoundTime - status.currentTime) >= status.cfg.soundInterval) {
            soundEvent();
            status.lastSoundTime = status.currentTime;
        }
    }
}