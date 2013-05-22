#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "utils.h"
#include "timer.h"
#include "counter.h"
#include "display_time.h"

#define MY_UUID { 0x66, 0x8E, 0x57, 0x84, 0x30, 0x9D, 0x43, 0x4A, 0xB0, 0xD6, 0xC8, 0xE2, 0xD7, 0x86, 0x69, 0x11 }
PBL_APP_INFO(MY_UUID,
             "HeliTimer", "mail@juliengrenier.cc",
             1, 2, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);

#define BUTTON_ENGINE_RUNTIME BUTTON_ID_UP
#define BUTTON_FLIGHT_TIME BUTTON_ID_SELECT
#define BUTTON_LIFTS BUTTON_ID_DOWN
Window window;
Layer flight_layer;
Layer aircraft_layer;
TextLayer flight_time_layer;
TextLayer aircraft_engine_runtime_layer;
Layer lands_layer;
TextLayer lands_title_text_layer;
TextLayer lands_text_layer;
Layer lifts_layer;
TextLayer lifts_title_text_layer;
TextLayer lifts_text_layer;
TextLayer time_layer;
TextLayer date_layer;


Timer flight_timer={  
  .text_layer = &flight_time_layer,
  .seconds = 0,
  .started = false,
  .vibrate_interval = 30 * 60 , //Vibrate every 30 minutes
  .vibrate_interval_offset = -2 * 60 //Make it vibrate two minutes before (e.g : 28, 58, 1:28, etc.)
};

Timer aircraft_engine_timer = {
  .text_layer = &aircraft_engine_runtime_layer,
  .seconds = 0,
  .started = false,
  .vibrate_interval = 0
};

static Counter lift_counter = {
  .text_layer = &lifts_text_layer,
  .counts = 0
};

static Counter land_counter = {
  .text_layer = &lands_text_layer,
  .counts = 0
};

void config_provider(ClickConfig **config, Window *window); 

void toggle_aircraft_timer(ClickRecognizerRef recognizer, Window *window);
void reset_aircraft_timer(ClickRecognizerRef recognizer, Window *window);

void toggle_flight_timer(ClickRecognizerRef recognizer, Window *window);
void reset_flight_timer(ClickRecognizerRef recognizer, Window *window);

void increase_lifts_counter(ClickRecognizerRef recognizer, Window *window);
void reset_lifts_counter(ClickRecognizerRef recognizer, Window *window);
void aircraft_layer_update_proc(Layer *me, GContext* ctx);
void flight_layer_update_proc(Layer *me, GContext* ctx);
void lifts_layer_update_proc(Layer *me, GContext* ctx);
void lands_layer_update_proc(Layer *me, GContext* ctx);

void handle_tick(AppContextRef ctx, PebbleTickEvent *t);
void init_display(Layer *root_layer);

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "HeliTimer");
  window_stack_push(&window, true /* Animated */);
  // Arrange for user input.
  window_set_click_config_provider(&window, (ClickConfigProvider) config_provider);
  window_set_background_color(&window, GColorWhite);
  window_set_fullscreen(&window, true);
  Layer *root_layer = window_get_root_layer(&window);
  init_display(root_layer);
  aircraft_layer.update_proc = &aircraft_layer_update_proc;
  flight_layer.update_proc = &flight_layer_update_proc;
  lifts_layer.update_proc = &lifts_layer_update_proc;
  lands_layer.update_proc = &lands_layer_update_proc;

}

void config_provider(ClickConfig **config, Window *window) {
  (void)window;
  config[BUTTON_ENGINE_RUNTIME]->click.handler = (ClickHandler)toggle_aircraft_timer;
  config[BUTTON_ENGINE_RUNTIME]->long_click.release_handler = (ClickHandler) reset_aircraft_timer;
  config[BUTTON_ENGINE_RUNTIME]->long_click.delay_ms = 700;

  config[BUTTON_FLIGHT_TIME]->click.handler = (ClickHandler)toggle_flight_timer;
  config[BUTTON_FLIGHT_TIME]->long_click.release_handler = (ClickHandler) reset_flight_timer;
  config[BUTTON_FLIGHT_TIME]->long_click.delay_ms = 700;

  config[BUTTON_LIFTS]->click.handler = (ClickHandler)increase_lifts_counter;
  config[BUTTON_LIFTS]->long_click.release_handler = (ClickHandler) reset_lifts_counter;
  config[BUTTON_LIFTS]->long_click.delay_ms = 700;
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}

