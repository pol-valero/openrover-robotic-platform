/*Mars rover replica - by Pol V

Creation date: 5/10/23
Last modification date: 9/7/24
*/


//IMPORTANT: We use software PWM which uses TIMER1, we cannot use analogWrite() on pins 13,12,11, as it uses TIMER1 too.
//We must use software PWM because there are not enough usable PWM ports in the arduino mega. 

#include <Arduino.h>

#include <Servo.h>  

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <PalatisSoftPWM.h>

#define MAX_INTEGER 32766

#define FWD true
#define BCK false

#define RADIO_OK true
#define RADIO_KO false

#define JOY_LEFT 1
#define JOY_RIGHT 2

#define OP_CONVENTIONAL_DRIVING 1
#define OP_360_DEGREE_TURN_CONTROL 2
#define OP_ROBOTIC_ARM_CONTROL 3
#define OP_HEAD_CONTROL 4
//We will use the RC remote to enable this operations and then the RC can be turned off. When turned on again, we will be able to change the operation to another one. 
#define OP_WEB_CONTROL 5
#define SUB_OP_360_DEGREE_TO_CONVENTIONAL 6

//TODO: Maybe we will need to distinguish between RC_OP_WHATEVER and WEB_OP_WHATEVER (if we want to do exactly the same operations via the web as in the remote control)
//TODO:IMPORTANT: Maybe it would be interesting to put all the WEB control operations and functions in another .c. So that we would have a main.c with just the void (setup, loop), a rcControl.c, a webControl.c (with its corresponding .h) and a variables.h (or globalVariables.h) with all the shared variables.
//(it would also be interesting to have other modules motors.c, wheelServos.c... with the functions that need to be shared ex.- setMotorSpeed(), calculateWheelServosAngle()...). Alternatively, we could just have a sharedFunctions.c to group them all. 
 

#define W_SERVO_1 1
#define W_SERVO_2 2
#define W_SERVO_3 3
#define W_SERVO_4 4

#define MOTOR_1 1
#define MOTOR_2 2
#define MOTOR_3 3
#define MOTOR_4 4
#define MOTOR_5 5
#define MOTOR_6 6


int op_mode = OP_CONVENTIONAL_DRIVING;
bool entered_new_op_mode;


#define MOT1_FWD 0
#define MOT1_BCK 1
#define MOT2_FWD 2
#define MOT2_BCK 3
#define MOT3_FWD 4
#define MOT3_BCK 5

#define MOT4_FWD 6
#define MOT4_BCK 7
#define MOT5_FWD 8
#define MOT5_BCK 9
#define MOT6_FWD 10
#define MOT6_BCK 11

//The rover has two arms, each one with 3 motors and 2 servomotors for steerin.

//Right arm motors, from front to back
const int mot1_fwd = 8; //Motor 1 forward rotation pin
const int mot1_bck = 7; //Motor 1 backward rotation pin

const int mot2_fwd = 10;
const int mot2_bck = 9; 

const int mot3_fwd = 12;
const int mot3_bck = 11; 


//Left arm motors, from front to back
const int mot4_fwd = A0; 
const int mot4_bck = A1; 

const int mot5_fwd = A2;
const int mot5_bck = A3; 

const int mot6_fwd = A4;
const int mot6_bck = A5; 


SOFTPWM_DEFINE_PIN8_CHANNEL(MOT1_FWD);
SOFTPWM_DEFINE_PIN7_CHANNEL(MOT1_BCK);
SOFTPWM_DEFINE_PIN10_CHANNEL(MOT2_FWD);
SOFTPWM_DEFINE_PIN9_CHANNEL(MOT2_BCK);
SOFTPWM_DEFINE_PIN12_CHANNEL(MOT3_FWD);
SOFTPWM_DEFINE_PIN11_CHANNEL(MOT3_BCK);

SOFTPWM_DEFINE_PINA0_CHANNEL(MOT4_FWD);
SOFTPWM_DEFINE_PINA1_CHANNEL(MOT4_BCK);
SOFTPWM_DEFINE_PINA2_CHANNEL(MOT5_FWD);
SOFTPWM_DEFINE_PINA3_CHANNEL(MOT5_BCK);
SOFTPWM_DEFINE_PINA4_CHANNEL(MOT6_FWD);
SOFTPWM_DEFINE_PINA5_CHANNEL(MOT6_BCK);

