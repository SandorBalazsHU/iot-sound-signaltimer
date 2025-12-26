#ifndef SCREENS
#define SCREENS

#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "status.h"
#include "timer.h"
#include "events.h"

//Főprograbeli függvények definíciói.
void printLcdInt(const char* pattern, int value, bool showZero = true);
void printLcdFloat(const char* pattern, float value, bool showZero = true);
void saveConfig();
void readButtons();
void resetButtons();

//Globálisok áthívása a főprogramból.
extern LiquidCrystal_I2C lcd;
extern StatusStore status;

//Képernyőkezelő lista tároló
typedef void ScreenFunction(void);

//Képernyőkezelő lista
extern ScreenFunction *screenHandlers[];

//Képernyő kezelő
void screenHandler();

//Kezdőképernyő
void screen_0_start();

//Időzítő képernyő
void screen_1_running();

//Szünet képernyő
void screen_2_pause();

void screen_3_menu();

//Kilépés képernyő
void screen_4_are_you_shure();

//Kilépés képernyő
void screen_5_end();

//Akkumulátor alacsomy képernyő
void screen_6_battery_low();

//Meccs gossza beállítás
void screen_7_set_fullLength();

//KEZD.KESLELTETES beállítás
void screen_8_set_startDelay();

//HANG FREKVENCIA beállítás
void screen_9_set_soundFreq();

//HANG HOSSZ beállítás
void screen_10_set_soundLength();

//HANG IDOKOZ beállítás
void screen_11_set_soundInterval();

//START FREKVENCIA beállítás
void screen_12_set_startSoundFreq();

//START HOSSZ beállítás
void screen_14_set_startSoundLength();

//START SZAM beállítás
void screen_15_set_startSoundNumber();

//KOZEP FREKVENCIA beállítás
void screen_16_set_middleSoundFreq();

//KOZEP HOSSZ beállítás
void screen_17_set_middleSoundLenght();

//KOZEP SZAM beállítás
void screen_18_set_middleSoundNumber();

//VEG FREKVENCIA beállítás
void screen_19_set_endSoundFreq();

//VEG HOSSZ beállítás
void screen_20_set_endSoundLength();

//VEG SZAM beállítás
void screen_21_set_endSoundNumber();

//Alap beállítások visszaállítása.
void screen_22_reset_default();

//Hang teszter
void screen_23_soundFreqTest();

//Akkumulátor teszter
void screen_24_battery_status();

#endif