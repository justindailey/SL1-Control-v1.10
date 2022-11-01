#include<Arduino.h>
#include <functions.h>

//WATER LEVEL LOW
void waterLevelError(){
    //TURN OFF WATER SYSTEM
    nozzlesOFF();
    digitalWrite(waterHeater1, LOW);
    digitalWrite(waterHeater2, LOW);
    digitalWrite(waterPump, LOW);
    digitalWrite(airPumpPin, LOW);
    digitalWrite(airDryerPin, LOW);
    digitalWrite(airFanPin, LOW);
    digitalWrite(greasePump, LOW);
    error = 1;
    sendFastDataSet();
    tft.fillScreen(ILI9341_RED);
    tft.setFont();
    tft.setCursor(70, 20);
    tft.setTextSize(4);
    tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
    tft.println("WARNING!");
    tft.setCursor(15, 70);
    tft.setTextSize(3);  
    tft.println("WATER LEVEL LOW!");
    tft.setCursor(50, 120);
    tft.setTextSize(2);  
    tft.println("Please Add Coolant");
    tft.setCursor(60, 210);
    tft.setTextSize(2);  
    tft.println("Resume When Full");

    //READ WATER LEVEL
    while (waterLevelStatus == 1){
        sensorRead();    
    }
    error = 0;
    diagMode = 0;
    greaseStatus = 0;
}

//LOW VOLTAGE ERROR
void lowVoltage(){
//TURN OFF SYSTEM
    nozzlesOFF();
    digitalWrite(waterHeater1, LOW);
    digitalWrite(waterHeater2, LOW);
    digitalWrite(waterPump, LOW);
    digitalWrite(airPumpPin, LOW);
    digitalWrite(airDryerPin, LOW);
    digitalWrite(airFanPin, LOW);
    digitalWrite(greasePump, LOW);
    error = 2;
    sendFastDataSet();
    tft.fillScreen(ILI9341_YELLOW);
    tft.setFont();
    tft.setCursor(70, 20);
    tft.setTextSize(4);
    tft.setTextColor(ILI9341_RED, ILI9341_YELLOW);
    tft.println("WARNING!");
    tft.setCursor(50, 70);
    tft.setTextSize(3);  
    tft.println("VOLTAGE LOW!");
    tft.setCursor(80, 120);
    tft.setTextSize(2);  
    tft.println("Please Charge");
    tft.setCursor(60, 220);
    tft.setTextSize(2);  
    tft.println("Resume When Full");       
     
    while (vin <= vinLOW + 1.00){
        sensorRead();
        if(doorSensorStatus == 1 && vin >= 10.50){digitalWrite(ledPin, HIGH);}
        if(vin <= 10.00){digitalWrite(ledPin, LOW);}
        tft.setTextColor(ILI9341_BLACK, ILI9341_YELLOW);
        tft.setCursor(35, 160);
        tft.setTextSize(3);  
        tft.print("Voltage: ");
        tft.println(vin);
        delay(500);
    }
    error = 0;
    diagMode = 0;
    greaseStatus = 0;
}  

//LOW GREASE ERROR
void lowGrease(){
    if(lowGreaseStatus == 0){
        lowGreaseTriggerCount = cycleCount;
        lowGreaseStatus = 1;
    }
    if(lowGreaseStatus == 1){
        lowGreaseCycleCount = cycleCount - lowGreaseTriggerCount;
        lowGreaseCyclesRemaining = lowGreaseCyclesSet - lowGreaseCycleCount;
    }
    if(lowGreaseCycleCount >=  lowGreaseCyclesSet){
        nozzlesOFF();
        digitalWrite(airPumpPin, LOW);
        digitalWrite(airDryerPin, LOW);
        digitalWrite(airFanPin, LOW);
        digitalWrite(greasePump, LOW);
        error = 3;
        sendFastDataSet();
        tft.fillScreen(ILI9341_ORANGE);
        tft.setFont();
        tft.setCursor(70, 20);
        tft.setTextSize(4);
        tft.setTextColor(ILI9341_RED, ILI9341_ORANGE);
        tft.println("WARNING!");
        tft.setCursor(65, 70);
        tft.setTextSize(3);  
        tft.println("GREASE LOW!");
        tft.setCursor(55, 120);
        tft.setTextSize(2);  
        tft.println("Please Add Grease");
        tft.setCursor(35, 220);
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_WHITE, ILI9341_ORANGE);  
        tft.println("Press Start to Resume");
        int startButtonState = digitalRead(startPin);
        while(startButtonState == 1){
            sensorRead();
            tft.setTextColor(ILI9341_BLUE, ILI9341_ORANGE);
            tft.setCursor(40, 160);
            tft.setTextSize(2);  
            tft.print("Temperature: ");
            tft.println(waterTempIn);
            // WATER TEMP CONTROL
            //TURN ON HEATERS
            if (waterTempIn <= waterSet && waterTempStatus == 2){
                digitalWrite(waterHeater1, HIGH);
                digitalWrite(waterHeater2, HIGH);
                waterTempStatus = 1;
                heater1status = 1;
                heater2status = 1;
                status = 0;    
            }
            //TURN OFF HEATERS & SET WATER TEMP STATUS TO READY
            if (waterTempIn >= (waterSet+5)){
                digitalWrite(waterHeater1, LOW);
                digitalWrite(waterHeater2, LOW);
                heater1status = 0;
                heater2status = 0;
                waterTempStatus = 2;                   
            }
            // WATER LEVEL CONTROL
            if(waterLevelStatus == 1 ){
                digitalWrite(waterPump, LOW);
                waterTempStatus = 0;
                } 
            if(waterLevelStatus == 0){digitalWrite(waterPump, HIGH);}
            // VOLTAGE CONTROL
            if(vin <= vinLOW){
                nozzlesOFF();
                lowVoltage();
            }
            startButtonState = digitalRead(startPin);
            delay(500);
        }
        error = 0;
        diagMode = 0;
        greaseStatus = 0;
    }
}

