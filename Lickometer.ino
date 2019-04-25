#include <Wire.h>
#include <Adafruit_CAP1188.h>
#include "RTClib.h"

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET 9


RTC_DS3231 rtc;

// Use I2C, with reset pin
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

void setup() {
  Serial.begin(9600);
  delay(500)
  Serial.println("CAP1188 test!");

  // Initialize the sensor, if using i2c you can pass in the i2c address
  if (!cap.begin(0x28)) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }


}

void loop() {
  uint8_t touched = cap.touched();

  if (touched == 0) {
    // No touch detected
    return;
  }

  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {
      Serial.print("C"); Serial.print(i+1); Serial.print("\t");
    }
  }
  Serial.println();
  delay(50);

  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print('\t')
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}
