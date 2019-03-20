 #include "defines.h"
 #include "TickTimer.h"
#include "GatewayMessages.h"
 #include "Globals.h"

 
extern Carloop<CarloopRevision2> carloop;

 static const unsigned int scawGateCanTimers[NUM_GATE_CAN_TIMERS] =
 {
   /* CT_GATE_CAN_LOST_DELAY   */   1000,  //ms -delay before BMS R3 CAN is assumed lost
 };

 static TICK_TIMER_S scastGateCanTimers[NUM_GATE_CAN_TIMERS];

void ResetGatewayTimer(GATE_CAN_TIMERS eGateCanTimer);

void initGatewayMessages(void){
  unsigned int wIndex;
  for (wIndex = 0; wIndex < ((unsigned int)NUM_GATE_CAN_TIMERS); wIndex++){
   RegisterTimer(&scastGateCanTimers[wIndex]);
   ResetGatewayTimer((GATE_CAN_TIMERS)wIndex);
  }
  //battType = VALENCE_REV3;
}

void receiveGatewayMesages(CANMessage message){
    if (message.id == TEST_CONFIG_ID){ //-----------------------------------------
        ResetGatewayTimer(CT_GATE_CAN_LOST_DELAY);
        testRoutineSource = message.data[0];
        testState = message.data[1];
    }
    if (message.id == BATT_STATS_ID){
        ResetGatewayTimer(CT_GATE_CAN_LOST_DELAY);
    }
    if (message.id == EL_LOAD_CMD_ID){
        ResetGatewayTimer(CT_GATE_CAN_LOST_DELAY);
    }

}

void transmitGatewayMsg1(void){
    uint16_t bitsVoltage = (uint16_t)(SD_Voltage*128);  //SD_Voltage is float
    int16_t bitsCurrent = (int16_t)(SD_Current*128);   //SD_Current is float
    int16_t bitsPower = (int16_t)(SD_Power);          //SD_Power is float
    uint8_t bitsTime = (uint8_t)(SD_Time);

    CANMessage message;
    message.id = GATEWAY_MSG1_ID;
    message.len = 8;
    message.data[0] = (byte)(bitsVoltage >> 0) & 0xFF;
    message.data[1] = (byte)(bitsVoltage >> 8) & 0xFF;
    message.data[2] = (byte)(bitsCurrent >> 0) & 0xFF;
    message.data[3] = (byte)(bitsCurrent >> 8) & 0xFF;
    message.data[4] = (byte)(bitsPower >> 0) & 0xFF;
    message.data[5] = (byte)(bitsPower >> 8) & 0xFF;
    message.data[6] = (byte)bitsTime;
    message.data[7] = 0x00;

    carloop.can().transmit(message);
}

void transmitGatewayMsg2(void){
    uint16_t bitsCycles = (uint16_t)(testSubCycleCount);
    //uint16_t bitsBattCycles = (uint16_t)(testCycleCount);


    CANMessage message;
    message.id = GATEWAY_MSG2_ID;
    message.len = 4;
    message.data[0] = (byte)(bitsCycles >> 0) & 0xFF;
    message.data[1] = (byte)(bitsCycles >> 8) & 0xFF;


    carloop.can().transmit(message);
}


void ResetGatewayTimer(GATE_CAN_TIMERS eGateCanTimer)
{
  if (eGateCanTimer < NUM_GATE_CAN_TIMERS)
  {
    SetTimerWithMilliseconds(&scastGateCanTimers[eGateCanTimer], scawGateCanTimers[eGateCanTimer]);
  }
}


int GatewayCANok(void){
    if (TimerExpired(&scastGateCanTimers[CT_GATE_CAN_LOST_DELAY])){
        return 0;
    }
    else{
        return 1;
    }
}