#ifndef BATTERY
#define BATTERY

#include <Arduino.h>
#include "status.h"

//Globálisok áthívása a főprogramból.
extern StatusStore status;

// Akkumulator_toltottseg_monitor.ino
// Arduino Uno
// Feltételek: hasznalhato osztok pl. R1=30k, R2=15k -> Vdiv = 1/3

const int ADC_PIN = A0;
const float ADC_REF = 5.0;        // AVcc (default). Ha belső 1.1V használva: állítsd át
const int ADC_RES = 1023;         // 10 bit ADC
const int BATT_LIMIT = 1;

// --- Feszultsegosztó parameterek ---
// Példa (ha R1 = 30k, R2 = 15k) Factor = (R1+R2)/R2 = 3.0
const float VOLTAGE_DIVIDER_FACTOR = 3.0;  // (= (R1+R2)/R2)
// Ha más értékeket használsz, számold ki és cseréld

// Kalibrációs tényező: ha a mért feszültség kicsit eltér a valóditól
const float CALIB_FACTOR = 1.00;  // finomhangold, pl. 0.995 vagy 1.002

// SOC táblázat (feszültség [V] --> SOC [%])
// Rendezett csökkenő sorrendben (vagy növekvő, az interpoláló logika ehhez igazítható).
const int SOC_POINTS = 11;

const float socVoltages[SOC_POINTS] = {
    12.85,      // 100%
    12.6325,    // 95%
    12.1975,    // 85%
    11.7625,    // 75%
    11.545,     // 70%
    10.8925,    // 55%
    10.0225,    // 35%
    9.37,       // 20%
    9.1525,     // 15%
    8.8045,     // 7%
    8.5         // 0%
};

const int socPercents[SOC_POINTS] = {
    100, 95, 85, 75, 70, 55, 35, 20, 15, 7, 0
};

//Akku őr
void batteryGuard();

// Visszaadja a tényleges akkufeszültséget voltban
float measureBatteryVoltage(int pin, int samples);

// Interpoláljuk a SOC-ot a táblázat alapján
float voltageToSoc(float vin);

#endif