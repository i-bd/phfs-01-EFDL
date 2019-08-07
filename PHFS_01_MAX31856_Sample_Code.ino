#include <Adafruit_MAX31856.h>
#define DATA_POINTS 300//number of readings 

Adafruit_MAX31856 amp_01 = Adafruit_MAX31856(2, 3, 4, 5);
Adafruit_MAX31856 amp_02 = Adafruit_MAX31856(6, 7, 8, 9);
Adafruit_MAX31856 amp_03 = Adafruit_MAX31856(10, 11, 12, 13);

float data[4]={0, 0, 0, 0};

/* For MAX31856 Setup, setThermocoupleType(#):
 * 
 * MAX31856_VMODE_32 = voltage mode w/ gain of 32 (multiplier code in main)
 * MAX31856_VMODE_8 = voltage mode w/ gain of 8
 * MAX31856_TCTYPE_# = Type-# thermocouple (e.g. E, T, K, etc.)
 * 
 */


void setup() {
  Serial.begin(9600);
  amp_01.begin();
  amp_02.begin();
  amp_03.begin();
  amp_01.setThermocoupleType(MAX31856_VMODE_G32);
  amp_02.setThermocoupleType(MAX31856_TCTYPE_E);
  amp_03.setThermocoupleType(MAX31856_VMODE_G32);
  
}

void loop() {
  int start = 0;
  if(Serial.available() > 0) {
    start = Serial.read();
  }

  if(start == 10)
  {
    Serial.print("Time Elapsed (s)");
    Serial.print("\t");
    Serial.print("Amp 1 (mV)");
    Serial.print("\t");
    Serial.print("E-Type TC Temp (C)");
    Serial.print("\t");
    Serial.print("Cold Junction Temp (C)");
    Serial.print("\t");
    Serial.println("Amp 3 (mV)");
    
    for(int i = 0; i<DATA_POINTS; i++){
      
      /*
       * MAX31856 Actions:
       * readThermocoupleTemperature() = read temp at thermocouple joint
       * readCJTemperature() = read temp at chip (reference temperature)
       * 
       * for Voltage mode with #-gain (8 or 32):
       * readThermocoupleTemperature()/(.0078125*pow(2,17)*32*1.6)
       * 
       */
      
      data[0] = (i + 1);
      data[1] = 1000 * amp_01.readThermocoupleTemperature()/(0.0078125*pow(2,17)*32*1.6);//Read voltage (mV)
      data[2] = 1000 * amp_02.readThermocoupleTemperature();//read TC temperature (C)
      data[3] = amp_02.readCJTemperature();//read reference/cold junction temperature (C)
      data[4] = 1000 * amp_03.readThermocoupleTemperature()/(0.0078125*pow(2,17)*32*1.6);
      
      Serial.print(data[0],0);
      Serial.print("\t");
      for(int j=1; j<5; j++){
        Serial.print(data[j], 6);
        Serial.print("\t");
      }
      Serial.println();
      delay(243);
    }
  }
}
