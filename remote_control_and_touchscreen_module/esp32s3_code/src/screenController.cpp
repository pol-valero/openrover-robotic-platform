#include <Arduino.h>
#include <lvgl.h>

#include "screenController.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "serialCommunication.h"
#include "uiObjVisibilityManager.h"
#include "frameTypesDefinition.h"
#include "sharedFunctions.h"
#include "valuesToFrameConversion.h"


int testValue = 0;


void headControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch checked");

    Frame frame = roverOpModeSelectionToFrame(OP_HEAD_CONTROL);
    serialSendFrame(frame);

    headControlSwUpdateRelatedObjectsVisibility(true);
}

void headControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch unchecked");

    Frame frame = roverOpModeSelectionToFrame(OP_CONVENTIONAL_DRIVING);
    serialSendFrame(frame);

    headControlSwUpdateRelatedObjectsVisibility(false);
}

void armControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch checked");

    Frame frame = roverOpModeSelectionToFrame(OP_ROBOTIC_ARM_CONTROL);
    serialSendFrame(frame);

    armControlSwUpdateRelatedObjectsVisibility(true);
}

void armControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch unchecked");

    Frame frame = roverOpModeSelectionToFrame(OP_CONVENTIONAL_DRIVING);
    serialSendFrame(frame);
    
    armControlSwUpdateRelatedObjectsVisibility(false);
}

void armServoSelectionSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm servo selection switch checked");

    Frame frame = armServoSelectionToFrame(true);
    serialSendFrame(frame);

}

void armServoSelectionSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm servo selection switch unchecked");

    Frame frame = armServoSelectionToFrame(false);
    serialSendFrame(frame);

}

void selfRotationControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degree rotation control switch checked");

    Frame frame = roverOpModeSelectionToFrame(OP_360_DEGREE_TURN_CONTROL);
    serialSendFrame(frame);

    selfRotationControlSwUpdateRelatedObjectsVisibility(true);
}

void selfRotationControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degree rotation control switch unchecked");

    Frame frame = roverOpModeSelectionToFrame(OP_CONVENTIONAL_DRIVING);
    serialSendFrame(frame);

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

    Frame frame = raspberryPiPowerStatusToFrame(true);
    serialSendFrame(frame);

    powerRpiSwUpdateRelatedObjectsVisibility(true);
}

void powerRpiSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Power Raspberry Pi switch unchecked");

    Frame frame = raspberryPiPowerStatusToFrame(false);
    serialSendFrame(frame);

    powerRpiSwUpdateRelatedObjectsVisibility(false);
}

void openHatchSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Open hatch switch checked");

}

void openHatchSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Open hatch switch unchecked");

    Frame frame;    //TODO: Delete. Just for test.
    frame.type = CMD_F_RC_BUZZER;
    serialSendFrame(frame);
}

void headRandMovSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head random movement switch checked");

    headRandMovSwUpdateRelatedObjectsVisibility(true);
}

void headRandMovSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head random movement switch unchecked");

    headRandMovSwUpdateRelatedObjectsVisibility(false);
}

void enableRadioRcSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Enable radio RC switch checked");

    Frame frame = rcRadioEnableStatusToFrame(true);
    serialSendFrame(frame);

}

void enableRadioRcSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Enable radio RC switch unchecked");

    Frame frame = rcRadioEnableStatusToFrame(false);
    serialSendFrame(frame);

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
    lv_roller_get_selected_str(ui_WheelSelectorRoller, sharedBuffer, 0);
    lv_label_set_text(ui_NotificationsLabel, sharedBuffer);   //TODO:Just for testing, delete later


}

void plusDegreeBtnClicked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Plus degree button clicked");

    //TODO:Just for testing, delete later
    lv_label_set_text(ui_TrimValueLabel, intValueToString(++testValue, "degrees"));

}

void minusDegreeBtnClicked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Minus degree button clicked");

    //TODO:Just for testing, delete later
    lv_label_set_text(ui_TrimValueLabel, intValueToString(--testValue, "degrees"));

}
