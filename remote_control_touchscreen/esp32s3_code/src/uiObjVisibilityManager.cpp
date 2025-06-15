//Given a change in a button/switch, handles the visibility (enabled/disabled, hidden/shown) of other UI objects

#include <lvgl.h>
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates

void headControlSwUpdateRelatedObjectsVisibility(bool swChecked) {
    if (swChecked) {
        lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
    } else {
        lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
        if (lv_obj_has_state(ui_PowerRPIswitch, LV_STATE_CHECKED) && !lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
            lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
        }
    }
}

void armControlSwUpdateRelatedObjectsVisibility(bool swChecked) {
    if (swChecked) {
        lv_obj_clear_flag(ui_ControlClawServosLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_ArmServoSelectionSwitch, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    } else {
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
}

void selfRotationControlSwUpdateRelatedObjectsVisibility(bool swChecked) {
    if (swChecked) {
        lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    } else {
        if (!lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
            lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
        }
        lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
        if (lv_obj_has_state(ui_PowerRPIswitch, LV_STATE_CHECKED) && !lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {
            lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
        }
    }
}

void rpiGiveControlSwUpdateRelatedObjectsVisibility(bool swChecked) {
    if (swChecked) {
        lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_PowerRPIswitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_OpenHatchSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_EnableMotorsSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_EnableRadioRcSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_PlusDegreeButton, LV_STATE_DISABLED);
        lv_obj_add_state(ui_MinusDegreeButton, LV_STATE_DISABLED);
    } else {
        lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_ArmControlSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_SelfRotationControlSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_PowerRPIswitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_OpenHatchSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_HeadRandMovSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_EnableMotorsSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_EnableRadioRcSwitch, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_PlusDegreeButton, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_MinusDegreeButton, LV_STATE_DISABLED);
    }
}

void powerRpiSwUpdateRelatedObjectsVisibility(bool swChecked) {
    if (swChecked) {
        if (!lv_obj_has_state(ui_HeadControlSwitch, LV_STATE_CHECKED) 
        && !lv_obj_has_state(ui_ArmControlSwitch, LV_STATE_CHECKED) 
        && !lv_obj_has_state(ui_SelfRotationControlSwitch, LV_STATE_CHECKED) 
        && !lv_obj_has_state(ui_HeadRandMovSwitch, LV_STATE_CHECKED)) {

            lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
        }
    } else {
        lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    }
}

void headRandMovSwUpdateRelatedObjectsVisibility(bool swChecked) {
    if (swChecked) {
        lv_obj_add_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
    } else {
        if (!lv_obj_has_state(ui_ArmControlSwitch, LV_STATE_CHECKED) && !lv_obj_has_state(ui_SelfRotationControlSwitch, LV_STATE_CHECKED)) {
            lv_obj_clear_state(ui_HeadControlSwitch, LV_STATE_DISABLED);
            if(lv_obj_has_state(ui_PowerRPIswitch, LV_STATE_CHECKED)) {
                lv_obj_clear_state(ui_RPIgiveControlSwitch, LV_STATE_DISABLED);
            }
        }
    }
}