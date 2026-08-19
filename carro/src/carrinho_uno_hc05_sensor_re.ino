/* ============================================================
   Carrinho Bluetooth - Arduino Uno + HC-05 + L298N
   com Sensor de Ré (HC-SR04)
   Project-based Maker Lab
   ------------------------------------------------------------
   L298N        Arduino Uno
   IN1  ......  D5   (PWM)
   IN2  ......  D6   (PWM)
   IN3  ......  D9   (PWM)
   IN4  ......  D10  (PWM)
   ENA/ENB ...  jumpers instalados (velocidade máxima)

   HC-05        Arduino Uno
   TXD  ......  D2   (RX do SoftwareSerial)
   RXD  ......  D3   via divisor 1k/2k  (TX do SoftwareSerial)
   VCC  ......  5V     GND ...... GND

   HC-SR04 (traseira)
   TRIG ......  D7
   ECHO ......  D8
   BUZZER ....  D11
   ------------------------------------------------------------
   Comandos recebidos por Bluetooth (app de joystick serial):
   F = frente   B = ré   L = esquerda   R = direita   S = parar
   ============================================================ */

#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);   // RX, TX

// ---------- Motores ----------
#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

// ---------- Sensor de ré ----------
#define TRIG 7
#define ECHO 8
#define BUZZER 11

// ---------- Parâmetros de segurança ----------
const int DIST_PARADA = 20;   // cm -> para o carro
const int DIST_ALERTA = 40;   // cm -> apenas apita
const unsigned long INTERVALO_LEITURA = 60;  // ms

// ---------- Estado ----------
bool emRe = false;
bool reBloqueada = false;
long distancia = 999;
unsigned long ultimaLeitura = 0;
unsigned long ultimoBip = 0;

// ============================================================
void setup() {
  Serial.begin(9600);
  BT.begin(9600);            // baud padrão do HC-05 em modo comunicação

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);

  stopMotors();
  Serial.println("Carrinho pronto. Aguardando comandos...");
}

// ============================================================
void loop() {
  // 1) Comandos do Bluetooth
  if (BT.available()) {
    char comando = BT.read();
    executarComando(comando);
  }

  // 2) Sensor de ré (não bloqueante)
  if (millis() - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = millis();
    distancia = lerDistancia();
    verificarRe();
  }
}

// ============================================================
void executarComando(char c) {
  switch (c) {
    case 'F': case 'f': moveForward();  break;
    case 'B': case 'b': moveBackward(); break;
    case 'L': case 'l': turnLeft();     break;
    case 'R': case 'r': turnRight();    break;
    case 'S': case 's': emRe = false;
                        noTone(BUZZER);
                        stopMotors();   break;
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
  if (!emRe) {                       // só atua durante a marcha à ré
    reBloqueada = false;
    noTone(BUZZER);
    return;
  }

  if (distancia <= DIST_PARADA) {    // PERIGO -> para
    stopMotors();
    reBloqueada = true;
    tone(BUZZER, 2000);
    BT.println("Obstaculo! Re bloqueada.");
    Serial.println("Obstaculo! Re bloqueada.");
  }
  else if (distancia <= DIST_ALERTA) {  // ATENÇÃO -> bip intermitente
    reBloqueada = false;
    if (millis() - ultimoBip >= 250) {
      ultimoBip = millis();
      tone(BUZZER, 1500, 80);
    }
  }
  else {                                // livre
    reBloqueada = false;
    noTone(BUZZER);
  }
}

// ============================================================
//  MOVIMENTOS
// ============================================================
void moveForward() {
  emRe = false;
  reBloqueada = false;
  noTone(BUZZER);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  emRe = true;
  distancia = lerDistancia();        // confere ANTES de arrancar

  if (distancia <= DIST_PARADA) {
    reBloqueada = true;
    stopMotors();
    tone(BUZZER, 2000, 300);
    BT.println("Re bloqueada: obstaculo atras.");
    return;
  }

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  emRe = false;
  noTone(BUZZER);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  emRe = false;
  noTone(BUZZER);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
