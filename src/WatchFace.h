#pragma once
#include <pebble.h>

static int32_t _24HScaleFactor = 12;  // should be 24 for 24 H round watch face
static int32_t _12HScaleFactor = 12; 

static Window *s_main_window;
static Layer *s_draw_layer;
static TextLayer *s_time_layer;
static Layer *s_bg_layer;

static GPath *s_minute_arrow, *s_hour_arrow;

static const GPathInfo MINUTE_HAND_POINTS = {
  3, (GPoint []) {
    { -5, 20 },
    { 5, 20 },
    { 0, -80 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  3, (GPoint []){
    {-6, 20},
    {6, 20},
    {0, -40}
  }
};
