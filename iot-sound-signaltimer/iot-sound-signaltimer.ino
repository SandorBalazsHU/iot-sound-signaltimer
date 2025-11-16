#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "status.h"
#include "screens.h"

//Preprocesszor makrók
#define BUTTON_MENU_PIN   2
#define BUTTON_UP_PIN     3
#define BUTTON_DOWN_PIN   4
#define BUTTON_SET_PIN    5
#define BUZZER_PIN        8

//Beállítás állapotváltozók
#define EEPROM_ADDR 0
#define SIGNATURE_VALUE 42

//Főképernyők száma
#define MAIN_SCREEN_NUMBER 6

Config def_cfg;

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

LiquidCrystal_I2C lcd(0x27, 16, 2); 

StatusStore status;

//Állapotváltozók betöltése
void loadConfig(){
  EEPROM.get(EEPROM_ADDR, status.cfg);

  if(status.cfg.signature != SIGNATURE_VALUE) {
    EEPROM.put(EEPROM_ADDR, def_cfg);
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

void beep(int pin, int freq, int duration, int repeat) {
  if(freq == 0 || duration == 0 || repeat == 0) return;
  for (int i = 0; i < repeat; i++) {
    tone(pin, freq);           // hang indítása adott frekvencián
    delay(duration);           // hang ideje
    noTone(pin);               // leállítás
    delay(200);                // kis szünet az ismétlések között
  }
}

void printLcdInt(const char* pattern, int value, bool showZero = true) {
    if (value == 0 && !showZero) {
        lcd.print("-KI-");
        return;
    }
    char buf[16];
    sprintf(buf, pattern, value);
    lcd.print(buf);
}

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

void timerPause(){
  status.timerRunning = false;
}

void timerContinue(){
  status.timerRunning = true;
  status.lastMinuteTime = millis();
  status.lastSecondTime = millis();
}

void timerStart(){
  status.currentTime =  status.cfg.fullLength +  status.cfg.startDelay;
  status.currentHalfTime = status.cfg.fullLength / 2;
  status.lastSecondTime = millis();
  status.lastMinuteTime = millis();
  status.timerRunning = true;
  status.eventRunning = false;
}

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
    upddateTimeEvents();
}

void soundEvent(){
  beep(BUZZER_PIN, status.cfg.soundFreq, status.cfg.soundLength, 1);
}

void startEvent(){
  beep(BUZZER_PIN, status.cfg.startSoundFreq, status.cfg.startSoundLength, status.cfg.startSoundNumber);
}

void middleEvent(){
  beep(BUZZER_PIN, status.cfg.middleSoundFreq, status.cfg.middleSoundLenght, status.cfg.middleSoundNumber);
}

void endEvent(){
  beep(BUZZER_PIN, status.cfg.endSoundFreq, status.cfg.endSoundLength, status.cfg.endSoundNumber);
}

//Eseménykezelő
void upddateTimeEvents() {
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

//Kezdőképernyő
void screen_0_start(){
  lcd.setCursor(0,0);
  lcd.print("-MECCS IDOZITO!-");
  lcd.setCursor(0,1);
  lcd.print("MENU       START");

  readButtons();

  //Időzítő indítás
  if(status.setPressed) {
    status.currentScreen = 1;
    timerStart();
  }

  //Menü megnyitás
  if(status.menuPressed) {
    status.currentScreen = 3;
  }

  resetButtons();
}

typedef void ScreenFunction(void);

ScreenFunction *screenHandlers[] = {
    screen_0_start,
    screen_1_running,
    screen_2_pause,
    screen_3_menu,
    screen_4_are_you_shure,
    screen_5_end,
    screen_6_set_fullLength,
    screen_7_set_startDelay,
    screen_8_set_soundFreq,
    screen_9_set_soundLength
};

//Képernyő kezelő
void screenHandler() {
    if (status.currentScreen < sizeof(screenHandlers)/sizeof(screenHandlers[0])) {
        screenHandlers[status.currentScreen]();
    } else {
        screen_0_start();  //Érvénytelen képernyő ID esetére.
    }
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

/*readButtons();
  if(menuPressed) {
    lcd.print("menuPressed");
  }
  if(upPressed) {
    lcd.print("upPressed");
  }
  if(downPressed) {
    lcd.print("downPressed");
  }
  if(setPressed) {
    lcd.print("setPressed");
    beep(8, 1100, 1000, 1);
  }*/
}