#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "displayapp/screens/BatteryIcon.h"
#include "utility/DirtyValue.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
  }

  namespace Applications {
    namespace Screens {

      class WatchFaceConductor : public Screen {
      public:
        WatchFaceConductor(Controllers::DateTime& dateTimeController,
                        const Controllers::Battery& batteryController,
                        const Controllers::Ble& bleController,
                        Controllers::NotificationManager& notificationManager,
                        Controllers::Settings& settingsController);

        ~WatchFaceConductor() override;

        void Refresh() override;

      private:
        uint8_t sHour, sMinute, sSecond;

        Utility::DirtyValue<uint8_t> batteryPercentRemaining {0};
        Utility::DirtyValue<bool> isCharging {};
        Utility::DirtyValue<bool> bleState {};
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime;
        Utility::DirtyValue<bool> notificationState {false};
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::days>> currentDate;

        lv_obj_t* keys[12];

        lv_obj_t* plugIcon;
        lv_obj_t* notificationIcon;
        lv_obj_t* bleIcon;
        lv_obj_t* dayIcon;
        lv_obj_t* nightIcon;
        BatteryIcon batteryIcon;

        Controllers::DateTime& dateTimeController;
        const Controllers::Battery& batteryController;
        const Controllers::Ble& bleController;
        Controllers::NotificationManager& notificationManager;
        Controllers::Settings& settingsController;

        void UpdateClock();
        void SetBatteryIcon();

        lv_task_t* taskRefresh;
      };
    }

    template <>
    struct WatchFaceTraits<WatchFace::Conductor> {
      static constexpr WatchFace watchFace = WatchFace::Conductor;
      static constexpr const char* name = "Conductor face";

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::WatchFaceConductor(controllers.dateTimeController,
                                            controllers.batteryController,
                                            controllers.bleController,
                                            controllers.notificationManager,
                                            controllers.settingsController);
      };

      static bool IsAvailable(Pinetime::Controllers::FS& /*filesystem*/) {
        return true;
      }
    };
  }
}
