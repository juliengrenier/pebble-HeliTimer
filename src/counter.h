#pragma once

#include "pebble_os.h"

typedef struct Counter {
   TextLayer* text_layer;
   int counts;
} Counter;
void reset_counter(Counter *counter);
void increase_counter(Counter *counter);
void display_counter(Counter *counter);
