#pragma once
#include <pebble.h>

void accel_tap_handler(AccelAxisType axis, int32_t direction);

void init_accel_tap_handler();

void deinit_accel_tap_handler();