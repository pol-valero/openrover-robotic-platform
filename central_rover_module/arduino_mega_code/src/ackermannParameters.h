#ifndef ACKERMANN_STEERING_PARAMETERS_H
#define ACKERMANN_STEERING_PARAMETERS_H

const int d1_distance_front_middle_wheels = 300; //Distance between the front and middle wheel, in mm
const int d2_distance_back_middle_wheels = 275; //Distance between the back and middle wheel, in mm

//Distances between the longitudinal center of the wheel and the longitudinal center of the rover. 
const int d3_front_back_rover_width = 270; //The front and back wheels have the same distance to the longitudinal center of the rover
const int d4_center_rover_width = 300;

const int max_turning_radius = 4000;  //In mm
const int min_turning_radius = 700; //TODO: Change to 600 when I have done more tests with the rover

//TODO: Create extern for shared variables

#endif