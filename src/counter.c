#include "utils.h"
#include "counter.h"

void reset_counter(Counter *counter){
  counter->counts=0;
}
void increase_counter(Counter *counter){
  counter->counts++;
}
void display_counter(Counter *counter){
  static char string[3];
  itoa3(counter->counts, string);
  text_layer_set_text(counter->text_layer, string);
}
