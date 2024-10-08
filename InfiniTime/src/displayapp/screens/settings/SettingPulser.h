#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/ScreenList.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class SettingPulser : public Screen {
      public:
        SettingPulser(DisplayApp* app,
                      Pinetime::Controllers::Settings& settingsController);
        ~SettingPulser() override;

        bool OnTouchEvent(TouchEvents event) override;
        void Advance();
        void Quit();

      private:
        DisplayApp* app;
        Controllers::Settings& settingsController;

        ScreenList<4> screens;
        std::unique_ptr<Screen> screenSetPulse1();
        std::unique_ptr<Screen> screenSetPulse2();
        std::unique_ptr<Screen> screenSetPulse3();
        std::unique_ptr<Screen> screenSetPulse4();
      };
    }
  }
}
