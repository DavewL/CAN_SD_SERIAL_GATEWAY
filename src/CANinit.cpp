#include "CANinit.h"
//#include "Rev3Messages.h"
//#include "CumminsCAN.h"
#include "defines.h"
#include "Globals.h"
#include "Gateway.h"
#include <carloop.h>

Carloop<CarloopRevision2> carloop;

//-----------------INITIALIZE CANBUS---------------------------------------
void initCAN(void){
    //setup CANbus interface
    carloop.begin(CARLOOP_CAN);
    
    carloop.setCANSpeed(GATEWAY_BAUD);        //must set can speed before enabling bus!
    initGateway();
    
    carloop.enableCAN();                        //enable canbus
}
