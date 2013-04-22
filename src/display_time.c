#include "pebble_os.h"
#include "display_time.h"

void display_time(PblTm *time, TextLayer *date_layer, TextLayer *time_layer){

  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxx 00";

  char *time_format;

  // TODO: Only update the date when it's changed.
  string_format_time(date_text, sizeof(date_text), "%b %e", time);
  text_layer_set_text(date_layer, date_text);


  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(time_layer, time_text);
}
