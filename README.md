# Organização do Projeto — Réplica RC Relâmpago McQueen (V0.3)
---

## 1. MVP (Produto Mínimo Viável)

### Minimamente funcional (MVP)
O projeto é considerado **minimamente funcional** quando:
- Chassi do kit está montado (4 motores, rodas, L298N, suporte de pilhas)
- ESP32-CAM está programado e conecta via Bluetooth
- O carro responde aos comandos básicos: `F` (frente), `R` (ré), `L` (esquerda), `D` (direita), `S` (parar)
- O carro anda e faz curva (diferencial entre os lados)

> Neste ponto o carro já é um carrinho-robô funcional, mesmo sem sensor de ré e sem a carenagem do Relâmpago McQueen.

### Concluído (versão final)
O projeto é considerado **concluído** quando, além do MVP:
- O sensor de ré (HC-SR04) está integrado e funcionando (detecta obstáculo ao dar ré)
- A carenagem do Relâmpago McQueen está impressa em 3D e encaixada no chassi
- A fiação está organizada com abraçadeiras
- Toda a documentação técnica está publicada no GitHub (ficha de requisitos, diagramas, croqui, tabela dimensional, custos)

---

## 2. MoSCoW

### Must have (essencial)
- Chassi montado com os 4 motores funcionando
- ESP32-CAM programado com controle Bluetooth básico (F/R/L/D/S)
- Carro anda para frente, dá ré e vira para os dois lados
- Alimentação por pilhas funcionando

### Should have (importante, mas não bloqueia o MVP)
- Sensor de ré (HC-SR04) funcional
- Fiação organizada com abraçadeiras
- Documentação técnica completa no GitHub

### Could have (desejável, se sobrar tempo)
- Carenagem 3D do Relâmpago McQueen impressa e encaixada
- Streaming de vídeo da câmera do ESP32-CAM
- App de controle com interface gráfica (em vez de comandos soltos)

### Won't have (fora do escopo desta versão)
- Direção Ackermann com servo (abandonada ao trocar para o kit com direção diferencial)
- Navegação autônoma / desvio automático de obstáculo (o sensor de ré apenas informa distância, não toma decisão sozinho)
- GPS ou qualquer forma de localização

---

## 3. Backlog

| # | Tarefa | Responsável | Categoria |
|---|---|---|---|
| 1 | Comprar componentes (kit, FTDI, sensor, resistores, filamento, pilhas) | Integrante 1 | Compras |
| 2 | Montar chassi mecânico (motores, rodas, chassi acrílico) | Integrante 2 | Hardware |
| 3 | Instalar L298N e fazer a fiação de potência | Integrante 2 | Hardware |
| 4 | Configurar Arduino IDE para ESP32-CAM (placa + driver FTDI) | Integrante 3 | Firmware |
| 5 | Programar firmware base: controle diferencial via Bluetooth (F/R/L/D/S) | Integrante 3 | Firmware |
| 6 | Testar movimento básico (frente/ré/curvas) — **marco do MVP** | Integrante 1 e 2 | Testes |
| 7 | Integrar sensor de ré (HC-SR04) ao firmware | Integrante 4 | Firmware |
| 8 | Testar sensor de ré (detecção de obstáculo) | Integrante 4 | Testes |
| 9 | Modelar carenagem 3D do Relâmpago McQueen | Integrante 3 | Design |
| 10 | Imprimir e encaixar a carenagem no chassi | Integrante 2 | Hardware |
| 11 | Organizar fiação com abraçadeiras | Integrante 1 | Hardware |
| 12 | Escrever/atualizar documentação técnica no GitHub | Todos | Documentação |
| 13 | Preencher planilha de custos com valores finais de compra | Integrante 1 | Documentação |
| 14 | Testes finais e ajustes gerais — **marco de conclusão** | Todos | Testes |
| 15 | (Opcional) Configurar streaming de vídeo da câmera | Integrante 4 | Firmware (bônus) |

---

## 4. Dependências

| Tarefa | Depende de |
|---|---|
| 3 (fiação de potência) | 2 (chassi montado) |
| 5 (firmware base) | 4 (ambiente configurado) |
| 6 (teste de movimento) | 3 e 5 |
| 7 (integrar sensor) | 5 (firmware base já funcionando) |
| 8 (testar sensor) | 7 |
| 10 (imprimir/encaixar carenagem) | 9 (modelo pronto) e 2 (chassi montado, para saber medidas de encaixe) |
| 11 (organizar fiação) | 6 (só organiza depois de validar que funciona) |
| 14 (testes finais) | 6, 8, 10, 11 |
| 15 (streaming, opcional) | 6 |

---

## 5. Kanban

| Backlog | A Fazer | Em Andamento | Em Teste | Concluído |
|---|---|---|---|---|
| 9. Modelar carenagem 3D | 1. Comprar componentes | 2. Montar chassi mecânico | 6. Testar movimento básico | — |
| 10. Imprimir/encaixar carenagem | 4. Configurar Arduino IDE | 5. Programar firmware base | 8. Testar sensor de ré | — |
| 13. Preencher planilha de custos | 3. Instalar L298N + fiação | | | |
| 15. Streaming de vídeo (opcional) | 7. Integrar sensor de ré | | | |
| 14. Testes finais e ajustes | 11. Organizar fiação | | | |
| | 12. Documentação no GitHub | | | |

> Mover as tarefas entre colunas conforme o grupo avança. Sugestão: recriar este quadro como um GitHub Project (Kanban nativo do GitHub) para facilitar a atualização colaborativa.

---

## 6. Planilha de custos

Ver arquivo `planilha-custos.xlsx` — contém item, loja, link, preço unitário, quantidade e valor estimado total, com fórmulas (soma automática).
