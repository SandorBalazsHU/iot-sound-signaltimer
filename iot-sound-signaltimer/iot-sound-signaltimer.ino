#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Preprocesszor makrók ---
#define BUTTON_MENU_PIN   2
#define BUTTON_UP_PIN     3
#define BUTTON_DOWN_PIN   4
#define BUTTON_SET_PIN    5
#define BUZZER_PIN        8   // Hangjel kimenet

// --- Globális állapotváltozók ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 
bool menuPressed  = false;
bool upPressed    = false;
bool downPressed  = false;
bool setPressed   = false;

// --- Gomb inicializálás ---
void buttonSetup() {
  pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SET_PIN, INPUT_PULLUP);
}

// --- Gombok leolvasása ---
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

  // ha pontosan egy gombot nyomtak, beállítjuk a megfelelő flag-et
  if (menuState) menuPressed = true;
  else if (upState) upPressed = true;
  else if (downState) downPressed = true;
  else if (setState) setPressed = true;
}

// --- Reset függvény (pergésmentesítéssel) ---
void resetButtons() {
  menuPressed = false;
  upPressed = false;
  downPressed = false;
  setPressed = false;
  delay(200); // egyszerű pergésmentesítés
}

void setup() {
  delay(200);
  buttonSetup();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);

  /*delay(200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("IDOZITO FUT!");
  lcd.setCursor(0,1);
  lcd.print("10:30 1.FELIDO");

  delay(200); 
  lcd.clear();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("500");
  beep(8, 500, 1000, 1);
  delay(1000);


  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("800");
  beep(8, 800, 1000, 1);
  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1100");
  beep(8, 1100, 1000, 1);
  delay(1000);*/
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("IDOZITO FUT!");
  lcd.setCursor(0,1);
  lcd.print("10:30 1.FELIDO");

  readButtons();
  lcd.setCursor(0,0);

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
  }

  resetButtons();
  delay(100);
  //lcd.clear();
}

void beep(int pin, int freq, int duration, int repeat) {
  for (int i = 0; i < repeat; i++) {
    tone(pin, freq);           // hang indítása adott frekvencián
    delay(duration);           // hang ideje
    noTone(pin);               // leállítás
    delay(100);                // kis szünet az ismétlések között
  }
}
