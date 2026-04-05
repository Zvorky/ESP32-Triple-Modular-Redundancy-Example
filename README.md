# Triple Modular Redundancy (TMR) with LDRs
A practical implementation of Fault Tolerance and Real-Time Systems concepts on ESP32.

[Leia a versão em Português (LEIAME.md)](LEIAME.md)

---

## 📚 Didactic Concepts

### What is Fault Tolerance?
**Fault Tolerance** is the property that enables a system to continue operating properly in the event of the failure of some of its components. In critical systems (like aerospace, medical equipment, or industrial control), a single point of failure can lead to catastrophic results. To prevent this, we introduce **redundancy**—having backup components that can take over or correct the system's output when a primary component fails.

### What is Triple Modular Redundancy (TMR)?
Triple Modular Redundancy (TMR) is a specific form of hardware redundancy where three identical components perform a process, and that result is processed by a **majority-voting system (voter)** to produce a single output. 



If any one of the three components fails or produces an erroneous reading, the other two will override it, effectively **masking the fault**. The boolean logic for a TMR voter receiving inputs A, B, and C is:

`Majority = (A AND B) OR (A AND C) OR (B AND C)`

**Voter Truth Table:**

| Sensor A | Sensor B | Sensor C | Majority Output |
| :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | **0** |
| 0 | 0 | 1 | **0** |
| 0 | 1 | 0 | **0** |
| 0 | 1 | 1 | **1** |
| 1 | 0 | 0 | **0** |
| 1 | 0 | 1 | **1** |
| 1 | 1 | 0 | **1** |
| 1 | 1 | 1 | **1** |

In this project, we use three Light Dependent Resistors (LDRs) to measure ambient light. Even if one LDR is physically blocked, damaged, or disconnected, the system will still make the correct decision based on the remaining two functional sensors.

---

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 (Using ADC1 pins to avoid Wi-Fi conflicts)
* **Sensors:** 3x LDRs (Light Dependent Resistors)
* **Resistors:** 3x ~10kΩ resistors for the LDR voltage dividers.
* **Actuators:** * 1x Main LED (Represents the final system decision)
    * 3x Auxiliary LEDs (Represent the individual vote of each sensor. **Note: These are optional and used purely for didactic visualization.**)

### Pin Configuration
| Component | ESP32 Pin | Description |
| :--- | :--- | :--- |
| **LDR 1** | GPIO 32 | Analog Input for Sensor 1 |
| **LDR 2** | GPIO 35 | Analog Input for Sensor 2 |
| **LDR 3** | GPIO 34 | Analog Input for Sensor 3 |
| **Main LED** | GPIO 2 | Final system actuator (turns on in the dark) |
| **LED LDR 1*** | GPIO 26 | Shows individual vote of Sensor 1 *(Optional)* |
| **LED LDR 2*** | GPIO 25 | Shows individual vote of Sensor 2 *(Optional)* |
| **LED LDR 3*** | GPIO 33 | Shows individual vote of Sensor 3 *(Optional)* |

---

## 🚀 How It Works

1.  **Reading & Discretization:** The system reads the analog values from the 3 LDRs. Since LDRs are analog and TMR uses boolean logic, the code discretizes the readings using a defined threshold (default: `2000`). If the reading is below the threshold (dark), the sensor casts a vote (`true` or `1`).
2.  **The Voter:** The ESP32 acts as the voter, applying the TMR boolean logic to the three votes. 
3.  **Actuation:** If the majority (at least 2 out of 3 sensors) detect a dark environment, the main LED turns on.
4.  **System Diagnostics:** * **Total Consensus:** If all three sensors agree, the system logs `CONSENSUS`.
    * **Masked Fault:** If one sensor disagrees (e.g., it is covered or broken), the system logs `MASKED FAILURE` but continues to operate correctly.

## 💻 Usage
1.  Upload the code to your ESP32.
2.  Open the Serial Monitor at `9600` baud rate.
3.  (Optional) Enter a custom light threshold via the serial input when prompted.
4.  Test the fault tolerance by covering one LDR with your finger. Notice that the main LED does not change state incorrectly, demonstrating **fault masking**.