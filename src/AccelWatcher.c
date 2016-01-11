#include "AccelWatcher.h"

static SimpleTapHandler s_tapHandler;

void init_accel_tap_handler(SimpleTapHandler tapHandler) {
	s_tapHandler = tapHandler;
	accel_tap_service_subscribe(accel_tap_handler);
}

void deinit_accel_tap_handler() {
	accel_tap_service_unsubscribe();
	s_tapHandler = NULL;
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Tapped");
	s_tapHandler();
}