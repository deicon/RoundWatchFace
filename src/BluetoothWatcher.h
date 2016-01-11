

#pragma once
#include <pebble.h>

/**
Simple Layer that displays Bluetooh Icon if connected
*/

static BitmapLayer *s_bluetooth_layer;
static GBitmap *s_bt_bitmap; 

Layer* init_bluetooth_layer(GRect bounds);

void deinit_bluetooth_layer();
