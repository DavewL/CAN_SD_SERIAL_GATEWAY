#include "Gateway.h"
#include "GatewayMessages.h"
#include "TickTimer.h"
//#include "application.h"
#include "SdCard.h"
#include "defines.h"
#include "Serial_El_Load.h"
#include "Globals.h"
//#include "CANtx.h"

int testRoutineSource;
int testState;

float SD_Voltage = 0.0;
float SD_Current = 0.0;
float SD_Power = 0.0;
int SD_Time = 0;


static const unsigned int scawGatewayTimers[NUM_GATEWAY_TIMERS] =
{
  /* CT_EL_INIT_DELAY       */   1000,  //ms - DELAY AFTER INITIALIZING THE ELECTRONIC LOAD BEFORE REQUESTING "REMOTE"
  /* CT_RESERVED2           */   9000,  //ms 
  /* CT_RESERVED3           */   200,   //ms -
  /* CT_RESERVED4           */   2000,  //ms 
  /* CT_RESERVED5           */   8000, //ms -
  /* CT_RESERVED6           */   6000,  //ms -
  /* CT_RESERVED7           */   4000,   //ms -
};

static TICK_TIMER_S scastGatewayTimers[NUM_GATEWAY_TIMERS];

void ResetGatwayTimer(GATEWAY_TIMERS eGatewayTimer);
int elLoadDelayExpired(void);


void initGateway(void){
    unsigned int wIndex;
    for (wIndex = 0; wIndex < ((unsigned int)NUM_GATEWAY_TIMERS); wIndex++)
    {
        RegisterTimer(&scastGatewayTimers[wIndex]);
        ResetGatwayTimer((GATEWAY_TIMERS)wIndex);
    }
    initSerialElLoad();
    delay(500);
    putElLoadIntoRemote();
    delay(500);
    turnElLoadOFF();
    delay(500);
    setElLoadToFixed();
    delay(500);
}

void ServiceGateway(void){

    static int GatewayReset = 0;
    static int DischargePrep = 0;
    static int DischargeStart = 0;
    static int DischargeFinished = 0;

    if (testRoutineSource == GATEWAY_AUTO){

        if (testState == INIT_STATE){
            if (GatewayReset == 0){
                //turnElLoadOFF();
                //setElLoadToFixed();
                GatewayReset = 1;
            }
        }
        else if (testState == CHARGE_STATE){
            //get ready for discharge cycle
            DischargePrep = 0;
            DischargeStart = 0;
            if (GatewayReset == 0){
                turnElLoadOFF();
                //setElLoadToFixed();
                GatewayReset = 1;
            }
        }
        else if (testState == DISCHARGE_STATE){
            //set the EL load to power mode and start SD card playback
            if (DischargePrep == 0){
                RestartFrameFileLines();
                testSubCycleCount = 0;
                setElLoad2PwrMode();
                ReadFrameLine();  //begin reading data frames from the SD card
                DischargePrep = 1;
            }
            /* else if (DischargeStart == 0){
                if (startListRunning()){  //when function returns a 1, all messages have been sent
                    DischargeStart = 1;
                }
            } */
            else{
                ReadFrameLine();  //try to read the next data frame from the SD card
                transmitGatewayMsg1();
            }
        }
        else if (testState == SHUTDOWN_STATE){
            if (DischargeFinished == 0){
                turnElLoadOFF();
                //setElLoadToFixed();
                DischargeFinished = 1;
            }
        }
        else if (testState == RESET_STATE){
            GatewayReset = 0;
        }
    }
    
    transmitGatewayMsg2();

}

int elLoadDelayExpired(void){
    if (TimerExpired(&scastGatewayTimers[CT_EL_INIT_DELAY])){
        return 0;
    }
    else{
        return 1;
    }
}

void ResetGatwayTimer(GATEWAY_TIMERS eGatewayTimer)
{
  if (eGatewayTimer < NUM_GATEWAY_TIMERS)
  {
    SetTimerWithMilliseconds(&scastGatewayTimers[eGatewayTimer], scawGatewayTimers[eGatewayTimer]);
  }
}
