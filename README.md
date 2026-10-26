
# DC Motor Control with ESP32-S3, L298N and Encoder

**Academic Electronics and Embedded Systems Project | Universidad Europea de Madrid | 2026**

## Project Overview

Development and physical testing of a DC motor control system based on an ESP32-S3 microcontroller, an L298N H-bridge motor driver and a DC geared motor with an incremental encoder.

The system allows the user to control motor rotation direction and PWM duty cycle through commands sent from a computer.

Encoder signals are acquired to monitor motor movement.

The project combines embedded programming, power electronics, digital inputs and outputs, and motor control fundamentals.

## Hardware Components

| Component | Function |
|---|---|
| ESP32-S3 | Main microcontroller |
| L298N | H-bridge motor driver |
| DC geared motor | Electromechanical actuator |
| Incremental encoder | Motor movement measurement |
| 12 V power supply | Motor power supply |
| Computer | Serial command interface |

## System Architecture

The system consists of three main blocks:

### 1. ESP32-S3 — Control Unit

- Receives commands through the serial interface
- Generates PWM signals
- Sets the direction of rotation using digital outputs
- Reads incremental encoder signals
- Processes motor movement information

### 2. L298N — Power Stage

The L298N acts as an interface between the ESP32-S3 and the DC motor.

- IN1 and IN2 determine motor rotation direction
- ENA receives the PWM control signal
- The H-bridge provides bidirectional motor actuation

### 3. DC Motor with Encoder

The DC motor converts electrical power into mechanical rotation.

The encoder generates pulses that can be used to monitor rotational movement and estimate position or speed when appropriately processed.

## Motor Control

The motor is controlled using pulse-width modulation (PWM).

The command value determines the requested PWM duty cycle, while digital signals determine the direction of rotation.

### Serial Commands

| Command | Description |
|---|---|
| F 200 | Forward rotation with PWM value 200 |
| B 150 | Reverse rotation with PWM value 150 |
| STOP | Stop motor |

PWM commands use values from 0 to 255.

These values represent PWM command levels, not calibrated rotational speeds in RPM.

## Embedded Programming

The implementation uses:

- Microcontroller programming
- PWM generation
- Digital output control
- Serial communication
- Encoder signal acquisition
- Motor start, stop and direction logic

Selected source code will be added when the original program has been reviewed.

## Physical Validation

The prototype was assembled and tested using real hardware.

**Functions demonstrated during laboratory testing:**

- Motor rotation
- Forward and reverse operation
- Speed variation through PWM commands
- Encoder signal reading
- Communication between the computer and microcontroller

These tests demonstrated the operation of the main hardware and software components.

## Project Gallery

Photographs of the physical setup, wiring and control electronics will be added.

## Limitations and Future Development

- Academic laboratory prototype
- No industrial reliability validation
- No quantified speed-control accuracy or response-time results
- Closed-loop speed regulation has not been documented as implemented
- Future work may include encoder-based speed estimation, feedback control and PID regulation

## Academic Context

**Course:** Digital Electronics and Microprocessors

**University:** Universidad Europea de Madrid

**Academic year:** 2026

This project demonstrates embedded system development, hardware-software integration and physical motor-control testing.
