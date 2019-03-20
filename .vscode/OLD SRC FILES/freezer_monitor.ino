// This #include statement was automatically added by the Particle IDE.
#include "Display.h"

// This #include statement was automatically added by the Particle IDE.
#include "Log2SdCard.h"

// This #include statement was automatically added by the Particle IDE.
#include "Rev3Messages.h"

// This #include statement was automatically added by the Particle IDE.
#include "Tasks.h"

// This #include statement was automatically added by the Particle IDE.
#include "CANrec.h"

// This #include statement was automatically added by the Particle IDE.
#include "Rev2Messages.h"

// This #include statement was automatically added by the Particle IDE.
#include <ITEADLIB_Nextion.h>

// This #include statement was automatically added by the Particle IDE.
#include <LiquidCrystal_I2C_Spark.h>

// This #include statement was automatically added by the Particle IDE.
#include <I2cMaster.h>

// This #include statement was automatically added by the Particle IDE.
#include <SdFat.h>

// This #include statement was automatically added by the Particle IDE.
#include <carloop.h>

// This #include statement was automatically added by the Particle IDE.
//#include <SparkIntervalTimer.h>
//#include "LED.h"
#include "TickTimer.h"
#include "defines.h"
//#include "Globals.h"
#include "CycleTest.h"

SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(MANUAL);

//global variable declarations
String test;                    //test variable, may be removed
//IntervalTimer myTimer;          //used to create the 1ms timer
String strWrFileName;           //name of file for writing data
String strRdFileName;           //name of file for read only
//int SDcardInitOK = 0;         //did SD card initialize properly?
//int msgRec = 1;               //count seconds between received messages, init to 1

//int msCounter = 0;                //global that gets incremented on a one mS timer
bool scheduler5msExpired = 0;       //flag to for os timer
int timedLoop = 0;                  //used for the task scheduler

void fiveMsTimer(void);

//#define FLOAT 1
//#define COMPLETE 2

#define OS_10MS_MASK    0x0001
#define OS_20MS_MASK    0x0002
#define OS_40MS_MASK    0x0004
#define OS_80MS_MASK    0x0008
#define OS_160MS_MASK   0x0010
#define OS_320MS_MASK   0x0020
#define OS_640MS_MASK   0x0040
#define OS_1280MS_MASK  0x0080

//battery test variables
const int vMinThresh = 2250; //mv - default alarm is 2300
const int minDischargeLevel = 0;    //% - set to zero to force low voltage alarm to cut off discharger
const int rechargeCutoff = 101;     //% - set to greater than 100 to force charge complete
int keySignalStatus = 0;
int rechargeState = 0;
int cumminsSystemState = 0;
int cumminsDischrgEnabled = 0;
int cumminsInterlockDetected = 0;
int battType = UNKNOWN_BATT;

//battery status variables
float moduleSOCscale = 0.0;
float battVoltage;
float battCurrent = 2.2;
float maxDischargeCurrent;
float maxRegenCurrent;
int chargeStatus =3;
int BMSstatus;
int critDischAlarm;
int warningFlags;
int moduleMinTemperature;
int moduleMaxTemperature;
int battCell1mv = 111;
int battCell2mv = 222;
int battCell3mv = 333;
int battCell4mv = 444;
int battCell5mv = 555;
int battCell6mv = 666;
int battCell7mv = 777;
int battCell8mv = 888;
int moduleMaxMvolts = 0;
int moduleMinMvolts = 0;
int cellsDeltaV = 0;
int battSN = 1;
int underTempChargeStatus = 4;
int underTempDischargeStatus = 4;
int overTemperatureStatus = 4;
int PCBAoverTempStatus = 4;
int battTerminalOverTempStatus = 4;
int overVoltageStatus = 4;
int underVoltageStatus = 4;
int overCurrentCharge = 4;
int overCurrentDischarge = 4;
int cellDeltaTempStatus = 4;
int ModuleLostState = 4;
unsigned int BMSstatusWord = 0;

//CSM module variables
int CsmTemps[8] = {-1,10,100,10,0,0,0,0};
int contactorMain = 0;
int contactorCharge = 0;

//battery test odometry variables
float ampHours = 0;
float cumlAmpHrs = 0;
float wattHours = 0;
float cumlWattHrs = 0;

