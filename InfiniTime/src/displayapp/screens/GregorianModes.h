#pragma once
#include "systemtask/SystemTask.h"
#include "displayapp/screens/Screen.h"
#include "Symbols.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class GregorianModes:
      public Screen {
      public:
        GregorianModes( System::SystemTask& systemTask);
        ~GregorianModes() override;
        void Refresh() override;
	void SetNewMode(uint16_t mode);
	void CloseAppAndRestoreBackground();
        void OnEvent(lv_obj_t* obj, lv_event_t event);
        bool OnTouchEvent(TouchEvents event) override;
      private:
        System::SystemTask& systemTask;
        bool allowExit = false;
	uint16_t  gregorianMode = 1;
	uint16_t  tonic = 0;
	lv_obj_t* keys[12];
        lv_task_t* taskRefresh;
      };
    }

    template <>
    struct AppTraits<Apps::GregorianModes> {
      static constexpr Apps app = Apps::GregorianModes;
      static constexpr const char* icon = Screens::Symbols::music;
      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::GregorianModes(*controllers.systemTask);
      }
    };
  }
}
