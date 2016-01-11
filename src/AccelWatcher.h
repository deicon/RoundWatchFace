#pragma once
#include <pebble.h>

typedef void (* SimpleTapHandler)(void);

void accel_tap_handler(AccelAxisType axis, int32_t direction);

void init_accel_tap_handler(SimpleTapHandler tapHandler);

void deinit_accel_tap_handler();