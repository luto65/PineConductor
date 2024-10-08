#pragma once

#include "systemtask/SystemTask.h"
#include "components/motor/MotorController.h"
#include "displayapp/screens/Screen.h"
#include "Symbols.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Pulser :
      public Screen {
      public:
        Pulser(Controllers::MotorController& motorController, Controllers::Settings& settingsController, System::SystemTask& systemTask);
        ~Pulser() override;
        void Refresh() override;
	void CloseAppAndRestoreBackground();
        void OnEvent(lv_obj_t* obj, lv_event_t event);
        bool OnTouchEvent(TouchEvents event) override;

      private:
        TickType_t startTime = 0;
        TickType_t tappedTime = 0;
        Controllers::MotorController& motorController;
        Controllers::Settings& settingsController;

        System::SystemTask& systemTask;
        int16_t bpm = 120;
//        uint8_t bpb = 4;
        uint8_t counter = 10;


        bool metronomeStarted = false;
        bool allowExit = false;

        lv_obj_t *bpmArc, *bpmTap, *bpmValue;
        lv_obj_t *bpbDropdown, *currentBpbText;
        lv_obj_t *btn1, *btn2, *btn3, *btn4;

        lv_task_t* taskRefresh;

      };
    }

    template <>
    struct AppTraits<Apps::Pulser> {
      static constexpr Apps app = Apps::Pulser;
      static constexpr const char* icon = Screens::Symbols::tachometer;
      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::Pulser(controllers.motorController, controllers.settingsController, *controllers.systemTask);
      }
    };
  }
}
