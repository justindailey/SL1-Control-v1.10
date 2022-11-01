#include<Arduino.h>
#include <errors.h>

void setup() {
  T0 = 25 + 273.15;
  //OUTPUT PIN MODE SETUP
  pinMode(waterHeater1, OUTPUT);
  digitalWrite(waterHeater1, LOW);
  pinMode(waterHeater2, OUTPUT);
  digitalWrite(waterHeater2, LOW);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, LOW);
  pinMode(greasePump, OUTPUT);
  digitalWrite(greasePump, LOW);
  pinMode(airPumpPin, OUTPUT);
  digitalWrite(airPumpPin, LOW);
  pinMode(airPurgePin, OUTPUT);
  digitalWrite(airPurgePin, LOW);
  pinMode(airDryerPin, OUTPUT);
  digitalWrite(airDryerPin, LOW);  
  pinMode(air1, OUTPUT);
  digitalWrite(air1, LOW);
  pinMode(air2, OUTPUT);
  digitalWrite(air2, LOW);
  pinMode(air3, OUTPUT);
  digitalWrite(air3, LOW);
  pinMode(air4, OUTPUT);
  digitalWrite(air4, LOW);
  pinMode(air5, OUTPUT);
  digitalWrite(air5, LOW);
  pinMode(air6, OUTPUT);
  digitalWrite(air6, LOW);
  pinMode(air7, OUTPUT);
  digitalWrite(air7, LOW);
  pinMode(air8, OUTPUT);
  digitalWrite(air8, LOW);
  pinMode(air9, OUTPUT);
  digitalWrite(air9, LOW);
  pinMode(air10, OUTPUT);
  digitalWrite(air10, LOW);
  pinMode(backlightPin, OUTPUT);
  digitalWrite(backlightPin, LOW);
  pinMode(interuptPin, OUTPUT);
  digitalWrite(interuptPin, HIGH);

  //ANALOG INPUT PINS
  pinMode(voltInputPin, INPUT);
  pinMode(airInputPin, INPUT);
  pinMode(waterInPin, INPUT);
  pinMode(waterOutPin, INPUT);
  pinMode(greasePsiPin, INPUT);
  pinMode(inputVoltagePin, INPUT);

  //DIGITAL INPUT PINS
  pinMode(waterLevelPin, INPUT_PULLUP);
  pinMode(greaseLevelPin, INPUT_PULLUP);
  pinMode(doorSensorPin, INPUT_PULLUP);
  pinMode(signalPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(tempUpPin, INPUT_PULLUP);
  pinMode(tempDownPin, INPUT_PULLUP);
  pinMode(greaseUpPin, INPUT_PULLUP);
  pinMode(greaseDownPin, INPUT_PULLUP);  
  pinMode(eStopPin, INPUT_PULLUP);
  pinMode(modemReadyPin, INPUT_PULLUP);

//Serial.begin(9600);
//Wire.begin(); 
tft.begin();
tft.setRotation(3);
dht.begin();
//ads1115.begin(0x48);

//SPLASH SCREEN
tft.fillScreen(ILI9341_WHITE);

tft.setCursor(65, 30);
tft.setTextColor(ILI9341_BLUE);  
tft.setTextSize(4);
tft.println("RBL Inc.");

tft.setCursor(65, 80);
tft.setTextColor(ILI9341_RED);  
tft.setTextSize(4);
tft.println("RoboLube");  

tft.setCursor(110, 130);
tft.setTextColor(ILI9341_DARKGREY);  
tft.setTextSize(4);
tft.println("SL-1");

tft.setCursor(50, 170);
tft.setTextColor(ILI9341_BLACK);  
tft.setTextSize(2);
tft.println("Waiting for Modem..");

tft.setCursor(70, 200);
tft.setTextColor(ILI9341_BLACK);  
tft.setTextSize(2);
tft.println("www.rblinc.com");

tft.setCursor(2, 2);
tft.setTextColor(ILI9341_BLACK);  
tft.setTextSize(1);
tft.println("Version: 1.00");

digitalWrite(backlightPin, HIGH);

//WAIT FOR MODEM TO BE READY
modemReadyStatus = digitalRead(modemReadyPin);
while(modemReadyStatus == !waitformodem){
  modemReadyStatus = digitalRead(modemReadyPin);
}
delay(splashDelay);
digitalWrite(backlightPin, LOW);
Wire.begin();
}

