#include "BatteryWatcher.h"

Layer *init_battery_watcher(GRect bounds) {
	
	s_battery_layer = text_layer_create(bounds);

	text_layer_set_background_color(s_battery_layer, GColorBlack);
  	text_layer_set_text_color(s_battery_layer, GColorWhite);
  	text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  	text_layer_set_font(s_battery_layer,fonts_get_system_font(FONT_KEY_GOTHIC_14));
	handle_battery_state(battery_state_service_peek());
	battery_state_service_subscribe(handle_battery_state);

	return text_layer_get_layer(s_battery_layer);
}

void deinit_battery_watcher() {
	battery_state_service_unsubscribe();
	text_layer_destroy(s_battery_layer);
}

void handle_battery_state(BatteryChargeState charge) {
	s_charge_percent = charge.charge_percent;
	snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d %%", s_charge_percent); 
	APP_LOG(APP_LOG_LEVEL_INFO, s_battery_buffer);
	text_layer_set_text(s_battery_layer, s_battery_buffer);
}

