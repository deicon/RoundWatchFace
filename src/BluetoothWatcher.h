#pragma once
#include <pebble.h>

/**
Simple Layer that displays Bluetooh Icon if connected
*/

static BitmapLayer *s_bluetooth_layer;
static GBitmap *s_bt_bitmap; 
static GBitmap *s_plane_bitmap; 

Layer* init_bluetooth_layer(GRect bounds);

void deinit_bluetooth_layer();

void handle_connection_state(bool connected);