void toggle_aircraft_timer(ClickRecognizerRef recognizer, Window *window){
  (void)window;
  toggle_timer(&aircraft_engine_timer);
  
}


void reset_aircraft_timer(ClickRecognizerRef recognizer, Window *window){
  reset_timer(&aircraft_engine_timer);
}

void toggle_flight_timer(ClickRecognizerRef recognizer, Window *window){
  (void)window;
  toggle_timer(&flight_timer);
  if(!flight_timer.started){
    increase_counter(&land_counter);
    layer_mark_dirty(&lands_layer);
  }

}

void reset_flight_timer(ClickRecognizerRef recognizer, Window *window){
  (void)window;
  reset_timer(&flight_timer);
  reset_counter(&land_counter);
  layer_mark_dirty(&lands_layer);
}

void increase_lifts_counter(ClickRecognizerRef recognizer, Window *window){
  (void)window;
  increase_counter(&lift_counter);
  layer_mark_dirty(&lifts_layer);
}
void reset_lifts_counter(ClickRecognizerRef recognizer, Window *window){
  (void)window;
  reset_counter(&lift_counter);
  layer_mark_dirty(&lifts_layer);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
  display_time(t->tick_time, &date_layer, &time_layer);

  timer_tick(&flight_timer);
  layer_mark_dirty(&flight_layer);

  timer_tick(&aircraft_engine_timer);
  layer_mark_dirty(&aircraft_layer);

}

void init_text_layer(TextLayer *text_layer, Layer *parent, GColor bg_color, GColor text_color, GRect frame){

  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24);
  text_layer_init(text_layer, frame);
  text_layer_set_font(text_layer, font);
  text_layer_set_background_color(text_layer, bg_color);
  text_layer_set_text_color(text_layer, text_color);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(parent, &text_layer->layer);
}

void init_display(Layer *root_layer){
  layer_init(&aircraft_layer, GRect(0,0,144,37));
  layer_add_child(root_layer, &aircraft_layer);
  init_text_layer(&aircraft_engine_runtime_layer, 
      &aircraft_layer, 
      GColorBlack,
      GColorWhite, 
      GRect(0,0, 144, 37));

  layer_init(&flight_layer, GRect(0,38, 144, 37));
  layer_add_child(root_layer, &flight_layer);
  init_text_layer(&flight_time_layer, 
      &flight_layer, 
      GColorWhite, 
      GColorBlack,
      GRect(0,0, 144, 37));
  

  init_text_layer(&date_layer,
      root_layer,
      GColorBlack,
      GColorWhite,
      GRect(0,66, 72, 37));

  init_text_layer(&time_layer,
      root_layer,
      GColorBlack,
      GColorWhite,
      GRect(73,66, 72, 37));

  layer_init(&lands_layer, GRect(0,104, 72, 64)); 
  layer_add_child(root_layer, &lands_layer);
  init_text_layer(&lands_title_text_layer, 
      &lands_layer,
      GColorBlack,
      GColorWhite,
      GRect(0,0, 72, 25));
  text_layer_set_text(&lands_title_text_layer, "LANDS");
  init_text_layer(&lands_text_layer, 
      &lands_layer,
      GColorBlack,
      GColorWhite,
      GRect(0,25, 72, 39));

  layer_init(&lifts_layer, GRect(73,104, 72, 64)); 
  layer_add_child(root_layer, &lifts_layer);
  init_text_layer(&lifts_title_text_layer, 
      &lifts_layer,
      GColorBlack,
      GColorWhite,
      GRect(0,0, 72, 25));
  text_layer_set_text(&lifts_title_text_layer, "LIFTS");
  init_text_layer(&lifts_text_layer, 
      &lifts_layer,
      GColorBlack,
      GColorWhite,
      GRect(0,25, 72, 39));
}

void aircraft_layer_update_proc(Layer *me, GContext* ctx){
  char *aircraft_timer_display = "00:00:00";
  display_timer(&aircraft_engine_timer, aircraft_timer_display);
}
void flight_layer_update_proc(Layer *me, GContext* ctx){
  char *flight_timer_display = "00:00:00";
  display_timer(&flight_timer, flight_timer_display);
}
void lifts_layer_update_proc(Layer *me, GContext* ctx){
  display_counter(&lift_counter);
}
void lands_layer_update_proc(Layer *me, GContext* ctx){
  display_counter(&land_counter);
}

