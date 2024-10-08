#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "displayapp/screens/Screen.h"
#include "displayapp/widgets/Counter.h"
#include "displayapp/widgets/DotIndicator.h"
#include "displayapp/screens/settings/SettingPulser.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class SettingPulse3 : public Screen {
      public:
        SettingPulse3(Pinetime::Controllers::Settings& settingsController,
                      Pinetime::Applications::Screens::SettingPulser& settingPulser);
        ~SettingPulse3() override;

        void HandleButtonPress();
        void CheckBpm();

      private:
        Pinetime::Controllers::Settings& settingsController;
        Pinetime::Applications::Screens::SettingPulser& settingPulser;
        lv_obj_t* btnPulse3;
        lv_obj_t* lblPulse3;

        Widgets::Counter bpmCounter = Widgets::Counter(40, 180, jetbrains_mono_bold_20);
      };
    }
  }
}
