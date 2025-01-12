#include <lvgl.h>

#include "screenController.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "serialCommunication.h"

char response[100];


void headControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch checked");

    lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
}

void headControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head control switch unchecked");

    lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
    if (lv_obj_has_state(ui_PowerRPIswitch, LV_STATE_CHECKED) && !lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
        lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    }


}

void armControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch checked");

    lv_obj_clear_flag(ui_ControlClawServosLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ArmServoSelectionSwitch, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
}

void armControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Arm control switch unchecked");
    
    lv_obj_add_flag(ui_ControlClawServosLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ArmServoSelectionSwitch, LV_OBJ_FLAG_HIDDEN);

    if (!lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
        lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    }
    lv_obj_clear_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
    if (lv_obj_has_state(ui_PowerRPIswitch, LV_STATE_CHECKED) && !lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
        lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    }


}

void selfRotationControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degree rotation control switch checked");

    lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
}

void selfRotationControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "360 degree rotation control switch unchecked");

    if (!lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
        lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    }
    lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    if (lv_obj_has_state(ui_PowerRPIswitch, LV_STATE_CHECKED) && !lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
        lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    }
}

void rpiGiveControlSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Raspberry Pi control switch checked");

    lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_PowerRPIswitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_OpenHatchSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_EnableMotorsSwitch, LV_STATE_DISABLED);
}

void rpiGiveControlSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Raspberry Pi control switch unchecked");

    lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_PowerRPIswitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_OpenHatchSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_EnableMotorsSwitch, LV_STATE_DISABLED);
}

void powerRpiSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Power Raspberry Pi switch checked");

    if (!lv_obj_has_state(ui_HeadControlSwitch, LV_STATE_CHECKED) 
        && !lv_obj_has_state(ui_ArmControlSwitch, LV_STATE_CHECKED) 
        && !lv_obj_has_state(ui_SelfRotationControlSwitch, LV_STATE_CHECKED) 
        && !lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {

        lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    }
    
}

void powerRpiSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Power Raspberry Pi switch unchecked");

    lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
}

void openHatchSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Open hatch switch checked");

}

void openHatchSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Open hatch switch unchecked");

}

void headRandMovSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head random movement switch checked");

    lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
    lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
}

void headRandMovSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Head random movement switch unchecked");

    if (!lv_obj_has_state(ui_ArmControlSwitch, LV_STATE_CHECKED) && !lv_obj_has_state(ui_SelfRotationControlSwitch, LV_STATE_CHECKED)) {
        lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    }
    
}

void enableMotorsSwChecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Enable motors switch checked");

}

void enableMotorsSwUnchecked(lv_event_t *e) {
    lv_label_set_text(ui_NotificationsLabel, "Enable motors switch unchecked");

}


void screenControllerExecution() {

    serialReceiveResponse(response);
    //usbSerialPrint(response);
    if (strlen(response) > 0) {
        //lv_label_set_text(ui_Label1, response);
    }

    serialSendTestValues();
}