#pragma once

#include "systemtask/SystemTask.h"
#include "displayapp/screens/Screen.h"
#include "Symbols.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Transposer :
      public Screen {
      public:
        Transposer( System::SystemTask& systemTask);
        ~Transposer() override;
        void Refresh() override;
        void CloseAppAndRestoreBackground();
        uint16_t transposeConcert(uint16_t input, int8_t offset);
        void OnEvent(lv_obj_t* obj, lv_event_t event);
        bool OnTouchEvent(TouchEvents event) override;
      private:
        System::SystemTask& systemTask;
        bool allowExit = false;
	lv_obj_t* keys[12];
        lv_task_t* taskRefresh;
      };
    }

    template <>
    struct AppTraits<Apps::Transposer> {
      static constexpr Apps app = Apps::Transposer;
      static constexpr const char* icon = Screens::Symbols::dice;
      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::Transposer(*controllers.systemTask);
      }
    };
  }
}