//heater control variable
int heaterStatus = 0;

//charger control variables
int DQchargerEnable = 0;
int IC1200Enable = 0;
int DQoperational = 0;
float setDQVoltage = 29;
float setDQCurrent = 0.5;
float setIC1200Voltage = 0;
float setIC1200Current = 0;
int DQfixedCurrOverride = 0;
int DQfixedVoltOverride = 0;
float DQvoltage = 0;
float DQcurrent = 0;
float IC1200voltage = 0;
float IC1200current = 0;
float BMSchargeCurrSetpoint = 0;
float BMSchargeVoltSetpoint = 0;
int DQwallPluggedIn = 0;
int IC1200wallPluggedIn = 0;

//int dischargeListFromSD =1;      ///can be removed

//SD Card variables
int readFrameFileExists = 0;
int myFileOk =0;
int testType = CYCLE;
int dischargerType = LIST;
float sdLogRate = 4;
int what2logTxt;
unsigned int testSubCycleCount = 0;
unsigned int testCycleCount = 0;
int flagSDPause = 0;

//int frameFileOk = 0;

//Display cloud connectivity status variable
int SparkNetOk = 0;

//task timer variable
unsigned long lastTime;
int onceThrough = 0;   //init tasks in periodic scheduler

extern String testState2String;

void setup() {

    Serial.begin(9600);
    //delay(1000);
    //myTimer.begin(fiveMsTimer, 4999, uSec);
    TasksInit();
}

void loop() {
    unsigned long thisTime = millis();
    static unsigned long tempTime = thisTime;
    static unsigned long prev10msTime = thisTime;
    static unsigned long prev20msTime = thisTime;
    static unsigned long prev40msTime = thisTime;
    static unsigned long prev80msTime = thisTime;
    static unsigned long prev160msTime = thisTime;
    static unsigned long prev320msTime = thisTime;
    static unsigned long prev640msTime = thisTime;
    static unsigned long prev1280msTime = thisTime;

    if (onceThrough == 1){
      canReceiveMessage();          //check for new CAN messages
    }
    //if (scheduler5msExpired){
    if ((thisTime-lastTime) >= 5){
        //scheduler5msExpired = 0;
        lastTime = thisTime;
        timedLoop++;

        //Serial.printf("millis: %d\n",(thisTime-tempTime));
        //tempTime = thisTime;
        Tasks5ms();

        if ((timedLoop & OS_10MS_MASK) == OS_10MS_MASK){
          //Serial.printf("millis: %d\n",(thisTime-tempTime));
          //tempTime = thisTime;
          Tasks10ms();
        }

        else if ((timedLoop & OS_20MS_MASK) == OS_20MS_MASK){
          Tasks20ms();
        }

        else if ((timedLoop & OS_40MS_MASK) == OS_40MS_MASK){
          Tasks40ms();
        }

        else if ((timedLoop & OS_80MS_MASK) == OS_80MS_MASK){
          Tasks80ms();
        }

        //else if ((timedLoop & OS_160MS_MASK) == OS_160MS_MASK){
        else if ((thisTime-prev160msTime) >= 160){
          //Serial.println(testState2String);
          //Serial.printf("millis: %d\n",(thisTime-tempTime));
          //tempTime = thisTime;
          prev160msTime = thisTime;
          Tasks160ms();
        }

        else if ((thisTime-prev320msTime) >= 320){
          //Serial.printf("millis: %d\n",(thisTime-tempTime));
          //tempTime = thisTime;
          prev320msTime = thisTime;
          Tasks320ms();
        }

        else if ((thisTime-prev640msTime) >= 640){
          //Serial.printf("millis: %d\n",(thisTime-tempTime));
          //tempTime = thisTime;
          prev640msTime = thisTime;
          Tasks640ms();
        }

        else if ((thisTime-prev1280msTime) >= 1280){
          //Serial.printf("millis: %d\n",(thisTime-tempTime));
          //tempTime = thisTime;
          //Serial.println(testState2String);
          prev1280msTime = thisTime;
          Tasks1280ms();
        }
    }
}

void fiveMsTimer(){
    scheduler5msExpired = 1;
}
