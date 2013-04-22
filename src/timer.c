#include "pebble_os.h"

#include "utils.h"
#include "timer.h"

void display_timer(Timer *timer, char* display){
  format_seconds(timer->seconds, display);
  text_layer_set_text(timer->text_layer, display);
}

void timer_tick(Timer *timer){
  if (timer->started){
    timer->seconds++;
  }
}


void reset_timer(Timer *timer){
  timer->seconds = 0;
  timer->started = false;
}

void toggle_timer(Timer *timer){
  if (timer->started){
    timer->started = false;
  }else{
    timer->started = true;
  }
}
