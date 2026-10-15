/*
 * ===================================================================
 * CP2 - TinyML — Coleta de dados do sensor ultrassônico
 * ===================================================================
 * Sketch SEPARADO do firmware principal do carrinho. Usar só durante
 * a coleta de dados para o Edge Impulse — depois volta pro firmware
 * normal (CameraWebServer.ino) pra fazer a inferência embarcada.
 *
 * IMPORTANTE sobre os pinos: o sensor foi testado primeiro nos pinos
 * U0T/U0R (GPIO1/GPIO3), mas esses são os MESMOS pinos usados pela
 * Serial — configurar pinMode() neles "rouba" a função de Serial do
 * ESP32 e nenhuma mensagem aparece mais no Serial Monitor. Por isso,
 * para este sketch de coleta (que PRECISA imprimir o CSV), o sensor
 * foi religado em dois pinos livres:
 *   TRIG -> GPIO2
 *   ECHO -> GPIO4
 * (os outros pinos já estavam ocupados pelo driver dos motores).
 *
 * MODO CONTÍNUO: este sketch roda em loop(), repetindo a captura
 * sozinho a cada ~13s, sem precisar resetar a placa manualmente —
 * isso evita o problema de o Serial Monitor perder a única execução
 * de um setup() que roda uma vez só.
 *
 * COMO USAR:
 *   1) Faça o upload deste sketch (com o jumper IO0-GND, como de
 *      costume), depois remova o jumper e reinicie a placa.
 *   2) Abra o Serial Monitor (115200 baud) — a captura já fica
 *      rodando sozinha, repetindo pra sempre.
 *   3) A cada ciclo aparece "Proxima captura #N em 5s." — use esses
 *      5s + a contagem 3-2-1 pra posicionar o objeto na distância da
 *      classe desejada (livre: >60cm / atencao: 20-60cm / perigo: <20cm).
 *   4) Durante os 5s de "Capturando...", não mexa no objeto.
 *   5) Quando aparecer "=== FIM DA CAPTURA ===", copie o bloco CSV
 *      (da linha "timestamp_ms,distancia_cm" até antes de "FIM DA
 *      CAPTURA") para um arquivo nome_da_classe_NN.csv.
 *   6) Repita pros próximos ciclos, variando a classe/posição do
 *      objeto, até ter uns 8-10 capturas limpas por classe.
 * ===================================================================
 */

#define TRIG_PIN 2
#define ECHO_PIN 4

#define N_SAMPLES 100          // amostras por captura
#define SAMPLE_INTERVAL_MS 50  // 20Hz -> janela de 5s por captura (100 x 50ms)

float buffer_cm[N_SAMPLES];
int numeroCaptura = 0;

long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 15000);
  if (duracao == 0) return 999; // nada detectado -> "bem longe"
  return duracao * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println();
  Serial.println(F("=== CP2 - Coleta de dados do sensor ultrassonico (modo continuo) ==="));
  Serial.println(F("Uma nova captura comeca sozinha a cada poucos segundos."));
}

void loop() {
  numeroCaptura++;
  Serial.println();
  Serial.print(F("--- Proxima captura #"));
  Serial.print(numeroCaptura);
  Serial.println(F(" em 5s. Posicione o objeto agora. ---"));
  delay(5000);

  Serial.println(F("3..."));
  delay(1000);
  Serial.println(F("2..."));
  delay(1000);
  Serial.println(F("1..."));
  delay(1000);
  Serial.println(F("Capturando (5s, nao mexa no objeto)..."));

  for (int i = 0; i < N_SAMPLES; i++) {
    buffer_cm[i] = medirDistancia();
    delay(SAMPLE_INTERVAL_MS);
  }

  Serial.println(F("timestamp_ms,distancia_cm"));
  for (int i = 0; i < N_SAMPLES; i++) {
    Serial.print(i * SAMPLE_INTERVAL_MS);
    Serial.print(",");
    Serial.println(buffer_cm[i], 1);
  }
  Serial.println(F("=== FIM DA CAPTURA ==="));
  Serial.println(F("Copie as linhas do CSV acima (da linha 'timestamp_ms,distancia_cm' ate antes de 'FIM DA CAPTURA') para um arquivo .csv"));
}
