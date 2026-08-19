/* ============================================================
   Carrinho Bluetooth - ESP32 (BLE) + L298N + Dabble
   com Sensor de Ré (HC-SR04)
   Project-based Maker Lab
   ------------------------------------------------------------
   PINAGEM CONFORME O SLIDE:
   L298N        ESP32
   IN1  ......  GPIO 16
   IN2  ......  GPIO 17
   ENA  ......  GPIO 5    (Enable Motor A)
   IN3  ......  GPIO 18
   IN4  ......  GPIO 19
   ENB  ......  GPIO 6    (Enable Motor B)  <-- ver AVISO abaixo

   ACRESCENTADO (não consta no slide):
   HC-SR04 TRIG ...... GPIO 25
   HC-SR04 ECHO ...... GPIO 26   (divisor 1k/2k: 5V -> 3.3V)
   Buzzer ............ GPIO 27   (buzzer ATIVO)
   ------------------------------------------------------------
   AVISO SOBRE O GPIO 6:
   No ESP32 os GPIOs 6 a 11 ligam-se à memória flash SPI interna.
   Se a placa resetar em loop ou não iniciar, troque a linha
   #define ENB 6  por  #define ENB 21  (ou 4 / 23) e mova o fio.
   ============================================================ */

#include <DabbleESP32.h>

// ---------- Motores (pinagem do slide) ----------
#define IN1 16
#define IN2 17
#define ENA 5
#define IN3 18
#define IN4 19
#define ENB 6        // se a placa não bootar, use 21

// ---------- Sensor de ré ----------
#define TRIG 25
#define ECHO 26
#define BUZZER 27

// ---------- Parâmetros de segurança ----------
const int DIST_PARADA = 20;   // cm -> para o carro
const int DIST_ALERTA = 40;   // cm -> apenas apita
const unsigned long INTERVALO_LEITURA = 60;   // ms

// ---------- Estado ----------
bool emRe = false;
bool reBloqueada = false;
bool buzzerLigado = false;
long distancia = 999;
unsigned long ultimaLeitura = 0;
unsigned long ultimoBip = 0;

// ============================================================
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  digitalWrite(ENA, HIGH);   // motores habilitados
  digitalWrite(ENB, HIGH);
  stopMotors();

  Dabble.begin("My Bluetooth Car");
  Serial.println("Carrinho pronto.");
}

// ============================================================
void loop() {
  Dabble.processInput();

  // 1) Comandos do GamePad (polling - compatível com todas as
  //    versões da biblioteca Dabble)
  if      (GamePad.isUpPressed())    moveForward();
  else if (GamePad.isDownPressed())  moveBackward();
  else if (GamePad.isLeftPressed())  turnLeft();
  else if (GamePad.isRightPressed()) turnRight();
  else                               parar();

  // 2) Sensor de ré (não bloqueante)
  if (millis() - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = millis();
    distancia = lerDistancia();
    verificarRe();
  }
}

// ============================================================
//  SENSOR DE RÉ
// ============================================================
long lerDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH, 25000);   // timeout ~4 m
  if (duracao == 0) return 999;                // nada detectado
  return duracao * 0.034 / 2;                  // cm
}

void verificarRe() {
  if (!emRe) {                          // só atua na marcha à ré
    reBloqueada = false;
    desligarBuzzer();
    return;
  }

  if (distancia <= DIST_PARADA) {       // PERIGO -> para o carro
    stopMotors();
    reBloqueada = true;
    ligarBuzzer();                      // som contínuo
    Serial.println("Obstaculo! Re bloqueada.");
  }
  else if (distancia <= DIST_ALERTA) {  // ATENÇÃO -> bip intermitente
    reBloqueada = false;
    if (millis() - ultimoBip >= 250) {
      ultimoBip = millis();
      buzzerLigado = !buzzerLigado;
      digitalWrite(BUZZER, buzzerLigado);
    }
  }
  else {                                // caminho livre
    reBloqueada = false;
    desligarBuzzer();
  }
}

void ligarBuzzer()   { buzzerLigado = true;  digitalWrite(BUZZER, HIGH); }
void desligarBuzzer(){ buzzerLigado = false; digitalWrite(BUZZER, LOW);  }

// ============================================================
//  MOVIMENTOS
// ============================================================
void moveForward() {
  emRe = false;
  reBloqueada = false;
  desligarBuzzer();
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  emRe = true;

  if (reBloqueada) {      // já detectou obstáculo -> não anda
    stopMotors();
    return;
  }

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  emRe = false;
  desligarBuzzer();
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  emRe = false;
  desligarBuzzer();
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void parar() {
  emRe = false;
  desligarBuzzer();
  stopMotors();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
