#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

//Preprocesszor makrók
#define BUTTON_MENU_PIN   2
#define BUTTON_UP_PIN     3
#define BUTTON_DOWN_PIN   4
#define BUTTON_SET_PIN    5
#define BUZZER_PIN        8

//Globális állapotváltozók
LiquidCrystal_I2C lcd(0x27, 16, 2); 
bool menuPressed  = false;
bool upPressed    = false;
bool downPressed  = false;
bool setPressed   = false;
char currentScreen = 0;
char tempCurrentScreen = 0;
bool timerRunning = false;
char lastPressedKey = 0;
int  currentTime = 0;

//Debouncing időzítő
unsigned long lastDebounceTime = 0;
const unsigned long debounceTime = 150;

//Fő időzítő
unsigned long lastMainTime = 0;
const unsigned long mainTime = 1000;

//Beállítás állapotváltozók
#define EEPROM_ADDR 0
#define SIGNATURE_VALUE 42
struct Config {
  uint8_t signature = SIGNATURE_VALUE;
  int soundFreq = 1100;
  int soundLength = 800;
  int soundInterval = 2;
  int startSoundFreq = 800;
  int startSoundLength = 2000;
  int startSoundNumber = 1;
  int endSoundFreq = 800;
  int endSoundLength = 2000;
  int endSoundNumber = 1;
  int middleSoundFreq = 1500;
  int middleSoundLenght = 1000;
  int middleSoundNumber = 2;
  int fullLength = 60;
  int startDelay = 2;
};

//Konfigurációs változók
Config def_cfg;
Config cfg;

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

//Aktuális menüpont
int8_t menuIndex = 0;

//Állapotváltozók betöltése
void loadConfig(){
  EEPROM.get(EEPROM_ADDR, cfg);

  if(cfg.signature != SIGNATURE_VALUE) {
    EEPROM.put(EEPROM_ADDR, def_cfg);
    lcd.setCursor(0,0);
    lcd.print("  EEPROM ELSO   ");
    lcd.setCursor(0,1);
    lcd.print("     IRAS       ");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
  } else {
    lcd.setCursor(0,0);
    lcd.print(" EEPROM MEMORIA ");
    lcd.setCursor(0,1);
    lcd.print("   KIOLVASVA    ");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
  }
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
  if (menuPressed || upPressed || downPressed || setPressed) return;

  bool menuState = !digitalRead(BUTTON_MENU_PIN);
  bool upState   = !digitalRead(BUTTON_UP_PIN);
  bool downState = !digitalRead(BUTTON_DOWN_PIN);
  bool setState  = !digitalRead(BUTTON_SET_PIN);

  int pressedCount = menuState + upState + downState + setState;

  // ha egynél több gomb aktív, hibás bemenet
  if (pressedCount > 1) return;

  unsigned long now = millis();
  if(now - lastDebounceTime > debounceTime){
    lastDebounceTime = now;
    if (menuState) menuPressed = true;
    else if (upState) upPressed = true;
    else if (downState) downPressed = true;
    else if (setState) setPressed = true;
  }
}

//Reset függvény
void resetButtons() {
  menuPressed = false;
  upPressed = false;
  downPressed = false;
  setPressed = false;
}

void beep(int pin, int freq, int duration, int repeat) {
  for (int i = 0; i < repeat; i++) {
    tone(pin, freq);           // hang indítása adott frekvencián
    delay(duration);           // hang ideje
    noTone(pin);               // leállítás
    delay(10);                // kis szünet az ismétlések között
  }
}

void printLcdInt(const char* pattern, int value) {
  char buf[16]; // 4 karakter + lezáró null
  sprintf(buf, pattern, value); // 4 széles leading null-val
  lcd.print(buf);
}

void timerReset(){}
void timerStart(){}
void updateTimer(){
  if(timerRunning){
    unsigned long now = millis();
    if(now - lastMainTime > mainTime){
      lastMainTime = now;
      currentTime++;
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
  if(setPressed) {
    currentScreen = 1;
    timerRunning = true;
    timerStart();
  }
  //Menü megnyitás
  if(menuPressed) {
    currentScreen = 3;
  }
  /*//DEBUG SOUND
  if(upPressed) {
    beep(8, 1100, 1000, 1);
  }*/
  resetButtons();
}

//Időzítő képernyő
void screen_1_running(){
    lcd.setCursor(0,0);
    lcd.print("FEL:");
    printLcdInt("%03d", currentTime);
    lcd.print(" VEG:");
    printLcdInt("%04d", currentTime);
    lcd.setCursor(0,1);
    lcd.print("KILEPES   SZUNET");

    readButtons();
    //Szünet
    if(setPressed) {
      currentScreen = 2;
      timerRunning = false;
    }
    //Kilépés
    if(menuPressed) {
      currentScreen = 4;
      timerRunning = false;
    }
    resetButtons();
}

//Szünet képernyő
void screen_2_pause(){
    lcd.setCursor(0,0);
    lcd.print("----SZUNETEL----");
    lcd.setCursor(0,1);
    lcd.print(" VEG:");
    printLcdInt("%04d", currentTime);
    lcd.print(" VISSZA");

    readButtons();
    //Kilépés
    if(setPressed) {
      currentScreen = 1;
      timerRunning = true;
    }
    resetButtons();
}

//Menü képernyő
void screen_3_menu() {
    timerRunning = false;

    // Felső sor: statikus felirat
    lcd.setCursor(0, 0);
    lcd.print("VISSZA FEL LE BE");

    // Második sor: aktuális menüpont
    lcd.setCursor(0, 1);
    lcd.print(menuItems[menuIndex]);
    lcd.print("                ");  // maradék törlése

    readButtons();

    // Kilépés gomb
    if (menuPressed) {
        currentScreen = 0;
        resetButtons();
        return;
    }

    // FEL gomb – csökkentjük az indexet
    if (upPressed) {
        menuIndex--;
        if (menuIndex < 0) menuIndex = menuCount - 1; // körbeforgatás
        resetButtons();
        return;
    }

    // LE gomb – növeljük az indexet
    if (downPressed) {
        menuIndex++;
        if (menuIndex >= menuCount) menuIndex = 0; // körbeforgatás
        resetButtons();
        return;
    }

    // BE (set) – belépés
    if (setPressed) {
        currentScreen = menuIndex + 4;
        resetButtons();
        return;
    }

    resetButtons();
}

//Kilépés képernyő
void screen_4_are_you_shure(){
    lcd.setCursor(0,0);
    lcd.print("BIZTOS  KILEPSZ?");
    lcd.setCursor(0,1);
    lcd.print("IGEN         NEM");
    lcd.setCursor(0,0);

    readButtons();
    //Kilépés
    if(menuPressed) {
      currentScreen = 0;
      timerRunning = false;
      timerReset();
    }
    //Vissza
    if(setPressed) {
      currentScreen = 1;
      timerRunning = true;
    }
    resetButtons();
}

//Képernyő kezelő
void screenHandler() {
  switch (currentScreen) {
      case 0:
        screen_0_start();
        break;
      case 1:
        screen_1_running();
        break;
      case 2:
        screen_2_pause();
        break;
      case 3:
        screen_3_menu();
        break;
      case 4:
        screen_4_are_you_shure();
        break;
      default:
        // statements
        break;
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
  updateTimer();
  screenHandler();

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