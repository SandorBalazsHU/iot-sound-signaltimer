#ifndef CONFIG
#define CONFIG

struct Config {
  uint8_t signature = 42;
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

extern const char* menuItems[];
extern const uint8_t menuCount;

const int speakerPin = 8;

#endif