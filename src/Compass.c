#include "Compass.h"


Layer *init_compass_service(GRect bounds) {
	
	GPoint center = grect_center_point(&bounds);

	// Initialize and define the two paths used to draw the needle to north and to south
  	s_needle_north = gpath_create(&NEEDLE_NORTH_POINTS);
  	s_needle_south = gpath_create(&NEEDLE_SOUTH_POINTS);

  	// Move the needles to the center of the layer
  	gpath_move_to(s_needle_north, center);
  	gpath_move_to(s_needle_south, center);

	   
    compass_service_set_heading_filter(50);
  	compass_service_subscribe(compass_callback);
  	

  	s_path_layer = layer_create(bounds);
  	layer_set_update_proc(s_path_layer, compass_update_proc);  

	return s_path_layer;
}

void deinit_compass_service() {
	layer_destroy(s_path_layer);
	gpath_destroy(s_needle_south);
	gpath_destroy(s_needle_north);
}

void compass_callback(CompassHeadingData heading_data) {
 	// rotate needle accordingly
 	gpath_rotate_to(s_needle_north, heading_data.magnetic_heading);
  gpath_rotate_to(s_needle_south, heading_data.magnetic_heading);
}


void compass_update_proc(Layer *layer, GContext *ctx) {
	  graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
  	gpath_draw_filled(ctx, s_needle_north);

	  graphics_context_set_fill_color(ctx, GColorWhite);
  	gpath_draw_outline(ctx, s_needle_south);

  	// creating centerpoint
  	GRect bounds = layer_get_frame(s_path_layer);
  	GPoint path_center = grect_center_point(&bounds);
    graphics_context_set_fill_color(ctx, GColorBlack);
  	graphics_fill_circle(ctx, path_center, 3);

  	// then put a white circle on top
  	graphics_context_set_fill_color(ctx, GColorWhite);
  	graphics_fill_circle(ctx, path_center, 2);
}