//E-STOP ERROR
void eStopActive(){
    int eStopButtonState = digitalRead(eStopPin);
    error = 4;
    sendFastDataSet();
    tft.fillScreen(ILI9341_RED);
    tft.setFont();
    tft.setCursor(70, 20);
    tft.setTextSize(4);
    tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
    tft.println("WARNING!");
    tft.setCursor(10, 70);
    tft.setTextSize(3);  
    tft.println("E-Stop Activated!");
    tft.setCursor(60, 120);
    tft.setTextSize(2);  
    tft.println("Machine Disabled");
    tft.setCursor(25, 210);
    tft.setTextSize(2);  
    tft.println("Reset E-Stop to Resume");
    while(eStopButtonState == 1){
        //TURN OFF SYSTEM
        nozzlesOFF();
        digitalWrite(waterHeater1, LOW);
        digitalWrite(waterHeater2, LOW);
        digitalWrite(waterPump, LOW);
        digitalWrite(airPumpPin, LOW);
        digitalWrite(airDryerPin, LOW);
        digitalWrite(airFanPin, LOW);
        digitalWrite(greasePump, LOW);
        eStopButtonState = digitalRead(eStopPin);
        }
    error = 0;    
    diagMode = 0;
    greaseStatus = 0;
}

//HIGH GREASE PRESSURE ERROR
void greasePsiHigh(){
    nozzlesOFF();
    digitalWrite(airPumpPin, LOW);
    digitalWrite(airDryerPin, LOW);
    digitalWrite(airFanPin, LOW);
    digitalWrite(greasePump, LOW);
    error = 5;
    sendFastDataSet();
    tft.fillScreen(ILI9341_ORANGE);
    tft.setFont();
    tft.setCursor(70, 20);
    tft.setTextSize(4);
    tft.setTextColor(ILI9341_RED, ILI9341_ORANGE);
    tft.println("WARNING!");
    tft.setCursor(20, 70);
    tft.setTextSize(3);  
    tft.println("GREASE PSI HIGH!");
    tft.setCursor(90, 120);
    tft.setTextSize(2);  
    tft.println("Please Wait");
    tft.setCursor(15, 220);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_ORANGE);  
    tft.println("Lower Pressure to Resume");
    while(greasePsi >= greasePsiSet){
        sensorRead();
        tft.setTextColor(ILI9341_BLUE, ILI9341_ORANGE);
        tft.setCursor(40, 160);
        tft.setTextSize(2);  
        tft.print("Grease PSI: ");
        tft.println(greasePsi);
    }
    error = 0;
    diagMode = 0;   
}

//AIR PUMP ERROR
void airSystemLeak(){
    nozzlesOFF();
    digitalWrite(waterHeater1, LOW);
    digitalWrite(waterHeater2, LOW);
    digitalWrite(waterPump, LOW);
    digitalWrite(airPumpPin, LOW);
    digitalWrite(airDryerPin, LOW);
    digitalWrite(airFanPin, LOW);
    digitalWrite(greasePump, LOW);
    error = 6;
    sendFastDataSet();
    tft.fillScreen(ILI9341_ORANGE);
    tft.setFont();
    tft.setCursor(70, 20);
    tft.setTextSize(4);
    tft.setTextColor(ILI9341_RED, ILI9341_ORANGE);
    tft.println("WARNING!");
    tft.setCursor(65, 70);
    tft.setTextSize(3);  
    tft.println("AIR LEAK!");
    tft.setCursor(55, 120);
    tft.setTextSize(2);  
    tft.println("Please Leak Test");
    tft.setCursor(35, 220);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_ORANGE);  
    tft.println("Press Start to Resume");    
    waterTempStatus = 2;
    int startButtonState = digitalRead(startPin);
    while(startButtonState == 1){
        sensorRead();
        tft.setTextColor(ILI9341_BLUE, ILI9341_ORANGE);
        tft.setCursor(40, 160);
        tft.setTextSize(2);  
        tft.print("Temperature: ");
        tft.println(waterTempIn);
        // WATER TEMP CONTROL        

        //TURN ON HEATERS
        if (waterTempIn <= waterSet && waterTempStatus == 2){
            digitalWrite(waterHeater1, HIGH);
            digitalWrite(waterHeater2, HIGH);
            waterTempStatus = 1;
            heater1status = 1;
            heater2status = 1;
            status = 0;    
        }
        //TURN OFF HEATERS & SET WATER TEMP STATUS TO READY
        if (waterTempIn >= (waterSet+5)){
            digitalWrite(waterHeater1, LOW);
            digitalWrite(waterHeater2, LOW);
            heater1status = 0;
            heater2status = 0;
            waterTempStatus = 2;                   
        }
        // WATER LEVEL CONTROL
        if(waterLevelStatus == 1 ){
            digitalWrite(waterPump, LOW);
            digitalWrite(waterHeater1, LOW);
            digitalWrite(waterHeater2, LOW);
            heater1status = 0;
            heater2status = 0;            
            waterTempStatus = 0;
            waterLevelError();
            } 
        if(waterLevelStatus == 0){digitalWrite(waterPump, HIGH);}
        // VOLTAGE CONTROL
        if(vin <= vinLOW){
            nozzlesOFF();
            lowVoltage();
        }
        startButtonState = digitalRead(startPin);
        delay(500);
    }
}
