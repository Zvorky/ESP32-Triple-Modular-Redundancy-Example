/*
Engenharia de Computação - UPF
Tolerência a Falhas e Sistemas de Tempo Real
Marcelo Trindade Rebonatto
31/03/2026
Exemplo de uso do sensor LDR
*/
#include <Arduino.h>

// Definição dos pinos analógicos (ADC1)
const int pinoLDR1 = 34;

void setup() {
  Serial.begin(9600);
  Serial.println("--- Teste de uso do LDR  ---");
}

void loop() {
  // Realiza a leitura do sensor
  int leitura1 = analogRead(pinoLDR1);
  
  // Exibe os valores formatados para fácil leitura
  Serial.print("Sensor 1: ");
  Serial.println(leitura1);
  
  // Pequeno atraso para não inundar o console
  delay(1000); 
}