#include <Arduino.h>
#include <lvgl.h>

#include "screenController.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "serialCommunication.h"
#include "uiObjVisibilityManager.h"
#include "receivedFramesHandling.h"
#include "frameTypesDefinition.h"

char testString[100];
int testValue = 0;


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

    //TODO: Delete. Just for test. 
    Frame frame;
    frame.type = CMD_F_TEST;
    frame.data2B[1] = 26;
    frame.data1B[3] = 06;
    serialSendFrame(frame);

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


void screenControllerExecution() {

    Frame frame = serialReceiveFrame();
    handleReceivedFrame(frame);

}