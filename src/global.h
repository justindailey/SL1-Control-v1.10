#include<Arduino.h>
#include<Wire.h>
#include<SPI.h>
//#include <LiquidCrystal_I2C.h>
#include <DHT.h>
//#include <Adafruit_ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_SPIDevice.h>
//#include <Adafruit_I2CDevice.h>
//#include <Adafruit_SPIFlash.h>
//#include <Fonts/FreeSansBold9pt7b.h>
#include <filters.h>


#define TFT_DC 48              
#define TFT_CS 53             
#define TFT_RST 47             
#define TFT_MISO 50           
#define TFT_MOSI 51           
#define TFT_CLK 52 
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//LiquidCrystal_I2C lcd(0x27,20,4);
//Adafruit_ADS1115 ads1115;

//SETTINGS
float version = 1.01;             //Program Version 
int diagMode = 0;                  //0=Default, 3=Diag
boolean i2cDisable = false;        //Completely Disable I2C Sending
boolean startUpLoop = true;        //Run Functions 1X Times Only
int waterSet = 60;                 //Default Water Temp Setting AT Startup
int airTime = 2;                   //Time Air Solenoid is OPEN
int cycleDelay = 20;                //Wait time for switch to move
int airCycles = 3;                 //How Many Times Air Cycles to Every 1 Grease Pump Cycle
int airInterval = 2;               //Time Between Air Solenoids Opening
int greaseTime = 30;               //Time Grease Pump Turns ON
int dwellTime = 1;                 //Time After Grease Pump Turns OFF to Equalize Pressure
int quickHeatDelta = 40;           //Degrees Below Setting for Quick Heat Activation
int airSet = 120;                  //AIR COMP ON PRESSURE
int airOffSet = 150;               //TARGET AIR PRESSURE WHEN FILLING
int airPurgeTime = 1;              //TIME PURGE VALVE IS OPEN (SECONDS)  
int sensorReadTime = 1;            //SENSOR READ INTERVAL (SECONDS)
int iotSendTime = 20;              //SEND DATA TO THINGSPEAK INTERVAL (SECONDS)
int displayRefresh = 1;            //REFRESH LCD DISPLAY (SECONDS)
int ledDelay = 300;                //INTERIOR LED TIMER (SECONDS)
int vinOK = 13;                    //POWER SYSTEM OK
float vinCheck = 12.50;            //CHECK CHARGING SYSTEM
float vinLOW = 11.50;              //LOW VOLTAGE THRESHOLD
int splashDelay = 5000;            //SPLASH SCREEN TIME (MS)
int lowGreaseCyclesSet = 50;        //NUMBER OF CYCLES ALLOWED AFTER LOW GREASE TRIGGER
int greasePsiSet = 1000;           //GREASE PRESSURE WARNING THRESHOLD
int airCompTimeMax = 10;            //MAX COMPRESSOR ON TIME CONTINUOUS (MINS) 
int airCompCoolDown = 10;          //AIR COMP COOL DOWN TIME (MINS)
int waterSetMax = 85;
int waterSetMin = 75;
int greaseTimeMax = 60;
int greaseTimeMin = 10;
int modemDataInterval = 1;        //TIME BETWEEN TX TO MODEM
int modemi2cAddress = 0x9;
int waitformodem = 0;               // 0=ON 1=OFF
float voltFactor = 0.077;
int smoothingFactor = 5;


//VARIABLES
int cycleActive = 0;
int error = 0;
int status = 0;
int deviceCount = 0;
float waterTempIn = 0;
float waterTempOut = 0;
float ambientTemp = 0;
float vout = 0.0;
float vin = 0.0;
int voltValue = 0;                  //BATTERY VOLTAGE VARIABLE
int cycleStatus = 0;
int activeSide = 1;
int activeNozzle = 0;
int nozzleStatus = 0;               //USED TO BYPASS AIRTIME TIMER UPDATE
int activeHeater = 1;
float airRead = 0;
float airVolt = 0;
float airPsi = 0;
boolean runAirPump = true;
boolean sendI2CData = false;
float greaseRead = 0;
float greaseVolt = 0;
float greasePsi = 0;
int greaseLevelStatus = 0;
int tempF = 0;
int humidity = 0;
int eStopStatus = 0;
int doorSensorStatus = 0;
int lastDoorSensorStatus = 0;
int waterLevelStatus = 0;
int airStatus = 1;              //0=PSI LOW, 1=PSI FULL, 2=PURGED, 3=ERROR
int greaseStatus = 0;
int waterTempStatus = 0;
int voltageStatus = 0;
int switchPosition = 0;
int heater1status = 0;
int heater2status = 0;
int lastCycleStatus = 0;
int lastSwitchPosition = 0;
int lastAirStatus = 0;
int lastGreaseStatus = 0;
int lastWaterTempStatus = 0;
int lastWaterLevelStatus = 1;
int lastActiveside = 0;
int lastGreaseLevelStatus = 1;
int lastActiveNozzle = 0;
int lastStatus = 0;
int lastGreaseTime = 0;
int lastWaterSet = 0;
int lastLowGreaseCycleCount = 0;
int cycleCount = 0;                     //FULL CYCLES RAN SINCE BOOT
int lowGreaseCycleCount = 0;            //NUMBER OF CYCLES COMPLETED WHILE LOW GREASE
int lowGreaseStatus = 0;
int lowGreaseTriggerCount = 0;          //CYCLE COUNT AT WHICH LOW GREASE WAS TRIGGERED
int lowGreaseCyclesRemaining = 0;       //NUMBER OF CYCLES REMAINING BEFORE DISABLE
int dataSet = 0;
int modemReadyStatus = 1;               //SIGNAL FROM MODEM 0 = READY
char vinChar [10];
char waterTempInChar [10];
char ambientTempChar [10];
char airPsiChar [10];
char greasePsiChar [10];
float acVoltReading = 0;
float testFrequency = 60;                     // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist
float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float acVolts; // Voltage
//boolean i2cEnable = true;

