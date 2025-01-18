#include <lvgl.h>

#include "receivedFramesHandling.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "valuesFromFrameConversion.h"
#include "frameTypesDefinition.h"


char* intToChars(int value, char* string) { //TODO: Maybe this function will go to "sharedFunctions.h"
    sprintf(string, "%d", value);
    return string;
}

void updateRcValueLabels(Frame frame) {

    RcValues rcValues = rcValuesFromFrame(frame);

    char buffer[50];

    lv_label_set_text(ui_JoyX1valueLabel, intToChars(rcValues.x1, buffer));
    lv_label_set_text(ui_JoyY1valueLabel, intToChars(rcValues.y1, buffer));
    lv_label_set_text(ui_JoyX2valueLabel, intToChars(rcValues.x2, buffer));
    lv_label_set_text(ui_JoyY2valueLabel, intToChars(rcValues.y2, buffer));
    lv_label_set_text(ui_AUX1valueLabel, intToChars(rcValues.aux1, buffer));
    lv_label_set_text(ui_AUX2valueLabel, intToChars(rcValues.aux2, buffer));
    lv_label_set_text(ui_AUX3valueLabel, intToChars(rcValues.aux3, buffer));
    lv_label_set_text(ui_AUX4valueLabel, intToChars(rcValues.aux4, buffer));

}

void handleReceivedFrame(Frame frame) {

    switch (frame.type) {
        case INF_F_RC_VALUES:
            updateRcValueLabels(frame);
            break;
        case CMD_F_TEST:
            //TODO: Delete. Just for test.
            char buffer[100];
            sprintf(buffer, "Frame type %d received. Data2B[1]: %d, Data1B[3]: %d", frame.type, frame.data2B[1], frame.data1B[3]);
            lv_label_set_text(ui_NotificationsLabel, buffer);

            break;

        default:
            break;
    }

}