SOFTPWM_DEFINE_OBJECT(12); //Number of Software PWM Channels



int outer_wheels_speed;
int inner_front_back_wheels_speed;
int inner_middle_wheel_speed;


//Right arm servos, from front to back
const int w_servo1_pin = 22; //Wheel servo1 pin number
const int w_servo2_pin = 23;

Servo w_servo1;
Servo w_servo2;

//Left arm servos, from front to back
const int w_servo3_pin = 24;
const int w_servo4_pin = 25;

Servo w_servo3;
Servo w_servo4;

//Wheel servos center position, in degrees. 
// {BLANK SPACE, servo1_angle, servo2_angle, servo3_angle, servo4_angle}
const int w_servo_center[] = {0,98,95,90,103};

//The turning radius of the whole rover, measured from the rover's center in mm
int turning_radius;

const int d1_distance_front_middle_wheels = 300; //Distance between the front and middle wheel, in mm
const int d2_distance_back_middle_wheels = 275; //Distance between the back and middle wheel, in mm

//Distances between the longitudinal center of the wheel and the longitudinal center of the rover. 
const int d3_front_back_rover_width = 270; //The front and back wheels have the same distance to the longitudinal center of the rover
const int d4_center_rover_width = 300;

//To have a certain turning radius, this are the angles that we will add or subtract to each servo center angle 
int outer_servo_front_angle_beta; 
int outer_servo_back_angle_beta;
int inner_servo_front_angle_beta;
int inner_servo_back_angle_beta;

const int max_turning_radius = 4000;  //In mm
const int min_turning_radius = 700; //TODO: Change to 600 when I have tested the rover


RF24 radio(48, 49); //CE, CSN pins
const byte address[6] = "00001";
int radio_data[6];

typedef struct {


  int lever_1;  //TODO: When updating the transmitter code to send structs, we will change these ints to bool
  int lever_2;

  //TODO: IMPORTANT: Maybe it is a good idea that the arduino in the RC is in charge of sending this values set to 0 if they are within a certain center range and send (1 to 255, 1 to -255) otherwise. 

  //(Positive axis: 1 to 255)
  //(Negative axis: -1 to -255)
  int joystick1_x; 
  int joystick1_y;
  int joystick2_x;
  int joystick2_y;

} Rc_data;

Rc_data rc_data;

bool rcLinkStatus = RADIO_KO;

char buffer[200];

const int batt_input_pin = A15; 

const int servo_relay_pin = 28;

const int buzzer_pin = 29;

void setup() {

  //DO NOT ALTER THE POSITION OF THESE 2 LINES//
  pinMode(servo_relay_pin, OUTPUT);
  digitalWrite(servo_relay_pin, LOW); //We immediately put the relay that powers the servos in an OPEN position to make sure that no power gets to the servos before the program initializes
  //////////////////////////////////////////////

  w_servo1.attach(w_servo1_pin);
  w_servo2.attach(w_servo2_pin);
  w_servo3.attach(w_servo3_pin);
  w_servo4.attach(w_servo4_pin);
  
  PalatisSoftPWM.begin(15); ///15Hz PWM

  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();


  pinMode(mot1_fwd, OUTPUT);
  pinMode(mot1_bck, OUTPUT);

  pinMode(mot2_fwd, OUTPUT);
  pinMode(mot2_bck, OUTPUT);

  pinMode(mot3_fwd, OUTPUT);
  pinMode(mot3_bck, OUTPUT);

  pinMode(mot4_fwd, OUTPUT);
  pinMode(mot4_bck, OUTPUT);

  pinMode(mot5_fwd, OUTPUT);
  pinMode(mot5_bck, OUTPUT);

  pinMode(mot6_fwd, OUTPUT);
  pinMode(mot6_bck, OUTPUT);

  pinMode(batt_input_pin, INPUT);

  pinMode(buzzer_pin, OUTPUT);


  //DO NOT ALTER THE POSITION OF THESE 5 LINES//
  digitalWrite(servo_relay_pin, HIGH);  //We power up the servos and we send them their initial positions
  w_servo1.write(w_servo_center[1]);
  w_servo2.write(w_servo_center[2]);
  w_servo3.write(w_servo_center[3]);
  w_servo4.write(w_servo_center[4]);
  //////////////////////////////////////////////

}

