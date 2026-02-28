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

led_cover_size = 10;
led_cover_radius = 5;
led_cover_hole_length = 10;
led_cover_hole_width = 8;
led_height = 3.6;
led_cover_hole_tolerance = 0.1;

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

module led_detail_border() {
    difference() {
        hull() {
            translate([-led_cover_size / 2, 0]) sphere(led_cover_radius);
            translate([led_cover_size / 2, 0]) sphere(led_cover_radius);
        }
        cube([led_cover_hole_length, led_cover_hole_width, 10], center = true);
        translate([0, 0, -50]) cube([100, 100, 100], center = true);
    }
}

module led_detail_inside() {
    difference() {
        intersection() {
            hull() {
                translate([-led_cover_size / 2, 0]) sphere(led_cover_radius);
                translate([led_cover_size / 2, 0]) sphere(led_cover_radius);
            }
            cube([led_cover_hole_length, led_cover_hole_width, 10], center = true);
        }
        translate([0, 0, -50]) cube([100, 100, 100], center = true);
        cylinder(led_height, led_radius + led_cover_hole_tolerance, led_radius + led_cover_hole_tolerance);
    }
}

module pot_cover() {
    corner_radius = 1;
    cylinder_count = 8;
    difference() {
        translate([0, 0, corner_radius]) minkowski() {
            cylinder(10 - 2 * corner_radius, 15 / 2 - corner_radius / 2, 15 / 2 - corner_radius / 2);
            sphere(corner_radius);
        }
        cylinder(9, 5.9 / 2, 5.9 / 2);
        cylinder(5, 4, 4);
        for (i = [0:cylinder_count - 1]) {
            rotate([0, 0, i * 360 / cylinder_count]) translate([15 / 2 + 1, 0]) cylinder(100, 2, 2);
        }
    }
}

top();
bottom();
rectangular_button(2.5, 8.4, button_length - tolerance, button_width - tolerance, 1, 1, "TX", 6, 1);
led_detail_border();
led_detail_inside();
pot_cover();
