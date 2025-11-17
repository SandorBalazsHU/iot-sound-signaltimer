#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "config.h"
#include "status.h"
#include "screens.h"
#include "timer.h"

//Preprocesszor makrók
#define BUTTON_MENU_PIN   2
#define BUTTON_UP_PIN     3
#define BUTTON_DOWN_PIN   4
#define BUTTON_SET_PIN    5

//EEPROM kezdőcím
#define EEPROM_ADDR 0

//Kijelző kezelő
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//Aktuális státusz tároló
StatusStore status;

//Állapotváltozók betöltése
void loadConfig(){
  EEPROM.get(EEPROM_ADDR, status.cfg);

  if(status.cfg.signature != status.def_cfg.signature) {
    EEPROM.put(EEPROM_ADDR, status.def_cfg);
    lcd.setCursor(0,0);
    lcd.print("  EEPROM ELSO   ");
    lcd.setCursor(0,1);
    lcd.print("     IRAS       ");
    delay(2000);
    lcd.clear();
  } else {
    lcd.setCursor(0,0);
    lcd.print(" EEPROM MEMORIA ");
    lcd.setCursor(0,1);
    lcd.print("   KIOLVASVA    ");
    delay(2000);
    lcd.clear();
  }
}

//Állapotváltozók mentése
void saveConfig(){
    EEPROM.put(EEPROM_ADDR, status.cfg);
    lcd.setCursor(0,0);
    lcd.print("     EEPROM     ");
    lcd.setCursor(0,1);
    lcd.print("     MENTVE     ");
    delay(2000);
    lcd.clear();
}

//Gomb inicializálás
void buttonSetup() {
  pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SET_PIN, INPUT_PULLUP);
}

//Gombok leolvasása
void readButtons() {
  // ha bármelyik már aktív, nem vizsgálunk újat
  if (status.menuPressed || status.upPressed || status.downPressed || status.setPressed) return;

  bool menuState = !digitalRead(BUTTON_MENU_PIN);
  bool upState   = !digitalRead(BUTTON_UP_PIN);
  bool downState = !digitalRead(BUTTON_DOWN_PIN);
  bool setState  = !digitalRead(BUTTON_SET_PIN);

  int pressedCount = menuState + upState + downState + setState;

  // ha egynél több gomb aktív, hibás bemenet
  if (pressedCount > 1) return;

  unsigned long now = millis();
  if(now - status.lastDebounceTime > status.debounceTime){
    status.lastDebounceTime = now;
    if (menuState) status.menuPressed = true;
    else if (upState) status.upPressed = true;
    else if (downState) status.downPressed = true;
    else if (setState) status.setPressed = true;
  }
}

//Reset függvény
void resetButtons() {
  status.menuPressed = false;
  status.upPressed = false;
  status.downPressed = false;
  status.setPressed = false;
}

//Fix széles szám írása LCD-re
void printLcdInt(const char* pattern, int value, bool showZero = true) {
    if (value == 0 && !showZero) {
        lcd.print("-KI-");
        return;
    }
    char buf[16];
    sprintf(buf, pattern, value);
    lcd.print(buf);
}

//Rendszer előkészítése
void setup() {
  delay(200);
  Serial.begin(9600);
  buttonSetup();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  loadConfig();
  delay(100);
}

//Központi ciklus
void loop() {
  screenHandler();
  updateTimer();
}