void setMotorSpeed(int motor_num, int speed, bool direction) {

  int mot6_correction_factor = -4; //The mot6 is a bit faster than the rest at low speeds, we apply this correction factor to solve this problem

  if (motor_num == MOTOR_6) {
    speed = speed + mot6_correction_factor;
  }

  if (speed < 0) {
    speed = 0;
  }

  switch (motor_num) {

      case MOTOR_1:
        if (direction == FWD) {
           digitalWrite(mot1_bck, LOW);
           //analogWrite(mot1_fwd, speed);
           PalatisSoftPWM.set(MOT1_FWD, speed);
        } else {
          digitalWrite(mot1_fwd, LOW);
          //analogWrite(mot1_bck, speed);
          PalatisSoftPWM.set(MOT1_BCK, speed);
        }
        break;

      case MOTOR_2:
        if (direction == FWD) {
           digitalWrite(mot2_bck, LOW);
           //analogWrite(mot2_fwd, speed);
           PalatisSoftPWM.set(MOT2_FWD, speed);
        } else {
          digitalWrite(mot2_fwd, LOW);
          //analogWrite(mot2_bck, speed);
          PalatisSoftPWM.set(MOT2_BCK, speed);
        }
        break;

      case MOTOR_3:
        if (direction == FWD) {
           digitalWrite(mot3_bck, LOW);
           //analogWrite(mot3_fwd, speed);
           PalatisSoftPWM.set(MOT3_FWD, speed);
        } else {
          digitalWrite(mot3_fwd, LOW);
          //analogWrite(mot3_bck, speed);
          PalatisSoftPWM.set(MOT3_BCK, speed);
        }
        break;

      case MOTOR_4:
        if (direction == FWD) {
           digitalWrite(mot4_bck, LOW);
           //analogWrite(mot4_fwd, speed);
           PalatisSoftPWM.set(MOT4_FWD, speed);
        } else {
          digitalWrite(mot4_fwd, LOW);
          //analogWrite(mot4_bck, speed);
          PalatisSoftPWM.set(MOT4_BCK, speed);
        }
        break;

      case MOTOR_5:
        if (direction == FWD) {
           digitalWrite(mot5_bck, LOW);
           //analogWrite(mot5_fwd, speed);
           PalatisSoftPWM.set(MOT5_FWD, speed);
        } else {
          digitalWrite(mot5_fwd, LOW);
          //analogWrite(mot5_bck, speed);
          PalatisSoftPWM.set(MOT5_BCK, speed);
        }
        break;

      case MOTOR_6:
        if (direction == FWD) {
           digitalWrite(mot6_bck, LOW);
           //analogWrite(mot6_fwd, speed);
            PalatisSoftPWM.set(MOT6_FWD, speed);
        } else {
          digitalWrite(mot6_fwd, LOW);
          //analogWrite(mot6_bck, speed);
          PalatisSoftPWM.set(MOT6_BCK, speed);
        }
        break;

      default:
        break;

  }

}

void calculateBatteryPercentage() {

  //TODO: Check timer before doing function so that the procedure is only done a couple of times per second and not continuously 

  int batt_divider_voltage_analog_value;  //Analog value read from the voltage divider output, which is 4V when the battery is fully charged
  float batt_divider_voltage; //Volts of the voltage divider output
  int batt_percentage; //Percentage of the battery, from 0% (3.6V per cell) to 100% (4.2V per cell)

  batt_divider_voltage_analog_value = analogRead(batt_input_pin);

  batt_divider_voltage = (5.00 / 1023) * batt_divider_voltage_analog_value;

  //Calculations: The max charge of the battery will be 4.2V per cell, 12.6V in total. 
  //The voltage divider output when fully charged is 4V, therefore if we apply multiply by a correction factor of 4.2 / 4 = 1.05 the resulting voltage will be roughly the same as each cell of the battery. 

  batt_percentage = map(batt_divider_voltage * 1.05 * 100, 4.2 * 100, 3.6 * 100, 100, 0); //We multiply by 100 because the map() function does not accept floats.

  Serial.print("Battery cell voltage: ");
  Serial.print(batt_divider_voltage * 1.05);

  sprintf(buffer, "\tBattery percentage: %d", batt_percentage);
  Serial.println(buffer);

}

