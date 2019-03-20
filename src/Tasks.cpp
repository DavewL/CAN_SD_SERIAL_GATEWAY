#include "Tasks.h"
#include "SdCard.h"
//#include "CANrec.h"
//#include "Display.h"
//#include "LED.h"
//#include <LiquidCrystal_I2C_Spark.h>
//#include <ITEADLIB_Nextion.h>
//#include "CycleTest.h"
#include "TickTimer.h"
//#include "CANtx.h"
#include "CANinit.h"
#include "Globals.h"
//#include "DeltaQ_CANopen.h"
#include "defines.h"
//#include "Network.h"
#include "Gateway.h"

int OneSecCounter = 0; //used to run a 1 second timer
int TwoSecCounter = 0;
int FiveSecCounter = 0;
int FifteenSecCounter = 0;

//extern String battCurr2String;
//extern String battVolt2String;
//extern String battMaxTemp2String;
//extern String battSOC2String;
//int lastTime = millis();

void TasksInit(){
    //initLED();  //heartbeat LED
    //Serial.begin(9600);
    //battType = UNKNOWN_BATT; //CUMMINS_REV1;
    //initSDcard();
    //openReadWriteFiles(what2logTxt);
    //initSD(BMS_LOG_MASK|TEST_SUPERVISOR_LOG_MASK); //CSM_TEMP_LOG_MASK
    //initCAN();
    //initDisplay(TWOFORTY_BY_THREETWENTY_SERIAL);  //THREEPOINTTWO_INCH_NEXTION
    //initCycleTest();
    //Log2SD(TEST_SUPERVISOR_LOG_MASK);
    //initNetwork();
    //dispBatteryHeading();
    //initSD(BMS_LOG_MASK);
}

void Tasks5ms(){
  UpdateTimers();
}

void Tasks10ms(){
}

void Tasks20ms(){
  //Serial.printf("%d\n",(millis()-lastTime));
  //lastTime = millis();
}

void Tasks40ms(){
    static int intervalCounter = 0;
    //String battTypeString;    ///debug only

    OneSecCounter++;
    TwoSecCounter++;
    FiveSecCounter++;
    FifteenSecCounter++;

    if (OneSecCounter >= ONESECCOUNT){    ///////----ONE SECOND TIMER
      //put 1 Second tasks here
        if ((onceThrough == 0)){
          initSDcard();
          openReadWriteFiles(what2logTxt);
          initCAN();

          onceThrough = 1;
        }
        OneSecCounter = 0;
      //toggleLED();    //toggle heartbeat LED
    }
    if (TwoSecCounter >= TWOSECCOUNT){   ////////---TWO SECOND TIMER
      //put 2 second tasks here

      //else {Serial.println("onceThrough = 1");}

      TwoSecCounter = 0;
    }
    if (FiveSecCounter >= FIVESECCOUNT){   //////----FIVE SECOND TIMER
      //put 5 Second tasks here
      //Serial.printf("millis: %d\n",(millis()-lastTime));
      //lastTime = millis();
      FiveSecCounter = 0;
    }
    if (FifteenSecCounter >= FIFTEENSECCOUNT){   //////----FIFTEEN SECOND TIMER
      //put 15 Second tasks here
      //dispBatterySOC();
      /*
      if (intervalCounter == 1){
        Particle.publish("current", battCurr2String, PRIVATE);
      }
      else if (intervalCounter == 2){
        Particle.publish("voltage", battVolt2String, PRIVATE);
      }
      else if (intervalCounter == 3){
        Particle.publish("temperature", battMaxTemp2String, PRIVATE);
      }
      else if (intervalCounter == 4){
        Particle.publish("SOC", battSOC2String, PRIVATE);
        intervalCounter = 0;
      }
      intervalCounter++; 
      */
      FifteenSecCounter = 0;
    }
}

void Tasks80ms(){
  if (onceThrough == 1){
    ServiceGateway();
    //sendStatusCAN();
  }
}

void Tasks160ms(){
  if (onceThrough == 1){
    //transmitIC1200VoltsAmps();
    //if(battType==VALENCE_REV3){
      //transmitRPDO1();
    //}
  }
}

void Tasks320ms(){
}

void Tasks640ms(){
  //Particle.process();
  //checkNetwork();
}

void Tasks1280ms(){
 // static int intervalCounter = 1;
  if (onceThrough == 1){
    /*
    dispBatteryAmps();

    if (intervalCounter == 1){
      dispBatteryStats();
    }
    else if (intervalCounter == 2){
      dispBatteryVolt();
    }
    else if (intervalCounter == 3){
      dispBatteryTemps();
    }
    else if (intervalCounter == 4){
      dispBatteryCSM();
      intervalCounter = 0;
    }
    else{
      intervalCounter=0;
    }
    intervalCounter++;
    */
  }

  //checkNetwork();
  //dispBattery();
  //Serial.printf("millis: %d\n",millis());
  //lastTime = millis();

}
