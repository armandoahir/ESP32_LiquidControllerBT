# ESP32_LiquidControllerBT

An Arduino program written and developed for an ESP32 VROOM 32 bit by Espressif

## Project of Digital Electronic Systems, Unibs

**ESP32 Bluetooth Liquid Controller**

**Created by: Ahir Armando**

### Introduction

I was inspired to create this project after a trip I took in the distant summer of 2012 to visit my grandparents in India. Every house in this region of India had water tanks that were filled twice a day by a hydraulic system managed by the guaranteeing company. This meant that if water was wasted unnecessarily, there was a risk of running out, or having to use pumps (electric or manual) to draw water from the groundwater, waiting for the next refill.

During my stay in India, I happened to completely empty the tank to clean it. At that moment, I began to think about a system that could be useful for monitoring the water level and alerting when the tank was running out of water. I wanted to create this water level monitoring module, controlled via Bluetooth, as a challenge to improve my microcontroller programming skills. To develop this project, I used the ESP32 microcontroller interfacing it with the Arduino development environment.

ESP32 is a 32-bit microcontroller made by the Chinese company "Espressif Systems." It features a dual-core processor at 240 MHz, integrated flash memory, Wi-Fi connection, Bluetooth, Bluetooth Low Energy, and a wide range of input/output peripherals. It was developed as a successor to the popular ESP8266 microcontroller, with increased performance and functionality. This microcontroller is widely used in the area of the Internet of Things (IoT) to create home projects, monitoring sensors, and much more. The flexibility of the ESP32, combined with its low power consumption, makes it suitable for a wide range of applications, from controlling low-power devices to processing complex signals and interfacing with cloud services.

### Circuit Implementation and Component List

- ESP32 microcontroller SoC (System on Chip)
- HC-SR04 depth sensor
- Buzzer (YXDZ)

### Work Tasks

- Drilling and threading
- Soldering on PCB
- Edge chamfering

### HC-SR04 Sensor

The HC-SR04 sensor is a distance measuring device that uses ultrasound. To understand how it works, it's helpful to know that ultrasound are high-frequency sound waves that travel through the air at a speed of about 340 meters per second. The HC-SR04 sensor works by sending an ultrasound pulse, which is emitted by the sensor's transmitter. This pulse travels through the air until it encounters an object, which reflects it. The reflected pulse is then detected by the sensor's receiver.

The time taken for the pulse to travel from the emitter to the object and back to the receiver is then measured by the HC-SR04 sensor. This flight time is then used to calculate the distance between the object and the sensor using the formula:

\[ \text{distance} = \left(\text{flight time} \times \text{speed of sound}\right) / 2 \]

where the speed of sound in air is generally considered to be about 340 m/s. To make the measurement more accurate, the HC-SR04 sensor usually sends several ultrasound pulses and averages the flight times. The sensor also has an integrated circuit that manages the emission and reception of ultrasound pulses and converts the flight time into a distance measurement.

### Buzzer

In general, a buzzer is an electronic device that produces a sound when activated. There are different types of buzzers, including piezoelectric and electromagnetic buzzers.

A piezoelectric buzzer consists of a plate of piezoelectric material that vibrates when an electric current is applied to it. This vibration produces a sound at a frequency determined by the design of the buzzer. Piezoelectric buzzers are widely used in acoustic signaling applications, such as alarm systems, toys, and sound alert devices.

An electromagnetic buzzer, on the other hand, uses a metal armature that is attracted to a magnet when an electric current is applied to the magnet coil. This movement produces a sound at a specific frequency.

### Source Code

Below is the code written to program the AZ-Delivery ESP32 board, based on the ESP32-WROOM module. The code is compacted using patterns to outline independent components, particularly:

