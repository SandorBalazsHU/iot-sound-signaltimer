#include <Arduino.h>
#include "config.h"

//Menü lista
const char* menuItems[] = {
    "MECCS HOSSZA",
    "KEZD.KESLELTETES",
    "HANG FREKVENCIA",
    "HANG HOSSZ",
    "HANG IDOKOZ",
    "START FREKVENCIA",
    "START HOSSZ",
    "START SZAM",
    "KOZEP FREKVENCIA",
    "KOZEP HOSSZ",
    "KOZEP SZAM",
    "VEG FREKVENCIA",
    "VEG HOSSZ",
    "VEG SZAM"
};

//Menü hossza
const uint8_t menuCount = sizeof(menuItems) / sizeof(menuItems[0]);