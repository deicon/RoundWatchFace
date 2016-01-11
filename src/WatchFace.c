#include "WatchFace.h"
#include "BatteryWatcher.h"
#include "AccelWatcher.h"
#include "Compass.h"
#include "BluetootWatcher.h" 

static Window *s_main_window;
static Layer *s_draw_layer;
static TextLayer *s_time_layer;
static Layer *s_bg_layer;

static GPath *s_minute_arrow, *s_hour_arrow;

static void calculate_pointer_end(int16_t t, const int16_t length_from_center, GPoint *center, GPoint *point, long scale, long offset) {
  // rotate more if any other number than scale has to be on top 
  int32_t offset_angle = TRIG_MAX_ANGLE * offset / scale;
  long angle = TRIG_MAX_ANGLE * t / scale;
  
  point->y = (-cos_lookup(angle+offset_angle) * length_from_center / TRIG_MAX_RATIO) + center->y;
  point->x = (sin_lookup(angle+offset_angle) * length_from_center / TRIG_MAX_RATIO) + center->x;
}

static void time_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

/*Update Drawing Handler*/
static void hands_update_proc(Layer *layer, GContext *ctx) {
  
 
  int16_t scale = 12*60; 
  if ( clock_is_24h_style() ) {
    scale = 24*60;
  }

  
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  
  GPoint secondHand;
  GPoint hourHand1, hourHand2;
  
  time_t now = time(NULL);
  struct tm* t = localtime(&now); 

  int16_t ms = time_ms(NULL, NULL);
  
  const int16_t second_hand_length = PBL_IF_ROUND_ELSE((bounds.size.w / 2) - 19, (bounds.size.w / 2) - 2);
  const int16_t hour_tick_length = PBL_IF_ROUND_ELSE((bounds.size.w / 2) - 28, bounds.size.w / 2);
  //const int16_t hour_hand_length = PBL_IF_ROUND_ELSE((bounds.size.w / 2) - 60, bounds.size.w / 2);
  
  calculate_pointer_end((t->tm_sec*100)+ms/10, second_hand_length, &center, &secondHand, 60*100, 0);    
  //calculate_pointer_end(t->tm_hour*60+t->tm_min, hour_hand_length, &center, &hourHand, scale, 0);    
  //calculate_pointer_end(t->tm_min, minute_hand_length, &center, &minuteHand, 60, 0);    
  
  // draw seconds hand. This always goes one round per 60 secs
 
// minute/hour hand
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * t->tm_min / 60);
  gpath_draw_filled(ctx, s_minute_arrow);
  gpath_draw_outline(ctx, s_minute_arrow);
  //graphics_draw_line(ctx, minuteHand, center);
    
  graphics_context_set_stroke_color(ctx, GColorWhite);
  //gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (t->tm_hour*60+t->tm_min ))/ scale);
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);
  //graphics_draw_line(ctx, hourHand, center);
  
  /**graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_line(ctx, secondHand, center);
*/

  // draw hour lines
  int hours = clock_is_24h_style() ? 24 : 12;
  for (int h = 0; h < hours; h++) {
    calculate_pointer_end(h, hour_tick_length, &center, &hourHand1, hours, 0);
    calculate_pointer_end(h, hour_tick_length-7, &center, &hourHand2, hours, 0);
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_line(ctx, hourHand1, hourHand2);
  }

  // draw second ticks 
  for (int i = 0; i < t->tm_sec; i++) {
    calculate_pointer_end(i, second_hand_length, &center, &secondHand, 60, 0);
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, GRect(secondHand.x, secondHand.y, 3, 3), 0, GCornerNone);
  }
  
  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(bounds.size.w / 2 - 1, bounds.size.h / 2 - 1, 3, 3), 0, GCornerNone);
}



static void tick_handler(struct tm *tick_time, TimeUnits units_changed) { 
  
  static char buff[] = "00:00:00:";
  strftime(buff, sizeof(buff), "%I:%M:%S", tick_time);

  text_layer_set_text(s_time_layer, buff);
  
  // rotate path  
  layer_mark_dirty(window_get_root_layer(s_main_window));
}

static void app_timer_handler(void *data) {
	time_t tt = time(NULL);

	struct tm *t = localtime(&tt);

	APP_LOG(APP_LOG_LEVEL_INFO, "Timer expired");

	tick_handler(t, SECOND_UNIT);

	//app_timer_register(TIMER_INTERVALL,app_timer_handler, data);

}


/* Window handler */
static void main_window_load(Window *window) {
   // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);

  // init hand paths
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);


  // background all black
  s_bg_layer = layer_create(bounds);
  layer_set_update_proc(s_bg_layer, bg_update_proc);  
  layer_add_child(window_layer, s_bg_layer);
  
  s_draw_layer = layer_create(bounds);
  layer_set_update_proc(s_draw_layer, hands_update_proc);  
  
  s_time_layer = text_layer_create(PBL_IF_ROUND_ELSE(
    GRect(63, center.y / 2 - 15, 56, 20),
    GRect(46, center.y / 2 - 15, 56, 20)));  
  
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  

  
  layer_add_child(window_layer, s_draw_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  // register timetick handler
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  //s_app_timer = app_timer_register(TIMER_INTERVALL,app_timer_handler,window);

  Layer *batteryLayer = init_battery_watcher(GRect(PBL_IF_ROUND_ELSE(30,1), PBL_IF_ROUND_ELSE(19,5), 35, 20));
  if (batteryLayer != NULL) {
    layer_add_child(window_layer, batteryLayer);
  }

  init_accel_tap_handler();

  Layer *compassLayer = init_compass_service(GRect(bounds.size.w / 2 , bounds.size.h / 2 + 30, 90, 90));
  if (compassLayer != NULL) {
    layer_add_child(window_layer, compassLayer);  
  }

  Layer *btLayer = init_bluetooth_layer(GRect(PBL_IF_ROUND_ELSE(144-50,144-30), PBL_IF_ROUND_ELSE(19+20,25), 20, 20));
  if (btLayer != NULL) {
    layer_add_child(window_layer, btLayer);  
  }


  
  
}

static void main_window_unload(Window *window) {
  layer_destroy(s_draw_layer);  
  layer_destroy(s_bg_layer);
  text_layer_destroy(s_time_layer);
  deinit_battery_watcher();
  deinit_accel_tap_handler();
  deinit_compass_service();
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);
}


static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // now push on stack to display window
  window_stack_push(s_main_window, true); 
}

static void deinit() {
  window_destroy(s_main_window);
}



int main(void) {
  init();
  
  app_event_loop();
  
  deinit();
}