void printRcValues() {

  //Print it every 500ms
  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 500) {
    previousMillis = millis();
  
    if (rcLinkStatus == RADIO_OK) {
      sprintf(buffer, "Lever 1: %d\t\tLever 2: %d\t\tJoystick1_x: %d\t\tJoystick1_y: %d\t\tJoystick2_x: %d\t\tJoystick2_y: %d", 
      rc_data.lever_1, rc_data.lever_2, rc_data.joystick1_x, rc_data.joystick1_y, rc_data.joystick2_x, rc_data.joystick2_y);

      Serial.println(buffer);

    } else {
      Serial.println("ERROR: NO CONNECTION WITH RC TRANSMITTER OR ABNORMAL VALUES RECEIVED");
    }

  }

}

//TODO: Remove. Temporal function until we change the tramsitter code
void correctRcValuesReceived() {

  if (rc_data.lever_1 == 1) {
    rc_data.lever_1 = 0;
  } else {
    rc_data.lever_1 = 1;
  }

  if (rc_data.lever_2 == 1) {
    rc_data.lever_2 = 0;
  } else {
    rc_data.lever_2 = 1;
  }


  if (rc_data.joystick1_x > 45 && rc_data.joystick1_x < 75) {
   rc_data.joystick1_x  = 0;
  } else {
    if (rc_data.joystick1_x <= 45) {
      rc_data.joystick1_x = map(rc_data.joystick1_x, 45, -255, 1, 255);
    } else {
      rc_data.joystick1_x = map(rc_data.joystick1_x, 75, 255, -1, -255);
    }
  }

   if (rc_data.joystick1_y > 35 && rc_data.joystick1_y < 65) {
   rc_data.joystick1_y  = 0;
  } else {
    if (rc_data.joystick1_y <= 35) {
      rc_data.joystick1_y = map(rc_data.joystick1_y, 35, -255, -1, -255);
    } else {
      rc_data.joystick1_y = map(rc_data.joystick1_y, 65, 255, 1, 255);
    }
  }

 if (rc_data.joystick2_x > 45 && rc_data.joystick2_x < 85) {
   rc_data.joystick2_x  = 0;
  } else {
    if (rc_data.joystick2_x <= 45) {
      rc_data.joystick2_x = map(rc_data.joystick2_x, 45, -255, 1, 255);
    } else {
      rc_data.joystick2_x = map(rc_data.joystick2_x, 85, 255, -1, -255);
    }
  }

  if (rc_data.joystick2_y > 45 && rc_data.joystick2_y < 75) {
   rc_data.joystick2_y  = 0;
  } else {
    if (rc_data.joystick2_y <= 45) {
      rc_data.joystick2_y = map(rc_data.joystick2_y, 45, -255, 1, 255);
    } else {
      rc_data.joystick2_y = map(rc_data.joystick2_y, 75, 255, -1, -255);
    }
  }
  
  


}

void stopMotors() {

  setMotorSpeed(MOTOR_1, 0, FWD);
  setMotorSpeed(MOTOR_2, 0, FWD);
  setMotorSpeed(MOTOR_3, 0, FWD);
  setMotorSpeed(MOTOR_4, 0, FWD);
  setMotorSpeed(MOTOR_5, 0, FWD);
  setMotorSpeed(MOTOR_6, 0, FWD);

  setMotorSpeed(MOTOR_1, 0, BCK);
  setMotorSpeed(MOTOR_2, 0, BCK);
  setMotorSpeed(MOTOR_3, 0, BCK);
  setMotorSpeed(MOTOR_4, 0, BCK);
  setMotorSpeed(MOTOR_5, 0, BCK);
  setMotorSpeed(MOTOR_6, 0, BCK);

}

