# Tarefa 13 — Requisitos Carrinho-Robô (Chassi V0.2)

**Projeto:** Réplica RC do Relâmpago McQueen (Carros)
**Disciplina/Curso:** Project-based Maker Lab — Start-up One
**Integrantes:** [nome dos integrantes do grupo]
**Data:** 19/08/2026

---

## 0. Registro de mudanças (V0.1 → V0.2)

- Adicionado **sensor de ré HC-SR04** (ultrassônico), na traseira do chassi, voltado para trás
- Sensor é ativado junto com o comando `R` (marcha à ré), para detectar obstáculos atrás do carro
- Adicionados: diagrama de blocos (conexões e pinos) e diagrama de alimentação
- Tabela dimensional atualizada com a linha do sensor de ré

---

## 1. Ficha de Requisitos

### 1.1 Dimensões do chassi

| Medida | Valor sugerido |
|---|---|
| Comprimento total | 200 mm |
| Largura total | 95 mm |
| Altura total (sem antena/asa) | 65 mm |
| Entre-eixos (distância eixo dianteiro–traseiro) | 140 mm |
| Bitola (distância entre rodas do mesmo eixo) | 80 mm |
| Altura livre do solo (ground clearance) | 10 mm |
| Diâmetro das rodas traseiras | 65 mm |

### 1.2 Quantidade de motores

| Função | Quantidade | Tipo |
|---|---|---|
| Tração (rodas traseiras, com marcha à ré) | 1 | Motor DC N20 6V 1000 RPM, com caixa de redução |
| Direção (rodas dianteiras, esquerda/direita) | 1 | Micro servo SG90 (direção tipo Ackermann) |

**Total: 2 atuadores** (1 motor de tração + 1 servo de direção)

A marcha à ré é feita por software: o driver de motor (ponte H) inverte a polaridade da corrente no motor, sem necessidade de motor ou hardware extra.

### 1.3 Placa controladora

- **ESP32-WROOM-32** (módulo Devkit V1, 30 pinos)
  - Programado pela IDE do Arduino (usa o núcleo/"core" oficial do Arduino para ESP32), cumprindo o requisito de uso de placa Arduino
  - Bluetooth Classic + BLE embutidos no próprio chip — dispensa módulo Bluetooth externo (como o HC-05)
- Driver de motor: **TB6612FNG** (ponte H dupla)
- Alimentação de potência (motor/servo): bateria **LiPo 2S 7.4V**
- Alimentação lógica do ESP32: regulada via módulo step-down (buck) LM2596, de 7.4V para 5V/3.3V

### 1.4 Controle via Bluetooth

- App de controle (celular ou PC) conecta ao ESP32 via Bluetooth
- Comandos enviados como caracteres simples, interpretados pelo firmware:

| Comando | Ação |
|---|---|
| `F` | Anda para frente |
| `R` | Anda em marcha à ré |
| `L` | Vira à esquerda (servo) |
| `D` | Vira à direita (servo) |
| `S` | Para o motor / centraliza direção |

### 1.5 Posição dos componentes (layout)

| Componente | Posição no chassi |
|---|---|
| ESP32 + driver TB6612FNG | Centro-traseiro, sobre o eixo traseiro |
| Bateria LiPo | Centro do chassi, o mais baixo possível (baixa o centro de gravidade) |
| Módulo step-down (5V) | Ao lado do ESP32, próximo à fiação de alimentação |
| Motor de tração | Traseiro, acoplado ao eixo das rodas traseiras |
| Servo de direção | Dianteiro, centralizado, ligado à barra de direção das rodas da frente |
| Rodas dianteiras | Extremidade dianteira, articuladas (viram com o servo) |
| Rodas traseiras | Extremidade traseira, fixas (recebem a tração e a marcha à ré) |
| Fiação | Canaletas laterais internas, longe do eixo de direção |

### 1.6 Carenagem (cobertura)

- Material: **PLA**, impressa em 3D, casca única ou em 2 partes (superior/inferior) unidas por encaixes tipo clipe + 2 parafusos M2
- Modelada no formato do Relâmpago McQueen (Carros), com aberturas de ventilação sobre o driver de motor (dissipação de calor)
- Fixação removível, para permitir acesso à bateria (troca/recarga externa) e à fiação

---

## 2. Lista de componentes (compra)

| Item | Modelo/versão | Qtd |
|---|---|---|
| Placa controladora | ESP32-WROOM-32 Devkit V1 | 1 |
| Driver de motor | TB6612FNG | 1 |
| Motor de tração | N20 6V 1000 RPM c/ redução | 1 |
| Servo de direção | SG90 | 1 |
| Rodas traseiras | 65mm, pneu silicone | 2 |
| Rodas dianteiras | ~40-50mm, com manga de eixo articulado | 2 |
| Bateria | LiPo 2S 7.4V 1000-1500mAh | 1 |
| Carregador (externo, não vai no carro) | Balanceador B3/B6 | 1 |
| Módulo step-down | LM2596 | 1 |
| Filamento 3D | PLA 1.75mm | 1 rolo |

