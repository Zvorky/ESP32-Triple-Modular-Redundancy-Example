/*
Triple Modular Redundancy (TMR) com LDRs
Enzo Zavorski Delevatti - 199575

Engenharia de Computação - UPF
Tolerência a Falhas e Sistemas de Tempo Real
Professor Marcelo Trindade Rebonatto

Abril 2026
*/


#include <Arduino.h>


// Definição dos pinos analógicos (ADC1)
const int pinoLED = 2;
const int pinoLDR1 = 32;
const int pinoLDR2 = 35;
const int pinoLDR3 = 34;

const int limiar = 2000;

void setup() {
  Serial.begin(9600);
  Serial.println("--- Teste de uso do LDR  ---");
  pinMode(pinoLED, OUTPUT);
}

void loop() {
  // Realiza a leitura do sensor
  int leitura1 = analogRead(pinoLDR1);
  int leitura2 = analogRead(pinoLDR2);
  int leitura3 = analogRead(pinoLDR3);

  // Exibe os valores formatados para fácil leitura
  Serial.print("\nSensor 1: ");
  Serial.println(leitura1);
  Serial.print("Sensor 2: ");
  Serial.println(leitura2);
  Serial.print("Sensor 3: ");
  Serial.println(leitura3);

  // Pequeno atraso para não inundar o console
  delay(500); 
}