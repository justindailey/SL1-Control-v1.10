#include<Arduino.h>
#include <setup.h>

void loop() {
//UPDATE MILLIS
  currentMillis = millis();
  greaseTimeMillis = greaseTime * 1000UL;

//E-STOP READ
  int eStopButtonState = digitalRead(eStopPin);
  if(eStopButtonState == 1){
    eStopActive();
  }

//DISPLAY LCD LABLES
  if(diagMode == 3){
    displaydiaglabels();
    digitalWrite(backlightPin, HIGH);
    diagMode = 4;
  }
  if(diagMode == 0){
    displayDefaultLabels();
    digitalWrite(backlightPin, HIGH);
    diagMode = 1;
  }//
  
//READ SENSORS
  if(currentMillis - lastSensorReadMillis >= sensorReadMillis){sensorRead();}

//UPDATE DISPLAY
  if(diagMode == 1){
    if(currentMillis - lastDisplayMillis >= displayMillis){displayUpdate();}
  }
  if(diagMode == 4){
    if(currentMillis - lastDisplayMillis >= displayMillis){displaydiagdata();}
  }
  
//READ BUTTON STATES & DIRECT
  int tempUpButtonState = digitalRead(tempUpPin);
  if (tempUpButtonState == 0){adjusttempup();}
  int tempDownButtonState = digitalRead(tempDownPin);
  if (tempDownButtonState == 0){adjusttempdown();}
  int greaseUpButtonState = digitalRead(greaseUpPin);
  if (greaseUpButtonState == 0){adjustgreaseup();}
  int greaseDownButtonState = digitalRead(greaseDownPin);
  if (greaseDownButtonState == 0){adjustgreasedown();}  
  int startButtonState = digitalRead(startPin);
  if (startButtonState == 0){
    if(diagMode == 1){
      diagMode = 3;
      iniTFT();
    }
    if(diagMode == 4){
      diagMode = 0;
      iniTFT();
    }    
  }

// WATER TEMP CONTROL
  //QUICK HEAT MODE
  if(waterTempIn < waterSet - quickHeatDelta){
    waterTempStatus = 3;
    digitalWrite(waterHeater1, HIGH);
    digitalWrite(waterHeater2, HIGH);
  }
  //TURN ON HEATERS
  if (waterTempIn <= waterSet && waterTempIn > waterSet - quickHeatDelta){
    if(activeHeater == 1){
    digitalWrite(waterHeater1, HIGH);
    heater1status = 1;
    }
    if(activeHeater == 2){
    digitalWrite(waterHeater2, HIGH);
    heater2status = 1;
    }
    waterTempStatus = 1;
    //status = 0;    
  }
  //TURN OFF HEATERS & SET WATER TEMP STATUS TO READY
  if (waterTempIn >= (waterSet+10)){
    digitalWrite(waterHeater1, LOW);
    digitalWrite(waterHeater2, LOW);
    if(activeHeater == 1 && heater1status == 1){
    activeHeater = 2;
    }
    if(activeHeater == 2 && heater2status == 1){
    activeHeater = 1;
    }  
    heater1status = 0;
    heater2status = 0;     
  }
  if(waterTempIn > waterSet){
    waterTempStatus = 2;
  }
  if(waterTempIn < waterSet - 20){
    status = 0;
  }

// AIR PRESSURE CONTROL
  //UPDATE TIMERS  

  //TURN ON AIR COMP
  if(airPsi <= airSet){
    runAirPump = true;
  }
  if (runAirPump == true){
    digitalWrite(airPumpPin, HIGH);
    digitalWrite(airDryerPin, HIGH);
    digitalWrite(airFanPin, HIGH);    
    airStatus = 0;
    status = 0;
    if(airCompTimeStore == true){
        lastAirCompTimeMax = millis();
    }
    airCompTimeStore = false;    
  }

  //AIR SYSTEM FULL
  if (airPsi >= (airOffSet) && airStatus == 0){
    digitalWrite(airPumpPin, LOW);
    digitalWrite(airDryerPin, LOW);
    digitalWrite(airFanPin, LOW);
    airStatus = 1;  
    
    lastAirCompTimeMax = millis();  
    //status = 1;
  }
  //RUN DRYER PURGE
  if(airStatus == 1){
    airPurgeCycle();
    runAirPump = false;
    airStatus = 2;
  }
  // AIR COMP TIMEOUT; START COOL DOWN
  currentMillis = millis();
  if(currentMillis - lastAirCompTimeMax >= airCompTimeMaxMillis
        && airStatus == 1){
    digitalWrite(airPumpPin, LOW);
    digitalWrite(airDryerPin, LOW);
    airStatus = 3;
    runAirPump = false;
    lastairCompCoolDown = millis();
  }

  if(currentMillis - lastairCompCoolDown >= airCompCoolDownMillis
        && airStatus == 3){
    runAirPump = true;
    airPumpCycles++;
  }

  if(airPumpCycles >= 5){
    airPumpCycles = 0;
    airSystemLeak();
  }

// LED LIGHTING CONTROL
  if(doorSensorStatus != lastDoorSensorStatus){
    iniTFT(); 
    displayDefaultLabels();
  }

  if (doorSensorStatus == 1){
    digitalWrite(ledPin, HIGH);
    digitalWrite(backlightPin, HIGH);
    lastDoorSensorStatus = doorSensorStatus;
  }

  if (doorSensorStatus == 0){
    digitalWrite(ledPin, LOW);
    digitalWrite(backlightPin, LOW);
    lastDoorSensorStatus = doorSensorStatus;
  }


// WATER LEVEL CONTROL
  if(waterLevelStatus == 1 ){
    nozzlesOFF();
    waterLevelError();
    error = 0;} 
  if(waterLevelStatus == 0){digitalWrite(waterPump, HIGH);} 

// GREASE LEVEL CONTROL
  if(greaseLevelStatus == 0){
    lowGrease();
  } 

// PRIME GREASE SYSTEM
  if(greaseStatus == 0){
    g3startMillis = millis();
    currentMillis = millis();
    digitalWrite(greasePump, HIGH);
    greaseStatus = 1;
    status = 0;
    }
    //add grease pump total time counter
  if(currentMillis - g3startMillis >= greaseTimeMillis){
    g3stopMillis = millis();
    digitalWrite(greasePump, LOW);
    greaseStatus = 2;
    } 

  if (greaseStatus == 2 && airStatus == 2 && waterTempStatus == 2){
    status = 1;
  }     

// GREASE PRESSURE MONITORING
  if(greasePsi >= greasePsiSet){
    greasePsiHigh();
  }

// VOLTAGE CONTROL
  if(vin >= vinOK){voltageStatus = 1;}
  if(vin < vinCheck){voltageStatus = 2;}
  if(vin <= vinLOW){
    nozzlesOFF();
    lowVoltage();
  }  

//CYCLE LOGIC
  //CHECK NOZZLE & CYCLE STATUS
  if (nozzleStatus == 1 && status == 0){
    nozzlesOFF();
  }
  //START ODD CYCLE
  if (switchPosition == 1 && cycleStatus == 0){
    lastCycleDelayMillis = millis();
    currentMillis = millis();
    cycleStatus = 1;
  }
  if (cycleStatus == 1 && currentMillis - lastCycleDelayMillis >= cycleDelayMillis){
    cycleStatus = 2;
  }
  //ODD CYCLE STATUS UPDATE & CHECK
  if (cycleStatus == 2 && status == 1){
    activeNozzle = 1;
    cycleStatus = 3;
    cycleODD();
  }
  //ODD CYCLE CONTINUE & CHECK
  if(cycleStatus == 3 && status == 1){
    cycleODD();
  }

  //START EVEN CYCLE
  if (switchPosition == 0 && cycleStatus == 4){
    lastCycleDelayMillis = millis();
    currentMillis = millis();
    cycleStatus = 5;
  }
  if (cycleStatus == 5 && currentMillis - lastCycleDelayMillis >= cycleDelayMillis){
    cycleStatus = 6;
  }  
  //EVEN CYCLE STATUS UPDATE & CHECK
  if (cycleStatus == 6 && status == 1){
    cycleStatus = 7;
    cycleEVEN();  
  }
  //EVEN CYCLE CONTINUE & CHECK
  if(cycleStatus == 7 && status == 1){
    cycleEVEN();
  }
  
//MODEM DATA TX
  currentMillis = millis();
  modemReadyStatus = digitalRead(modemReadyPin);
  if(currentMillis - lastModemDataInterval >= modemDataIntervalMillis && 
        modemReadyStatus == 0 && runAirPump == false){
    sendI2CData = true;    
    sendFastDataSet();
    lastModemDataInterval = millis();
  }
//   if(airStatus == 2){
//     sendI2CData = true;
//     lastModemDataInterval = millis();
//   }
//   if(airStatus == !2){
//     sendI2CData = false;
//     lastModemDataInterval = millis();
//   }  
//   if(i2cDisable = true){
//     sendI2CData = false;
//   }
//   if(sendI2CData == true){
//     sendFastDataSet();
//     lastModemDataInterval = millis();
//   }
  if(startUpLoop == true){
    sendFastDataSet();
    lastModemDataInterval = millis();
    startUpLoop = false;
  }

}   //END LOOP


