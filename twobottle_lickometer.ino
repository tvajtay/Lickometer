//TJV 05/03/2019

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

const int chipSelect = 4;

//Define rtc
RTC_DS3231 rtc;

//Declare filename as a global variable
String filename = ""; 

//Setup script to run once at startup
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("Card initialized.");

  //See if RTC is present
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  //Check if RTC lostpower and no longer has correct time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  //Setup the two input signals and the output LED on pin 13
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);

  //Use current time for filename
  DateTime now = rtc.now();
  filename += String(String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + "_" + String(now.hour()) + ".txt");
  
  //Create header for data file
  File dataFile = SD.open(filename, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.print("Time");
      dataFile.print("/t");
      dataFile.print("Bottle 1");
      dataFile.print("/t");
      dataFile.print("Bottle 2");
      dataFile.print("/t");
      dataFile.print("Bottle 3");
      dataFile.print("/t");
      dataFile.println("Bottle 4");
      dataFile.close();
      
      // print to the serial port too for debugging
      Serial.print("Time");
      Serial.print("/t");
      Serial.print("Bottle 1");
      Serial.print("/t");
      Serial.print("Bottle 2");
      Serial.print("/t");
      Serial.print("Bottle 3");
      Serial.print("/t");
      Serial.println("Bottle 4");
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
}


//This is the continuous script
void loop() {

  //Read the signal pins to see if there was a lick
  int Bottle1 = digitalRead(11);
  int Bottle2 = digitalRead(12);

  //If either return a high signal then the arduino will immediately write the current values to file
  if (Bottle1 == HIGH || Bottle2 == HIGH) {
    //So we have a easy read out of licks being counted
    digitalWrite(13, HIGH);
    
    
    // make a string for assembling the data to log:
    DateTime now = rtc.now();
    String cur_time = String(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));

    File dataFile = SD.open(filename, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.print(cur_time);
      dataFile.print("/t");
      dataFile.print(Bottle1);
      dataFile.print("/t");
      dataFile.print(Bottle2);
      dataFile.print("/t");
      dataFile.print("0");
      dataFile.print("/t");
      dataFile.println("0");
      dataFile.close();
      
      // print to the serial port too for debugging
      Serial.print(cur_time);
      Serial.print("/t");
      Serial.print(Bottle1);
      Serial.print("/t");
      Serial.print(Bottle2);
      Serial.print("/t");
      Serial.print("0");
      Serial.print("/t");
      Serial.println("0");
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
    //Turn off LED 
    digitalWrite(13, LOW);
  }
}
