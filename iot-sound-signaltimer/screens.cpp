#include "screens.h"

#define MAIN_SCREEN_NUMBER 6

//Időzítő képernyő
void screen_1_running(){
    if(status.endEventTriggered) {
      status.currentScreen = 0;
      timerReset();
      return;
    }

    lcd.setCursor(0,0);
    lcd.print("FEL:");
    printLcdInt("%03d",  status.currentHalfTime);
    lcd.print(" VEG:");
    printLcdInt("%04d",  status.currentTime);
    lcd.setCursor(0,1);
    lcd.print("KILEPES "); 
    if(status.secondMarker){
      if(status.eventRunning){
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
    if(status.setPressed) {
      status.currentScreen = 2;
      timerPause();
    }

    //Kilépés
    if(status.menuPressed) {
      status.currentScreen = 4;
    }

    resetButtons();
}

//Szünet képernyő
void screen_2_pause(){
    lcd.setCursor(0,0);
    lcd.print("----SZUNETEL----");
    lcd.setCursor(0,1);
    lcd.print(" VEG:");
    printLcdInt("%04d", status.currentTime);
    lcd.print(" VISSZA");

    readButtons();

    //Kilépés
    if(status.setPressed) {
      status.currentScreen = 1;
      timerContinue();
    }

    resetButtons();
}

//Menü képernyő !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!menuItems!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void screen_3_menu() {
    // Felső sor: statikus felirat
    lcd.setCursor(0, 0);
    lcd.print("VISSZA FEL LE BE");

    // Második sor: aktuális menüpont
    lcd.setCursor(0, 1);
    lcd.print(menuItems[status.menuIndex]);
    lcd.print("                ");  // maradék törlése

    readButtons();

    // Kilépés gomb
    if (status.menuPressed) {
        status.currentScreen = 0;
        resetButtons();
        return;
    }

    // FEL gomb – csökkentjük az indexet
    if (status.upPressed) {
        status.menuIndex--;
        if (status.menuIndex < 0) status.menuIndex = menuCount - 1; // körbeforgatás
        resetButtons();
        return;
    }

    // LE gomb – növeljük az indexet
    if (status.downPressed) {
        status.menuIndex++;
        if (status.menuIndex >= menuCount) status.menuIndex = 0; // körbeforgatás
        resetButtons();
        return;
    }

    // BE (set) – belépés
    if (status.setPressed) {
        status.currentScreen = MAIN_SCREEN_NUMBER + status.menuIndex;
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
    if(status.menuPressed) {
      status.currentScreen = 0;
      timerReset();
    }
    //Vissza
    if(status.setPressed) {
      status.currentScreen = 1;
    }
    resetButtons();
}

//Kilépés képernyő
void screen_5_end(){
    lcd.setCursor(0,0);
    lcd.print("A MECCSNEK VEGE!");
    lcd.setCursor(0,1);
    lcd.print("    KILEPES     ");
    lcd.setCursor(0,0);

    readButtons();
    //Kilépés
    if(status.menuPressed) {
      status.currentScreen = 0;
      timerReset();
    }
    if(status.upPressed) {
      status.currentScreen = 0;
      timerReset();
    }
    if(status.downPressed) {
      status.currentScreen = 0;
      timerReset();
    }
    if(status.setPressed) {
      status.currentScreen = 0;
      timerReset();
    }
    resetButtons();
}

//################
//Meccs gossza beállítás
void screen_6_set_fullLength(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("HOSSZ: ");
    printLcdInt("%04d", status.cfg.fullLength, false);
    lcd.print(" PERC");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.fullLength+5 < 9999) status.cfg.fullLength += 5;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.fullLength-5 >= 5) status.cfg.fullLength -= 5;
        resetButtons();
        return;
    }

    //Mentés
    if(status.setPressed) {
      status.currentScreen = 3;
      saveConfig();
    }
    //Vissza
    if(status.menuPressed) {
      status.currentScreen = 3;
    }
    resetButtons();
}

//KEZD.KESLELTETES beállítás
void screen_7_set_startDelay(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("KESLEL:");
    printLcdInt("%04d", status.cfg.startDelay, false);
    lcd.print(" PERC");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.startDelay+1 < 9999) status.cfg.startDelay += 1;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.startDelay-1 >= 0) status.cfg.startDelay -= 1;
        resetButtons();
        return;
    }

    //Mentés
    if(status.setPressed) {
      status.currentScreen = 3;
      saveConfig();
    }
    //Vissza
    if(status.menuPressed) {
      status.currentScreen = 3;
    }
    resetButtons();
}

//HANG FREKVENCIA beállítás
void screen_8_set_soundFreq(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("H.FREKV: ");
    printLcdInt("%04d", status.cfg.soundFreq, false);
    lcd.print(" HZ");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.soundFreq+10 < 9999) status.cfg.soundFreq += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.soundFreq-10 >= 0) status.cfg.soundFreq -= 10;
        resetButtons();
        return;
    }

    //Mentés
    if(status.setPressed) {
      status.currentScreen = 3;
      saveConfig();
    }
    //Vissza
    if(status.menuPressed) {
      status.currentScreen = 3;
    }
    resetButtons();
}

//HANG HOSSZ beállítás
void screen_9_set_soundLength(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("H.HOSSZ: ");
    printLcdInt("%04d", status.cfg.soundLength, false);
    lcd.print(" ms");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.soundLength+10 < 9999) status.cfg.soundLength += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.soundLength-10 >= 0) status.cfg.soundLength -= 10;
        resetButtons();
        return;
    }

    //Mentés
    if(status.setPressed) {
      status.currentScreen = 3;
      saveConfig();
    }
    //Vissza
    if(status.menuPressed) {
      status.currentScreen = 3;
    }
    resetButtons();
}