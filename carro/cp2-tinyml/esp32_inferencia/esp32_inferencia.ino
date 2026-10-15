/*
 * ===================================================================
 * CP2 - TinyML — Inferência embarcada no ESP32 (esqueleto)
 * ===================================================================
 * ATUALIZAÇÃO: esse código já foi integrado de verdade em
 * ../../src/CameraWebServer.ino (seção "INFERÊNCIA TinyML (CP2 -
 * Edge Impulse)"). Este arquivo fica só como referência/documentação
 * isolada do que foi adicionado — para rodar o carrinho, use o
 * CameraWebServer.ino.
 *
 * Isso NÃO é um sketch pronto — é o ponto de partida pra integrar no
 * firmware principal (CameraWebServer.ino) depois que a biblioteca
 * do Edge Impulse estiver exportada.
 *
 * Passos antes de usar:
 *   1) Exportar a biblioteca Arduino no Edge Impulse (ver
 *      ../GUIA_EDGE_IMPULSE.md, seção 7) e instalar no Arduino IDE
 *      (Sketch > Include Library > Add .ZIP Library...).
 *   2) Trocar a linha do #include abaixo pelo nome real gerado
 *      (algo como "nome-do-projeto_inferencing.h" — aparece na tela
 *      de Deployment do Edge Impulse e no nome do arquivo .zip).
 *   3) Conferir em EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE se o tamanho
 *      bate com as 100 amostras usadas na coleta (deve bater
 *      sozinho, já que o CSV tinha a coluna de timestamp).
 *   4) Colar o conteúdo relevante (as funções abaixo + os #include)
 *      dentro do CameraWebServer.ino, chamando rodarInferenciaSeNecessario()
 *      dentro do loop() principal, junto com a lógica de ré que já existe.
 *   5) Dentro de agirComBaseNaClasse(), ligar a ação real no carrinho
 *      (reduzir velocidade / parar / acionar LED ou buzzer) usando as
 *      funções de motor que o kit já disponibiliza.
 * ===================================================================
 */

#include <Arthurpian-project-1_inferencing.h>

// Nota: na coleta de dados (coleta_dados.ino) o sensor foi ligado em
// GPIO2/GPIO4 porque esse sketch precisa imprimir o CSV inteiro no Serial
// Monitor, e GPIO1/GPIO3 (U0T/U0R) quebram a Serial quando configurados
// via pinMode(). Aqui, na inferência final (dentro do CameraWebServer.ino),
// pode manter GPIO1/GPIO3 como no CP1 — é seguro porque essa parte do
// código evita Serial.print() durante a leitura do sensor.
#define TRIG_PIN 1
#define ECHO_PIN 3

// Mesmo tamanho de janela usado na coleta (100 amostras, ver coleta_dados.ino)
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
static size_t amostras_coletadas = 0;
unsigned long ultimaLeituraMs = 0;
const unsigned long INTERVALO_MS = 50; // 20Hz, igual ao usado na coleta

// Funções do firmware principal (motor) que esse esqueleto vai acionar.
// Ajustar os nomes/assinaturas de acordo com o que o kit LAFVIN expõe.
extern void robot_stop();
// extern void robot_set_speed(int velocidade); // se existir algo assim no kit

long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH, 15000);
  if (duracao == 0) return 999;
  return duracao * 0.034 / 2;
}

// Desloca o buffer (janela deslizante) e insere a leitura mais nova no final.
void empurrarLeituraNoBuffer(float valor) {
  for (size_t i = 1; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i++) {
    features[i - 1] = features[i];
  }
  features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1] = valor;
  if (amostras_coletadas < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) amostras_coletadas++;
}

// Aplica no carrinho o resultado da classificação -- AQUI é onde a
// inferência "age de verdade" no robô (item exigido pelo CP2).
void agirComBaseNaClasse(const char* classe, float confianca) {
  if (strcmp(classe, "perigo") == 0) {
    robot_stop();                 // obstaculo muito perto -> para
    // TODO: acionar LED/buzzer de alerta aqui, se tiver

  } else if (strcmp(classe, "atencao") == 0) {
    // TODO: reduzir velocidade (ex: robot_set_speed(100) se existir,
    // ou ajustar o duty cycle do PWM nos pinos ENA/ENB do L298N)

  } else { // "livre"
    // segue normal -- nenhuma ação extra necessária
  }
}

// Roda a inferência quando o buffer já tiver amostras suficientes.
// Chamar isso dentro do loop() principal, a cada ciclo.
void rodarInferenciaSeNecessario() {
  if (millis() - ultimaLeituraMs < INTERVALO_MS) return;
  ultimaLeituraMs = millis();

  float d = medirDistancia();
  empurrarLeituraNoBuffer(d);

  if (amostras_coletadas < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) return; // ainda enchendo a janela

  signal_t signal;
  int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
  if (err != 0) return;

  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false /* debug */);
  if (res != EI_IMPULSE_OK) return;

  // Acha a classe com maior probabilidade
  float melhorProb = 0;
  const char* melhorClasse = "";
  for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    if (result.classification[i].value > melhorProb) {
      melhorProb = result.classification[i].value;
      melhorClasse = result.classification[i].label;
    }
  }

  agirComBaseNaClasse(melhorClasse, melhorProb);

  // Debug opcional -- cuidado: só imprimir aqui, nunca durante a
  // leitura do sensor (mesmo motivo do CP1: Serial e sensor dividem
  // os mesmos pinos U0T/U0R).
  // Serial.printf("Classe: %s (%.2f)\n", melhorClasse, melhorProb);
}

/*
 * Integração no CameraWebServer.ino:
 *
 *   void loop() {
 *     bool dandoRe = (digitalRead(IN2) == HIGH) || (digitalRead(IN4) == HIGH);
 *     if (dandoRe) {
 *       long distancia = medirDistancia();
 *       if (distancia > 0 && distancia <= DISTANCIA_MINIMA_CM) robot_stop();
 *     }
 *
 *     rodarInferenciaSeNecessario();  // <-- adicionar esta linha
 *
 *     delay(30);
 *   }
 *
 * Observação: como os dois (radar de ré simples do CP1 e a inferência
 * do CP2) usam o mesmo sensor, dá pra manter o radar de ré como
 * segurança extra e usar a classificação do TinyML pra reagir também
 * quando o carrinho está indo PRA FRENTE (coisa que o radar antigo
 * não fazia) -- é isso que torna a ação do modelo visivelmente
 * diferente do que já existia no CP1.
 */
