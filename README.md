# Final Project: Self-Navigating RC Car
Repository containing code, sketches and diagrams of my final graduating project from high school.

The project was to take a regular RC Car and modify it so that it could navigate a course on its own, without the user needing to steer the car themselves.
This was achieved by using IRToF Sensors on the front bumper of the car, on each side, angled approximately 45° outwards. This would give a good indication
of how far each side of the car was from the boundaries of the course, allowing it to correct its own course and try to stay as centered as possible.

## Hardware
* Any PWM-Controlled RC Car will do (Which is most of them.) This inherently includes:
    * 2.4GHz Remote
    * 2.4GHz Receiver
    * Electronic Speed Controller
    * DC Motor (For Throttle)
    * Servomotor (For Steering)
* [ESP32-DevKitC](https://www.espressif.com/en/products/devkits/esp32-devkitc/overview)
* 2x [Adafruit VL53L0X](https://www.adafruit.com/product/3317)

## Schematics
Principle of how the car should function.

<img src="https://raw.githubusercontent.com/Tjorven-V/Final-Project-Self-Navigating-RC-Car/main/Schematics/Principle.png" alt="drive" width="256"/>

(Outdated) Schematic showing connections between hardware.

<img src="https://raw.githubusercontent.com/Tjorven-V/Final-Project-Self-Navigating-RC-Car/main/Schematics/Schematic.png" alt="connect" width="256"/>

Schematic of how hardware interacts with one another. Arrows indicate direction of data (Read/Write.) 

<img src="https://raw.githubusercontent.com/Tjorven-V/Final-Project-Self-Navigating-RC-Car/main/Schematics/Diagram.png" alt="schematic" width="256"/>



## Demonstration
Below is a video of an early prototype of the car in action (on a hastily put together course.)

[![Early Prototype of RC Car](https://img.youtube.com/vi/tfrhoDUVheA/mqdefault.jpg)](https://www.youtube.com/shorts/tfrhoDUVheA)
