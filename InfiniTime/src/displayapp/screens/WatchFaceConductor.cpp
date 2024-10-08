#include "displayapp/screens/WatchFaceConductor.h"
#include <cmath>
#include <lvgl/lvgl.h>
#include "displayapp/screens/BatteryIcon.h"
#include "displayapp/screens/BleIcon.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/screens/NotificationIcon.h"
#include "components/settings/Settings.h"
#include "displayapp/InfiniTimeTheme.h"

using namespace Pinetime::Applications::Screens;


WatchFaceConductor::WatchFaceConductor(Controllers::DateTime& dateTimeController,
                                 const Controllers::Battery& batteryController,
                                 const Controllers::Ble& bleController,
                                 Controllers::NotificationManager& notificationManager,
                                 Controllers::Settings& settingsController)
  : currentDateTime {{}},
    batteryIcon(true),
    dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    notificationManager {notificationManager},
    settingsController {settingsController} {

  sHour = 99; // initialization for the hour mark counter
  sMinute = 99; // initialization for the minute mark counter

  // the horizontal positioning of the keys should be calculated however for the first
  // iteration let's try to do it per hand relying on graph paper
  const int kp[] = {0,40,60,100,120,0,40,60,100,120,160,180};
  // the order of object creation must consider the fact that new objects
  // stay on top of previous objects
  // however the key index are done according to music theory
  // and therefore the key order array that specifies in which order
  // the keys have to be constructed
  int ww =  60;  // key width white
  int wb =  40;  // key width black
  int hw = 100;  // key heigth white
  int hb =  70;  // key heigth black
  int tr =   0;  // top row vertical position
  //int mr = 100;  // mid row vertical position
  int br = 140;  // bottom row vertical position
  const int ko[] = {0, 2, 4, 1, 3, 5, 7, 9, 11, 6, 8, 10};
  int cr = tr;
  int i = -1;
  for (int j=0; j<12; j++) {
    cr = tr;
    i = ko[j];
    keys[i]=lv_obj_create(lv_scr_act(),NULL);
    lv_obj_set_style_local_radius(keys[i], LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,0);
    if (i > 4) {
     cr = br ;
    }
    lv_obj_set_pos(keys[i],kp[i],cr);
    switch (i) {
      case 0: case 2: case 4: case 5: case 7: case 9: case 11:
       lv_obj_set_style_local_border_color(keys[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
       lv_obj_set_style_local_bg_color(keys[i],     LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
       lv_obj_set_style_local_border_width(keys[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);
       lv_obj_set_size(keys[i],ww,hw);
       break;
     default:
       lv_obj_set_style_local_bg_color(keys[i],LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
       lv_obj_set_size(keys[i],wb,hb);
    }
  }
//
  batteryIcon.Create(lv_scr_act());
  lv_obj_align(batteryIcon.GetObject(), nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  plugIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(plugIcon, Symbols::plug);
  lv_obj_align(plugIcon, nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  bleIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(bleIcon, "");
  lv_obj_align(bleIcon, nullptr, LV_ALIGN_IN_TOP_RIGHT, -30, 0);

  dayIcon = lv_label_create(lv_scr_act(),nullptr);
  lv_label_set_text_static(dayIcon, Symbols::sun);
  lv_obj_align(dayIcon, nullptr, LV_ALIGN_IN_TOP_RIGHT, 220, 20);

  nightIcon = lv_label_create(lv_scr_act(),nullptr);
  lv_label_set_text_static(nightIcon, Symbols::snowflake);
  lv_obj_align(nightIcon, nullptr, LV_ALIGN_IN_TOP_RIGHT, 220, 20);

  notificationIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(notificationIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_LIME);
  lv_label_set_text_static(notificationIcon, NotificationIcon::GetIcon(false));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, -100, 100);
  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);


  Refresh();
}

WatchFaceConductor::~WatchFaceConductor() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void WatchFaceConductor::UpdateClock() {
  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  int keyhour = hour % 12;
  int keymin = ( minute / 5 ) % 12;
  if (sHour != keyhour || sMinute != keymin) {
    sHour = keyhour;
    sMinute = keymin;
    // only change every 5 minutes
    // reset all to standard colours
    for (int i=0; i<12; i++) {
       switch (i) {
         case 0: case 2: case 4: case 5: case 7: case 9: case 11:
          lv_obj_set_style_local_bg_color(keys[i], LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
          break;
        default:
          lv_obj_set_style_local_bg_color(keys[i],LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
       }
    }
    // set the new colours
    lv_obj_set_style_local_bg_color(keys[keyhour], LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_obj_set_style_local_bg_color(keys[keymin],  LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
    //
    if ( (hour > 6) && ( hour < 20 ) ) {
      lv_obj_set_hidden(dayIcon,false);
      lv_obj_set_hidden(nightIcon,true);
    } else {
      lv_obj_set_hidden(dayIcon,true);
      lv_obj_set_hidden(nightIcon,false);
    }
  }
}

void WatchFaceConductor::SetBatteryIcon() {
  auto batteryPercent = batteryPercentRemaining.Get();
  batteryIcon.SetBatteryPercentage(batteryPercent);
}

void WatchFaceConductor::Refresh() {
  isCharging = batteryController.IsCharging();
  if (isCharging.IsUpdated()) {
    if (isCharging.Get()) {
      lv_obj_set_hidden(batteryIcon.GetObject(), true);
      lv_obj_set_hidden(plugIcon, false);
    } else {
      lv_obj_set_hidden(batteryIcon.GetObject(), false);
      lv_obj_set_hidden(plugIcon, true);
      SetBatteryIcon();
    }
  }
  if (!isCharging.Get()) {
    batteryPercentRemaining = batteryController.PercentRemaining();
    if (batteryPercentRemaining.IsUpdated()) {
      SetBatteryIcon();
    }
  }

  bleState = bleController.IsConnected();
  if (bleState.IsUpdated()) {
    if (bleState.Get()) {
      lv_label_set_text_static(bleIcon, Symbols::bluetooth);
    } else {
      lv_label_set_text_static(bleIcon, "");
    }
  }

  notificationState = notificationManager.AreNewNotificationsAvailable();

  if (notificationState.IsUpdated()) {
    lv_label_set_text_static(notificationIcon, NotificationIcon::GetIcon(notificationState.Get()));
  }

  currentDateTime = dateTimeController.CurrentDateTime();
  if (currentDateTime.IsUpdated()) {
    UpdateClock();
  }
}
