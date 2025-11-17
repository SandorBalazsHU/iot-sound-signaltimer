#include "screens.h"

//Képernyőkezelő lista
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
    screen_9_set_soundLength,
    screen_10_set_soundInterval,
    screen_11_set_startSoundFreq,
    screen_12_set_startSoundLength,
    screen_14_set_startSoundNumber,
    screen_15_set_middleSoundFreq,
    screen_16_set_middleSoundLenght,
    screen_17_set_middleSoundNumber,
    screen_18_set_endSoundFreq,
    screen_19_set_endSoundLength,
    screen_20_set_endSoundNumber,
    screen_21_reset_default
};

//Képernyő kezelő
void screenHandler() {
    if (status.currentScreen < sizeof(screenHandlers)/sizeof(screenHandlers[0])) {
        screenHandlers[status.currentScreen]();
    } else {
        screen_0_start();  //Érvénytelen képernyő ID esetére.
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

//Menü képernyő
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
        status.currentScreen = mainScreenNumber + status.menuIndex;
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

//HANG IDOKOZ beállítás
void screen_10_set_soundInterval(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("INTERV:");
    printLcdInt("%04d", status.cfg.soundInterval, false);
    lcd.print(" PERC");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.soundInterval+1 < 9999) status.cfg.soundInterval += 1;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.soundInterval-1 >= 0) status.cfg.soundInterval -= 1;
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

//START FREKVENCIA beállítás
void screen_11_set_startSoundFreq(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("S.FREKV: ");
    printLcdInt("%04d", status.cfg.startSoundFreq, false);
    lcd.print(" HZ");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.startSoundFreq+10 < 9999) status.cfg.startSoundFreq += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.startSoundFreq-10 >= 0) status.cfg.startSoundFreq -= 10;
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

//START HOSSZ beállítás
void screen_12_set_startSoundLength(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("S.HOSSZ: ");
    printLcdInt("%04d", status.cfg.startSoundLength, false);
    lcd.print(" ms");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.startSoundLength+10 < 9999) status.cfg.startSoundLength += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.startSoundLength-10 >= 0) status.cfg.startSoundLength -= 10;
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

//START SZAM beállítás
void screen_14_set_startSoundNumber(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("START DB:");
    printLcdInt("%04d", status.cfg.startSoundNumber, false);
    lcd.print(" DB");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.startSoundNumber+1 < 9999) status.cfg.startSoundNumber += 1;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.startSoundNumber-1 >= 0) status.cfg.startSoundNumber -= 1;
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

//KOZEP FREKVENCIA beállítás
void screen_15_set_middleSoundFreq(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("K.FREKV: ");
    printLcdInt("%04d", status.cfg.middleSoundFreq, false);
    lcd.print(" HZ");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.middleSoundFreq+10 < 9999) status.cfg.middleSoundFreq += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.middleSoundFreq-10 >= 0) status.cfg.middleSoundFreq -= 10;
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

//KOZEP HOSSZ beállítás
void screen_16_set_middleSoundLenght(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("K.HOSSZ: ");
    printLcdInt("%04d", status.cfg.middleSoundLenght, false);
    lcd.print(" ms");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.middleSoundLenght+10 < 9999) status.cfg.middleSoundLenght += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.middleSoundLenght-10 >= 0) status.cfg.middleSoundLenght -= 10;
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

//KOZEP SZAM beállítás
void screen_17_set_middleSoundNumber(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("KOZEP DB:");
    printLcdInt("%04d", status.cfg.middleSoundNumber, false);
    lcd.print(" DB");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.middleSoundNumber+1 < 9999) status.cfg.middleSoundNumber += 1;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.middleSoundNumber-1 >= 0) status.cfg.middleSoundNumber -= 1;
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

//VEG FREKVENCIA beállítás
void screen_18_set_endSoundFreq(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("V.FREKV: ");
    printLcdInt("%04d", status.cfg.endSoundFreq, false);
    lcd.print(" HZ");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.endSoundFreq+10 < 9999) status.cfg.endSoundFreq += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.endSoundFreq-10 >= 0) status.cfg.endSoundFreq -= 10;
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

//VEG HOSSZ beállítás
void screen_19_set_endSoundLength(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("V.HOSSZ: ");
    printLcdInt("%04d", status.cfg.endSoundLength, false);
    lcd.print(" ms");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.endSoundLength+10 < 9999) status.cfg.endSoundLength += 10;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.endSoundLength-10 >= 0) status.cfg.endSoundLength -= 10;
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

//VEG SZAM beállítás
void screen_20_set_endSoundNumber(){
    lcd.setCursor(0,0);
    lcd.print("VISSZA FEL LE BE"); 
    lcd.setCursor(0,1);
    lcd.print("VEG DB:  ");
    printLcdInt("%04d", status.cfg.endSoundNumber, false);
    lcd.print(" DB");

    readButtons();

    // FEL gomb
    if (status.upPressed) {
        if (status.cfg.endSoundNumber+1 < 9999) status.cfg.endSoundNumber += 1;
        resetButtons();
        return;
    }

    // LE gomb
    if (status.downPressed) {
        if (status.cfg.endSoundNumber-1 >= 0) status.cfg.endSoundNumber -= 1;
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

//Alap beállítások visszaállítása.
void screen_21_reset_default(){
    lcd.setCursor(0,0);
    lcd.print("  GYARI  RESET  "); 
    lcd.setCursor(0,1);
    lcd.print("VISSZA     RESET");
    readButtons();

    //Mentés
    if(status.setPressed) {
      status.currentScreen = 3;
      status.cfg = status.def_cfg;
      saveConfig();
    }
    //Vissza
    if(status.menuPressed) {
      status.currentScreen = 3;
    }
    resetButtons();
}