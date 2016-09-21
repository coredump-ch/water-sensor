$fn = 20;

module bottom() {
  linear_extrude(0.8)
    square([17.78, 26.67]);
}

module slope() {
  translate([10, 0.66, 3.6])
    rotate([-45, 0, 0])
      cube([20, 1, 10], center=true);
  translate([10, 26.67-1.42, 3.6])
    rotate([45, 0, 0])
      cube([20, 1, 10], center=true);
  translate([1.42, 15, 3.6])
    rotate([45, 0, 90])
      cube([30, 1, 10], center=true);
  translate([17.78-1.42, 15, 3.6])
    rotate([-45, 0, 90])
      cube([30, 1, 10], center=true);
}
module top() {
  difference() {
    translate([1.42, 0.66, 0.8])
      linear_extrude(3.34-0.8)
      square([17.78-1.42*2, 26.67-0.66-1.42]);
    slope();
  }
}

module base() {
  color([8/255, 161/255, 104/255, 1])
    bottom();
  color([0.89, 0.89, 0.89, 1.0])
    top();
  color("black")
    translate([4, 17.8, 3.34])
    scale(0.2)
    rotate([0, 0, -90])
    linear_extrude(0.1)
    text("RN2483");
}

module cutout_row() {
  for (i=[0:19]) {
    translate([0, i*1.27])
      circle(d=0.8);
  }
}

module pad_row() {
  for (i=[0:19]) {
    translate([0, i*1.27])
       square([1.524/2, 1.016], center=true);
  }
}

module cutout() {
  linear_extrude(0.8) {
    translate([0, 1.27])
      cutout_row();
    translate([17.78, 1.27])
      cutout_row();
  }
}

module pads() {
  linear_extrude(0.8) {
    translate([1.524/4, 1.27])
      pad_row();
    translate([17.78-1.524/4, 1.27])
      pad_row();
  }
}

difference() {
  union() {
    base();
    pads();
  }
  cutout();
}