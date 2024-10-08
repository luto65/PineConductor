#include "displayapp/screens/Transposer.h"
#include "lvgl/lvgl.h"
#include <cmath>
#include <iostream>
#include <cstdio>
using namespace Pinetime::Applications::Screens;

namespace {
  void event_handler(lv_obj_t* obj, lv_event_t event) {
    if(event == LV_EVENT_CLICKED) {
      auto* screen = static_cast<Transposer*>(obj->user_data);
      screen->OnEvent(obj, event);
    }
  }
}

uint16_t Transposer::transposeConcert(uint16_t input, int8_t offset) {
  if (input > 11) {
    return 0;  
  }
  int16_t result = static_cast<int16_t>(input) + offset;
  if (result < 0) {
    result += 12;
  } else if (result > 11) {
    result -= 12;
  }
  return static_cast<uint16_t>(result);
}

Transposer::Transposer( System::SystemTask& systemTask) : systemTask {systemTask}{
  // create the keys, by splitting the keyboard over two lanes so that the keys are bigger
  // and this makes them easier to use. removed labels as much as possible to facilitate use
  // the keys are of two different sizes: 60x100(whites) and 40x60 (blacks)
  int ww =  60;  // key width white
  int wb =  40;  // key width black
  int hw = 100;  // key heigth white
  int hb =  70;  // key heigth black
  int tr =   0;  // top row vertical position
  //int mr = 100;  // mid row vertical position
  int br = 140;  // bottom row vertical position
  // gray area filler is used to colour the otherwise black background
  // the alternative of changing the overall screen background gave problems
  // since after closing the app the background of the whole screen remained fixd
  // attempts in resetting the screen background to the default black failed
  // therefore the following workaround 
  // by creating it as first object it automatically fills any empty space
  lv_obj_t *filler = lv_obj_create(lv_scr_act(),NULL);
  lv_obj_set_style_local_radius(filler, LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,0);
  lv_obj_set_style_local_bg_color(filler,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_obj_set_style_local_border_width(filler, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(filler,240,240);
  lv_obj_set_pos(filler,0,0);
  // no colour labels are needed since by clicking on any key and see where the green goes
  // it automatically let the brain realise that *that* is the is the C for or that transposing 
  // instrument. less labels more brain 
  // however a title might be useful too.. therefore here the app title
  
  const char* labels[] = {"Concert", "Alto Sax", "Tenor Sax", "French Horn"};
  const int positions[][2] = {{0, 100}, {110,100}, {0, 120}, {110, 120}};
  
  for (int i = 0; i < 4; i++) {
    lv_obj_t* label = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_bold_20);
    lv_obj_set_pos(label, positions[i][0], positions[i][1]);
    lv_label_set_text(label, labels[i]);
    switch (i) {
     case 0:
      lv_obj_set_style_local_text_color(label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_GREEN);
      break;
     case 1:
      lv_obj_set_style_local_text_color(label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_MAROON);
      break;
     case 2:
      lv_obj_set_style_local_text_color(label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLUE);
      break;
     case 3:
      lv_obj_set_style_local_text_color(label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_ORANGE);
      break;
     default:
      break;
    }
  }
  
  // the horizontal positioning of the keys should be calculated however for the first
  // iteration let's try to do it per hand relying on graph paper 
  const int kp[] = {0,40,60,100,120,0,40,60,100,120,160,180};  
  // the order of object creation must consider the fact that new objects
  // stay on top of previous objects 
  // however the key index are done according to music theory
  // and therefore the key order array that specifies in which order
  // the keys have to be constructed
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
    lv_obj_set_event_cb(keys[i], event_handler);
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
    keys[i] ->user_data = this;
   }
   taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

void Transposer::Refresh() {
}

// 
Transposer::~Transposer() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(System::Messages::EnableSleeping);
}

// this function is called when a button is clicked.
void Transposer::OnEvent(lv_obj_t* obj, lv_event_t event) {
  switch (event) {
    case LV_EVENT_CLICKED: {
      // reset to standard values
      for (int i=0; i<12; i++) {
       switch (i) {
         case 0: case 2: case 4: case 5: case 7: case 9: case 11:
          lv_obj_set_style_local_bg_color(keys[i], LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
          break;
        default:
          lv_obj_set_style_local_bg_color(keys[i],LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
       }
      }
      int concertkey=-1;
      for (int i=0; i<12; i++) {
       if (obj == keys[i]) {
        concertkey=i;
       }
      }
      int ebkey= Transposer::transposeConcert(concertkey,-3);
      int bkey = Transposer::transposeConcert(concertkey,2);
      int fkey = Transposer::transposeConcert(concertkey,-5);
      lv_obj_set_style_local_bg_color(keys[concertkey], LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_OLIVE);
      lv_obj_set_style_local_bg_color(keys[ebkey],      LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAROON);
      lv_obj_set_style_local_bg_color(keys[bkey],       LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
      lv_obj_set_style_local_bg_color(keys[fkey],       LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
    }
    default:
      break;
  }
}

bool Transposer::OnTouchEvent(TouchEvents event) {
  if (event == TouchEvents::SwipeDown && allowExit) {
    running = false;
  }
  return true;
}
