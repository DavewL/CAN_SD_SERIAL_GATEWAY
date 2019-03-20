/*
 * Project CAN_SD_SERIAL_GATEWAY
 * Description:
 * Author:
 * Date:
 */
#include "TickTimer.h"
#include "defines.h"
#include "Tasks.h"
#include "CANrec.h"
#include "../LIBRARIES/SdFat/src/SdFat.h"
#include "../LIBRARIES/SparkIntervalTimer/src/SparkIntervalTimer.h"
#include "../LIBRARIES/carloop-library/src/carloop.h"

SYSTEM_THREAD(ENABLED);

void fiveMsTimer(void);
bool scheduler5msExpired = 0;   //flag for os timer
IntervalTimer myTimer;          //used to create the 1ms timer
int timedLoop = 0;              //used for the task scheduler



String strWrFileName;           //name of file for writing data
String strRdFileName;           //name of file for read only

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

//task timer variable
unsigned long lastTime;
int onceThrough = 0;   //init tasks in periodic scheduler


#define OS_10MS_MASK    0x0001
#define OS_20MS_MASK    0x0002
#define OS_40MS_MASK    0x0004
#define OS_80MS_MASK    0x0008
#define OS_160MS_MASK   0x0010
#define OS_320MS_MASK   0x0020
#define OS_640MS_MASK   0x0040
#define OS_1280MS_MASK  0x0080

void setup() {

    Serial.begin(9600);
    //delay(1000);
    myTimer.begin(fiveMsTimer, 4999, uSec);
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
    if (scheduler5msExpired){
    //if ((thisTime-lastTime) >= 5){
        scheduler5msExpired = 0;
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
