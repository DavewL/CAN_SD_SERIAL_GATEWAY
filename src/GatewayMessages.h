#include <carloop.h>

#define GATEWAY_MSG1_ID 0x017
#define GATEWAY_MSG2_ID 0x018
#define TEST_CONFIG_ID 0x019
#define BATT_STATS_ID 0x020
#define EL_LOAD_CMD_ID 0x021
//#define SYS_MEAS_TRACE_ID 0x0F0
// #define CONTACTOR_STATES_ID 0x200
// #define MODULE_INFO_ID 0x320
// #define BMS_ID_INFO_ID 0x340
// #define MODULE_ID_INDO_ID 0x380

typedef enum
{
  CT_GATE_CAN_LOST_DELAY,
  NUM_GATE_CAN_TIMERS,
  FIRST_GATE_CAN_TIMER = 0
} GATE_CAN_TIMERS;

void initGatewayMessages(void);
void receiveGatewayMesages(CANMessage);
int GatewayCANok(void);
void transmitGatewayMsg1(void);
void transmitGatewayMsg2(void);