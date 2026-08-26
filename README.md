
**Projeto:** Réplica RC do Relâmpago McQueen (Carros)
**Disciplina/Curso:** Project-based Maker Lab — Start-up One
**Data:** 19/08/2026

---

## 0. Registro de mudanças

**V0.1 → V0.2:** Adicionado sensor de ré HC-SR04 (traseira do chassi).

**V0.2 → V0.3:** Migração para o kit **LAFVIN 4WD com ESP32-CAM**:
- Chassi passou de 100% impresso em 3D para **base acrílica pronta do kit** (a carenagem do Relâmpago McQueen continua sendo impressa em 3D, encaixada por cima da base)
- Direção mudou de **servo + Ackermann** para **diferencial (skid-steer)** — sem servo, 4 motores
- Motor de tração único (N20) substituído por **4 motores TT** (2 por lado, ligados em paralelo)
- Driver TB6612FNG substituído por **L298N** (o que vem no kit)
- Placa ESP32-WROOM-32 Devkit substituída por **ESP32-CAM** (câmera OV2640 embutida)
- Bateria LiPo 2S substituída por **suporte de 4x pilha AA** (incluso no kit)
- Sensor de ré HC-SR04 mantido, comprado à parte (não vem no kit)

---

## 1. Ficha de Requisitos

### 1.1 Dimensões do chassi

| Medida | Valor (conforme especificação do kit) |
|---|---|
| Comprimento total | ~250 mm |
| Largura total | ~150 mm |
| Altura total (base, sem carenagem) | ~50 mm |
| Peso do kit (sem bateria/eletrônica extra) | ~650 g |
| Diâmetro das rodas | ~65 mm (rodas do kit) |

> Medidas conforme embalagem do fabricante. Recomenda-se medir o chassi físico com paquímetro ao receber o kit e atualizar esta tabela.

### 1.2 Quantidade de motores

| Função | Quantidade | Tipo |
|---|---|---|
| Tração + direção (diferencial) | 4 | Motor TT DC 3-6V, com caixa de redução, eixo D-shaft 5mm |

Os 4 motores são agrupados em 2 pares controlados pelo L298N:
- **Motor esquerdo** = motor dianteiro-esquerdo + motor traseiro-esquerdo (ligados em paralelo, mesmo canal)
- **Motor direito** = motor dianteiro-direito + motor traseiro-direito (ligados em paralelo, mesmo canal)

A curva é feita por **diferença de velocidade entre os lados** (skid-steer), não por rodas articuladas. A ré é feita por software (inversão de polaridade no L298N), assim como antes.

### 1.3 Placa controladora

- **ESP32-CAM** (AI-Thinker), com câmera OV2640 integrada e Bluetooth/WiFi embutidos
- **Não possui porta USB** — a gravação do firmware exige um adaptador externo **FTDI USB-TTL (FT232RL ou CP2102)**, usado só na hora de programar (não fica no carro)
- Driver de motor: **L298N** (ponte H dupla, o que vem no kit)
- Alimentação: suporte **4x pilha AA** (6V nominal), incluso no kit

### 1.4 Controle via Bluetooth (comandos atualizados p/ diferencial)

| Comando | Ação |
|---|---|
| `F` | Os 4 motores giram para frente |
| `R` | Os 4 motores giram em marcha à ré |
| `L` | Lado esquerdo reduz/para, lado direito anda → vira à esquerda |
| `D` | Lado direito reduz/para, lado esquerdo anda → vira à direita |
| `S` | Todos os motores param |

### 1.5 Posição dos componentes (layout)

| Componente | Posição no chassi |
|---|---|
| L298N | Centro do chassi acrílico, sobre a camada superior |
| ESP32-CAM | Ao lado do L298N, câmera voltada para a frente |
| Suporte 4x pilha AA | Entre as duas camadas do chassi acrílico (embaixo), baixa o centro de gravidade |
| Motores TT (4x) | Um em cada canto, já com furação pronta no chassi do kit |
| Sensor de ré (HC-SR04) | Traseira, voltado para trás, em suporte impresso em 3D (não incluso no kit) |
| Carenagem (Relâmpago McQueen) | Encaixada por cima da estrutura acrílica, presa nos furos de montagem do chassi |

### 1.6 Carenagem (cobertura)

- Material: **PLA**, impressa em 3D, modelada no formato do Relâmpago McQueen (Carros)
- Encaixa por cima do chassi acrílico do kit, aproveitando os furos de montagem já existentes na placa superior
- Fixação removível (clipes + parafusos M3), para acesso à troca das pilhas e à fiação

---

## 2. Lista de componentes (compra)

