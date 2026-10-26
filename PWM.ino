/*
 * Control de Motor con Encoder y Serial (ESP32 Core v3.0+)
 * CORREGIDO: Orden de funciones para evitar errores de compilación
 */

#include <Arduino.h>

// --- Definición de Pines ---
const int PIN_IN1 = 4;
const int PIN_IN2 = 5;
const int PIN_ENA = 6;  // Pin PWM

// Pines del Encoder
const int PIN_ENC_A = 13;
const int PIN_ENC_B = 14;

// --- Configuración del Motor ---
const float RELACION_TRANSMISION = 34.0; 
const float PULSOS_POR_VUELTA = 48.0 * RELACION_TRANSMISION;

volatile long posicionEncoder = 0;

// Configuración PWM
const int pwmFreq = 30000;
const int pwmResolution = 8; 

// --- 1. DEFINICIÓN DE FUNCIONES BASE (Arriba para evitar errores) ---

// Escribe el PWM en el pin
void escribirPWM(int pin, int duty) {
  ledcWrite(pin, duty);
}

void frenarMotor() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  escribirPWM(PIN_ENA, 0);
}

void controlarMotor(int dir, int pwm) {
  if (dir == 1) { // Adelante
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
  } else if (dir == -1) { // Atrás
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
  } else {
    frenarMotor();
    return;
  }
  escribirPWM(PIN_ENA, pwm); 
}

// --- 2. INTERRUPCIÓN DEL ENCODER ---

void IRAM_ATTR leerEncoder() {
  int b = digitalRead(PIN_ENC_B);
  if (digitalRead(PIN_ENC_A) == HIGH) {
    if (b == LOW) posicionEncoder++;
    else posicionEncoder--;
  } else {
    if (b == LOW) posicionEncoder--;
    else posicionEncoder++;
  }
}

// --- 3. FUNCIONES DE POSICIÓN ---

void irAPosicionAbsoluta(long objetivo, int velocidad) {
  int direccion = 0;
  
  // Mientras la distancia al objetivo sea mayor a 15 pulsos
  while (abs(posicionEncoder - objetivo) > 15) { 
    
    if (posicionEncoder < objetivo) direccion = 1;
    else direccion = -1;

    long diferencia = abs(posicionEncoder - objetivo);
    int velActual = velocidad;
    
    // Rampa de frenado: si faltan menos de 200 pulsos, baja la velocidad
    if (diferencia < 200) {
      velActual = map(diferencia, 0, 200, 70, velocidad);
    }

    controlarMotor(direccion, velActual);
    delay(1); // Estabilidad
  }
  frenarMotor();
}

void moverGrados(float grados, int velocidad) {
  long pulsosObjetivo = (grados / 360.0) * PULSOS_POR_VUELTA;
  long posicionFinal = posicionEncoder + pulsosObjetivo;
  irAPosicionAbsoluta(posicionFinal, velocidad);
}

// --- 4. PROCESAMIENTO DE COMANDOS ---

void procesarComando(String input) {
  input.trim();
  input.toUpperCase();
  
  int espacio = input.indexOf(' ');
  String cmd = (espacio == -1) ? input : input.substring(0, espacio);
  int valor = (espacio == -1) ? 0 : input.substring(espacio + 1).toInt();
  
  if (cmd == "F" || cmd == "B") {
    valor = constrain(valor, 0, 255);
  }

  if (cmd == "F") {
    Serial.printf("ADELANTE (Vel: %d)\n", valor);
    controlarMotor(1, valor);
  } 
  else if (cmd == "B") {
    Serial.printf("ATRAS (Vel: %d)\n", valor);
    controlarMotor(-1, valor);
  } 
  else if (cmd == "STOP") {
    Serial.println("PARAR");
    frenarMotor();
  } 
  else if (cmd == "G") {
    Serial.printf("MOVER %d GRADOS\n", valor);
    moverGrados(valor, 200); 
    Serial.println("Fin Movimiento.");
  }
  else if (cmd == "POS") {
    Serial.printf("Encoder: %ld\n", posicionEncoder);
  }
}

// --- 5. SETUP Y LOOP ---

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), leerEncoder, CHANGE);

  // Configuración PWM (ESP32 Core v3.0)
  ledcAttach(PIN_ENA, pwmFreq, pwmResolution);
  
  Serial.println("\n=== SISTEMA LISTO ===");
  Serial.println("Comandos: F 200 | B 200 | STOP | G 90 | POS");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    procesarComando(input);
  }
}