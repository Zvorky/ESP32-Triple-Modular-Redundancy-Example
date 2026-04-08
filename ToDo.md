# To-Do List: Hardware Redundancy Project (TMR & FDI)

This document tracks the progress of the Fault Tolerance hardware project using an ESP32, LDR sensors, and Triple Modular Redundancy (TMR). It is structured didactically to support future articles and video content.

## 1. Basic Hardware Setup & TMR Implementation
The foundational stage involves reading the physical environment and applying basic redundancy logic to mask single points of failure.

- [x] Wire 3 LDR sensors to independent analog pins (ADC1) and 1 main LED as the actuator.
- [x] Read analog values from the sensors and discretize them using a darkness `threshold`.
- [x] Implement the TMR Voter using the boolean logic formula: $V=(A\wedge B)\vee(A\wedge C)\vee(B\wedge C)$.
- [x] Actuate the main LED based on the voter's final decision.
- [x] Compare individual votes to detect and log if the system is in "CONSENSUS: all sensors agree" or if there is a "MASKED FAILURE: sensors disagree".

## 2. Fault Detection and Isolation (FDI)
This stage upgrades the system from simply masking faults to actively detecting and isolating faulty components.

- [x] Introduce active status tracking variables for each sensor (e.g., `bool isActive1 = true;`).
- [x] Implement a pre-processing step to count the number of currently active sensors (`activeSensors`) before calculating the final decision.
- [x] Only count votes (`yesVotes`) from sensors that are currently marked as active.
- [x] Compare the majority decision (`finalDecision`) against each individual sensor's vote.
- [x] Isolate a discordant sensor by updating its status to inactive (`isActive = false`) and emit an alert to the serial monitor (e.g., "ALERT: Sensor 1 failed and was ISOLATED").
- [x] Add a visual log to display the overall health of the system (e.g., `Health [1, 2, 3] : [OK | --]`).

## 3. Graceful Degradation & Fail-Safe Mechanisms
With sensors being isolated, the system must dynamically adapt its decision-making rules based on the remaining functional hardware.

- [x] Implement the rule for **3 Active Sensors**: The decision requires 2 or more votes to function properly.
- [x] Implement the rule for **2 Active Sensors**: Both remaining sensors MUST agree for the system to operate.
- [x] Implement a **Fail-Safe state**: If only 2 sensors are active and they disagree, force the system into a safe state (turn off the actuator) and log a critical error.
- [x] Implement the rule for **1 Active Sensor**: Keep the system operating in degraded mode using the single remaining sensor, while emitting a warning.
- [x] Implement the rule for **0 Active Sensors**: Declare total system failure, log critical status, and force fail-safe output (actuator off).

## 4. Self-Healing
Advanced fault tolerance: distinguishing between permanent hardware failure and transient faults (e.g., a temporary shadow or loose wire).

- [ ] Track isolated sensors to check if their readings start matching the active majority again.
- [ ] Implement a timer to require the isolated sensor to maintain agreement for a specific duration (e.g., 10 seconds).
- [ ] Reintegrate the sensor by setting its active status back to `true`, assuming the fault was transient, and log the recovery ("RECOVERY: Sensor 1 reintegrated.").

## 5. Future Improvements (Backlog)
Planned improvements that are intentionally not part of the current implementation state.

- [ ] Add noise-robust isolation logic with disagreement/agreement counters per sensor (configurable threshold).
- [ ] Add optional filtering for sensor readings (e.g., median or moving average) before voting.
- [ ] Add per-sensor tuning gain/offset and optional startup calibration modes: None, Manual, Automatic.
- [ ] Improve open-circuit diagnostics using plausibility checks over time (stuck value, saturation persistence, impossible transitions), not only a single raw value.
- [ ] Add structured telemetry logs for post-analysis (raw reads, filtered reads, votes, active mask, health, final decision).
- [ ] Add configurable log verbosity: full logs (all channels/states) or important events only.
