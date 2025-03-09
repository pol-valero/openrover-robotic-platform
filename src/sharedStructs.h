#ifndef SHARED_STRUCTS_H
#define SHARED_STRUCTS_H

#include <Arduino.h>

typedef struct {
  int y1;
  int x1;
  int y2;
  int x2;
  int aux1;
  int aux2;
  int aux3;
  int aux4;
} RcValues;

typedef struct {
  float cellVoltage;
  int percentage;
} BatteryValues;

typedef struct __attribute__((packed)) {
  uint8_t type; //Integer value identifying the type of frame
  int16_t data2B[4]; //Large data to be sent (2 bytes each) (ex.- joystick values -255...255)
  uint8_t data1B[5]; //Small data to be sent (1 byte each)
  //TODO: Add field for checksum/validation?
} Frame;

typedef struct {
  int temperature;
  int humidity;
  int pressure;
  int altitude;
} EnvironmentalValues;

typedef struct {
  int rpm;
  int metersPerHour;
  int distance;
} SpeedometerValues;

typedef struct {
  bool online;
  bool cameraOn;
  int cpuTemperature;
  int cpuWorkload;
} RaspberryPiStatusValues;

#endif