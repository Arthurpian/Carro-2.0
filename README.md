# Organização do Projeto — Réplica RC Relâmpago McQueen (V0.3)

Fabrício Gutierrez Saavedra - RM97631
Márcio Gastaldi - RM98811
Arthur Bessa Pian - RM99215
Davi Desenzi - RM550849
João Victor - RM551410
---

## Sumário
 
- [Hardware usado](#hardware-usado)
- [Como o programa funciona](#como-o-programa-funciona)
- [Carroceria 3D](#carroceria-3d)
- [Como usar](#como-usar)
- [Testes e resultados](#testes-e-resultados)
- [Evidências finais](#evidências-finais)
- [Estrutura do repositório](#estrutura-do-repositório)
## Hardware usado
 
- **Kit base:** [LAFVIN 4WD Smart Robot Car Kit ESP32-CAM, WiFi](https://a.aliexpress.com/_mM8Qqan) — chassi acrílico 4WD, 4 motores TT + driver L298N, placa ESP32-CAM, suporte para pilhas.
- **Sensor ultrassônico HC-SR04** — comprado separadamente, não vem no kit. Usado como sensor de ré.
- **Impressora 3D:** Bambu Lab A1 — usada para imprimir a carroceria própria do carrinho.
Medidas reais do chassi (usadas para desenhar a carroceria):
 
| Medida | Valor |
|---|---|
| Comprimento (frente-trás) | 152 mm |
| Largura roda-a-roda (fora a fora) | 220 mm |
| Diâmetro da roda | 65 mm |
| Vão roda ↔ placa central (cada lado) | 10 mm |
| Altura total até o topo da antena | 157 mm |
 
## Como o programa funciona
 
O firmware roda inteiro na placa **ESP32-CAM** e faz três coisas ao mesmo tempo:
 
**1) Controle dos motores.** O ESP32 comanda o driver L298N, que aciona os 4 motores TT (tração 4WD). O comando de frente/ré/esquerda/direita chega pela página web de controle (parte pronta do kit LAFVIN — funções `robot_setup()` e `robot_stop()`), e o driver liga os pinos `IN1`–`IN4` de acordo com a direção desejada. O programa usa a leitura desses mesmos pinos (`IN2`/`IN4`) pra saber, em tempo real, se o carrinho está de fato dando ré — sem precisar de nenhuma variável extra de estado.
 
**2) Comunicação sem fio.** O ESP32-CAM não se conecta a uma rede existente: ele **cria a própria rede WiFi** (modo Access Point, SSID `ESP32-CAM Robot`, sem senha). O celular ou notebook conecta direto nessa rede e abre o IP do robô no navegador, o que carrega a página de controle (setas de direção) junto com o vídeo da câmera ao vivo (stream MJPEG). Não depende de internet nem de roteador — funciona em qualquer lugar.
 
**3) Sensor de ré (adição nossa).** Um HC-SR04 foi ligado nos pinos `U0T`/`U0R` da ESP32-CAM (os únicos GPIOs livres nessa placa, já que câmera e motores ocupam quase todos os outros). A lógica funciona como um "radar de ré": o sensor só liga quando o carrinho está de fato indo pra trás (lendo os pinos do driver). Se detectar um obstáculo a 20 cm ou menos, chama `robot_stop()` e trava o movimento na hora, sobrepondo o comando do celular — a frente do carrinho fica livre o tempo todo, sem nenhuma interferência do sensor.
 
Código completo comentado: (src/CameraWebServer.ino).
 
> **Nota:** as funções `robot_setup()`, `robot_stop()` e `startCameraServer()` ficam em outras abas do sketch, fornecidas prontas pelo tutorial do kit LAFVIN (controle de motor e servidor de câmera/streaming). Elas não foram alteradas — o que adicionamos foi só a parte do sensor de ré.
 
## Carroceria 3D
 
Desenhamos uma carroceria própria (estilo buggy de corrida, design original — não é uma réplica do personagem licenciado, já que isso é protegido por direitos autorais) para encaixar por cima do chassi acrílico do kit. Ela é impressa em 2 peças (frente + traseira) que se encaixam com uma espiga/soquete e são coladas.

 
Arquivos: pasta [`3d-model/`](3d-model/) (STL das duas peças + script Python parametrizado usado para gerar/ajustar o modelo).
 
## Como usar
 
1. Ligue o carrinho (pilhas/bateria conectadas).
2. No celular ou notebook, conecte na rede WiFi **`ESP32-CAM Robot`** (sem senha).
3. Abra o navegador no endereço IP mostrado no Monitor Serial ao ligar (também aparece na tela inicial da página de controle, geralmente `192.168.4.1`).
4. A página carrega o vídeo ao vivo da câmera e as setas de direção — use-as para mover o carrinho.
5. Ao dar ré perto de um obstáculo (≤ 20 cm), o carrinho trava sozinho, mesmo segurando o comando de ré — solte e ande pra frente pra liberar de novo.
*(Adicionar aqui prints ou GIF da página de controle, se quiser deixar mais visual.)*
 
## Testes e resultados
 
| Data/Versão | Teste / mudança | Problema encontrado | Correção | Resultado |
|---|---|---|---|---|
| V0.1 | Primeira versão, controle via Bluetooth | — | — | Funcionava, mas trocamos por WiFi+câmera na V0.3 |
| V0.2 | Adição do sensor de ré | — | — | Sensor detectando obstáculos na ré |
| V0.3 | Troca para o kit LAFVIN 4WD + ESP32-CAM | Sensor ligado nos pinos `U0T`/`U0R`, que são os mesmos da Serial | Os caracteres enviados por `Serial.print()` geravam pulsos elétricos nesses pinos e atrapalhavam a leitura do `ECHO`, fazendo o sensor ler distância errada | Removemos todo `Serial.print()` de dentro do `loop()` enquanto o carrinho está em marcha à ré; sensor passou a ler de forma confiável |
| V0.3 | Teste do radar de ré | Sensor podia travar o carrinho mesmo andando pra frente, se ligado o tempo todo | Sensor agora só é lido quando os pinos do driver indicam marcha à ré | Frente 100% livre, ré protegida |
 
> Preencher com testes adicionais conforme forem feitos (alcance real do WiFi, autonomia da bateria, ajuste da distância mínima de segurança, testes com a carroceria nova montada, etc.).
 
## Evidências finais
 
**Vídeo do funcionamento:** **
 
**Demonstração do controle remoto:** **
 
**Demonstração do sensor:** **
