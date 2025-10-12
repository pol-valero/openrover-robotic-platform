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

void updateRaspberryPiStatusValueElements(Frame frame) {

    RaspberryPiStatusValues rpiStatusValues = rpiStatusValuesFromFrame(frame);

    if (rpiStatusValues.online) {
        lv_label_set_text(ui_OnlineStatusLabel, "Online");
        lv_obj_clear_state(ui_OnlineStatusIndicatorPanel, LV_STATE_DISABLED);
    } else {
        lv_label_set_text(ui_OnlineStatusLabel, "Offline");
        lv_obj_add_state(ui_OnlineStatusIndicatorPanel, LV_STATE_DISABLED);
    }

    if (rpiStatusValues.cameraOn) {
        lv_label_set_text(ui_CameraStatusLabel, "Camera On");
        lv_obj_clear_state(ui_CameraStatusIndicatorPanel, LV_STATE_DISABLED);
    } else {
        lv_label_set_text(ui_CameraStatusLabel, "Camera Off");
        lv_obj_add_state(ui_CameraStatusIndicatorPanel, LV_STATE_DISABLED);
    }

    lv_label_set_text(ui_CpuTempValueLabel, intValueToString(rpiStatusValues.cpuTemperature, "°C"));
    lv_bar_set_value(ui_CpuTempBar, rpiStatusValues.cpuTemperature, LV_ANIM_OFF);

    lv_label_set_text(ui_CpuWorkloadValueLabel, intValueToString(rpiStatusValues.cpuWorkload, "%"));
    lv_bar_set_value(ui_CpuWorkloadBar, rpiStatusValues.cpuWorkload, LV_ANIM_OFF);

}

void checkRcLinkStatus(Frame frame) {

    static long msTimeSinceLastPacket = 0;
  
    if(frame.type == INF_F_ENVIRONMENTAL_MONITORIZATION) {

        msTimeSinceLastPacket = 0;

        //rcLinkStatusOK
        if (lv_obj_has_flag(ui_SignalBar, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(ui_SignalBar, LV_OBJ_FLAG_HIDDEN);
        }
    
    } else {
  
        static unsigned long previousMillis1 = 0;
  
        if (millis() - previousMillis1 >= 1000) {
            previousMillis1 = millis();

            msTimeSinceLastPacket += 1000;
        }
  
        //If we spend more than 5 seconds without receiving a packet from the Rover, the rcLink status will be considered as lost
        if (msTimeSinceLastPacket >= 5000) {
  
            //rcLinkStatusKO

            static unsigned long previousMillis2 = 0;

            if (millis() - previousMillis2 >= 1000) {
                previousMillis2 = millis();
                
                //We flash the signal icon on the top bar, to indicate that the rc connection is lost
                if (lv_obj_has_flag(ui_SignalBar, LV_OBJ_FLAG_HIDDEN)) {
                    lv_obj_clear_flag(ui_SignalBar, LV_OBJ_FLAG_HIDDEN);
                } else {
                    lv_obj_add_flag(ui_SignalBar, LV_OBJ_FLAG_HIDDEN);
                }

            }
  
        }
  
    }
  
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
        case INF_F_RASPBERRYPI_STATUS:
            updateRaspberryPiStatusValueElements(frame);
            break;

        default:
            break;
    }

    checkRcLinkStatus(frame);

}