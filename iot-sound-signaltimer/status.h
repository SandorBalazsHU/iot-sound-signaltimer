#ifndef STATUS
#define STATUS

#define SIGNATURE_VALUE 42

#include <stdint.h>

struct Config {
  uint8_t signature = SIGNATURE_VALUE;
  int soundFreq = 500;
  int soundLength = 500;
  int soundInterval = 2;
  int startSoundFreq = 250;
  int startSoundLength = 2000;
  int startSoundNumber = 2;
  int endSoundFreq = 250;
  int endSoundLength = 2000;
  int endSoundNumber = 2;
  int middleSoundFreq = 250;
  int middleSoundLenght = 2000;
  int middleSoundNumber = 1;
  int fullLength = 60;
  int startDelay = 2;
};

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

  //Debouncing időzítő
  unsigned long lastDebounceTime = 0;
  const unsigned long debounceTime = 160;

  //Fő időzítő
  unsigned long lastSecondTime = 0;
  unsigned long lastMinuteTime = 0;
  bool secondMarker = true;

  //Konfigurációs változók
  Config cfg;

  //Aktuális menüpont
  int8_t menuIndex = 0;
};

#endif