#include <lvgl.h>

#include "receivedFramesHandling.h"
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "valuesFromFrameConversion.h"
#include "frameTypesDefinition.h"
#include "sharedFunctions.h"



void updateRcValueLabels(Frame frame) {

    RcValues rcValues = rcValuesFromFrame(frame);

    lv_label_set_text(ui_JoyX1valueLabel, intValueToString(rcValues.x1, ""));
    lv_label_set_text(ui_JoyY1valueLabel, intValueToString(rcValues.y1, ""));
    lv_label_set_text(ui_JoyX2valueLabel, intValueToString(rcValues.x2, ""));
    lv_label_set_text(ui_JoyY2valueLabel, intValueToString(rcValues.y2, ""));
    lv_label_set_text(ui_AUX1valueLabel, intValueToString(rcValues.aux1, ""));
    lv_label_set_text(ui_AUX2valueLabel, intValueToString(rcValues.aux2, ""));
    lv_label_set_text(ui_AUX3valueLabel, intValueToString(rcValues.aux3, ""));
    lv_label_set_text(ui_AUX4valueLabel, intValueToString(rcValues.aux4, ""));

}

void updateRcBattValuesLabelBar(Frame frame) {

    BatteryValues rcBattValues = battValuesFromFrame(frame);

    lv_label_set_text(ui_RcBatteryLabel, floatValueToString(rcBattValues.cellVoltage, "V"));
    lv_bar_set_value(ui_RcBatteryBar, rcBattValues.percentage, LV_ANIM_OFF);

}

void updateRoverBattValuesLabelBar(Frame frame) {

    BatteryValues roverBattValues = battValuesFromFrame(frame);

    lv_label_set_text(ui_RoverBatteryLabel, floatValueToString(roverBattValues.cellVoltage, "V"));
    lv_bar_set_value(ui_RoverBatteryBar, roverBattValues.percentage, LV_ANIM_OFF);

}

void doAuxChannelsActions(Frame frame) {
    //Depending on the value of the AUX channels, we trigger some actions (ex.- changing the status of a switch on the screen)

    bool aux1ValueChanged = false;
    static int lastAux1Value = 0;

    RcValues rcValues = rcValuesFromFrame(frame);   
    
    if (rcValues.aux1 != lastAux1Value) {
        aux1ValueChanged = true;
        lastAux1Value = rcValues.aux1;
    }

    if (lv_obj_has_state(ui_ArmControlSwitch, LV_STATE_CHECKED) && aux1ValueChanged) {
        if (rcValues.aux1 == 1 && !lv_obj_has_state(ui_ArmServoSelectionSwitch, LV_STATE_CHECKED)) {
            lv_obj_add_state(ui_ArmServoSelectionSwitch, LV_STATE_CHECKED);
            lv_event_send(ui_ArmServoSelectionSwitch, LV_EVENT_VALUE_CHANGED, NULL);
        } else if (rcValues.aux1 == 0 && lv_obj_has_state(ui_ArmServoSelectionSwitch, LV_STATE_CHECKED)) {
            lv_obj_clear_state(ui_ArmServoSelectionSwitch, LV_STATE_CHECKED);
            lv_event_send(ui_ArmServoSelectionSwitch, LV_EVENT_VALUE_CHANGED, NULL);
        }
    }
    
}

void handleReceivedFrame(Frame frame) {

    switch (frame.type) {
        case INF_F_RC_VALUES:
            updateRcValueLabels(frame);
            doAuxChannelsActions(frame);
            break;
        case CMD_F_TEST:
            //TODO: Delete. Just for test.
            char buffer[100];
            sprintf(buffer, "Frame type %d received. Data2B[1]: %d, Data1B[3]: %d", frame.type, frame.data2B[1], frame.data1B[3]);
            lv_label_set_text(ui_NotificationsLabel, buffer);
            //
            break;
        case INF_F_RC_BAT_LEVEL:
            updateRcBattValuesLabelBar(frame);
            break;
        case INF_F_ROVER_BAT_LEVEL:
            updateRoverBattValuesLabelBar(frame);
            break;

        default:
            break;
    }

}