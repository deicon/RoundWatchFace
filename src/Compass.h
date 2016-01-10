#pragma once
#include <pebble.h>

// Vector paths for the compass needles
static const GPathInfo NEEDLE_NORTH_POINTS = {
  3,
  (GPoint[]) { { -8, 0 }, { 8, 0 }, { 0, 14 } }
};

static const GPathInfo NEEDLE_SOUTH_POINTS = {
  3,
  (GPoint[]) { { 8, 0 }, { 0, 14 }, { -8, 0 } }
};

static Layer *s_path_layer;
static GPath *s_needle_north, *s_needle_south;


Layer *init_compass_service(GRect bounds);

void deinit_compass_service();

void compass_callback(CompassHeadingData heading);

void compass_update_proc(Layer *layer, GContext *ctx);