#include "displayapp/screens/Pulser.h"
#include "nrf_log.h"
#include <cstdio>

using namespace Pinetime::Applications::Screens;

namespace {
  void event_handler(lv_obj_t* obj, lv_event_t event) {
    if(event == LV_EVENT_CLICKED) {
      auto* screen = static_cast<Pulser*>(obj->user_data);
      screen->OnEvent(obj, event);
    }
  }
}

Pulser::Pulser(Controllers::MotorController& motorController,
               Controllers::Settings& settingsController,
               System::SystemTask& systemTask) : motorController {motorController}, settingsController {settingsController}, systemTask {systemTask}{
  //lv_obj_t* app_screen= lv_obj_create(NULL,NULL);
  //lv_obj_set_style_local_bg_color(app_screen, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(211,211,211));
  //lv_obj_set_style_local_border_width(app_screen, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
  //lv_scr_load(app_screen);

  // the labels used to show the app name and credit the author
   lv_obj_t * lblApp;

   // the labels with the text for each button, that actually is the target bpm
   lv_obj_t * lbl1 ;
   lv_obj_t * lbl2;
   lv_obj_t * lbl3;
   lv_obj_t * lbl4;

   // the buttons are all childs of the main screen
   btn1 = lv_btn_create(lv_scr_act(),NULL);
   btn2 = lv_btn_create(lv_scr_act(),NULL);
   btn3 = lv_btn_create(lv_scr_act(),NULL);
   btn4 = lv_btn_create(lv_scr_act(),NULL);

   // all buttons have the same size
   lv_obj_set_size (btn1 , 100,100);
   lv_obj_set_size (btn2, 100,100);
   lv_obj_set_size (btn3, 100,100);
   lv_obj_set_size (btn4, 100,100);

   // set the position of the buttons to simulate a grid
   lv_obj_set_pos (btn1 ,   0,  0);
   lv_obj_set_pos (btn2, 140,  0);
   lv_obj_set_pos (btn3, 140, 140);
   lv_obj_set_pos (btn4,   0,140);

   // set style all of all buttons the same gray
   lv_obj_set_style_local_bg_color(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
   lv_obj_set_style_local_bg_color(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
   lv_obj_set_style_local_bg_color(btn3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
   lv_obj_set_style_local_bg_color(btn4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);

   // the labels for each button
   lbl1 = lv_label_create(btn1 ,NULL);
   lbl2 = lv_label_create(btn2 ,NULL);
   lbl3 = lv_label_create(btn3 ,NULL);
   lbl4 = lv_label_create(btn4 ,NULL);

   // the label for the appname and author's credit
   lblApp = lv_label_create(lv_scr_act(),NULL);

   // this is needed to support the event handling
   btn1->user_data = this;
   btn2->user_data = this;
   btn3->user_data = this;
   btn4->user_data = this;

   // maximise the size of the text by using the biggest available font
   // this font had first to be enabled in both the build/lv_conf.h and the src/libs/lv_conf.h
   lv_obj_set_style_local_text_font(lbl1 ,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&jetbrains_mono_42);
   lv_obj_set_style_local_text_font(lbl2 ,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&jetbrains_mono_42);
   lv_obj_set_style_local_text_font(lbl3 ,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&jetbrains_mono_42);
   lv_obj_set_style_local_text_font(lbl4 ,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&jetbrains_mono_42);
   // use the smaller font for the label of the appname and for the author's credit
   lv_obj_set_style_local_text_font(lblApp ,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&jetbrains_mono_bold_20);

   // position the labels of the appname and of the author's credit in the white space between the buttons
   lv_obj_set_pos(lblApp,0,110);

   // set the value of the buttons, this could be in the future changed through a configuration app

   const uint8_t bpmPulse1 = settingsController.GetPulse1();
   const uint8_t bpmPulse2 = settingsController.GetPulse2();
   const uint8_t bpmPulse3 = settingsController.GetPulse3();
   const uint8_t bpmPulse4 = settingsController.GetPulse4();
   NRF_LOG_INFO("Setting Pulser 1 to %02d from settings", bpmPulse1);
   NRF_LOG_INFO("Setting Pulser 2 to %02d from settings", bpmPulse2);
   NRF_LOG_INFO("Setting Pulser 3 to %02d from settings", bpmPulse3);
   NRF_LOG_INFO("Setting Pulser 4 to %02d from settings", bpmPulse4);

   lv_label_set_text_fmt(lbl1 ,"%d",bpmPulse1 );
   lv_label_set_text_fmt(lbl2 ,"%d",bpmPulse2 );
   lv_label_set_text_fmt(lbl3 ,"%d",bpmPulse3 );
   lv_label_set_text_fmt(lbl4 ,"%d",bpmPulse4 );
   lv_label_set_text(lblApp,"PineConductor Pulser");

   // required to enable events to be fired by each button
   lv_obj_set_event_cb(btn1 , event_handler);
   lv_obj_set_event_cb(btn2, event_handler);
   lv_obj_set_event_cb(btn3, event_handler);
   lv_obj_set_event_cb(btn4, event_handler);

   // required for refreshing
   taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

// same as heartrate.cpp
Pulser::~Pulser() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(System::Messages::EnableSleeping);
}

// this is the code executed at refreshing.
// the vibration is executed for 30 milliseconds at the interval specified
// in the future this could as well be changed through the configuration app
void Pulser::Refresh() {
  if (metronomeStarted) {
    if (xTaskGetTickCount() - startTime > 60u * configTICK_RATE_HZ / static_cast<uint16_t>(bpm)) {
      startTime += 60 * configTICK_RATE_HZ / bpm;
      motorController.RunForDuration(30);
    }
  }
}

// this function is called when a button is clicked.
void Pulser::OnEvent(lv_obj_t* obj, lv_event_t event) {
  switch (event) {
    case LV_EVENT_CLICKED: {
      lv_color_t current_color = lv_obj_get_style_bg_color(obj, LV_BTN_PART_MAIN);
      if (lv_color_to32(current_color) == lv_color_to32(LV_COLOR_OLIVE) ){
        lv_obj_set_style_local_bg_color(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
        metronomeStarted = false;
        systemTask.PushMessage(System::Messages::EnableSleeping);
      } else {
        lv_obj_set_style_local_bg_color(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
        lv_obj_set_style_local_bg_color(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
        lv_obj_set_style_local_bg_color(btn3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
        lv_obj_set_style_local_bg_color(btn4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
        lv_obj_t * child = lv_obj_get_child(obj,0);
        lv_obj_set_style_local_bg_color(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_OLIVE);
        bpm = static_cast<int16_t>(std::atoi(lv_label_get_text(child)));
        metronomeStarted = true;
        systemTask.PushMessage(System::Messages::DisableSleeping);
        startTime = xTaskGetTickCount();
      }
      break;
    }
    default:
      break;
  }
}
// This function restores the default background color when closing the app
void Pulser::CloseAppAndRestoreBackground() {
    // Create a new screen for the next app (e.g., watch face or menu)
    lv_obj_t* next_screen = lv_obj_create(NULL, NULL);

    // Set the background color of the next screen (black in this case)
    lv_obj_set_style_local_bg_color(next_screen, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    // Load the next screen (replacing the current app screen)
    lv_scr_load(next_screen);
}



bool Pulser::OnTouchEvent(TouchEvents event) {
  if (event == TouchEvents::SwipeDown && allowExit) {
    running = false;
    Pulser::CloseAppAndRestoreBackground();
  }
  return true;
}