#define RT0 10000   // Ω
#define B 3977      // K
#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ
float RT, VR, ln, TX, T0, VRT;

float R1 = 30000.0;
float R2 = 7500.0;

float temperatureC = 0;
float temperatureF = 0;

//SMOOTHING VARIABLES
int waterTempInReadIndex = 0;
float wti1 = 0;
float wti2 = 0;
float wti3 = 0;
float wti4 = 0;
float wti5 = 0;



//TIMER VARIABLES
unsigned long startMillis;  
unsigned long currentMillis;
unsigned long sensorReadMillis = sensorReadTime * 1000UL;
unsigned long lastSensorReadMillis = 0;
unsigned long iotSendMillis = iotSendTime * 1000UL;
unsigned long lastIotSendMillis = 0;
unsigned long displayMillis = displayRefresh * 1000UL;
unsigned long lastDisplayMillis = 0;
unsigned long airIntervalMillis = airInterval * 1000UL;
unsigned long lastAirIntervalMillis = 0;
unsigned long airTimeMillis = airTime * 1000UL;
unsigned long lastAirTimeMillis = 0;
unsigned long airOnMillis = 0;
unsigned long airPurgeTimeMillis = airPurgeTime * 1000UL;
unsigned long cycleDelayMillis = cycleDelay * 1000UL;
unsigned long lastCycleDelayMillis = 0;
unsigned long greaseTimeMillis = greaseTime * 1000UL;
unsigned long lastGreaseTimeMillis = 0;
unsigned long ledDelayMillis = ledDelay * 1000UL;
unsigned long lastledDelayMillis = 0;
unsigned long g3startMillis = 0;
unsigned long g3stopMillis = 0;
unsigned long modemDataIntervalMillis = (modemDataInterval * 981UL) * 60UL;
unsigned long lastModemDataInterval = 0;
unsigned long airCompTimeMaxMillis = (airCompTimeMax * 1000UL) * 60UL;
unsigned long lastAirCompTimeMax = 0;
unsigned long airCompCoolDownMillis = (airCompCoolDown * 1000UL) * 60UL;
unsigned long lastairCompCoolDown = 0;
bool airCompTimeStore = true;

//COUNTERS
float heaterOn = 0;
float heaterOff = 0;
float totalOn = 0;
int tempError = 0;
int airPumpCycles = 0;

//PIN SETUP
//INPUTS
const int voltInputPin = A4;
const int waterOutPin = A6;
const int inputVoltagePin = A5;
const int waterInPin = A8;
const int airInputPin = A10;
const int greasePsiPin = A12;
const int signalPin = 55;
const int startPin = 23;
const int tempUpPin = 26;
const int tempDownPin = 25;
const int greaseUpPin = 28;
const int greaseDownPin = 27;
const int waterLevelPin = 54;
const int greaseLevelPin = 57;
const int doorSensorPin = 56;
const int eStopPin = 22;
const int modemReadyPin = 19;


//OUTPUTS
const int air1 = 31;
const int air2 = 32;
const int air3 = 33;
const int air4 = 34;
const int air5 = 35;
const int air6 = 36;
const int air7 = 37;
const int air8 = 38;
const int air9 = 39;
const int air10 = 40;
const int greasePump = 7;
const int waterPump = 6;
const int waterHeater1 = 30;
const int waterHeater2 = 29;
const int airPumpPin = 8;
const int airPurgePin = 11;
const int airDryerPin = 9;
const int airFanPin = 10;
const int ledPin = 12;
const int backlightPin = 44;
const int interuptPin = 24;

//DEFAULT LCD LAYOUT
    int c1lx = 10;
    int clxSpace = 85;
    int c1dx = 2;
    int cdxSpace = 90;

    int c2lx = c1lx + clxSpace;
    int c3lx = c2lx + clxSpace;
    int c4lx = c3lx + clxSpace;
    int r1ly = 25;

//DIAGNOSTIC LCD LAYOUT  
    //FONT SIZES
    int label = 1;
    int data = 1;

    //GRID LAYOUT
    int colspace = 65;
    int rowspace = 42;
    int rowspacedata = 15;
    int col1 = 3;
    int row1label = 30;

    //DISPLAY GRID SETUP
    int col2 = col1 + colspace;
    int col3 = col2 + colspace;
    int col4 = col3 + colspace;
    int col5 = col4 + colspace;
    int row1data = row1label + rowspacedata;
    int row2label = row1label + rowspace;
    int row2data = row2label + rowspacedata;
    int row3label = row2label + rowspace;
    int row3data = row3label + rowspacedata;
    int row4label = row3label + rowspace;
    int row4data = row4label + rowspacedata;
    int row5label = row4label + rowspace;
    int row5data = row5label + rowspacedata;
