#include "Gateway.h"
#include "TickTimer.h"
//#include "application.h"
#include "SdCard.h"
#include "defines.h"
#include "Serial_El_Load.h"
#include "Globals.h"
#include "CANtx.h"

static const unsigned int scawGatewayTimers[NUM_GATEWAY_TIMERS] =
{
  /* CT_RESERVED1           */   2000,  //ms -
  /* CT_RESERVED2           */   9000,  //ms 
  /* CT_RESERVED3           */   200,   //ms -
  /* CT_RESERVED4           */   2000,  //ms 
  /* CT_RESERVED5           */   8000, //ms -
  /* CT_RESERVED6           */   6000,  //ms -
  /* CT_RESERVED7           */   4000,   //ms -
};

static TICK_TIMER_S scastGatewayTimers[NUM_GATEWAY_TIMERS];

void ResetGatwayTimer(GATEWAY_TIMERS eGatewayTimer);






void ResetGatwayTimer(GATEWAY_TIMERS eGatewayTimer)
{
  if (eGatewayTimer < NUM_GATEWAY_TIMERS)
  {
    SetTimerWithMilliseconds(&scastGatewayTimers[eGatewayTimer], scawGatewayTimers[eGatewayTimer]);
  }
}
