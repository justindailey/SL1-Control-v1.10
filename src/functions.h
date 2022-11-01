#include<Arduino.h>
#include <global.h>

void sensorRead(){
  //READ DHT11 SENSOR  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  //float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true); 
  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);
  tempF = f;
  humidity = h;

//READ WATER TEMP IN
  int tempReadIndex = 0;
  float tempReading [tempReadIndex];
  float tempReadingTotal = 0;
  float tempReadingAverage = 0;
  for(tempReadIndex = 0; tempReadIndex < smoothingFactor; tempReadIndex++) {  
    tempReading [tempReadIndex] = analogRead(waterInPin);
    tempReadingTotal = tempReadingTotal + tempReading [tempReadIndex];
  }
  tempReadingAverage = tempReadingTotal / smoothingFactor;
  VRT = tempReadingAverage;
  VRT = (5.00 / 1024.00) * VRT;
  temperatureC = (VRT - 0.5) * 100 ; 
  temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  //STORE & AVERAGE READINGS EVERY LOOP FOR SMOOTHING DATA
  if(waterTempInReadIndex == 0){    //SET VARIABLES TO AMBIENT TEMP AT START-UP
    wti1 = temperatureF;
    wti2 = temperatureF;
    wti3 = temperatureF;
    wti4 = temperatureF;
    wti5 = temperatureF;  
    waterTempInReadIndex = 1;
  }
  if(waterTempInReadIndex == 1){
    wti1 = temperatureF;
    waterTempInReadIndex = 2;
  }
  if(waterTempInReadIndex == 2){
    wti2 = temperatureF;
    waterTempInReadIndex = 3;
  }
  if(waterTempInReadIndex == 3){
    wti3 = temperatureF;
    waterTempInReadIndex = 4;
  }
  if(waterTempInReadIndex == 4){
    wti4 = temperatureF;
    waterTempInReadIndex = 5;
  }
  if(waterTempInReadIndex == 5){
    wti5 = temperatureF;
    waterTempInReadIndex = 1;
  }
  waterTempIn = (wti1+wti2+wti3+wti4+wti5)/5;

  //READ WATER TEMP OUT
  VRT = analogRead(waterOutPin);
  VRT = (5.00 / 1024.00) * VRT;
  temperatureC = (VRT - 0.5) * 100; 
  temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  waterTempOut = temperatureF;


  //READ BATTERY VOLTAGE
  voltValue = analogRead(voltInputPin);
  //voltValue = ads1115.readADC_SingleEnded(0);
  vout = (voltValue * 5.0) / 1024.0; 
  vin = vout / voltFactor; 

  //READ INPUT VOLTAGE
  acVoltReading = analogRead(inputVoltagePin);
  acVolts = ((acVoltReading*5) / 1024);
  acVolts = acVolts * 47.2656;

  // RunningStatistics inputStats;        
  // inputStats.setWindowSecs( windowLength );
  // acVolts = intercept + slope * inputStats.sigma(); //Calibartions for offset and amplitude
  // acVolts= acVolts*(40.3231);                //Further calibrations for the amplitude

  //READ AIR PRESSURE
  airRead = analogRead(airInputPin);
  airVolt = ((airRead*5)/1023.00);
  //airRead = ads1115.readADC_SingleEnded(1);
  //airVolt = (airRead*6.144)/32768;
  airPsi = (airVolt*51.05)-20.04;

  //READ GREASE PRESSURE
  greaseRead = analogRead(greasePsiPin);
  greaseVolt = ((greaseRead*5)/1023);
  //greaseRead = ads1115.readADC_SingleEnded();
  //greaseVolt = (greaseRead * 6.144)/32768;
  greasePsi = (greaseVolt*833)+42;

  //READ WATER LEVEL
  waterLevelStatus = digitalRead(waterLevelPin);

  //READ GREASE LEVEL
  greaseLevelStatus = digitalRead(greaseLevelPin);

  //READ DOOR SENSOR
  doorSensorStatus = digitalRead(doorSensorPin);

  //READ SWITCH POSITION SENSOR
  switchPosition = digitalRead(signalPin);

  //UPDATE TIMER
  lastSensorReadMillis = millis();
}

void iniTFT (){
  tft.begin();
  tft.setRotation(3);
}

