#ifndef STATUS
#define STATUS

#include <Arduino.h>
#include "config.h"

const int mainScreenNumber = 6;

//Globális állapotváltozók
struct StatusStore {
  bool menuPressed  = false;
  bool upPressed    = false;
  bool downPressed  = false;
  bool setPressed   = false;
  char currentScreen = 0;
  bool timerRunning = false;
  int  currentTime = 0;
  int  currentHalfTime = 0;
  bool eventRunning = false;
  bool middleEventTriggered = false;
  bool startEventTriggered = false;
  bool endEventTriggered = false;
  int lastSoundTime = -1;
  int testSoundFreq = 500;

  //Debouncing időzítő
  unsigned long lastDebounceTime = 0;
  const unsigned long debounceTime = 300;

  //Fő időzítő
  unsigned long lastSecondTime = 0;
  unsigned long lastMinuteTime = 0;
  bool secondMarker = true;

  //Konfigurációs változók
  Config cfg;

  //Alapértelmezett konfiguráció
  Config def_cfg;

  //Aktuális menüpont
  int8_t menuIndex = 0;
};

#endif