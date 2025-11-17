# 🏟️ Hangvezérelt automata időzítő sporteseményekhez

## Egy Arduino-alapú, rugalmas és testreszabható hangjelző-rendszer

Ez a projekt egy Arduino alapú, programozható automata időzítő, amely hangjelzésekkel támogat sport- és versenyhelyzeteket.  
A rendszer többféle eseményt és időtartamot kezel, rugalmas hangfrekvenciákkal, ismétlésszámokkal, start–közép–vég jelzésekkel, valamint menüvezérelt konfigurációs felülettel.

### A készülék célja:
egy megbízható, egyszerűen kezelhető, fizikai gombokkal és LCD kijelzővel vezérelt időzítő biztosítása, amely alkalmas például:

- csapatmeccsek időkezelésére  
- edzések hangos vezérlésére  
- versenyek strukturált hangjelzésére  
- sportbírói munkához, edzéstervezéshez  

---

## ⚙️ Fő funkciók

### ⏱️ Időzítő funkciók
- Folyamatos másodperc-alapú számlálás  
- Meccsidő kezelése  
- Kezdési késleltetés  
- Félidő (középidő) felismerése  
- Végidő esemény  

### 🔊 Hangjelzések

Minden esemény külön konfigurálható:

- Start jelzés  
- Középidő jelzés  
- Végjelzés  

Mindegyik finoman paraméterezhető:

- hangfrekvencia  
- hanghossz  
- ismétlésszám  
- időköz ismétlések között  

### 🧰 Menüvezérlés
- Többszintű menü LCD kijelzővel  
- Navigáció négy gombbal  
- Azonnali vizuális visszajelzés  
- Mentett konfiguráció kezelése  
- Alapértelmezett értékek visszaállítása  

---

## 🔩 Hardver követelmények

- Arduino Uno / Nano / kompatibilis board  
- LiquidCrystal_I2C 16x2 LCD (0x27 cím)  
- Aktív vagy passzív piezo hangszóró  
- 4 darab nyomógomb (menü, fel, le, beállítás)  
- 10kΩ felhúzóellenállások (ha nincs belső INPUT_PULLUP használva)  
- Tápellátás 5V  

---

## 🛠️ Projekt felépítése

A projekt kódszerkezete moduláris, átlátható és bővíthető.

/src
|-- iot-sound-signaltimer.ino // főprogram, belépési pont
|-- status.h / status.cpp // StatusStore: globális állapot és logika
|-- config.h / config.cpp // menüelemek, konstansok
|-- screens.h / screens.cpp // kijelzőkezelés, menük és képernyők
|-- buttons.h / buttons.cpp // gombkezelés, debouncing
|-- sound.h / sound.cpp // hanggenerálás, beep logika


---

## 📦 StatusStore

A StatusStore egy struktúra, amely a teljes futási állapotot tárolja:

- gombállapotok  
- futó időzítő értékek  
- eseményjelzők  
- konfiguráció (cfg)  
- alapértelmezett konfiguráció (def_cfg)  
- LCD objektum (LiquidCrystal_I2C)  
- menüpozíciók  
- időzítési jelölők (másodperc, debouncing stb.)

Ez az objektum a projekt gerince, és egy példányban létezik (`status`).

---

## 📚 Telepítés és használat

### 1. A kód felmásolása
Másold a projekt teljes mappáját az Arduino IDE „sketchbook” könyvtárába.  
Nyisd meg az `iot-sound-signaltimer.ino` fájlt.

### 2. Függőségek

Szükséges library:

- `LiquidCrystal_I2C` (több kompatibilis változat is használható)

### 3. Feltöltés
Válaszd ki az Arduino modellt (UNO/Nano) és töltsd fel a kódot.

### 4. Használat

- **MENÜ** gomb → belépés a beállításokba  
- **FEL / LE** → menüben navigálás  
- **SET** → érték szerkesztése  
- **START** → időzítő elindítása  

Indítás után a készülék automatikusan jelzi:
- start esemény  
- középidő  
- meccs végét  

Az események hangjai és időzítései tetszőlegesen módosíthatók.

---

## 🧩 Fájlstruktúra és extern-alapú modulok

A projektben a globális objektumok (például `status`, `menuItems`, `lcd`)  
header fájlokban `extern` segítségével vannak deklarálva, és pontosan egy helyen definiálva, hogy elkerüljük a linker hibákat.

Ez a megoldás robusztusabbá és tisztábbá teszi az egész kódot.

---

## 📈 Jövőbeli bővítési lehetőségek

- EEPROM-alapú tartós mentés  
- LED visszajelzők  
- Rotary encoder  
- ESP32 port WiFi-vel és webes felülettel  
- Többszörös eseményprofilok mentése  
- Bluetooth vezérlés  

---

## ❤️ Motiváció

Ez a projekt azért született meg, hogy egy egyszerű, mégis okos,  
fizikai érzetű időzítő készüljön, ami:

- sportolókat segít,  
- edzéseket strukturál,  
- bírói munkát könnyít,  
- és közben élmény legyen használni.

A rendszer teljes egészében nyílt forráskódú, szabadon módosítható és bővíthető.
