#include <lvgl.h>

#include "screenController.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "serialCommunication.h"

char response[100];


void headControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch checked");

    lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
}

void headControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch unchecked");

    lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
}

void armControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch checked");

    lv_obj_clear_flag(ui_ControlClawServosLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ArmServoSelectionSwitch, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
}

void armControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch unchecked");
    
    lv_obj_add_flag(ui_ControlClawServosLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ArmServoSelectionSwitch, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
}

void selfRotationControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degree rotation control switch checked");

    lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
}

void selfRotationControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degre rotation control switch unchecked");

    lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
}


void screenControllerExecution() {

    serialReceiveResponse(response);
    //usbSerialPrint(response);
    if (strlen(response) > 0) {
        //lv_label_set_text(ui_Label1, response);
    }

    serialSendTestValues();
}