- `void loop()` and `void setup()` which are mandatory functions for creating an Arduino program. `setup()` is a function used to configure declared variables; for example, if I have a LED on pin 13, I declare `int led = 13` and in `setup()`, configure pin 13 as an output to see the signal on the LED. `void loop()` is a function that, as the name suggests, will continuously loop the code written in it. In this project, `loop()` provides a connection between an Android device and the ESP32's Bluetooth module (using the `void bluetooth()` function). For communication on Android, I installed the "Serial Bluetooth Terminal" application. When the Android device is paired, the loop will wait for a specific input from the device, which is the command named "control_ON"; the command starts a "do-while" loop present in the `sensor()` function.
- `void sensor()`, a function that manages the HC-SR04 sensor, cyclically calculating the distance between the sensor and the object detected by the ultrasound. The cycle stops if the sensor reads a distance not between `THRESHOLD_MAX` and `THRESHOLD_MIN`.
- `void buzzAlarm()`, the function that manages the alarm, activating the buzzer.

### Timer and ISR (Interrupt Service Routine) Deep Dive

In creating this project, a critical part was exiting a loop function. I considered using an Arduino construct for timers to implement the microcontroller's hardware interrupt function.

Arduino timers are hardware components that can be used to count time or generate clock signals. Arduino microcontrollers have at least one built-in timer, while some models have more. Arduino timers can be configured to generate an interrupt at regular intervals, i.e., each time the timer reaches a specific value. This way, you can create functions that are executed at specific time intervals. For example, you can use a timer to turn a LED on and off at a specific frequency or sample an analog signal at certain time intervals.

To use Arduino timers, you can use the "millis()" function, which returns the number of milliseconds since the microcontroller was started. This value can be used to set an internal timer and manage timed events. The "delay()" function is another way to manage time in Arduino, but it blocks code execution for a certain period, whereas the millis() function allows other activities to be performed in the meantime.

The `attachInterrupt()` command is an Arduino function that activates a hardware interrupt interface, allowing the main program to be interrupted immediately and a user-defined function to be executed when a specific event occurs.

The hardware interrupt interface can be used, for example, to respond to a signal from a sensor or external input, such as a button or a signal from another microcontroller. When the specific event occurs (e.g., when the button is pressed), the hardware interrupt interface generates an interrupt signal that immediately disrupts the main program flow and starts executing the user-defined function.

The `attachInterrupt()` function has three input parameters: the first is the number of the interrupt pin you want to use, the second is the function you want to execute when the interrupt occurs, and the third indicates the type of interrupt you want to use (e.g., if you want to activate the interrupt on a rising or falling edge of the signal).

The Interrupt Service Routine (ISR) is an interrupt management function executed immediately after the arrival of an interrupt signal generated by a hardware device, such as a sensor, button, network card, or timer. The ISR is used to momentarily interrupt the main program flow and handle the event that generated the interrupt.

The ISR must be written efficiently and quickly, as the microcontroller must return to the main program execution as soon as possible. The ISR must also be written with special attention to safety, as executing code within the ISR can cause problems if not managed correctly.

The "mode" parameter of the `attachInterrupt()` function in Arduino specifies the type of input signal transition on the interrupt pin that should activate the ISR (Interrupt Service Routine), i.e., the function that is executed when the interrupt occurs. 

There are three possible modes: CHANGE, FALLING, and RISING.

- `CHANGE`: activates the ISR when the input signal on the interrupt pin changes state, i.e., from HIGH to LOW or vice versa.
- `FALLING`: activates the ISR when the input signal on the interrupt pin goes from HIGH to LOW.
- `RISING`: activates the ISR when the input signal on the interrupt pin goes from LOW to HIGH.

For example, if you use the RISING parameter, the ISR will execute when the input signal on the interrupt pin goes from LOW to HIGH. This can be useful, for example, for managing a button: when the button is pressed, the signal on the interrupt pin goes from LOW to HIGH, and the ISR executes. On the other hand, using FALLING, the ISR would execute when the button is released, i.e., when the signal goes from HIGH to LOW.
