#ifndef FRAME_TYPES_DEFINITION_H
#define FRAME_TYPES_DEFINITION_H

//INF_F_ -> Info Frame -> Frame with monitoring data (ex.- temperature, battery level...)
//CMD_F_ -> Command Frame -> Frame with control commands that require extra handling (ex.- change Rover mode, power Raspberry Pi...)
#define NOT_VALID 0
#define INF_F_RC_VALUES 1
#define INF_F_RC_BAT_LEVEL 2
#define CMD_F_RC_BUZZER 3
#define CMD_F_RC_RADIO_ENABLING 4
#define CMD_F_TEST 250

#endif