void readRcValues() {

  static long msTimeSinceLastPacket = 0;

  Rc_data rc_data_received; //This variable will contain the data received when we change the trasmitter's code so that it sends a struct.
  //TODO: The fields in this variable will be checked to see if any value is off. Only if no value is off we will return RADIO_OK and we will modify the rc_data variable with the new values

  //TODO: Change transmitter code to send the Radio_data struct directly and not an array
   if(radio.available()){

    msTimeSinceLastPacket = 0;

    radio.read(&radio_data, sizeof(radio_data));

    rc_data.lever_1 = radio_data[2];
    rc_data.lever_2 = radio_data[1];

    rc_data.joystick1_x = radio_data[4];
    rc_data.joystick1_y = radio_data[5];
    rc_data.joystick2_x = radio_data[0];
    rc_data.joystick2_y = radio_data[3];

    //TODO: This function will cease to be used when I modify the transmitter code so that it sends 0 if the joysticks are within its center range and so that it sends the correct ranges (ex.- 1 to 255 if right) and so that the lever send 1 if they are up
    correctRcValuesReceived();

    //TODO: Check that ALL values are in range and return RADIO_KO if any value is off. 

    rcLinkStatus = RADIO_OK;
  
  } else {

    static unsigned long previousMillis1 = 0;

    if (millis() - previousMillis1 >= 50) {
      previousMillis1 = millis();

      msTimeSinceLastPacket += 50;
    }

    //If we spend more than 200ms without receiving a packet, the rc connection will be considered as lost
    if (msTimeSinceLastPacket >= 200) {

      rcLinkStatus = RADIO_KO;
      
      static unsigned long previousMillis2 = 0;

      if (millis() - previousMillis2 >= 3000) {
        previousMillis2 = millis();
        tone(buzzer_pin, 200, 500);
      }

    }

    //TODO: IMPORTANT: If we have no signal (RADIO_KO returned), we will immediately disable the motors. (The servo positions will remain with the same values as just before the signal was disconnected).
  }

}


void servoWrite(int servo_identifier,  int degrees) {

  const int w_servo_max_angle = 150;
  const int w_servo_min_angle = 45;

  
  switch (servo_identifier) {
    case W_SERVO_1:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo1.write(degrees);
      } else {
        Serial.println("Servo1 angle out of bounds");
      }
      break;

      case W_SERVO_2:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo2.write(degrees);
      } else {
        Serial.println("Servo2 angle out of bounds");
      }
      break;

      case W_SERVO_3:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo3.write(degrees);
      } else {
        Serial.println("Servo3 angle out of bounds");
      }
      break;

      case W_SERVO_4:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo4.write(degrees);
      } else {
        Serial.println("Servo4 angle out of bounds");
      }
      break;
  }

}


void doWheelServoTest() {

  int pos; 

  for (pos = 70; pos <= 110; pos += 1) { 
    
    servoWrite(W_SERVO_1,pos);              
    //servoWrite(W_SERVO_2,pos);              
    //servoWrite(W_SERVO_3,pos);              
    //servoWrite(W_SERVO_4,pos);              
         
    delay(15);                       
  }
  for (pos = 110; pos >= 70; pos -= 1) { 
    servoWrite(W_SERVO_1,pos);              
    //servoWrite(W_SERVO_2,pos);              
    //servoWrite(W_SERVO_3,pos);              
    //servoWrite(W_SERVO_4,pos);              
                    
    delay(15);                       
  }

}


bool joystickIsRight(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_x > 0;

  } else {

    return rc_data.joystick2_x > 0;

  }

}

bool joystickIsLeft(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_x < 0;

  } else {

    return rc_data.joystick2_x < 0;
    
  }
  
}

bool joystickIsUp(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_y > 0;

  } else {

    return rc_data.joystick2_y > 0;
    
  }

}

bool joystickIsDown(int joystickSelection) {

   if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_y < 0;

  } else {

    return rc_data.joystick2_y < 0;
    
  }

}

bool joystickX_isCentered(int joystickSelection) {
  return !joystickIsRight(joystickSelection) && !joystickIsLeft(joystickSelection);
}

bool joystickY_isCentered(int joystickSelection) {
  return !joystickIsUp(joystickSelection) && !joystickIsDown(joystickSelection);
}

bool joystickXY_isCentered(int joystickSelection) {
  return !joystickX_isCentered(joystickSelection) && !joystickY_isCentered(joystickSelection);
}


