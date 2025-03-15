#include <Arduino.h>
#include <Servo.h>

#include "headMovementsManager.h"
#include "BasicStepperDriver.h"
#include "rcValuesManager.h"
#include "statusDataManager.h"

#define MOTOR_STEPS 200
#define RPM 10

#define MICROSTEPS 4

#define HEAD_TILT_HORIZONTAL_ANGLE 90


const int stepperDirPin = 42;
const int stepperStepPin = 43;
const int stepperTravelLimitSensorPin = 44;

const int servoHeadTiltPin = 41;

int stepperHeadPanAngle = 0;
int servoHeadTiltAngle = HEAD_TILT_HORIZONTAL_ANGLE;

BasicStepperDriver stepper(MOTOR_STEPS, stepperDirPin, stepperStepPin);
Servo servoHeadTilt;


void setupHeadServoStepper() {
    stepper.begin(RPM, MICROSTEPS);
    pinMode(stepperTravelLimitSensorPin, INPUT_PULLUP);
    servoHeadTilt.attach(servoHeadTiltPin);
    servoHeadTilt.write(HEAD_TILT_HORIZONTAL_ANGLE);    //We set the head tilt servo to its initial position, horizontally level, which is 90 degrees
}

bool stepperHeadPanAngleInBounds(int angle) {
    //NOTE: Angle = 0 is the "home" position of the stepper, where the head is looking forward and is parallel to the front wall of the rover. 

    //As we are incrementing/decrementing the angle by 1 every 10ms (in order to keep track with the "stepperHeadPanAngle" variable of the position we are in),
    //the angle is not really in decimal degrees (as the library is designed to increment/decrement by an angle without the need of calling every X ms the "startRotate" function)
    return angle >= -220 && angle <= 350;
}

bool initializeStepperHomePosition() {

    static bool stepperHomePositionInitialized = false;

    //After 10 seconds from the first moment the Head Control SW in the RC was clicked, the stepper has had enough time to trigger the limit sensor and then return to its home position
    static unsigned long previousMillis1 = millis();
    if (millis() - previousMillis1 >= 10000) {
        previousMillis1 = millis();
        stepperHomePositionInitialized = true;
    }

    if (stepperHomePositionInitialized == false) {

        enableStatusData(false);    //We disable the sending of status data, as it is a bit slow and it can make the stepper movements less fluid.

        static bool travelLimitSensorTouched = false;

        if (digitalRead(stepperTravelLimitSensorPin) == LOW) {
            travelLimitSensorTouched = true;
            stepper.stop();
            stepper.startRotate(-160);
        }

        if (digitalRead(stepperTravelLimitSensorPin) == HIGH && travelLimitSensorTouched == false) {
            stepper.startRotate(80);    //80 is an arbitrary angle, could be 100, 360... we just want to make the stepper move clockwise until it touches the limit sensor
        }
        stepper.nextAction();

        return false; //We return "false", to indicate that the stepper is not yet in its home position
    }

    return true;    //We return "true", to indicate that the stepper is in its home position

}

void setStepperHeadPanAngle() {

    //We initialize the stepper to its home position, and before executing the rest of the code in the "setSepperHeadPanAngle" function, 
    //we wait for the "initializeStepperHomePosition" function to return true (meaning that the stepper finally is in its home position)
    if (!initializeStepperHomePosition()) {
        return;
    }

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

bool servoHeadTiltAngleInBounds(int angle) {
    return angle >= 60 && angle <= 120;
}

void setServoHeadTiltAngle() {
    
    static unsigned long previousMillis = 0;

    if (millis() - previousMillis >= 40) {
        previousMillis = millis();

        if (joystickIsUp(JOY_RIGHT) && servoHeadTiltAngleInBounds(servoHeadTiltAngle + 1)) {
            servoHeadTiltAngle++;
        }

        if (joystickIsDown(JOY_RIGHT) && servoHeadTiltAngleInBounds(servoHeadTiltAngle - 1)) {
            servoHeadTiltAngle--;
        }

        servoHeadTilt.write(servoHeadTiltAngle);

    }

}