#pragma once

#include <pebble.h>

#define DIALOG_MESSAGE_WINDOW_MESSAGE  "Bluetooth Connection Lost!"
#define DIALOG_MESSAGE_WINDOW_MARGIN   10

void dialog_message_window_push();

void dialog_message_window_pop();

