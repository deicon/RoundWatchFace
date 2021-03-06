#include "BluetoothWatcher.h"
#include "DialogMessageWindow.h"

Layer* init_bluetooth_layer(GRect bounds) {
	s_bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_APLITE);
	s_plane_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PLANE_APLITE);
	s_bluetooth_layer = bitmap_layer_create(bounds);
	handle_connection_state(connection_service_peek_pebble_app_connection());

	connection_service_subscribe((ConnectionHandlers) {
		.pebble_app_connection_handler = handle_connection_state
	});

	return bitmap_layer_get_layer(s_bluetooth_layer);
}

void deinit_bluetooth_layer() {
	bitmap_layer_destroy(s_bluetooth_layer);
	gbitmap_destroy(s_bt_bitmap);
	gbitmap_destroy(s_plane_bitmap);
	connection_service_unsubscribe();
}

void handle_connection_state(bool connected) {
	set_bluetooth_logo(connected);
	if (connected) {
		vibes_double_pulse();
		dialog_message_window_pop();
	} else {
		vibes_long_pulse();
		dialog_message_window_push();
		// if no wrist shake registered within 5 sec. remove window
		app_timer_register(5000, app_timer_callback, NULL);
	}
}

void set_bluetooth_logo(bool connected) {
	if (connected) {
		bitmap_layer_set_bitmap(s_bluetooth_layer, s_bt_bitmap);
	} else {
		bitmap_layer_set_bitmap(s_bluetooth_layer, s_plane_bitmap);
	}
}

void app_timer_callback(void *data) {
	dialog_message_window_pop();
}