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

// LEDs auxiliares para indicar o voto de cada sensor
const int pinoLEDLDR1 = 26;
const int pinoLEDLDR2 = 25;
const int pinoLEDLDR3 = 33;

int limiar = 2000;


void setup() {
  Serial.begin(9600);
  Serial.println("--- Teste de uso do LDR  ---");
  pinMode(pinoLED, OUTPUT);
  pinMode(pinoLEDLDR1, OUTPUT);
  pinMode(pinoLEDLDR2, OUTPUT);
  pinMode(pinoLEDLDR3, OUTPUT);

  // Configura limiar
  delay(1000);
  Serial.print("Digite o limiar para acender o LED (padrão ");
  Serial.print(limiar);
  Serial.print("): ");
  while (!Serial.available());
  String entrada = Serial.readString();
  if (entrada.length() > 0) {
    int valor = entrada.toInt();
    if (valor > 0) limiar = valor;
  }
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

  // Votador
  bool voto1 = leitura1 < limiar;
  bool voto2 = leitura2 < limiar;
  bool voto3 = leitura3 < limiar;
  bool maioria = (voto1 && voto2) || (voto1 && voto3) || (voto2 && voto3);

  // Tabela verdade do votador:
  // Voto1 | Voto2 | Voto3 | Maioria
  //   0   |   0   |   0   |   0
  //   0   |   0   |   1   |   0
  //   0   |   1   |   0   |   0
  //   0   |   1   |   1   |   1
  //   1   |   0   |   0   |   0
  //   1   |   0   |   1   |   1
  //   1   |   1   |   0   |   1
  //   1   |   1   |   1   |   1

  // Acende o LED se a maioria dos sensores detectar luz abaixo do limiar
  if (maioria) {
    digitalWrite(pinoLED, HIGH);
    Serial.println("LED ACESO");
  } else {
    digitalWrite(pinoLED, LOW);
    Serial.println("LED APAGADO");
  }

  // Acende os LEDs individuais para cada sensor
  digitalWrite(pinoLEDLDR1, voto1 ? HIGH : LOW);
  digitalWrite(pinoLEDLDR2, voto2 ? HIGH : LOW);
  digitalWrite(pinoLEDLDR3, voto3 ? HIGH : LOW);

  if (voto1 == voto2 && voto2 == voto3) {
    Serial.println("CONSENSO: todos os sensores concordam.");
  } else {
    Serial.println("FALHA MASCARADA: sensores discordantes.");
  }
  
  // Pequeno atraso para não inundar o console
  delay(500); 
}