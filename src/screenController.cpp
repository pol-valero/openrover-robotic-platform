#include <lvgl.h>

#include "screenController.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "serialCommunication.h"

char response[100];

void clickedButton(lv_event_t *e) {
    lv_label_set_text(ui_Label1, "Hey, this is a test!");
}

void screenControllerExecution() {

    serialReceiveResponse(response);
    //usbSerialPrint(response);
    if (strlen(response) > 0) {
        lv_label_set_text(ui_Label1, response);
    }

    serialSendTestValues();
}