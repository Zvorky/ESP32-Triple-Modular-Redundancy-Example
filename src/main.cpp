/*
Triple Modular Redundancy (TMR) with LDRs
Enzo Zavorski Delevatti - 199575

Computer Engineering - UPF
Fault Tolerance and Real-Time Systems
Professor Marcelo Trindade Rebonatto

April 2026
*/


#include <Arduino.h>


// Analog pin definitions (ADC1)
const int LED = 2;
const int LDR1 = 32;
const int LDR2 = 35;
const int LDR3 = 34;

// Auxiliary LEDs to indicate each sensor's vote
const int AUXLED1 = 26;
const int AUXLED2 = 25;
const int AUXLED3 = 33;

int threshold = 2000;


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(AUXLED1, OUTPUT);
  pinMode(AUXLED2, OUTPUT);
  pinMode(AUXLED3, OUTPUT);

  Serial.begin(9600);
  delay(1000);
  Serial.println("--- TMR Example ---");

  // Configure threshold
  Serial.print("Enter threshold to turn on the LED (default ");
  Serial.print(threshold);
  Serial.print("): ");

  while (!Serial.available()); // Wait for user input
  String input = Serial.readString();

  if (input.length() > 0) {
    int value = input.toInt();
    if (value > 0) threshold = value;
  }
}

void loop() {
  // Read the sensors
  int ldr1 = analogRead(LDR1);
  int ldr2 = analogRead(LDR2);
  int ldr3 = analogRead(LDR3);

  // Print the formatted values for easy reading
  Serial.print("\nSensor 1: ");
  Serial.println(ldr1);
  Serial.print("Sensor 2: ");
  Serial.println(ldr2);
  Serial.print("Sensor 3: ");
  Serial.println(ldr3);

  // Voter
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