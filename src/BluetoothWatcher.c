#include "BluetoothWatcher.h"

Layer* init_bluetooth_layer(GRect bounds) {
	s_bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_APLITE);
	s_bluetooth_layer = bitmap_layer_create(bounds);
	bitmap_layer_set_bitmap(s_bluetooth_layer, s_bt_bitmap);

	connection_service_subscribe((ConnectionHandlers) {
		.pebble_app_connection_handler = handle_connection_state
	});

	return bitmap_layer_get_layer(s_bluetooth_layer);
}

void deinit_bluetooth_layer() {
	bitmap_layer_destroy(s_bluetooth_layer);
	gbitmap_destroy(s_bt_bitmap);
	connection_service_unsubscribe();
}

void handle_connection_state(bool connected) {
	if (connected) {
		bitmap_layer_set_bitmap(s_bluetooth_layer, s_bt_bitmap);
		vibes_double_pulse();
	} else {
		bitmap_layer_set_bitmap(s_bluetooth_layer, NULL);
		vibes_long_pulse();
	}
}