void turnControl360Degree() {

  int speed; //From 0 to 255
  
  static int currentAngle = 0;
  const int finalAngle = 45;

  if (entered_new_op_mode == true) { 
    currentAngle = 0;
  }

  static unsigned long previousMillis = 0;

  //We increment/decrement the servos angle every 10ms
  if (millis() - previousMillis >= 10) {
    previousMillis = millis();

    if (currentAngle != finalAngle) {

        currentAngle++;

        servoWrite(W_SERVO_1, w_servo_center[1] - currentAngle);
        servoWrite(W_SERVO_2, w_servo_center[2] + currentAngle);
        servoWrite(W_SERVO_3, w_servo_center[3] + currentAngle);
        servoWrite(W_SERVO_4, w_servo_center[4] - currentAngle);

    }

  }

  if (joystickX_isCentered(JOY_RIGHT)) {

    stopMotors();

  } else {

    speed = abs(rc_data.joystick2_x);

    if (joystickIsLeft(JOY_RIGHT)) {

      setMotorSpeed(MOTOR_1, speed, FWD);
      setMotorSpeed(MOTOR_2, speed, FWD);
      setMotorSpeed(MOTOR_3, speed, FWD);
      setMotorSpeed(MOTOR_4, speed, BCK);
      setMotorSpeed(MOTOR_5, speed, BCK);
      setMotorSpeed(MOTOR_6, speed, BCK);

    } else {

      setMotorSpeed(MOTOR_1, speed, BCK);
      setMotorSpeed(MOTOR_2, speed, BCK);
      setMotorSpeed(MOTOR_3, speed, BCK);
      setMotorSpeed(MOTOR_4, speed, FWD);
      setMotorSpeed(MOTOR_5, speed, FWD);
      setMotorSpeed(MOTOR_6, speed, FWD);

    }

  }

}


void returnFrom360ToConventional () {
  
  static int currentAngle = 0;
  const int finalAngle = 45;

  if (entered_new_op_mode == true) { 
    currentAngle = 0;
  }

  static unsigned long previousMillis = 0;

  //We increment/decrement the servos angle every 10ms
  if (millis() - previousMillis >= 10) {
    previousMillis = millis();
  
    if (currentAngle != finalAngle) {
      currentAngle++;
      servoWrite(W_SERVO_1, (w_servo_center[1] - finalAngle) + currentAngle);
      servoWrite(W_SERVO_2, (w_servo_center[2] + finalAngle) - currentAngle);
      servoWrite(W_SERVO_3, (w_servo_center[3] + finalAngle) - currentAngle);
      servoWrite(W_SERVO_4, (w_servo_center[4] - finalAngle) + currentAngle);
    } else {
      //If we have reached the final angle, we can switch to the conventional driving mode
      op_mode = OP_CONVENTIONAL_DRIVING;
    }

  }

}

//Sets "entered_new_op_mode" to true if we have just changed modes and are executing the first iteration of a operation mode
void updateNewOperationIndicator() {

  static int last_op_mode = OP_CONVENTIONAL_DRIVING;

  if (last_op_mode != op_mode) {
    entered_new_op_mode = true;
  } else {
    entered_new_op_mode = false;
  }

  last_op_mode = op_mode;

}

void setOperationMode() {

  if (rc_data.lever_1 == 0 && rc_data.lever_2 == 0) {

    if (op_mode == OP_360_DEGREE_TURN_CONTROL || op_mode == SUB_OP_360_DEGREE_TO_CONVENTIONAL) {
      op_mode = SUB_OP_360_DEGREE_TO_CONVENTIONAL;
    } else {
      op_mode = OP_CONVENTIONAL_DRIVING;
    }

    updateNewOperationIndicator();

  }  else if (rc_data.lever_1 == 0 && rc_data.lever_2 == 1) {

    op_mode = OP_360_DEGREE_TURN_CONTROL;
    updateNewOperationIndicator();

  } else {
      op_mode = -1;
  }
}


void setWheelServosStraight() {

  servoWrite(W_SERVO_1, w_servo_center[1]);
  servoWrite(W_SERVO_2, w_servo_center[2]);
  servoWrite(W_SERVO_3, w_servo_center[3]);
  servoWrite(W_SERVO_4, w_servo_center[4]);

}


