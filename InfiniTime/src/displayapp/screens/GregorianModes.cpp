#include "displayapp/screens/GregorianModes.h"
#include "lvgl/lvgl.h"
#include <iostream>
#include "nrf_log.h"
#include <cstdio>
using namespace Pinetime::Applications::Screens;

namespace {
  void event_handler(lv_obj_t* obj, lv_event_t event) {
//    if( lv_obj_get_x(obj) > 1 ) {
     if(event == LV_EVENT_CLICKED) {
      auto* screen = static_cast<GregorianModes*>(obj->user_data);
      screen->OnEvent(obj, event);
    } else {
     if (event == LV_EVENT_VALUE_CHANGED) {
      uint16_t newGregorianMode = lv_dropdown_get_selected(obj);
      auto* screen = static_cast<GregorianModes*>(obj->user_data);
      printf("Error: screen user_data is null\n");
      if (screen) {
       screen->SetNewMode(newGregorianMode);
      } else { 
       printf("Error: screen user_data is null\n");
      }
     }
    }
  }
}

GregorianModes::GregorianModes( System::SystemTask& systemTask) : systemTask {systemTask}{
  // create the keys, by splitting the keyboard over two lanes so that the keys are bigger
  // and this makes them easier to use. removed labels as much as possible to facilitate use
  // the keys are of two different sizes: 60x100(whites) and 40x60 (blacks)
  int ww =  60;  // key width white
  int wb =  40;  // key width black
  int hw = 100;  // key heigth white
  int hb =  70;  // key heigth black
  int tr =   0;  // top row vertical position
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
  // create a dropdown menu 
  lv_obj_t * dd = lv_dropdown_create(lv_scr_act(),NULL);
  lv_dropdown_add_option(dd,"Lydian",0);
  lv_dropdown_add_option(dd,"Ionic",1);
  lv_dropdown_add_option(dd,"Mixolydian",2);
  lv_dropdown_add_option(dd,"Dorian",3);
  lv_dropdown_add_option(dd,"Aeolic",4);
  lv_dropdown_add_option(dd,"Phrygian",5);
  lv_dropdown_add_option(dd,"Locrian",6);
  lv_dropdown_set_selected(dd,1);
  lv_dropdown_set_dir(dd,LV_DROPDOWN_DIR_UP);
  lv_dropdown_set_show_selected(dd,true);
  lv_obj_set_width(dd,240);
  lv_obj_align(dd,NULL,LV_ALIGN_IN_TOP_LEFT,0,180);
  lv_obj_set_pos(dd,0,100);
  dd->user_data = this;
  lv_obj_set_event_cb(dd,event_handler);
  
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

void GregorianModes::SetNewMode(uint16_t mode) {
 // this method is setting now the global variable to the new value
 gregorianMode = mode;
}

void GregorianModes::Refresh() {
}

// 
GregorianModes::~GregorianModes() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(System::Messages::EnableSleeping);
}

// this function is called when a button is clicked.
void GregorianModes::OnEvent(lv_obj_t* obj, lv_event_t event) {
  const int modes[][6] ={
	  {2,2,2,1,2,2}, //Lydian     = Ionic +4
	  {2,2,1,2,2,2}, //Ionic      = Dur
	  {2,2,1,2,2,1}, //Mixolydian = Ionic -7
	  {2,1,2,2,2,1}, //Dorian     = Aeolic +6
	  {2,1,2,2,1,2}, //Aeolic     = Moll
	  {1,2,2,2,1,2}, //Phrygian     Aeolic - 2
	  {1,2,2,1,2,2}}; //Locrian    = Aeolic -2 -5
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
      // determine the tonic - the latest value and the standard value is in GregorianModes.h
      for (int i=0; i<12; i++) {
       if (obj == keys[i]) {
        tonic=i;
       }
      }
      //now set the tonic to olive
      lv_obj_set_style_local_bg_color(keys[tonic], LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_OLIVE);
      // and the corresponding keys for the specific mode to blue
      int colorkey = tonic; 
      for (int i =0; i<6; i++) {
        colorkey =colorkey +modes[gregorianMode][i];
        if ( colorkey > 11 ) {
	   colorkey -=12;
	 }
        lv_obj_set_style_local_bg_color(keys[colorkey],LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
      }
    }
    default:
      break;
  }
}

bool GregorianModes::OnTouchEvent(TouchEvents event) {
  if (event == TouchEvents::SwipeDown && allowExit) {
    running = false;
  }
  return true;
}
