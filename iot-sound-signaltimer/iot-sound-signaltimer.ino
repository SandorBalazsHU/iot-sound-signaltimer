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
int  currentHalfTime = 0;
bool eventRunning = false;
bool middleEventTriggered = false;
bool startEventTriggered = false;
int lastSoundTime = -1;

//Debouncing időzítő
unsigned long lastDebounceTime = 0;
const unsigned long debounceTime = 180;

//Fő időzítő
unsigned long lastSecondTime = 0;
unsigned long lastMinuteTime = 0;
bool secondMarker = true;

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

//Főképernyők száma
#define MAIN_SCREEN_NUMBER 5

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
    EEPROM.put(EEPROM_ADDR, cfg);
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

void timerReset(){
  currentTime = cfg.fullLength + cfg.startDelay;
  currentHalfTime = cfg.fullLength / 2;
  timerRunning = false;
  eventRunning = false;
}

void timerPause(){
  timerRunning = false;
}

void timerContinue(){
  timerRunning = true;
}

void timerStart(){
  currentTime = cfg.fullLength + cfg.startDelay;
  currentHalfTime = cfg.fullLength / 2;
  lastSecondTime = millis();
  lastMinuteTime = millis();
  timerRunning = true;
  eventRunning = false;
}

void updateTimer() {
    if (!timerRunning) return;

    unsigned long now = millis();

    // Másodperc marker váltása
    if (now - lastSecondTime >= 1000) {
        lastSecondTime += 1000;
        secondMarker = !secondMarker;
    }

    // Percenként csökkentjük az időt
    if (now - lastMinuteTime >= 60000) {
        lastMinuteTime += 60000;
        if (currentTime > 0) currentTime--;
        if (eventRunning && currentHalfTime > 0) currentHalfTime--;
    }
}

void soundEvent(){

}

void startEvent(){

}

void middleEvent(){

}

void endEvent(){

}

//Eseménykezelő
void eventHandler() {
    if(currentTime <= 0) return;

    // Ellenőrizzük, hogy elindulhatnak-e az események (start delay lejárt)
    if (!eventRunning) {
        if (currentTime <= cfg.fullLength - cfg.startDelay) {
            eventRunning = true;
            startEvent();              // Start esemény a start delay lejártakor
            startEventTriggered = true;
            lastSoundTime = currentTime; // Első hang azonnal lehet, ha soundInterval > 0
        } else {
            return; // Start delay még tart
        }
    }

    // Félidő esemény
    int halfTime = cfg.fullLength / 2;
    if (!middleEventTriggered && currentTime <= halfTime) {
        middleEvent();
        middleEventTriggered = true;
    }

    // Vége esemény
    if (currentTime <= 0) {
        endEvent();
        eventRunning = false; // Leállítjuk az események futását
        return;
    }

    // Hang események (soundInterval percenként), de félidőnél ne fusson
    if (cfg.soundInterval > 0) {
        if ((lastSoundTime - currentTime) >= cfg.soundInterval &&
            currentTime != halfTime) {
            soundEvent();
            lastSoundTime = currentTime;
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
    timerStart();
  }

  //Menü megnyitás
  if(menuPressed) {
    currentScreen = 3;
  }

  resetButtons();
}

//Időzítő képernyő
void screen_1_running(){
    lcd.setCursor(0,0);
    lcd.print("FEL:");
    printLcdInt("%03d", currentHalfTime);
    lcd.print(" VEG:");
    printLcdInt("%04d", currentTime);
    lcd.setCursor(0,1);
    lcd.print("KILEPES "); 
    if(secondMarker){
      if(eventRunning){
        lcd.print("*");
      }else{
        lcd.print("!");
      }
    }else{
      lcd.print(" ");
    }
    lcd.print(" SZUNET");

    readButtons();

    //Szünet
    if(setPressed) {
      currentScreen = 2;
      timerPause();
    }

    //Kilépés
    if(menuPressed) {
      currentScreen = 4;
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
      timerContinue();
    }

    resetButtons();
}

//Menü képernyő
void screen_3_menu() {
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
        currentScreen = MAIN_SCREEN_NUMBER + menuIndex;
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
      timerReset();
    }
    //Vissza
    if(setPressed) {
      currentScreen = 1;
    }
    resetButtons();
}

//################
//Meccs gossza beállítás
void screen_5_set_fullLength(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE");
    lcd.setCursor(0,1);
    lcd.print("HOSSZ: ");
    printLcdInt("%04d", cfg.fullLength);
    lcd.print(" PERC");

    readButtons();

    // FEL gomb
    if (upPressed) {
        if (cfg.fullLength+5 < 9999) cfg.fullLength += 5;
        resetButtons();
        return;
    }

    // LE gomb
    if (downPressed) {
        if (cfg.fullLength-5 >= 5) cfg.fullLength -= 5;
        resetButtons();
        return;
    }

    //Mentés
    if(setPressed) {
      currentScreen = 3;
      saveConfig();
    }
    //Vissza
    if(menuPressed) {
      currentScreen = 3;
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
    screen_5_set_fullLength
};


//Képernyő kezelő
void screenHandler() {
    if (currentScreen < sizeof(screenHandlers)/sizeof(screenHandlers[0])) {
        screenHandlers[currentScreen]();
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
  updateTimer();
  eventHandler();
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