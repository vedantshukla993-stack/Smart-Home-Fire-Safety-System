# 🔥 Smart Home Fire Safety System

An IoT-based Smart Home Fire Safety & Emergency Response System developed using **ESP32** and simulated in **Wokwi**. The system continuously monitors environmental conditions using a **DHT22 Temperature & Humidity Sensor** and an **MQ-2 Gas/Smoke Sensor**. When abnormal conditions are detected, it automatically activates emergency responses such as opening a ventilation window using a servo motor, updating the LCD display, changing LED status indicators, and sending sensor data to ThingSpeak over Wi-Fi.

---

## 🚀 Features

* Real-time Temperature & Humidity Monitoring
* Smoke/Gas Detection using MQ-2 Sensor
* Automatic Emergency Ventilation using Servo Motor
* LCD-based Safety Status Display
* RGB LED Status Indicators
* Wi-Fi Connectivity using ESP32
* Cloud Data Logging with ThingSpeak
* Wokwi Simulation Support

---

## 🛠 Hardware Components

* ESP32 DevKit V1
* DHT22 Temperature & Humidity Sensor
* MQ-2 Gas/Smoke Sensor
* SG90 Servo Motor
* 16×2 I2C LCD
* RGB LED
* Resistors
* Jumper Wires

---

## ⚙ Working Principle

### Safe Mode

* Displays **SYSTEM SECURE** on the LCD.
* Green LED remains ON.
* Servo remains in the closed position.
* Sensor values are periodically uploaded to ThingSpeak.

### Warning Mode

* Triggered when the temperature exceeds the warning threshold.
* LCD displays a high-temperature warning.
* LED changes to indicate caution.

### Fire Mode

Triggered when:

* Smoke level exceeds the predefined threshold, or
* Temperature rises above the emergency threshold.

During Fire Mode:

* LCD displays a fire alert.
* Servo motor opens the emergency ventilation window.
* RGB LED flashes red.
* Emergency status is uploaded to ThingSpeak.

---

## 💻 Software & Technologies

* Arduino C++
* ESP32
* Embedded Systems
* IoT
* Wokwi Simulator
* ThingSpeak Cloud
* HTTPClient
* I2C Communication

---

## 📂 Project Structure

```text
Smart-Home-Fire-Safety-System
│
├── sketch.ino
├── diagram.json
├── libraries.txt
├── README.md
└── LICENSE
```

---

## ▶️ How to Run

1. Open the project in Wokwi.
2. Install the required libraries listed in `libraries.txt`.
3. Add your ThingSpeak Write API Key inside the code.
4. Start the simulation.
5. Change the MQ-2 sensor value or temperature to observe Safe, Warning, and Fire modes.

---

## 🔮 Future Improvements

* Buzzer Alarm
* GSM/SMS Emergency Alerts
* Telegram Notifications
* Firebase Integration
* Mobile Application Dashboard
* Automatic Fire Suppression System

---

## 👨‍💻 Author

**Vedant Shukla**

Electrical & Electronics Engineering Student

Interested in Embedded Systems, IoT, AI, and Automation.
