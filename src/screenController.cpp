#include <Arduino.h>
#include <lvgl.h>

#include "screenController.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "serialCommunication.h"
#include "uiObjVisibilityManager.h"

char testString[100];
int testValue = 0;

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

RcValues rcValuesFromFrame(Frame frame) {

    RcValues rcValues;
    
    rcValues.y1 = frame.data2B[0];
    rcValues.x1 = frame.data2B[1];
    rcValues.y2 = frame.data2B[2];
    rcValues.x2 = frame.data2B[3];
    rcValues.aux1 = frame.data1B[0];
    rcValues.aux2 = frame.data1B[1];
    rcValues.aux3 = frame.data1B[2];
    rcValues.aux4 = frame.data1B[3];
    
    return rcValues;

}

void headControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch checked");

    headControlSwUpdateRelatedObjectsVisibility(true);
}

void headControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch unchecked");

    headControlSwUpdateRelatedObjectsVisibility(false);
}

void armControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch checked");

    armControlSwUpdateRelatedObjectsVisibility(true);
}

void armControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch unchecked");
    
    armControlSwUpdateRelatedObjectsVisibility(false);
}

void selfRotationControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degree rotation control switch checked");

    selfRotationControlSwUpdateRelatedObjectsVisibility(true);
}

void selfRotationControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degree rotation control switch unchecked");

    selfRotationControlSwUpdateRelatedObjectsVisibility(false);
}

void rpiGiveControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Raspberry Pi control switch checked");

    rpiGiveControlSwUpdateRelatedObjectsVisibility(true);
}

void rpiGiveControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Raspberry Pi control switch unchecked");

    rpiGiveControlSwUpdateRelatedObjectsVisibility(false);
}

void powerRpiSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Power Raspberry Pi switch checked");

    powerRpiSwUpdateRelatedObjectsVisibility(true);
}

void powerRpiSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Power Raspberry Pi switch unchecked");
    powerRpiSwUpdateRelatedObjectsVisibility(false);
}

void openHatchSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Open hatch switch checked");

}

void openHatchSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Open hatch switch unchecked");

}

void headRandMovSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head random movement switch checked");

    headRandMovSwUpdateRelatedObjectsVisibility(true);
}

void headRandMovSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head random movement switch unchecked");

    headRandMovSwUpdateRelatedObjectsVisibility(false);
}

void enableMotorsSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Enable motors switch checked");

}

void enableMotorsSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Enable motors switch unchecked");

}


void wheelSelectorReleased(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Wheel selector released");

    //Get string of the wheel selector
    lv_roller_get_selected_str(ui_WheelSelectorRoller, testString, 0);
    lv_label_set_text(ui_NotificationsLabel, testString);


}

void plusDegreeBtnClicked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Plus degree button clicked");

    sprintf(testString, "+%d degrees", ++testValue);    //Just for testing, delete later
    lv_label_set_text(ui_TrimValueLabel, testString);

}

void minusDegreeBtnClicked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Minus degree button clicked");

    sprintf(testString, "-%d degrees", --testValue);
    lv_label_set_text(ui_TrimValueLabel, testString);

}

char* intToChars(int value) {
    sprintf(testString, "%d", value);
    return testString;
}

void handleReceivedFrame(Frame frame) {

    RcValues rcValues = rcValuesFromFrame(frame);

    lv_label_set_text(ui_JoyX1valueLabel, intToChars(rcValues.x1));
    lv_label_set_text(ui_JoyY1valueLabel, intToChars(rcValues.y1));
    lv_label_set_text(ui_JoyX2valueLabel, intToChars(rcValues.x2));
    lv_label_set_text(ui_JoyY2valueLabel, intToChars(rcValues.y2));
    lv_label_set_text(ui_AUX1valueLabel, intToChars(rcValues.aux1));
    lv_label_set_text(ui_AUX2valueLabel, intToChars(rcValues.aux2));
    lv_label_set_text(ui_AUX3valueLabel, intToChars(rcValues.aux3));
    lv_label_set_text(ui_AUX4valueLabel, intToChars(rcValues.aux4));

}

void screenControllerExecution() {

    Frame frame = serialReceiveFrame();
    handleReceivedFrame(frame);
    //usbSerialPrint(response);
    //serialSendTestValues();   //TODO: Delete
}