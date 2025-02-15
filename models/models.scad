include <../../ktane-3d-models/variables.scad>;
use <../../ktane-3d-models/bomb_base.scad>;

$fn = $preview ? 50 : 100;

tolerance = 0.3;

button_width = 9;
button_length = 16;
button_distance_x = 30;
button_distance_y = 18.7;

knob_radius = 3.6;
knob_distance_x = size - button_distance_x;
knob_distance_y = button_distance_y;

display_length = 50.5;
display_width = 19.2;
display_position_x = 19.75 + display_length / 2;
display_position_y = 35.4 + display_width / 2;

led_radius = 2.5;
led_distance_x = 19;
led_distance_y = size - 17;

module top() {
    difference() {
        bomb_module_top(height_above_pcb = 12.5, module_type = 1);
        translate([button_distance_x, button_distance_y]) cube([button_length + tolerance, button_width + tolerance, 20], center = true);
        translate([knob_distance_x, knob_distance_y]) cylinder(h = 20, r1 = knob_radius + tolerance / 2, r2 = knob_radius + tolerance / 2, center = true);
        translate([display_position_x, display_position_y]) cube([display_length + tolerance, display_width + tolerance, 20], center = true);
        translate([led_distance_x, led_distance_y]) cylinder(h = 20, r1 = led_radius + tolerance / 2, r2 = led_radius + tolerance / 2, center = true);
    }
}

module bottom() {
    bomb_module_bottom(height_above_pcb = 12.5);
}

top();
bottom();
