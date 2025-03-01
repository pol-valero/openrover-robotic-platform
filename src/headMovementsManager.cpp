#include <Arduino.h>

#include "headMovementsManager.h"
#include "BasicStepperDriver.h"
#include "rcValuesManager.h"
#include "statusDataManager.h"

#define MOTOR_STEPS 200
#define RPM 10

#define MICROSTEPS 4

#define DIR 42
#define STEP 43

const int stepperTravelLimitSensorPin = 44;

int stepperHeadPanAngle = 0;

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

void setupHeadServoStepper() {
    stepper.begin(RPM, MICROSTEPS);
    pinMode(stepperTravelLimitSensorPin, INPUT_PULLUP);
}

void testHeadServoStepper() { 

    if (digitalRead(stepperTravelLimitSensorPin) == LOW) {
    stepper.stop();
    stepper.startRotate(-100);
    }

    static unsigned long previousMillis = 0;

    if (millis() - previousMillis >= 4000 && digitalRead(stepperTravelLimitSensorPin) == HIGH) {
    previousMillis = millis();
    stepper.startRotate(80);
    }
    stepper.nextAction();
    
}

bool stepperHeadPanAngleInBounds(int angle) {
    return angle >= -235 && angle <= 365;
}

void setStepperHeadPanAngle() {

    //If we are trying to pan the head (joystick X axis is not at the center), we disable the sending of status data, as it is a bit slow and it can make the stepper movements less fluid. 
    if (!joystickX_isCentered(JOY_LEFT)) {
        enableStatusData(false);
    } else {
        enableStatusData(true);
    }

    static unsigned long previousMillis = 0;

    //We increment/decrement the stepper angle every 10ms
    if (millis() - previousMillis >= 10) {
        previousMillis = millis();

        if (joystickIsRight(JOY_LEFT) && stepperHeadPanAngleInBounds(stepperHeadPanAngle + 1)) {
            stepper.startRotate(1);
            stepperHeadPanAngle += 1;
        } else if (joystickIsLeft(JOY_LEFT) && stepperHeadPanAngleInBounds(stepperHeadPanAngle - 1)) {
            stepper.startRotate(-1);
            stepperHeadPanAngle -= 1;
        } else {
            stepper.stop();
        }

        stepper.nextAction();
    }

}