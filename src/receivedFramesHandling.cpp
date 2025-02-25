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

void updateEnvMonitorValuesLabelsArcsChart(Frame frame) {

    char buffer[10];

    EnvironmentalValues envValues = environmentalValuesFromFrame(frame);

    //Update the data labels
    lv_label_set_text(ui_TempValueLabel, intValueToString(envValues.temperature, "°C"));
    lv_label_set_text(ui_HumValueLabel, intValueToString(envValues.humidity, "%"));
    lv_label_set_text(ui_PressureValueLabel, intValueToString(envValues.pressure, "hPa"));
    sprintf(buffer, "(%dm)", envValues.altitude);
    lv_label_set_text(ui_HeightValueLabel, buffer);

    //Update the arcs
    lv_arc_set_value(ui_TempArc, envValues.temperature);
    lv_arc_set_value(ui_HumArc, envValues.humidity);
    lv_arc_set_value(ui_PressureArc, envValues.pressure);

    //Update the chart with the new values
    lv_chart_set_next_value(ui_EnvDataChart, ui_EnvDataChart_series_1, envValues.temperature);
    lv_chart_set_next_value(ui_EnvDataChart, ui_EnvDataChart_series_2, envValues.humidity);
    lv_chart_refresh(ui_EnvDataChart);
    
}

void updateSpeedometerValueLabels(Frame frame) {

    SpeedometerValues speedometerValues = speedometerValuesFromFrame(frame);

    lv_label_set_text(ui_RevsValueLabel, intValueToString(speedometerValues.rpm, "rpm"));
    lv_label_set_text(ui_SpeedValueLabel, intValueToString(speedometerValues.metersPerHour, "m/h"));
    lv_label_set_text(ui_DistanceValueLabel, intValueToString(speedometerValues.distance, "m"));

}

void handleReceivedFrame(Frame frame) {

    switch (frame.type) {
        case INF_F_RC_VALUES:
            updateRcValueLabels(frame);
            doAuxChannelsActions(frame);
            break;
        case INF_F_RC_BAT_LEVEL:
            updateRcBattValuesLabelBar(frame);
            break;
        case INF_F_ROVER_BAT_LEVEL:
            updateRoverBattValuesLabelBar(frame);
            break;
        case INF_F_ENVIRONMENTAL_MONITORIZATION:
            updateEnvMonitorValuesLabelsArcsChart(frame);
            break;
        case INF_F_SPEEDOMETER_VALUES:
            updateSpeedometerValueLabels(frame);
            break;

        default:
            break;
    }

}