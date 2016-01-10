#pragma once
#include <pebble.h>

static uint8_t s_charge_percent;

static TextLayer *s_battery_layer;
static char s_battery_buffer[10];

void handle_battery_state(BatteryChargeState charge);

Layer *init_battery_watcher(GRect bounds);

void deinit_battery_watcher();
