/*
 * ===================================================================
 * Projeto: Carrinho RC "Relâmpago" — LAFVIN 4WD + ESP32-CAM
 * Arquivo: CameraWebServer.ino  (aba principal do sketch)
 * ===================================================================
 *
 * Este é o firmware que roda na placa ESP32-CAM do kit LAFVIN 4WD.
 * Ele faz três coisas ao mesmo tempo:
 *
 *   1) Cria uma rede WiFi própria (Access Point) e sobe um servidor
 *      web com o streaming da câmera + a página de controle remoto
 *      (isso já vem pronto no exemplo do fabricante — funções
 *      robot_setup(), robot_stop() e startCameraServer(), definidas
 *      nas outras abas do sketch fornecidas pelo kit LAFVIN).
 *
 *   2) Controla os motores (frente/ré/esquerda/direita) a partir dos
 *      comandos recebidos pela página web, via robot_setup()/robot_stop().
 *
 *   3) ADIÇÃO NOSSA: um sensor ultrassônico (HC-SR04) que funciona só
 *      durante a marcha à ré, travando o carrinho automaticamente se
 *      detectar um obstáculo a menos de 20 cm — como um "radar de ré".
 *
 * O código original de câmera/servidor é do fabricante (kit LAFVIN,
 * baseado no exemplo "CameraWebServer" da Espressif). A lógica do
 * sensor de ré (a partir da seção "SENSOR ULTRASSÔNICO" abaixo) foi
 * adicionada por nós para o projeto.
 * ===================================================================
 */

#include "esp_camera.h"
#include <WiFi.h>

// Modelo da placa de câmera (AI-Thinker é o modelo usado no kit LAFVIN)
#define CAMERA_MODEL_AI_THINKER

// Nome e senha da rede WiFi que o próprio robô cria (modo Access Point).
// Sem senha ("") = rede aberta; o celular/notebook conecta direto nela.
const char* ssid1 = "ESP32-CAM Robot";
const char* password1 = "";

// Funções de controle do robô, definidas nas outras abas do sketch
// (vêm prontas do exemplo do fabricante LAFVIN — controlam os motores
// e o servidor HTTP que recebe os comandos da página web de controle).
extern void robot_stop();
extern void robot_setup();

// =====================================
// SENSOR ULTRASSÔNICO (adição nossa) — radar de ré
// =====================================
// Sensor HC-SR04 comprado à parte (não vem no kit).
// Ligado nos pinos U0T/U0R da ESP32-CAM (os mesmos usados pela
// Serial), porque são os únicos GPIOs livres nessa placa depois que
// a câmera e os motores já ocupam quase todos os outros.
#define TRIG_PIN 1              // TRIG do sensor -> GPIO1 (U0T)
#define ECHO_PIN 3              // ECHO do sensor -> GPIO3 (U0R)
#define DISTANCIA_MINIMA_CM 20  // Distância de segurança para travar a ré

// Pinos do driver de motor (L298N) que indicam quando as rodas
// traseiras estão girando em marcha à ré. Usados para saber se o
// carrinho ESTÁ dando ré agora, sem precisar de outra variável de
// estado (lê direto o pino de saída do driver).
#define IN2 13
#define IN4 15

// =====================================
// Pinos da câmera (padrão da placa AI-Thinker / kit LAFVIN)
// =====================================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

extern int gpLed = 4;        // LED/flash da câmera
extern String WiFiAddr = ""; // guarda o IP do robô pra outras abas usarem

void startCameraServer(); // definida em app_httpd (aba do fabricante)

// =====================================
// medirDistancia()
// Dispara o pulso ultrassônico e devolve a distância em cm.
// Se não detectar nada dentro do timeout, devolve 999 (bem longe).
// =====================================
long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout de 15ms: se não ouvir o eco nesse tempo, desiste e
  // segue em frente (evita travar o loop do robô esperando o pino).
  long duracao = pulseIn(ECHO_PIN, HIGH, 15000);

  if (duracao == 0) return 999; // nada detectado -> "livre"

  return duracao * 0.034 / 2; // fórmula padrão: distância = (tempo x vel. do som) / 2
}

// =====================================
// setup()
// =====================================
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Pinos do sensor de ré
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Pinos do driver, usados aqui só para LEITURA (checar se está
  // em marcha à ré) — quem manda neles de verdade é robot_setup()/
  // a lógica de motor do kit.
  pinMode(IN2, OUTPUT);
  pinMode(IN4, OUTPUT);

  robot_setup(); // inicializa motores + servidor de controle (fabricante)
  pinMode(gpLed, OUTPUT);
  digitalWrite(gpLed, LOW);

  // --- Configuração da câmera (padrão do kit) ---
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Se a placa tiver PSRAM (é o caso da AI-Thinker), usa qualidade
  // maior; senão cai pra uma resolução menor pra não faltar memória.
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA); // resolução do streaming ao vivo
  s->set_hmirror(s, 0);
  s->set_vflip(s, 1); // imagem invertida verticalmente por causa do jeito que a câmera fica montada no chassi

  // --- Sobe o Access Point (o robô vira o próprio roteador WiFi) ---
  WiFi.softAP(ssid1, password1);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.softAPIP());
  WiFiAddr = WiFi.softAPIP().toString();
  Serial.println("' to connect");

  startCameraServer(); // sobe o servidor HTTP (stream + comandos de controle)
  digitalWrite(33, LOW);
}

// =====================================
// loop()
// "Radar de ré": só ativa o sensor quando o carrinho está dando ré.
// =====================================
void loop() {
  // 1) Verifica eletricamente (lendo os pinos do driver) se os
  //    motores traseiros estão recebendo o comando de marcha à ré.
  bool dandoRe = (digitalRead(IN2) == HIGH) || (digitalRead(IN4) == HIGH);

  // 2) Só liga o sensor enquanto está dando ré — assim a frente do
  //    carrinho fica 100% livre e o sensor não atrapalha em nada
  //    quando ele está andando pra frente ou parado.
  if (dandoRe) {
    long distancia = medirDistancia();

    // 3) Se detectar algo a 20cm ou menos, força a parada.
    if (distancia > 0 && distancia <= DISTANCIA_MINIMA_CM) {
      robot_stop(); // usa a função oficial de freio, sobrepõe o comando do celular

      // IMPORTANTE: não colocar Serial.print() aqui dentro.
      // PROBLEMA QUE ENCONTRAMOS: o sensor está ligado nos pinos
      // U0T/U0R, que são os MESMOS pinos usados pela Serial. Cada
      // caractere que a Serial manda gera pulsos elétricos nesses
      // pinos e "cega"/atrapalha a leitura do ECHO do sensor,
      // fazendo ele dar distância errada bem na hora de frear.
      // CORREÇÃO: tiramos qualquer Serial.print() de dentro do
      // loop principal quando o carrinho está em marcha à ré.
    }
  }

  delay(30); // pequena pausa entre leituras (~33 verificações por segundo)
}
