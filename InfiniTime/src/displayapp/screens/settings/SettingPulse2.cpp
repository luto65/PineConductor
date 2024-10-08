#include "displayapp/screens/settings/SettingPulse2.h"
#include <lvgl/lvgl.h>
#include <nrf_log.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"
#include "components/settings/Settings.h"
#include "displayapp/InfiniTimeTheme.h"

using namespace Pinetime::Applications::Screens;

namespace {
  constexpr int16_t POS_X = -72;
  constexpr int16_t POS_Y = -6;

  void event_handler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<SettingPulse2*>(obj->user_data);
    if (event == LV_EVENT_CLICKED) {
      screen->HandleButtonPress();
    }
  }

  void ValueChangedHandler(void* userData) {
    auto* screen = static_cast<SettingPulse2*>(userData);
    screen->CheckBpm();
  }
}

SettingPulse2::SettingPulse2(Pinetime::Controllers::Settings& settingsController,
  Pinetime::Applications::Screens::SettingPulser& settingPulser)
  : settingsController {settingsController},settingPulser {settingPulser} {

  lv_obj_t* title = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(title, "Set BPM 2");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 15, 15);

  lv_obj_t* icon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);

  lv_label_set_text_static(icon, Symbols::tachometer);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);
  const uint8_t bpmValue = settingsController.GetPulse2();
  NRF_LOG_INFO("Setting Pulser 2 (manually) to %02d from settings", bpmValue);

  bpmCounter.SetValueChangedEventCallback(this, ValueChangedHandler);
  bpmCounter.Create();
  bpmCounter.SetValue(bpmValue);
  lv_obj_align(bpmCounter.GetObject(), nullptr, LV_ALIGN_CENTER, POS_X, POS_Y);

  btnPulse2 = lv_btn_create(lv_scr_act(), nullptr);
  btnPulse2 ->user_data = this;
  lv_obj_set_size(btnPulse2, 120, 48);
  lv_obj_align(btnPulse2, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_local_bg_color(btnPulse2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
  lblPulse2 = lv_label_create(btnPulse2, nullptr);
  lv_label_set_text_static(lblPulse2, "Set");
  lv_obj_set_event_cb(btnPulse2, event_handler);
}

SettingPulse2::~SettingPulse2() {
  lv_obj_clean(lv_scr_act());
}

void SettingPulse2::HandleButtonPress() {
  const uint8_t bpmValue = bpmCounter.GetValue();
  NRF_LOG_INFO("Setting Pulser 2 (manually) to %02d", bpmValue);
  settingsController.SetPulse2(bpmValue);
  settingPulser.Advance();
}

void SettingPulse2::CheckBpm() {
//  pulse2Counter.SetValue(bpmCounter.GetValue());
}
