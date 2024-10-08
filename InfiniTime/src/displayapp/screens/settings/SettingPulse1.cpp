#include "displayapp/screens/settings/SettingPulse1.h"
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
    auto* screen = static_cast<SettingPulse1*>(obj->user_data);
    if (event == LV_EVENT_CLICKED) {
      screen->HandleButtonPress();
    }
  }

  void ValueChangedHandler(void* userData) {
    auto* screen = static_cast<SettingPulse1*>(userData);
    screen->CheckBpm();
  }
}

SettingPulse1::SettingPulse1(Pinetime::Controllers::Settings& settingsController,
  Pinetime::Applications::Screens::SettingPulser& settingPulser)
  : settingsController {settingsController},settingPulser {settingPulser} {

  lv_obj_t* title = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(title, "Set BPM 1");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 15, 15);

  lv_obj_t* icon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);

  lv_label_set_text_static(icon, Symbols::tachometer);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);
  const uint8_t bpmValue = settingsController.GetPulse1();
  NRF_LOG_INFO("Setting Pulser 1 (manually) to %02d from settings", bpmValue);

  bpmCounter.SetValueChangedEventCallback(this, ValueChangedHandler);
  bpmCounter.Create();
  bpmCounter.SetValue(bpmValue);
  lv_obj_align(bpmCounter.GetObject(), nullptr, LV_ALIGN_CENTER, POS_X, POS_Y);

  btnPulse1 = lv_btn_create(lv_scr_act(), nullptr);
  btnPulse1 ->user_data = this;
  lv_obj_set_size(btnPulse1, 120, 48);
  lv_obj_align(btnPulse1, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_local_bg_color(btnPulse1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x38, 0x38, 0x38));
  lblPulse1 = lv_label_create(btnPulse1, nullptr);
  lv_label_set_text_static(lblPulse1, "Set");
  lv_obj_set_event_cb(btnPulse1, event_handler);
}

SettingPulse1::~SettingPulse1() {
  lv_obj_clean(lv_scr_act());
}

void SettingPulse1::HandleButtonPress() {
  const uint8_t bpmValue = bpmCounter.GetValue();
  NRF_LOG_INFO("Setting Pulser 1 (manually) to %02d", bpmValue);
  settingsController.SetPulse1(bpmValue);
  settingPulser.Advance();
}

void SettingPulse1::CheckBpm() {
//  pulse1Counter.SetValue(bpmCounter.GetValue());
}
