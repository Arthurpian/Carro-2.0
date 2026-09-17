# Backlog — Réplica RC Relâmpago McQueen (V0.3)

**Integrantes:** [nome dos integrantes do grupo] — *substituir pelos nomes reais e realocar os responsáveis abaixo*

## Tarefas

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

## Dependências

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