void calculateMotorsSpeed() {
  //Ackerman steering geometry calculations

  int speed;
  int maxSpeed, minSpeed;

  int correctionFactor1 = 0;  //Value that we will subtract from the inner wheels speed, so that tire slipping does not occur when having high speed and high angle
  //These correction factors are necessary because even tough the wheel speed equations are correct, the inner tires slip a bit (due to unknown physical factors) at high speed and high angle
  int correctionFactor2 = 0;

  if (joystickX_isCentered(JOY_RIGHT)) {

    outer_wheels_speed = inner_front_back_wheels_speed = inner_middle_wheel_speed = abs(rc_data.joystick1_y);
    
  } else {
    
    speed = abs(rc_data.joystick1_y);
    speed = map(speed, 0, 255, 0, 100); //we convert the speed to a 0-100 range to make the calculations

    if (speed >= 30) {
      correctionFactor1 = map(speed, 30, 100, 0, 40);
      correctionFactor2 = map(speed, 30, 100, 0, 30);
    }

    outer_wheels_speed = speed;

    //TODO: Test with this code and apply correction factor to inner speeds if necessary (ex.- doing inner_.. -= correctionFactor)

    inner_front_back_wheels_speed = (float) (speed * (sqrt(pow(d1_distance_front_middle_wheels, 2) + pow((turning_radius - d3_front_back_rover_width), 2)))) / (turning_radius + d4_center_rover_width);

    if (correctionFactor1 < inner_front_back_wheels_speed) { //We make sure that when we apply the correctionFactor it does not result in a negative speed, which could cause unexpected motor behaviour
      inner_front_back_wheels_speed -= correctionFactor1;
    }

    inner_middle_wheel_speed = speed * ((float) (turning_radius - d4_center_rover_width) / (turning_radius + d4_center_rover_width));

    if (correctionFactor2 < inner_middle_wheel_speed) { //We make sure that when we apply the correctionFactor it does not result in a negative speed, which could cause unexpected motor behaviour
      inner_middle_wheel_speed -= correctionFactor2;
    }

    /*Serial.print("    outer_wheels_speed: ");
    Serial.print(outer_wheels_speed);

    Serial.print("    inner_front_back_wheels_speed: ");
    Serial.print(inner_front_back_wheels_speed);

    Serial.print("    inner_middle_wheel_speed: ");
    Serial.println(inner_middle_wheel_speed);*/


    //TODO: Adjust "toMin" if necessary so that when doing a close turn the inner middle wheel spins
    outer_wheels_speed = map(outer_wheels_speed, 0, 100, 0, 255);
    inner_front_back_wheels_speed = map(inner_front_back_wheels_speed, 0, 100, 0, 255);
    inner_middle_wheel_speed = map(inner_middle_wheel_speed, 0, 100, 0, 255);

  }


}

void calculateWheelServosAngle() {

  //Ackerman steering geometry calculations

  if (joystickX_isCentered(JOY_RIGHT)) {
    turning_radius = MAX_INTEGER; //We put the turning radius to the maximum integer to indicate that we are not turning
  } else if (joystickIsLeft(JOY_RIGHT)) {
      turning_radius = map(rc_data.joystick2_x, 0, -255, max_turning_radius, min_turning_radius); //4000mm to 700mm turning radius
  } else {
      turning_radius = map(rc_data.joystick2_x, 0, 255, max_turning_radius, min_turning_radius); 
  }

  outer_servo_front_angle_beta = round((atan(((float)d1_distance_front_middle_wheels / (turning_radius + d3_front_back_rover_width))) ) * (180 / PI)); //We multiply by 180/PI to convert radians to degrees
  outer_servo_back_angle_beta = round((atan(((float)d2_distance_back_middle_wheels / (turning_radius + d3_front_back_rover_width))) ) * (180 / PI));

  inner_servo_front_angle_beta = round((atan(((float)d1_distance_front_middle_wheels / (turning_radius - d3_front_back_rover_width))) ) * (180 / PI)); 
  inner_servo_back_angle_beta = round((atan(((float)d2_distance_back_middle_wheels / (turning_radius - d3_front_back_rover_width))) ) * (180 / PI)); 

  /*Serial.print("Inner servo front: ");
  Serial.print(inner_servo_front_angle_beta);

  Serial.print("     Outer servo front: ");
  Serial.print(outer_servo_front_angle_beta);

  Serial.print("     Inner servo back: ");
  Serial.print(inner_servo_back_angle_beta);

  Serial.print("     Outer servo back: ");
  Serial.println(outer_servo_back_angle_beta);*/
}

