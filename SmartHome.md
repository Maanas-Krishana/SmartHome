# Smart Home Lighting Control System 🏠💡

A low-cost, IoT-based home automation system designed to provide global remote control and automated scheduling of household appliances using the ESP32 microcontroller and the Blynk IoT platform.

## 📖 Project Overview

This project addresses the growing need for energy efficiency and convenience in modern homes. By leveraging Internet of Things (IoT) technology, this system allows users to monitor and control high-voltage AC appliances (like lights and fans) from anywhere in the world.

The core of the system is the ESP32, which connects to the local Wi-Fi and communicates with the Blynk Cloud. A key design priority was **reliability and safety**, implemented through an isolated power supply for the relay coils to prevent system resets during high-load switching.

## 📌 Features

* **Global Remote Control:** Toggle appliances in real-time via the Blynk mobile dashboard or web console.
* **Dual Channel Operation:** Independent control for two 220V AC appliances using a dual-relay module.
* **Intelligent Scheduling:** Integrated RTC (Real-Time Clock) sync via Blynk allows users to set daily "On/Off" timers directly from their smartphones.
* **Power Consumption Simulation:** Provides a real-time visualization of estimated energy usage.
* **Industrial-Grade Stability:** Features a split-power architecture that isolates the logic circuit (ESP32) from the mechanical switching circuit (Relays) to eliminate electrical noise and brownouts.

## 🛠️ Hardware Components

### Core Electronics
* **Microcontroller:** ESP-32S Dev Module (Dual-core, built-in Wi-Fi/Bluetooth).
* **Switching:** 2-Channel 5V Relay Module (Opto-isolated, Active-Low trigger).
* **Sensor Interface:** PZEM-004T v3.0 (Ready for hardware energy monitoring).

### Power Management
* **Logic Power:** Micro USB (5V) or 3.3V regulated input.
* **Coil Power:** Separate 5V DC Adapter (connected to `JD-VCC`) to ensure the ESP32 does not crash when relays click.

## 🔌 Pin Mapping

| **Component** | **ESP32 Pin** | **Blynk Virtual Pin** | 
| :--- | :--- | :--- |
| Relay 1 (Light) | GPIO 26 | V1 | 
| Relay 2 (Socket) | GPIO 27 | V2 | 
| Power Display | N/A | V3 | 
| Timer Settings | N/A | V5, V6, V7 | 

## 📱 Software & Platform

* **Firmware:** Developed in C++ using the Arduino Framework.
* **IoT Platform:** Blynk IoT 2.0.
* **Key Libraries:** * `BlynkSimpleEsp32.h`: For cloud connectivity.
  * `WiFi.h`: For network management.
  * `time.h`: For handling scheduling logic without external RTC modules.

## 🚀 Getting Started

### 1. Hardware Assembly
1. Connect the ESP32 GPIOs to the Relay IN1 and IN2 pins.
2. **Crucial Safety Step:** Remove the `VCC-JDVCC` jumper on the relay module.
3. Connect your ESP32 `GND` to the Relay `GND`.
4. Connect a separate 5V power source to the Relay `JD-VCC` and `GND`.

### 2. Blynk Cloud Configuration
1. Create a new Template in the Blynk Web Dashboard.
2. Configure Datastreams as per the Pin Mapping table above.
3. Set up the Mobile App dashboard with buttons (Switch mode) and a Time Input widget.

### 3. Firmware Deployment
1. Open `Blynk_Project_Final.ino` in the Arduino IDE.
2. Ensure you have the ESP32 board manager installed.
3. Input your unique `BLYNK_AUTH_TOKEN`, `SSID`, and `PASSWORD`.
4. Select "DOIT ESP32 DEVKIT V1" and hit **Upload**.

## 🔮 Future Enhancements
* **Voice Integration:** Adding support for Amazon Alexa and Google Assistant via Sinric Pro or Blynk integration.
* **Manual Override:** Adding physical push-buttons with interrupt-based logic for local control.
* **Real Energy Monitoring:** Integrating the PZEM-004T hardware to read actual Voltage, Current, and Power Factor.
* **OTA Updates:** Enabling Over-The-Air firmware updates for maintenance without physical access.

---
*Disclaimer: This project involves working with high-voltage AC electricity. Ensure all safety protocols are followed and the device is properly encased before connecting to mains power.*