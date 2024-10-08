#include "displayapp/screens/settings/SettingPulser.h"
#include "displayapp/screens/settings/SettingPulse1.h"
#include "displayapp/screens/settings/SettingPulse2.h"
#include "displayapp/screens/settings/SettingPulse3.h"
#include "displayapp/screens/settings/SettingPulse4.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/ScreenList.h"
#include "components/settings/Settings.h"
#include "displayapp/widgets/DotIndicator.h"

using namespace Pinetime::Applications::Screens;

bool SettingPulser::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

SettingPulser::SettingPulser(Pinetime::Applications::DisplayApp* app,
                             Pinetime::Controllers::Settings& settingsController)
  : app {app},
    settingsController {settingsController},
    screens {app,
             0, {[this]() -> std::unique_ptr<Screen> {
                return screenSetPulse1();
              },
             [this]() -> std::unique_ptr<Screen> {
                return screenSetPulse2();
              },
             [this]() -> std::unique_ptr<Screen> {
                return screenSetPulse3();
              },
             [this]() -> std::unique_ptr<Screen> {
                return screenSetPulse4();
            }},
             Screens::ScreenListModes::UpDown} {
}

std::unique_ptr<Screen> SettingPulser::screenSetPulse1() {
  Widgets::DotIndicator dotIndicator(0, 4);
  dotIndicator.Create();
  return std::make_unique<Screens::SettingPulse1>( settingsController, *this);
}

std::unique_ptr<Screen> SettingPulser::screenSetPulse2() {
  Widgets::DotIndicator dotIndicator(1, 4);
  dotIndicator.Create();
  return std::make_unique<Screens::SettingPulse2>( settingsController, *this);
}

std::unique_ptr<Screen> SettingPulser::screenSetPulse3() {
  Widgets::DotIndicator dotIndicator(2, 4);
  dotIndicator.Create();
  return std::make_unique<Screens::SettingPulse3>( settingsController, *this);
}

std::unique_ptr<Screen> SettingPulser::screenSetPulse4() {
  Widgets::DotIndicator dotIndicator(3, 4);
  dotIndicator.Create();
  return std::make_unique<Screens::SettingPulse4>( settingsController, *this);
}
SettingPulser::~SettingPulser() {
  lv_obj_clean(lv_scr_act());
}

void SettingPulser::Advance() {
  screens.OnTouchEvent(Pinetime::Applications::TouchEvents::SwipeUp);
}

void SettingPulser::Quit() {
  running = false;
}
