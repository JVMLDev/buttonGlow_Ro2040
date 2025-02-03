# 🌟 Controle de LED RGB e Matriz WS2812 com Interrupções (Raspberry Pi Pico) 🌟

Este projeto utiliza o Raspberry Pi Pico e a placa BitDogLab para controlar uma matriz de LEDs WS2812 e um LED RGB, permitindo a exibição de números de 0 a 9 de maneira interativa. A interação é feita por meio de dois botões físicos que utilizam interrupções (IRQ) e debounce via software.

## 👨‍💻 Desenvolvedor

João Vitor Maciel Lima

## 🛠️ Funcionalidades

### Piscar LED Vermelho do LED RGB 🔴

- O LED vermelho pisca 5 vezes por segundo de forma independente, utilizando multithreading no Raspberry Pi Pico.

### Controle de Números na Matriz WS2812 🔢

- **Botão A:** Incrementa o número exibido na matriz de LEDs.
- **Botão B:** Decrementa o número exibido na matriz de LEDs.
- Cada número (0 a 9) é representado visualmente na matriz 5x5 de LEDs WS2812.

### Uso de Interrupções e Debounce ⚡

- As funcionalidades dos botões são implementadas usando interrupções (IRQ), garantindo resposta rápida sem bloquear a execução principal.
- Implementação de debounce via software para evitar múltiplas detecções indesejadas dos botões.

## 👋 Requisitos

### 🔧 Hardware:

- Raspberry Pi Pico
- Placa BitDogLab
- Matriz de LEDs WS2812 (5x5), conectada à GPIO 7
- LED RGB integrado na BitDogLab (GPIOs 11, 12 e 13)
- Botão A conectado à GPIO 5
- Botão B conectado à GPIO 6

### 💻 Software:

- SDK do Raspberry Pi Pico configurado
- Compilador C compatível (como GCC)
- Ferramentas para upload do código para o Pico (como `picotool`)

## 🧙️ Configuração do Hardware

1. **Conecte a Matriz de LEDs WS2812:**
   - Pino de dados na GPIO 7 do Raspberry Pi Pico.

2. **Conecte os LEDs RGB:**
   - LED Vermelho na GPIO 13.

3. **Conecte os Botões:**
   - Botão A na GPIO 5.
   - Botão B na GPIO 6.



## 🚀 Compilação e Upload
### 1. Clone o repositório do código:
   
   git clone <URL_DO_REPOSITORIO>
### 2. Compile o programa usando CMake:
   mkdir build
   cd build
   cmake ..
   make

### 3.  Envie o arquivo `.uf2` gerado para o Raspberry Pi Pico:
   - Mantenha o botão `BOOTSEL` pressionado enquanto conecta o Pico ao computador.
   - Copie o arquivo `.uf2` gerado para a unidade montada.
#### 🎥 Demonstração
https://www.youtube.com/watch?v=evdOFoXNcsc