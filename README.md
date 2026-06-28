# Smart Helmet Safety System Using Arduino & RF

An embedded systems project designed to improve rider safety through real-time physiological and environmental monitoring. The system utilizes two Arduino Uno microcontrollers communicating via a 433 MHz RF link to prevent drunk driving and mitigate collision risks.

---

## Problem Statement & Solution

Traditional helmets offer passive physical protection but fail to actively prevent accidents. Two major causes of road fatalities are driving under the influence (DUI) and delayed reactions to obstacles. 

This system addresses these challenges by:
1. Preventing vehicle ignition/operation if alcohol is detected in the rider's breath.
2. Providing real-time proximity alerts and automatic motor cutoff when obstacles are too close.

---

## Key Features

* **Wireless Real-Time Monitoring:** Dual-unit architecture (Helmet & Vehicle) connected via RF.
* **Intoxication Prevention:** Automated ignition/motor lockout upon alcohol detection.
* **Collision Avoidance:** Active IR obstacle detection with automated motor braking.
* **Dual Alert System:** Visual diagnostics via an I2C LCD and audible alerts via a piezo buzzer.
* **Modular Design:** Low-cost, scalable framework compatible with IoT upgrades (GPS/GSM).

---

## System Architecture & Hardware

The system is divided into two functional nodes interacting over a 433 MHz ASK (Amplitude Shift Keying) wireless link.

### 1. Transmitter Unit (Helmet Side)
Monitors the rider and environment, encoding sensor states into compact data packets.
* **Microcontroller:** Arduino Uno
* **Sensors:** MQ-Series Alcohol Sensor, IR Obstacle Sensor
* **Communication:** 433 MHz RF Transmitter Module

### 2. Receiver Unit (Vehicle Side)
Processes incoming RF packets to manage vehicle mechanics and user feedback.
* **Microcontroller:** Arduino Uno
* **Communication:** 433 MHz RF Receiver Module
* **Actuators & Displays:** L298N Motor Driver, DC Motor (simulating the engine), Piezo Buzzer, 16x2 I2C LCD

---

## Communication Protocol

Data packets are transmitted efficiently via the RadioHead library as a comma-separated string:  
`[AlcoholStatus],[ObstacleStatus]`

| String State | Alcohol Detected? | Obstacle Detected? | System Action |
| :--- | :---: | :---: | :--- |
| `0,0` | No | No | **Safe Condition:** Motor RUNS, Buzzer OFF |
| `1,0` | Yes | No | **DUI Warning:** Motor RUNS, Buzzer ON |
| `0,1` | No | Yes | **Proximity Alert:** Motor STOPS, Buzzer OFF |
| `1,1` | Yes | Yes | **Critical Alert:** Motor STOPS, Buzzer ON |

---

## Logic & Working Principle

[ Sensors: MQ-Alcohol / IR Obstacle ]
                            |
                    (Transmitter Uno)
                            |
                 ((( 433 MHz RF Link )))
                            |
                     (Receiver Uno)
                            |
     +----------------------+----------------------+
     |                      |                      |
[16x2 LCD]            [L298N Driver]        [Piezo Buzzer]

* **Normal State:** LCD displays `SYSTEM SAFE / MOTOR ON`. The vehicle operates normally.
* **Alcohol Event:** Exceeding the predefined threshold triggers an audible alarm. LCD displays `ALCOHOL DETECTED / WARNING`.
* **Obstacle Event:** Detection of a close-range object cuts power to the motor driver. LCD displays `OBSTACLE DETECTED / MOTOR OFF`.

---

## Software & Libraries

The system firmware is developed in C++ via the Arduino IDE. 

### Dependencies:
* `RadioHead` - Manages packetization and ASK modulation over RF.
* `LiquidCrystal_I2C` - Drives the display via the I2C protocol to reduce pin count.

---

## Future Road Map

* **Geofencing & Tracking:** Integrate Neo-6M GPS and SIM800L GSM modules for automated SOS text alerts with location coordinates during a crash.
* **IMU Fall Detection:** Add an MPU6050 accelerometer/gyroscope to detect accidents and impact forces.
* **Smart Power Management:** Implement solar harvesting on the helmet shell to power the transmitter node.
* **Cloud Analytics:** Connect an ESP32 or Wi-Fi SoC for real-time fleet tracking via IoT platforms.
