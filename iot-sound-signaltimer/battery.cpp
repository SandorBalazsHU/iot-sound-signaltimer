#include "battery.h"
#include "timer.h"

//Akku őr
void batteryGuard(){
  float v = measureBatteryVoltage(ADC_PIN, 10); // átlag 10 mérés
  float soc = voltageToSoc(v);
  if(soc < BATT_LIMIT) {
    timerReset();
    status.currentScreen = 6;
  }
}

// Visszaadja a tényleges akkufeszültséget voltban
float measureBatteryVoltage(int pin, int samples) {
  long sum = 0;
  // discard first read to allow ADC sample/hold to settle (hasznos nagy R esetén)
  analogRead(pin);
  delay(5);
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delay(2); // rövid késleltetés, ha nagy R esetén szükséges
  }
  float adc_avg = (float)sum / samples;
  float vout = (adc_avg * ADC_REF) / ADC_RES;
  float vin = vout * VOLTAGE_DIVIDER_FACTOR * CALIB_FACTOR;
  return vin;
}

// Interpoláljuk a SOC-ot a táblázat alapján
float voltageToSoc(float vin) {
  // Ha túl magas, 100%
  if (vin >= socVoltages[0]) return 100.0;
  // Ha túl alacsony, 0%
  if (vin <= socVoltages[SOC_POINTS-1]) return 0.0;

  // Keresd meg a két pontot, amelyek közt vin van
  for (int i = 0; i < SOC_POINTS-1; i++) {
    float vHi = socVoltages[i];
    float vLo = socVoltages[i+1];
    int   sHi = socPercents[i];
    int   sLo = socPercents[i+1];
    if (vin <= vHi && vin >= vLo) {
      // lineáris interpoláció
      float t = (vin - vLo) / (vHi - vLo); // 0..1
      float soc = sLo + t * (sHi - sLo);
      return soc;
    }
  }
  return 0.0; // elvileg nem ide jut
}
