#include <lvgl.h>

#include "screenController.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "serialCommunication.h"
#include "uiObjVisibilityManager.h"

char response[100];


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


void screenControllerExecution() {

    serialReceiveResponse(response);
    //usbSerialPrint(response);
    if (strlen(response) > 0) {
        //lv_label_set_text(ui_Label1, response);
    }

    serialSendTestValues();
}