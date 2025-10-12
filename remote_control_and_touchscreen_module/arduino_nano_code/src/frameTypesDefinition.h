#ifndef FRAME_TYPES_DEFINITION_H
#define FRAME_TYPES_DEFINITION_H

//INF_F_ -> Info Frame -> Frame with monitoring data (ex.- temperature, battery level...)
//CMD_F_ -> Command Frame -> Frame with control commands that require extra handling (ex.- change Rover mode, power Raspberry Pi...)
#define NOT_VALID 0
#define INF_F_RC_VALUES 1
#define INF_F_RC_BAT_LEVEL 2
#define CMD_F_RC_BUZZER 3
#define CMD_F_RC_RADIO_ENABLING 4
#define CMD_F_ROVER_OP_MODE_SELECTION 5
#define INF_F_ROVER_BAT_LEVEL 6
#define CMD_F_ARM_SERVO_SELECTION 7
#define INF_F_ENVIRONMENTAL_MONITORIZATION 8
#define INF_F_SPEEDOMETER_VALUES 9
#define INF_F_RASPBERRYPI_STATUS 10
#define CMD_F_RASPBERRYPI_POWER 11
#define CMD_F_TEST 250

#endif