---

## 3. Tabela Dimensional

> **Observação:** o carrinho usa direção Ackermann (rodas dianteiras viram via servo), não tração diferencial. Por isso a linha "Motor esquerdo" corresponde ao único motor de tração (traseiro) e "Motor direito" foi substituída pelo servo de direção (dianteiro).

| Componente | Comprimento | Largura | Altura | Forma de fixação |
|---|---|---|---|---|
| Motor (tração) — subst. "Motor esquerdo" | 24 mm | 12 mm | 10 mm | 2 parafusos M2 na caixa de redução, presos no suporte impresso em 3D |
| Servo de direção — subst. "Motor direito" | 32 mm | 12 mm | 30 mm | Encaixe em suporte impresso + 2 parafusos M2 nas orelhas do servo |
| ESP32-WROOM-32 (Devkit V1) | 55 mm | 28 mm | 13 mm | 4 furos M2,5 nos cantos, parafusado em standoffs impressos |
| Ponte H (TB6612FNG) | 20 mm | 15 mm | 5 mm | Fita dupla-face de alta fixação ou parafuso M2 em suporte próprio |
| Bateria (LiPo 2S 1000mAh) | 65 mm | 35 mm | 12 mm | Velcro dentro de compartimento fechado com trava, no centro do chassi |
| Sensor de ré (HC-SR04) | 45 mm | 20 mm | 15 mm | Suporte impresso na traseira, voltado para trás, encaixe de pressão + 2 parafusos M2 |

## 4. Diagrama de blocos (conexões e pinos)

Ver arquivo `diagrama-blocos.svg` — mostra todos os componentes eletrônicos, quais pinos do ESP32 são usados em cada conexão, e quantos pinos cada componente ocupa.

![Diagrama de blocos](diagrama-blocos.svg)

| Componente | Pino no ESP32 | Função |
|---|---|---|
| Servo SG90 | GPIO 13 | Sinal PWM (direção esquerda/direita) |
| Driver TB6612FNG — AIN1 | GPIO 27 | Sentido de giro do motor (bit 1) |
| Driver TB6612FNG — AIN2 | GPIO 26 | Sentido de giro do motor (bit 2) — juntos definem frente/ré |
| Driver TB6612FNG — PWMA | GPIO 25 | Velocidade do motor (PWM) |
| Driver TB6612FNG — STBY | GPIO 33 | Habilita/desabilita o driver |
| Sensor HC-SR04 — TRIG | GPIO 5 | Envia o pulso ultrassônico |
| Sensor HC-SR04 — ECHO | GPIO 18 (via divisor de tensão 5V→3.3V) | Recebe o eco e mede a distância |
| Bluetooth | Interno ao ESP32 (não usa pino físico) | Comunicação com o app de controle |

**Total de pinos digitais usados: 7** (1 servo + 4 driver + 2 sensor)

## 5. Diagrama de alimentação

Ver arquivo `diagrama-alimentacao.svg`.

**Bateria escolhida: LiPo 2S 7.4V (íon-lítio)**, entre as opções consideradas pelo grupo:

| Opção considerada | Por que não foi escolhida |
|---|---|
| Pilhas AA (1.5V) | Precisaria de 5-6 pilhas em série pra chegar perto de 7-9V; tensão cai muito sob carga (motor perde força) |
| Bateria 9V (retangular) | Corrente de pico muito baixa, não sustenta a arrancada do motor |
| **LiPo 2S 7.4V (escolhida)** | Alta corrente de pico (25C+), leve, recarregável, tensão estável mesmo sob carga do motor |

A bateria alimenta o projeto em duas ramificações:
1. **7.4V direto** → Driver TB6612FNG (VM) → Motor DC
2. **7.4V → módulo step-down LM2596 → 5V regulado** → ESP32, Servo SG90 e Sensor HC-SR04

O GND de todos os componentes (bateria, driver, ESP32, servo, sensor) precisa ser comum.

## 6. Croqui do chassi (posição e organização dos componentes)

Ver arquivo `croqui-chassi.svg` (vista superior, com posição dos componentes).

![Croqui do chassi](croqui-chassi.svg)

**Organização física:**
- Fiação de potência (motor/bateria) separada da fiação de sinal (servo/sensor) por canaletas laterais distintas, evitando ruído elétrico
- Bateria presa com **velcro** dentro de um compartimento fechado com trava (fácil de remover pra recarregar)
- ESP32 e driver fixados em **standoffs impressos em 3D** (parafusos M2.5), elevados do chão do chassi
- Cabos excedentes organizados com **abraçadeiras plásticas pequenas (nylon, 2.5mm)** presas a suportes impressos junto ao chassi
