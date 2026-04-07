/*
Triple Modular Redundancy (TMR) with LDRs
Enzo Zavorski Delevatti - 199575

Computer Engineering - UPF
Fault Tolerance and Real-Time Systems
Professor Marcelo Trindade Rebonatto

April 2026
*/


#include <Arduino.h>
#include "utils.h"


// Analog pin definitions (ADC1)
#define LED 2
#define LDR1 32
#define LDR2 35
#define LDR3 34

// Auxiliary LEDs to indicate each sensor's vote
#define AUXLED1 26
#define AUXLED2 25
#define AUXLED3 33

int threshold = 2000; // 0-4095 for 12-bit ADC

// Fail Detection and Isolation variables
bool isActive1 = true;
bool isActive2 = true;
bool isActive3 = true;


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(AUXLED1, OUTPUT);
  pinMode(AUXLED2, OUTPUT);
  pinMode(AUXLED3, OUTPUT);

  Serial.begin(9600);
  delay(2000);
  Serial.println("\n\n---[ TMR Example ]---");

  // Configure threshold
  String s = " ";
  String msg = "Enter threshold to turn on the LED (default " + String(threshold) + ") [0-4095]: ";
  while (!isNumber(s) && s != "") s = input(msg, false);

  int value = s.toInt();
  if (value > 4095) {
    value = 4095; // Clamp to max ADC value
    Serial.println("Value clamped to 4095.");
  }
  if (value > 0) threshold = value;
  Serial.print("Threshold set to: ");
  Serial.println(threshold);
}

void loop() {
  // Read the sensors
  int ldr1 = analogRead(LDR1);
  int ldr2 = analogRead(LDR2);
  int ldr3 = analogRead(LDR3);

  // Print the formatted values for easy reading
  Serial.print("\nSensor 1: ");
  Serial.print(ldr1);
  if (isActive1) Serial.println(" \tOK");
  else Serial.println(" \tISOLATED");
  Serial.print("Sensor 2: ");
  Serial.print(ldr2);
  if (isActive2) Serial.println(" \tOK");
  else Serial.println(" \tISOLATED");
  Serial.print("Sensor 3: ");
  Serial.print(ldr3);
  if (isActive3) Serial.println(" \tOK");
  else Serial.println(" \tISOLATED");

  // Voter
  int activeSensors = 0;
  if (isActive1) activeSensors++;
  if (isActive2) activeSensors++;
  if (isActive3) activeSensors++;

  bool vote1 = ldr1 < threshold;
  bool vote2 = ldr2 < threshold;
  bool vote3 = ldr3 < threshold;
  bool majority = (vote1 && vote2) || (vote1 && vote3) || (vote2 && vote3);

  // Voter Truth Table:
  // vote1 | vote2 | vote3 | Majority
  //   0   |   0   |   0   |   0
  //   0   |   0   |   1   |   0
  //   0   |   1   |   0   |   0
  //   0   |   1   |   1   |   1
  //   1   |   0   |   0   |   0
  //   1   |   0   |   1   |   1
  //   1   |   1   |   0   |   1
  //   1   |   1   |   1   |   1

  // Turn the LED on if the majority of sensors detect light below the threshold
  if (majority) {
    digitalWrite(LED, HIGH);
    Serial.println("LED ON");
  } else {
    digitalWrite(LED, LOW);
    Serial.println("LED OFF");
  }

  // Turn on the individual LEDs for each sensor
  digitalWrite(AUXLED1, vote1 ? HIGH : LOW);
  digitalWrite(AUXLED2, vote2 ? HIGH : LOW);
  digitalWrite(AUXLED3, vote3 ? HIGH : LOW);

  if (vote1 == vote2 && vote2 == vote3) {
    Serial.println("CONSENSUS: all sensors agree.");
  } else {
    Serial.println("MASKED FAILURE: sensors disagree.");
  }
  
  // Small delay to avoid flooding the console
  delay(500); 
}