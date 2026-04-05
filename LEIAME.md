# Redundância Modular Tripla (TMR) com LDRs
Uma implementação prática de conceitos de Tolerância a Falhas e Sistemas de Tempo Real no ESP32.

[Read the English version (README.md)](README.md)

---

## 📚 Conceitos Didáticos

### O que é Tolerância a Falhas?
A **Tolerância a Falhas** é a capacidade de um sistema continuar operando corretamente mesmo na presença de defeitos em um ou mais de seus componentes. Em sistemas críticos (como aviação, equipamentos médicos ou controle industrial), uma falha única não pode paralisar a operação. Para evitar isso, utilizamos a **redundância** — a adição de componentes extras que não são estritamente necessários para o funcionamento normal, mas que assumem o controle caso ocorra um erro.

### O que é Redundância Modular Tripla (TMR)?
A Redundância Modular Tripla (TMR) é uma técnica de redundância de hardware onde três componentes idênticos executam a mesma tarefa. O resultado de cada um é enviado para um **sistema de votação (votador)**, que define a saída final com base na maioria simples.



Se um dos três componentes falhar e gerar uma leitura errada, os outros dois ainda concordarão entre si, anulando o erro. Esse fenômeno é chamado de **mascaramento de falha**. A lógica booleana do votador para as entradas A, B e C é:

`Maioria = (A E B) OU (A E C) OU (B E C)`

**Tabela Verdade do Votador:**

| Sensor A | Sensor B | Sensor C | Saída (Maioria) |
| :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | **0** |
| 0 | 0 | 1 | **0** |
| 0 | 1 | 0 | **0** |
| 0 | 1 | 1 | **1** |
| 1 | 0 | 0 | **0** |
| 1 | 0 | 1 | **1** |
| 1 | 1 | 0 | **1** |
| 1 | 1 | 1 | **1** |

Neste projeto, utilizamos três sensores de luminosidade (LDRs). Se um dos sensores for tampado acidentalmente, desconectado ou queimar, o sistema continuará tomando a decisão correta (acender ou apagar o LED principal) com base nos dois sensores restantes.

---

## 🛠️ Requisitos de Hardware
* **Microcontrolador:** ESP32 (Utilizando pinos do ADC1 para não conflitar com o Wi-Fi).
* **Sensores:** 3x LDRs (Resistores Dependentes de Luz).
* **Resistores:** 3x resistores de ~10kΩ para os divisores de tensão dos LDRs.
* **Atuadores:** * 1x LED Principal (Representa a decisão final do sistema).
    * 3x LEDs Auxiliares (Mostram o voto individual de cada sensor. **Nota: Estes LEDs são opcionais e servem apenas para facilitar a visualização didática do comportamento individual de cada sensor.**)

### Configuração de Pinos
| Componente | Pino ESP32 | Descrição |
| :--- | :--- | :--- |
| **LDR 1** | GPIO 32 | Entrada analógica do Sensor 1 |
| **LDR 2** | GPIO 35 | Entrada analógica do Sensor 2 |
| **LDR 3** | GPIO 34 | Entrada analógica do Sensor 3 |
| **LED Principal**| GPIO 2 | Atuador final do sistema (acende no escuro) |
| **LED LDR 1*** | GPIO 26 | Mostra o voto individual do Sensor 1 *(Opcional)* |
| **LED LDR 2*** | GPIO 25 | Mostra o voto individual do Sensor 2 *(Opcional)* |
| **LED LDR 3*** | GPIO 33 | Mostra o voto individual do Sensor 3 *(Opcional)* |

---

## 🚀 Como Funciona

1.  **Leitura e Discretização:** O sistema lê os valores analógicos dos três divisores de tensão. Como os LDRs são analógicos e a lógica TMR é digital (booleana), o código transforma os valores em `0` ou `1` usando um limiar (padrão: `2000`). Se a leitura for menor que o limiar (indicando escuro), o sensor gera um voto positivo.
2.  **O Votador:** O ESP32 processa os três votos usando a fórmula de maioria do TMR.
3.  **Atuação:** Se a maioria (pelo menos 2 dos 3 sensores) detectar que está escuro, o LED principal é aceso.
4.  **Diagnóstico do Sistema:** * **Consenso Total:** Se os três sensores tiverem o mesmo valor, o console exibe `CONSENSUS`.
    * **Falha Mascarada:** Se houver discordância (ex: um sensor coberto ou quebrado), o console exibe `MASKED FAILURE`, mas a saída do sistema permanece correta e estável.

## 💻 Uso
1.  Faça o upload do código para o seu ESP32.
2.  Abra o Monitor Serial na velocidade de `9600` baud.
3.  (Opcional) Digite um novo valor de limiar no console logo após a inicialização.
4.  Teste a tolerância a falhas tampando apenas um LDR com o dedo. Observe que o LED principal não apaga/acende de forma errônea, demonstrando o **mascaramento da falha** na prática.