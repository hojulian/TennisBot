# Robot Construction

![TennisBot v1 Front](../images/front_view.jpg)

Since the conception of the TennisBot v1 in 3D, we have been physically building the robot.

*As of December 8th, 2020, all hardware constructions are completed, the remaining work is software tuning.*

## Software Requirements

*For running shooter_controller on ESP32*

* Ubuntu 20.04
* [esptool](https://github.com/espressif/esptool) (for flashing ESP32)
* [odrivetool](https://github.com/madcowswe/ODrive) (for actuator tuning)
* [BLHeliSuite32](https://github.com/bitdump/BLHeli) (for motor tuning)

*For interfacing the ESP32 with ROS on RPi4*

* ROS Melodic
* [rosserial](http://wiki.ros.org/rosserial)

## Hardware Requirements

All material needed to build TennisBot v1 is included in the [hardware](hardware/BOM.xlsx) folder.

Most the components in the list are off-the-shelf items that can be easily acquired. The only custom parts are the 3D printed components, 4040 metal beams, and the carbon fiber plates.

Basic soldering skill is also required to build this robot.