| Item | Modelo/versão | Qtd | Origem |
|---|---|---|---|
| Kit base | LAFVIN 4WD ESP32-CAM Kit | 1 | Comprado (chassi, 4 motores TT, rodas, L298N, suporte pilhas, parafusos, jumpers) |
| Programador | Adaptador FTDI USB-TTL (FT232RL/CP2102) | 1 | Comprar à parte (não vem no kit) |
| Sensor de ré | HC-SR04 (ultrassônico) | 1 | Comprar à parte (não vem no kit) |
| Resistores (divisor de tensão do sensor) | 1kΩ + 2kΩ (1/4W) | 1 par | Comprar à parte |
| Filamento 3D | PLA 1.75mm | 1 rolo | Comprar à parte (carenagem) |
| Pilhas AA | 4 unidades (alcalinas ou recarregáveis NiMH) | 4 | Comprar à parte |

---

## 3. Tabela Dimensional

| Componente | Comprimento | Largura | Altura | Forma de fixação |
|---|---|---|---|---|
| Motor esquerdo (par, TT) | 66 mm | 22 mm | 20 mm | 2 parafusos M3 por motor, direto nos furos do chassi acrílico |
| Motor direito (par, TT) | 66 mm | 22 mm | 20 mm | 2 parafusos M3 por motor, direto nos furos do chassi acrílico |
| ESP32-CAM | 40 mm | 27 mm | 12 mm | Fita dupla-face isolante ou suporte impresso pequeno |
| Ponte H (L298N) | 43 mm | 43 mm | 27 mm (com dissipador) | Parafusos M3 nos furos do próprio módulo |
| Bateria (suporte 4x AA) | 60 mm | 55 mm | 20 mm | Fixação por parafuso/velcro entre as camadas do chassi (conforme kit) |
| Sensor de ré (HC-SR04) | 45 mm | 20 mm | 15 mm | Suporte impresso na traseira, voltado para trás, encaixe + 2 parafusos M2 |

## 4. Diagrama de blocos (conexões e pinos)

Ver arquivo `diagrama-blocos.svg`.

| Componente | Pino no ESP32-CAM | Função |
|---|---|---|
| L298N — IN1 | GPIO 12 | Direção motor esquerdo (bit 1) |
| L298N — IN2 | GPIO 13 | Direção motor esquerdo (bit 2) |
| L298N — IN3 | GPIO 14 | Direção motor direito (bit 1) |
| L298N — IN4 | GPIO 15 | Direção motor direito (bit 2) |
| L298N — ENA / ENB | Jumper fixo (sempre habilitado) | Velocidade fixa (sem controle de PWM, por limitação de pinos livres do ESP32-CAM) |
| Sensor HC-SR04 — TRIG | GPIO 2 | Envia o pulso ultrassônico |
| Sensor HC-SR04 — ECHO | GPIO 16 (via divisor de tensão 5V→3.3V) | Recebe o eco / mede distância |
| Bluetooth | Interno ao ESP32 | Comunicação com o app de controle |
| FTDI (só na gravação) | GPIO 1 (TX), GPIO 3 (RX), GPIO 0 → GND (modo flash) | Usado apenas para gravar o firmware, depois desconectado |

> Nota: o ESP32-CAM tem poucos pinos livres (a maioria é usada pela câmera). Por isso a velocidade dos motores fica fixa (sem PWM), controlando apenas ligar/desligar e sentido de cada lado.

## 5. Diagrama de alimentação

Ver arquivo `diagrama-alimentacao.svg`.

**Bateria: suporte 4x pilha AA (6V nominal), incluso no kit.**

| Opção considerada | Situação |
|---|---|
| Pilhas AA (6V, 4 unidades) | **Escolhida** — já vem incluída no kit, simples de trocar, sem necessidade de carregador especial |
| Bateria 9V | Descartada — corrente de pico baixa, não sustenta 4 motores simultâneos |
| LiPo 2S 7.4V | Descartada nesta versão — exigiria comprar carregador/conector à parte; pode ser upgrade futuro se a autonomia com pilha for insuficiente |

A alimentação segue direto do suporte de pilhas (6V) para o L298N (VIN), e o próprio L298N tem um regulador interno de 5V que alimenta o ESP32-CAM e o sensor HC-SR04. GND comum entre bateria, L298N, ESP32-CAM e sensor.

## 6. Croqui do chassi (posição e organização dos componentes)

Ver arquivo `croqui-chassi.svg`.

**Organização física:**
- Suporte de pilhas fixado entre as duas camadas do chassi acrílico (peso baixo, mais estabilidade)
- L298N e ESP32-CAM na camada superior, fiação passando pelos furos já existentes na placa
- Cabos dos 4 motores organizados com abraçadeiras plásticas pequenas (nylon, 2,5mm) ao longo das laterais do chassi
- Sensor de ré fixado em suporte impresso à parte (não incluso no kit), atrás da placa traseira
