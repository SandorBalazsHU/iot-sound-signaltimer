#ifndef SCREENS
#define SCREENS

#include <LiquidCrystal_I2C.h>
#include "status.h"

//Főprograbeli függvények definíciói.
void timerReset();
void timerPause();
void timerContinue();
void saveConfig();
void printLcdInt(const char* pattern, int value, bool showZero = true);
void readButtons();
void resetButtons();

//Globálisok áthívása a főprogramból.
extern const char* menuItems[];
extern const uint8_t menuCount;
extern LiquidCrystal_I2C lcd;

extern StatusStore status;

//Időzítő képernyő
void screen_1_running();

//Szünet képernyő
void screen_2_pause();

void screen_3_menu();

//Kilépés képernyő
void screen_4_are_you_shure();

//Kilépés képernyő
void screen_5_end();

//Meccs gossza beállítás
void screen_6_set_fullLength();

//KEZD.KESLELTETES beállítás
void screen_7_set_startDelay();

//HANG FREKVENCIA beállítás
void screen_8_set_soundFreq();

//HANG HOSSZ beállítás
void screen_9_set_soundLength();

#endif