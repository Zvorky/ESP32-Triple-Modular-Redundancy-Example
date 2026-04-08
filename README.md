# Triple Modular Redundancy (TMR) with LDRs
A practical implementation of Fault Tolerance and Real-Time Systems concepts on ESP32.

- [Leia a versão em Português (LEIAME.md)](LEIAME.md)
- [ToDo list](ToDo.md)

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

### What is Fault Detection and Isolation (FDI)?
**Fault Detection and Isolation (FDI)** is the set of mechanisms used to identify that a fault has occurred and determine which component is responsible for it. In a redundant architecture, masking the fault is only part of the solution; the system also benefits from explicitly flagging the disagreement and removing the suspicious element from the decision process.

In this project, FDI is performed by comparing the three sensor votes against the TMR majority result:

- **Fault Detection:** when one sensor vote differs from the other two, the system detects a disagreement.
- **Fault Isolation:** the sensor that disagrees with the majority is marked as isolated and is no longer considered an active sensor in the next iterations.
- **Degraded Operation:** after isolation, the system keeps running with the remaining active sensors and reports whether they still agree.

This means the example goes beyond simple fault masking: it also demonstrates how a redundant system can identify the faulty channel and continue operating in a degraded but controlled mode.

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
3.  **Fault Detection and Isolation:** With three active sensors, if one vote differs from the majority, the ESP32 reports `MASKED FAILURE`, identifies the disagreeing sensor, and marks it as `ISOLATED`.
4.  **Degraded Operation:** After isolation, the system keeps working with the remaining sensors. With two active sensors, the architecture effectively becomes a **2MR (Dual Modular Redundancy)** system, which uses a comparator instead of majority voting. If both active sensors agree, the system reports `CONSENSUS`; if they disagree, it reports a `CRITICAL ERROR` because there is no longer enough redundancy to isolate another fault safely.
5.  **Actuation:** If the active voting result indicates a dark environment, the main LED turns on.

### FDI Behavior in Practice

- **3 active sensors:** TMR majority voting masks one faulty reading and isolates the disagreeing sensor.
- **2 active sensors:** the system degrades to **2MR**, using a comparator instead of majority voting; it can detect disagreement but cannot safely determine which of the two sensors is wrong.
- **1 active sensor:** the system still produces an output, but redundancy has been lost and the decision depends on a single channel.
- **0 active sensors:** the system reports total failure.

## 💻 Usage
1.  Upload the code to your ESP32.
2.  Open the Serial Monitor at `9600` baud rate.
3.  (Optional) Enter a custom light threshold via the serial input when prompted.
4.  Test the fault tolerance by covering one LDR with your finger. Notice that the main LED does not change state incorrectly, demonstrating **fault masking**.