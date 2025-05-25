# ✈️ MPiP-Project: Aerial Defence System

Welcome to the **MPiP-Project**, a final project for the Microcontrollers and Peripherals (MPiP) course, part of the Computer Science Engineering BSc at UAB. This project implements an **aerial defence system** using the **FRDM-MCXA153 development board**.

---

## 📜 Overview

The **MPiP-Project** is designed to detect and respond to aerial threats using sensors and actuators. It leverages the capabilities of the **FRDM-MCXA153 board** and integrates various peripherals to create a functional defence system.

### Key Features:

- **Ultrasonic Sensors**: Detect objects in the air.
- **LED Indicators**: Visual feedback for system status.
- **Buzzer**: Audible alerts for detected threats.
- **UART Communication**: Debugging and monitoring via serial terminal.

---

## 🛠️ Hardware Requirements

To replicate this project, you will need:

- **FRDM-MCXA153 Development Board**
- **Type-C USB Cable**
- **Ultrasonic Sensor (HC-SR04)**
- **LEDs (Red, Green, Yellow)**
- **Buzzer**
- **Personal Computer**

---

## 🧰 Software Requirements

Ensure you have the following tools installed:

- **MCUXpresso IDE** (Version 11.10.0 or later)
- **GCC ARM Embedded Toolchain** (Version 13.2.1 or later)
- **Serial Terminal Software** (e.g., PuTTY, Tera Term)

---

## 🚀 Getting Started

### 1️⃣ Setting Up the Hardware

1. Connect the **Type-C USB cable** between your PC and the **MCU-Link port (J15)** on the FRDM-MCXA153 board.
2. Wire the **HC-SR04 sensor**, **LEDs**, and **buzzer** to the appropriate GPIO pins as defined in the firmware.

### 2️⃣ Configuring the Software

1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/MPiP-Project.git
   ```
2. Open the project in **MCUXpresso IDE**.
3. Build the project and flash it to the board.

### 3️⃣ Running the System

1. Open a serial terminal with the following settings:
   - **Baud Rate**: 115200
   - **Data Bits**: 8
   - **Parity**: None
   - **Stop Bits**: 1
   - **Flow Control**: None
2. Reset the board or start the debugger to run the system.

---

## 📊 System Workflow

1. **Initialization**: The system initializes all peripherals (UART, GPIO, etc.).
2. **Detection**: The ultrasonic sensor continuously scans for objects.
3. **Response**:
   - **LEDs**: Indicate the proximity of detected objects.
   - **Buzzer**: Sounds an alarm if a threat is detected.
4. **Debugging**: Logs are sent to the serial terminal for monitoring.

---

## 🧑‍💻 Developer Information

### 👨‍💻 Lead Developer

- **Name**: [Nedal M. Benelmekki]
- **Email**: [nedal.martinez@uab.cat]
- **GitHub**: [nmb735](https://github.com/nmb735)

### 🤝 Contributors

- **Marc Serra Asensio**: [Lead Electrical and Mechanical Engineer]
- **Adrià Arús Setó**: [Project Manager]

---

## 📂 Project Structure (Firmware Information Only)

```
MPiP-Project/
├── Firmware/
│   ├── CMSIS/                # CMSIS Core files
│   ├── board/                # Board-specific configurations
│   ├── drivers/              # Peripheral drivers
│   ├── source/               # Application source code
│   └── doc/                  # Documentation
├── README.md                 # Project README
└── .mcuxpressoide_packages_support/ # IDE-specific configurations
```

---

## 📝 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

---

## 💡 Future Improvements

- Add support for **multiple sensors** for enhanced detection.
- Implement **wireless communication** for remote monitoring.
- Integrate **machine learning** for threat classification.

---

## 📬 Contact

For any questions or suggestions, feel free to reach out to the lead developer.

---
