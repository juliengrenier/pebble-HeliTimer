#pragma once

typedef struct Timer {
  TextLayer *text_layer;
  int32_t seconds;
  bool started;
} Timer;

void display_timer(Timer *timer, char* display);
void timer_tick(Timer *timer);
void reset_timer(Timer *timer);
void toggle_timer(Timer *timer);