void setMotors() {
  //Depending on the channels values (steering LEFT/RIGHT and throttle FWD/BCK) we will set each motor to its according 
  //(outer_wheels_speed, inner_front_back_wheels_speed, inner_middle_wheel_speed) and rotation direction using the "setMotorSpeed" function
  //(outer_wheels_speed, inner_front_back_wheels_speed, inner_middle_wheel_speed) will have the speed (0-100) calculated at "calculateMotorsSpeed"
  //We will map each of these speeds to a range of (0-255) and put them into the corresponding motorX_pwm_speed in this function. 

  if (rcLinkStatus == RADIO_KO) {
    stopMotors();
    return;
  }

  if (joystickY_isCentered(JOY_LEFT)) {

    stopMotors();

  } else {

      if (joystickIsUp(JOY_LEFT)) {

        if (joystickIsLeft(JOY_RIGHT)) {

          setMotorSpeed(MOTOR_1, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_2, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_3, outer_wheels_speed, FWD);

          setMotorSpeed(MOTOR_4, inner_front_back_wheels_speed, FWD);
          setMotorSpeed(MOTOR_5, inner_middle_wheel_speed, FWD);
          setMotorSpeed(MOTOR_6, inner_front_back_wheels_speed, FWD);

        } else {

          setMotorSpeed(MOTOR_1, inner_front_back_wheels_speed, FWD);
          setMotorSpeed(MOTOR_2, inner_middle_wheel_speed, FWD);
          setMotorSpeed(MOTOR_3, inner_front_back_wheels_speed, FWD);

          setMotorSpeed(MOTOR_4, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_5, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_6, outer_wheels_speed, FWD);

        }

      } else {

        if (joystickIsLeft(JOY_RIGHT)) {

          setMotorSpeed(MOTOR_1, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_2, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_3, outer_wheels_speed, BCK);

          setMotorSpeed(MOTOR_4, inner_front_back_wheels_speed, BCK);
          setMotorSpeed(MOTOR_5, inner_middle_wheel_speed, BCK);
          setMotorSpeed(MOTOR_6, inner_front_back_wheels_speed, BCK);

        } else {

          setMotorSpeed(MOTOR_1, inner_front_back_wheels_speed, BCK);
          setMotorSpeed(MOTOR_2, inner_middle_wheel_speed, BCK);
          setMotorSpeed(MOTOR_3, inner_front_back_wheels_speed, BCK);

          setMotorSpeed(MOTOR_4, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_5, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_6, outer_wheels_speed, BCK);

        }

      }

  }

}


void setWheelServos() {
  //Depending on the channels values (steering LEFT/RIGHT) we will add or subtract each servos'
  //(outer_servos_angle_beta, inner_servos_angle_beta) to its w_servo_center constant, so that it goes to the correct position
  //(outer_servos_angle_beta, inner_servos_angle_beta) will have the angle calculated at "calculateMotorsSpeed"
  //We will add or subtract each "angle beta" to the w_servo_center constant and put the result into the corresponding 
  //servoX_angle. Then we will write each servoX_angle to the corresponding servo using the Servo.write function

  if (joystickX_isCentered(JOY_RIGHT)) {

    setWheelServosStraight();

  } else {

    if (joystickIsLeft(JOY_RIGHT)) {
      servoWrite(W_SERVO_1, w_servo_center[1] - outer_servo_front_angle_beta);
      servoWrite(W_SERVO_2, w_servo_center[2] + outer_servo_back_angle_beta);
      servoWrite(W_SERVO_3, w_servo_center[3] - inner_servo_front_angle_beta);
      servoWrite(W_SERVO_4, w_servo_center[4] + inner_servo_back_angle_beta);

    } else {
      servoWrite(W_SERVO_1, w_servo_center[1] + inner_servo_front_angle_beta);
      servoWrite(W_SERVO_2, w_servo_center[2] - inner_servo_back_angle_beta);
      servoWrite(W_SERVO_3, w_servo_center[3] + outer_servo_front_angle_beta);
      servoWrite(W_SERVO_4, w_servo_center[4] - outer_servo_back_angle_beta);
    }

  }

}

void setArmServos() {

}


//TODO: change name to rcOperationModeExecution
void operationModeExecution() {

  switch (op_mode) {

    case OP_CONVENTIONAL_DRIVING:
      calculateMotorsSpeed();
      calculateWheelServosAngle();
      setMotors();
      setWheelServos();
      break;
    case OP_360_DEGREE_TURN_CONTROL:
      turnControl360Degree();
      break;
    case OP_ROBOTIC_ARM_CONTROL:
      //roboticArmControl();
      break;
    case OP_HEAD_CONTROL:
      //headControl();
      break;
    case OP_WEB_CONTROL:
      //webOperationModeExecution(); //This function will be in the webControl.c
      break;
    case SUB_OP_360_DEGREE_TO_CONVENTIONAL:
      returnFrom360ToConventional();
      break;

    default:
      break;

  }

}


void loop() {

  readRcValues();
  printRcValues();
  setOperationMode();
  operationModeExecution();

}