void displaydiaglabels(){
  label = 1;
  tft.fillScreen(ILI9341_WHITE);
//ROW 1
//AIR PRESSURE
  tft.setCursor(col1, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("AIR PSI:");

//GREASE PRESSURE
  tft.setCursor(col2, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("GRS. PSI:");

//WATER IN
  tft.setCursor(col3, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("H2O IN:");

//WATER OUT
  tft.setCursor(col4, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("H2O OUT:");

//VOLTS
  tft.setCursor(col5, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("BAT. VDC:");


//ROW 2
//AIR PRESSURE SET
  tft.setCursor(col1, row2label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("AIR SET:");

//GREASE TIME SET
  tft.setCursor(col2, row2label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("GRS.SET:");

//WATER TEMP SET
  tft.setCursor(col3, row2label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("H2O SET:");

//AMBIENT TEMP F
  tft.setCursor(col4, row2label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("TEMP F:");

//AMBIENT HUMIDITY %
  tft.setCursor(col5, row2label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("HUMI. %:");  

//ROW 3
//AIR STATUS
  tft.setCursor(col1, row3label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("AIR STAT:");

//GREASE STATUS
  tft.setCursor(col2, row3label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("GRS.STAT");

//WATER STATUS
  tft.setCursor(col3, row3label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("H2O STAT:");

//SWITCH POSITION
  tft.setCursor(col4, row3label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("SW.POS.");

//CYCLE STATUS
  tft.setCursor(col5, row3label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("CYC.STAT");    

//ROW 4
//ACTIVE NOZZLE
  tft.setCursor(col1, row4label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("ACT.NOZ.:");

//GREASE LEVEL
  tft.setCursor(col2, row4label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("GRS.LVL.");

//WATER LEVEL
  tft.setCursor(col3, row4label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("H2O LVL.");

//ACTIVE SIDE
  tft.setCursor(col4, row4label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("ACT.SIDE");

//DOOR
  tft.setCursor(col5, row4label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("DR.STAT");    

//ROW 5
//MACHINE STATUS
  tft.setCursor(col1, row5label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("STATUS:");

//GREASE PUMP HOURS
  tft.setCursor(col2, row5label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("GRS.HR:");

//HEATER ON %
  tft.setCursor(col3, row5label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("HTR.ON %");

//HEATER 1 HOURS
  tft.setCursor(col4, row5label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("HEAT 1:");

//HEATER 2 HOURS
  tft.setCursor(col5, row5label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("HEAT 2:");
}

void displaydiagdata(){
  
  if(modemReadyStatus == 0){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(0, 0);
    tft.println("MRD");      
  }
  if(modemReadyStatus == 1){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(0, 0);
    tft.println("MNR");      
  }  

  //tft.fillScreen(ILI9341_WHITE);
  tft.setFont();
  tft.setCursor(80, 5);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("Diagnostics:");
  data = 1;
//ROW 1
  //AIR PSI
  //tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
  tft.setTextSize(data);
  tft.setCursor(col1, row1data);
  if (airPsi <= 80){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(airPsi);
  }
  if (airPsi > 80 && airPsi < airSet){
    tft.setTextColor(ILI9341_ORANGE, ILI9341_WHITE);
    tft.println(airPsi);
  }
  if(airPsi > airSet) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.println(airPsi);    
  }

  //GREASE PSI
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col2, row1data);
  if(greasePsi <= 50){
    tft.println("0");
  }  
  if(greasePsi > 50){
    tft.println(greasePsi);
  }  
  if(greasePsi > 500){
    tft.setTextColor(ILI9341_ORANGE, ILI9341_WHITE);
    tft.println(greasePsi);
  }  
  if(greasePsi > 1000){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(greasePsi);
  } 
  
  //WATER IN
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col3, row1data);
  if(waterTempIn >= waterSet - 10){
    tft.println(waterTempIn);
  }
  if(waterTempIn < waterSet - 10){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(waterTempIn);
  }

  //WATER OUT
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col4, row1data);
  if(waterTempOut >= waterSet){
    tft.println(waterTempOut);
  }
  if(waterTempOut < waterSet){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(waterTempOut);
  }

  //VOLTS
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col5, row1data);
  if(vin >= 12.8){
    tft.println(vin);
  }  
  if(vin < 12.8){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(vin);
  }  

//ROW 2
  //AIR SET
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col1, row2data);
    tft.println(airSet);

  //GREASE SET
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col2, row2data);
    tft.println(greaseTime);

  //TEMP SET
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col3, row2data);
    tft.println(waterSet);    

  //TEMP
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col4, row2data);
    tft.println(tempF);

  //HUMIDITY
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col5, row2data);
    tft.println(humidity);

//ROW 3
//AIR STATUS
if(airStatus != lastAirStatus){
  tft.setTextSize(data);
  tft.setCursor(col1, row3data);
  if(airStatus == 0){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println("LOW    ");
  }
  if(airStatus == 1){
    tft.setTextColor(ILI9341_YELLOW, ILI9341_WHITE);
    tft.println("OK    ");
  }
  if(airStatus == 2){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.println("READY");
  }
  lastAirStatus = airStatus;
}

//GREASE STATUS
if(greaseStatus != lastGreaseStatus){
  tft.setTextSize(data);
  tft.setCursor(col2, row3data);
  if(greaseStatus == 0){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println("WAIT    ");
  }
  if(greaseStatus == 1){
    tft.setTextColor(ILI9341_YELLOW, ILI9341_WHITE);
    tft.println("PUMPING");
  }
  if(greaseStatus == 2){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.println("READY    ");
  }
  lastGreaseStatus = greaseStatus;
}

//WATER STATUS
if(waterTempStatus != lastWaterTempStatus){
  tft.setTextSize(data);
  tft.setCursor(col3, row3data);
  if(waterTempStatus == 0){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println("COLD    ");
  }
  if(waterTempStatus == 1){
    tft.setTextColor(ILI9341_YELLOW, ILI9341_WHITE);
    tft.println("HEATING");
  }
  if(waterTempStatus == 2){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.println("READY    ");
  }
  lastWaterTempStatus = waterTempStatus;
}

//SWITCH POSITION
if(switchPosition != lastSwitchPosition){
  tft.setTextSize(data);
  tft.setCursor(col4, row3data);
  if (switchPosition == 1){
    tft.setTextColor(ILI9341_MAGENTA, ILI9341_WHITE);
    tft.println("OPEN");
  }
  if(switchPosition == 0){
    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
    tft.println("CLSD");   
  }
  lastSwitchPosition = switchPosition;
}

//CYCLE STATUS
if(cycleStatus != lastCycleStatus){
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
  tft.setTextSize(data);
  tft.setCursor(col5, row3data);
  if(cycleStatus == 0){
    tft.println("WAITING ODD ");
  }
  if(cycleStatus == 1){
    tft.println("CYCLE DELAY ");
  }  
  if(cycleStatus == 2){
    tft.println("ODD BEGIN   ");
  }    
  if(cycleStatus == 3){
    tft.println("ODD RUNNING ");
  }  
  if(cycleStatus == 4){
    tft.println("WAITING EVEN");
  }  
  if(cycleStatus == 5){
    tft.println("CYCLE DELAY ");
  }  
  if(cycleStatus == 6){
    tft.println("EVEN BEGIN  ");
  }  
  if(cycleStatus == 7){
    tft.println("EVEN RUNNING");
  }  
  lastCycleStatus = cycleStatus; 
} 

//ROW 4
//ACTIVE NOZZLE
if(activeNozzle != lastActiveNozzle){
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
  tft.setTextSize(data);
  tft.setCursor(col1, row4data);
  tft.print(activeNozzle);
  tft.println("  ");
  lastActiveNozzle = activeNozzle;
}

//GREASE LEVEL
if(greaseLevelStatus != lastGreaseLevelStatus){
  tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
  tft.setTextSize(data);
  tft.setCursor(col2, row4data);
  tft.println("OK");
  lastGreaseLevelStatus = greaseLevelStatus;
}

//WATER LEVEL
if(waterLevelStatus != lastWaterLevelStatus){
  tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
  tft.setTextSize(data);
  tft.setCursor(col3, row4data);
  tft.println("OK");
  lastWaterLevelStatus = waterLevelStatus;
}

//ACTIVE SIDE
if(activeSide != lastActiveside){
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  
  tft.setTextSize(data);
  tft.setCursor(col4, row4data);
  tft.println(activeSide);
  lastActiveside = activeSide;
}

//DOOR
if(doorSensorStatus != lastDoorSensorStatus){
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
  tft.setTextSize(data);
  tft.setCursor(col5, row4data);
  if (doorSensorStatus == 1){
    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
    tft.println("OPEN");
  }
  else {
    tft.setTextColor(ILI9341_MAGENTA, ILI9341_WHITE);
    tft.println("CLSD");    
  }
  lastDoorSensorStatus = doorSensorStatus;
}

//ROW 5
//MACHINE STATUS
  if(status == 0){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col1, row5data);
    tft.println("INT.");
  }
  if(status == 1){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(col1, row5data);
    tft.println("READY");
  }

//HEAT 1    
  tft.setTextSize(data);
  tft.setCursor(col4, row5data);
  if (heater1status == 1){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.println("ON ");
  }
  else {
    tft.setTextColor(ILI9341_DARKGREEN, ILI9341_WHITE);
    tft.println("OFF");    
  }

//HEAT 2
  tft.setTextSize(data);
  tft.setCursor(col5, row5data);
  if (heater2status == 1){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.println("ON ");
  }
  else {
    tft.setTextColor(ILI9341_DARKGREEN, ILI9341_WHITE);
    tft.println("OFF");    
  }

  lastDisplayMillis = millis();
}

void displayDefaultLabels(){
  digitalWrite(backlightPin, LOW);
  tft.fillScreen(ILI9341_WHITE);
  tft.setFont();
  tft.setCursor(80, 5);
  tft.setTextColor(ILI9341_BLUE);  
  tft.setTextSize(2);
  tft.println("RoboLube SL1");
  tft.setCursor(2, 2);
  tft.setTextColor(ILI9341_RED);  
  tft.setTextSize(1);
  tft.print("v");
  tft.println(version);

//SET FONT SIZE
  label = 1;

//LABEL PLACEMENT

//AIR PRESSURE
  tft.setCursor(20, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("AIR PSI:");

//WATER IN
  tft.setCursor(100, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("WATER F:");

//AC VOLTS
  tft.setCursor(175, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("INPUT VAC:");

//VOLTS
  tft.setCursor(250, row1label);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(label);
  tft.println("BAT. VDC:");

//MACHINE STATUS
  tft.setCursor(20, 80);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(2);
  tft.println("MACHINE STATUS:");
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.setCursor(40, 110);
  tft.println("START UP");

//CYCLE STATUS
  tft.setCursor(30, 160);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(2);
  tft.println("CYCLE STATUS:");
  tft.setTextColor(ILI9341_MAGENTA, ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.setCursor(5, 190);
  tft.println(" SYNCING SW.");

//TEMPERATURE SETTING
  tft.setCursor(250, 70);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(1);
  tft.println("TEMP SET:");

//GREASE TIME SETTING
  tft.setCursor(250, 110);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(1);
  tft.println("GRS. SET:");

//SWITCH POSITION
  tft.setCursor(250, 150);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(1);
  tft.println("SW.POS.:");

//GREASE LEVEL
  tft.setCursor(250, 190);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setTextSize(1);
  tft.println("GRS.LVL:");

  digitalWrite(backlightPin, HIGH);
}

void displayUpdate(){
  //iniTFT();
//SET FONT SIZE
  data = 2;
//AIR PRESSURE  
  tft.setTextSize(data);
  tft.setCursor(2, row1data);
  if (airPsi < airSet - 20){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(airPsi);
  }
  if (airPsi < airSet && airPsi >= airSet - 20){
    tft.setTextColor(ILI9341_ORANGE, ILI9341_WHITE);
    tft.println(airPsi);
  }
  if(airPsi > airSet) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
    tft.println(airPsi);    
  }
//WATER IN
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(85, row1data);
  if(waterTempIn >= waterSet - 10){
    tft.println(waterTempIn);
  }
  if(waterTempIn < waterSet - 10){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(waterTempIn);
  }
// DC VOLTS
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(245, row1data);
  if(vin >= 12.0){
    tft.println(vin);
  }  
  if(vin < 12.0){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(vin);
  }  

// AC VOLTS
    tft.setCursor(160, row1data);
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);
  if(acVolts > 100){  
    tft.println(acVolts);  
  }
  if(acVolts <= 99 || acVolts >= 140){
    tft.setTextColor(ILI9341_ORANGE, ILI9341_WHITE);
    tft.println(acVolts);
  }   
  if(acVolts <= 70 || acVolts >= 150){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.println(acVolts);
  }   


//MACHINE STATUS
  if(status != lastStatus){
    if(status == 0){
      tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
      tft.setTextSize(3);
      tft.setCursor(40, 110);
      tft.println("WAITING ");
    }
    if(status == 1){
      tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
      tft.setTextSize(3);
      tft.setCursor(40, 110);
      tft.println(" READY  ");
    }
    lastStatus = status;
  }
//CYCLE STATUS
  if(cycleStatus != lastCycleStatus){
    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);  
    tft.setTextSize(3);
    tft.setCursor(5, 190);
    if(cycleStatus == 0){
      tft.println("WAITING ODD ");
    }
    if(cycleStatus == 1){
      tft.println("CYCLE DELAY ");
    }  
    if(cycleStatus == 2){
      tft.println("ODD BEGIN   ");
    }    
    if(cycleStatus == 3){
      tft.println("ODD RUNNING ");
    }  
    if(cycleStatus == 4){
      tft.println("WAITING EVEN");
    }  
    if(cycleStatus == 5){
      tft.println("CYCLE DELAY ");
    }  
    if(cycleStatus == 6){
      tft.println("EVEN BEGIN  ");
    }  
    if(cycleStatus == 7){
      tft.println("EVEN RUNNING");
    }  
    lastCycleStatus = cycleStatus;
  }
//TEMP SET
  if(lastWaterSet != waterSet || greaseStatus == 1){
    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(260, 85);
    tft.println(waterSet);
    lastWaterSet = waterSet; 
  }
//GREASE SET
  if(lastGreaseTime != greaseTime || greaseStatus == 1){
    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(260, 125);
    tft.println(greaseTime);
    lastGreaseTime = greaseTime;
  }
  if(modemReadyStatus == 0){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(0, 10);
    tft.println("MRD");      
  }
  if(modemReadyStatus == 1){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(0, 10);
    tft.println("MNR");      
  }  
//SWITCH POSITION
  if(switchPosition != lastSwitchPosition || greaseStatus == 1){
    tft.setTextSize(data);
    tft.setCursor(250, 165);
    if (switchPosition == 1){
      tft.setTextColor(ILI9341_CYAN, ILI9341_WHITE);
      tft.println("OPEN");
    }
    if(switchPosition == 0){
      tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
      tft.println("CLSD");   
    }
    lastSwitchPosition = switchPosition;
  }
//GREASE LEVEL
  if(greaseLevelStatus != lastGreaseLevelStatus || greaseStatus == 1 || lowGreaseCycleCount != lastLowGreaseCycleCount){
    tft.setTextColor(ILI9341_GREEN, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(260, 205);
        
    if(greaseLevelStatus == 1){
       tft.println("OK");
    }
    if(greaseLevelStatus == 0){
      tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
      tft.print(lowGreaseCyclesRemaining);
      tft.println("   ");
    }       

    lastGreaseLevelStatus = greaseLevelStatus;
  }
  lastDisplayMillis = millis();
}

void sendFastDataSet(){
  tft.setCursor(280, 2);
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
  tft.print("I2C");
  digitalWrite(interuptPin, LOW);
  delay(2000);
  Wire.beginTransmission(modemi2cAddress);
  // Wire.write(dataSet);            //0
  Wire.write(error);              //1
  Wire.write(status);             //2
  Wire.write(airStatus);          //3
  Wire.write(voltageStatus);      //4
  Wire.write(waterTempStatus);    //5
  Wire.write(greaseStatus);       //6
  Wire.write(cycleCount);         //7
  Wire.write(doorSensorStatus);   //8
  Wire.write(switchPosition);     //9
  Wire.write(waterSet);           //10
  Wire.write(greaseTime);         //11 
  Wire.endTransmission();
  tft.setCursor(280, 2);
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
  tft.print("   ");
  digitalWrite(interuptPin, HIGH);
  delay(100);
  lastModemDataInterval = millis();  
}

void adjusttempup(){
  if(waterSet >= waterSetMax){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(260, 85);
    tft.println("MAX");
    delay(500);
    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
    tft.setCursor(260, 85);
    tft.print(waterSet);
    tft.println("  ");
    sendFastDataSet();
  }

  if(waterSet < waterSetMax){
    waterSet = waterSet + 1;
  } 
  delay(1000);
  }
void adjusttempdown(){
  if(waterSet <= waterSetMin){
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);  
    tft.setTextSize(data);
    tft.setCursor(260, 85);
    tft.println("MIN");
    delay(500);
    tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
    tft.setCursor(260, 85);
    tft.print(waterSet);
    tft.println("  ");
    sendFastDataSet();
  }
  if(waterSet > waterSetMin){
    waterSet = waterSet - 1;
  } 
  delay(1000);}

void adjustgreaseup(){
  greaseTime = greaseTime + 1; 
  delay(1000);
  }
void adjustgreasedown(){
  greaseTime = greaseTime - 1; 
  delay(1000);
  }

//CYCLE ODD SIDE
void cycleODD(){
  currentMillis = millis();
  if (activeSide == 1){
    //ACTIVATE NOZZLE 1
    if (activeNozzle == 1 && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air1, HIGH);
    }
    if (activeNozzle == 1 && currentMillis - airOnMillis >= airTimeMillis){
      digitalWrite(air1, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 3;
    }

    //ACTIVATE NOZZLE 3
    if (activeNozzle == 3 && (currentMillis - lastAirIntervalMillis) >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air3, HIGH);
    }
    if (activeNozzle == 3 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air3, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 5;
    }

    //ACTIVATE NOZZLE 5
    if (activeNozzle == 5 && currentMillis - lastAirIntervalMillis >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air5, HIGH);
    }
    if (activeNozzle == 5 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air5, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 7;
    }    

    //ACTIVATE NOZZLE 7
    if (activeNozzle == 7 && currentMillis - lastAirIntervalMillis >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air7, HIGH);
    }
    if (activeNozzle == 7 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air7, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 9;
    }

    //ACTIVATE NOZZLE 9
    if (activeNozzle == 9 && currentMillis - lastAirIntervalMillis >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air9, HIGH);
    }
    if (activeNozzle == 9 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air9, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 2;
      cycleStatus = 4;        //UPDATE CYCLE PROCEEDURE STATUS
      activeSide = 2;
      runAirPump = true;
    }
  }
}

//CYCLE EVEN SIDE & RESET GREASE
void cycleEVEN() {
  currentMillis = millis();
  if (activeSide == 2){
    //ACTIVATE NOZZLE 2
    if (activeNozzle == 2 && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air2, HIGH);
    }
    if (activeNozzle == 2 && currentMillis - airOnMillis >= airTimeMillis){
      digitalWrite(air2, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 4;
    }

    //ACTIVATE NOZZLE 4
    if (activeNozzle == 4 && currentMillis - lastAirIntervalMillis >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air4, HIGH);
    }
    if (activeNozzle == 4 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air4, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 6;
    }

    //ACTIVATE NOZZLE 6
    if (activeNozzle == 6 && currentMillis - lastAirIntervalMillis >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air6, HIGH);
    }
    if (activeNozzle == 6 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air6, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 8;
    }    

    //ACTIVATE NOZZLE 8
    if (activeNozzle == 8 && currentMillis - lastAirIntervalMillis >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air8, HIGH);
    }
    if (activeNozzle == 8 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air8, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 10;
    }

    //ACTIVATE NOZZLE 10
    if (activeNozzle == 10 && currentMillis - lastAirIntervalMillis >= airIntervalMillis && nozzleStatus == 0){
      airOnMillis = millis();
      currentMillis = millis();
      nozzleStatus = 1;
      digitalWrite(air10, HIGH);
    }
    if (activeNozzle == 10 && currentMillis - airOnMillis >= airTimeMillis && nozzleStatus == 1){
      digitalWrite(air10, LOW);
      lastAirIntervalMillis = millis();
      nozzleStatus = 0;
      activeNozzle = 1;
      activeSide = 1;
      cycleStatus = 0;
      greaseStatus = 0;         //RESET GREASE
      runAirPump = true;
      cycleCount++;
    }
  }
}  

//TURN ALL NOZZLES OFF
void nozzlesOFF(){
  digitalWrite(air1, LOW);
  digitalWrite(air2, LOW);
  digitalWrite(air3, LOW);
  digitalWrite(air4, LOW);
  digitalWrite(air5, LOW);
  digitalWrite(air6, LOW);
  digitalWrite(air7, LOW);
  digitalWrite(air8, LOW);
  digitalWrite(air9, LOW);
  digitalWrite(air10, LOW);
  nozzleStatus = 0;
}

//PURGE CYCLE
void airPurgeCycle(){
  digitalWrite(airPurgePin, HIGH);
  delay(airPurgeTimeMillis); 
  digitalWrite(airPurgePin, LOW); 
  delay(100); 
  digitalWrite(airPurgePin, HIGH);
  delay(25); 
  digitalWrite(airPurgePin, LOW);
  delay(100);  
  digitalWrite(airPurgePin, HIGH);
  delay(25); 
  digitalWrite(airPurgePin, LOW);    
}