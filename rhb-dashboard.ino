
/*
 * 
    Copyright (C) 2017 Mauricio Bustos (m@bustos.org)
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
*/

#include <CmdMessenger.h>

//#include <SPI.h>
#include <OctoWS2811.h>

// LEDs
const int LED_COUNT = 8;
DMAMEM int displayMemory[LED_COUNT * 6];
int drawingMemory[LED_COUNT * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LED_COUNT, displayMemory, drawingMemory, config);
long ledValues[LED_COUNT];

// Digital Pots
const int POT_COUNT = 8;
int CS0 = 10;
int SCKO = 9;
int DOUT = 11;
byte potValues[POT_COUNT];

// Serial commands
bool ledState = 0;
const int kTeensyLed = 13;

CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum {
  kSetTeensyLed,
  kSetBacklightLed,
  kSetMeterVoltage,
  kSetDisplayPattern
};

void attachCommandCallbacks() {
  cmdMessenger.attach(kSetTeensyLed, OnSetTeensyLed);
  cmdMessenger.attach(kSetBacklightLed, OnSetBacklightLed);
  cmdMessenger.attach(kSetMeterVoltage, OnSetMeterVoltage);
  cmdMessenger.attach(kSetDisplayPattern, OnSetDisplayPattern);
}

void OnSetTeensyLed() {
  ledState = cmdMessenger.readBoolArg();
  digitalWrite(kTeensyLed, ledState?HIGH:LOW);
}

void OnSetBacklightLed() {
  int index = cmdMessenger.readInt16Arg();
  long value = cmdMessenger.readInt32Arg();

  if (index < LED_COUNT) {
    ledValues[index] = value;
    
    for (int i=0; i < leds.numPixels(); i++) {
      leds.setPixel(i, ledValues[i]);
      leds.show();
    }
  }
}

void OnSetMeterVoltage() {
  int index = cmdMessenger.readInt16Arg();
  byte value = cmdMessenger.readInt16Arg();
  if (index < POT_COUNT) {
    potValues[index] = value;
  
    Serial.print(value);

    digitalWrite(SCKO, LOW);
    digitalWrite(CS0, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    digitalWrite(CS0, HIGH);

    digitalWrite(CS0, LOW);
    for (int i = 0; i < 4; i++) {
      //SPI.transfer(B00010001);
      //SPI.transfer(potValues[i * 2]);
      //SPI.transfer(B00010010);
      //SPI.transfer(potValues[i * 2 + 1]);    
    }
    digitalWrite(CS0, HIGH);
  }
}

void OnSetDisplayPattern() {

}

void bitBang(int data, int dataPin, int clockPin) {
  int newData = data;
  for (int i = 0; i < 8; i++) {
    digitalWrite(dataPin, bitRead(newData, 0)); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
    newData = newData >> 1;
  }
}

void setup() {
  
  Serial.begin(115200); 

  cmdMessenger.printLfCr();   
  attachCommandCallbacks();

  leds.begin();
  leds.show();
  
  pinMode (CS0, OUTPUT);
  digitalWrite(CS0, HIGH);
  pinMode (SCKO, OUTPUT);
  pinMode (DOUT, OUTPUT);
  //SPI.begin();

  pinMode (kTeensyLed, OUTPUT);  

  for (int i = 0; i < POT_COUNT; i++) {
    potValues[i] = 0;
  }

  for (int i = 0; i < LED_COUNT; i++) {
    ledValues[i] = 0;
  }

  digitalWrite(SCKO, LOW);
  digitalWrite(CS0, LOW);
  //bitBang(B00010001, DOUT, SCKO);
  
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  
  digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);

  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  
  digitalWrite(DOUT, HIGH); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(DOUT, LOW); digitalWrite(SCKO, HIGH); digitalWrite(SCKO, LOW);
  digitalWrite(CS0, HIGH);
}

void loop() {
    
  cmdMessenger.feedinSerialData();

}
