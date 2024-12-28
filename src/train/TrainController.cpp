#include "TrainController.h"
#include <Arduino.h>

TrainController::TrainController()
{
}

void TrainController::initialize()
{
  // Configurar pines como salidas
  pinMode(PIN_MOTOR_A, OUTPUT);
  pinMode(PIN_MOTOR_B, OUTPUT);
  pinMode(PIN_VELOCIDAD, OUTPUT);

  // Configurar canal PWM (ESP32)
  ledcSetup(0, 5000, 8); // Canal 0, 5000Hz, 8 bits de resolución
  ledcAttachPin(PIN_VELOCIDAD, 0);

  // Iniciar detenido
  stop();
}

int TrainController::setSpeed(int speed)
{
  // Primero aplicamos los límites absolutos
  speed = abs(constrain(speed, 0, 255));

  // Luego aplicamos los límites configurados
  if (speed > 0)
  {
    speed = constrain(speed,
                      TrainConfig::getVelocidadMinima(),
                      TrainConfig::getVelocidadMaxima());
  }

  TrainConfig::setCurrentSpeed(speed);

  return speed;
}

void TrainController::forward(int finalSpeed)
{
  finalSpeed = setSpeed(finalSpeed);

  // Si está retrocediendo o en fade de retroceso, primero hacer fade hasta detenerse
  if ((isOnMovement || isFade) && digitalRead(PIN_MOTOR_B) == HIGH)
  {
    targetSpeed = 0;
    initialSpeed = abs(currentSpeed);
    timeStartFade = millis();
    isFade = true;
    isChangeDirection = true;
    pendingDirection = DIRECTION_FORWARD;
    pendingSpeed = finalSpeed;
    return;
  }

  // Comportamiento normal si no estaba retrocediendo
  initialSpeed = abs(currentSpeed);
  targetSpeed = finalSpeed;
  digitalWrite(PIN_MOTOR_A, HIGH);
  digitalWrite(PIN_MOTOR_B, LOW);
  timeStartFade = millis();
  isFade = true;
  isOnMovement = true;
}

void TrainController::backward(int finalSpeed)
{
  finalSpeed = setSpeed(finalSpeed);

  // Si está avanzando o en fade de avance, primero hacer fade hasta detenerse
  if ((isOnMovement || isFade) && digitalRead(PIN_MOTOR_A) == HIGH)
  {
    targetSpeed = 0;
    initialSpeed = abs(currentSpeed);
    timeStartFade = millis();
    isFade = true;
    isChangeDirection = true;
    pendingDirection = DIRECTION_BACKWARD;
    pendingSpeed = finalSpeed;
    return;
  }

  // Comportamiento normal si no estaba avanzando
  initialSpeed = abs(currentSpeed);
  targetSpeed = finalSpeed;
  digitalWrite(PIN_MOTOR_A, LOW);
  digitalWrite(PIN_MOTOR_B, HIGH);
  timeStartFade = millis();
  isFade = true;
  isOnMovement = true;
}

void TrainController::stop()
{
  // Si ya está detenido, no hacer nada
  if (!isOnMovement && currentSpeed == 0)
    return;

  // Solo cancelar cambio de dirección si no estamos en medio de uno
  if (!isFade)
  {
    isChangeDirection = false;
  }

  // Iniciar fade hasta velocidad 0
  initialSpeed = abs(currentSpeed);
  targetSpeed = 0;
  timeStartFade = millis();
  isFade = true;
  isOnMovement = false; // Marcamos como no en movimiento inmediatamente
}

void TrainController::updateFade()
{
  if (!isFade)
    return;

  unsigned long tiempoActual = millis();
  unsigned long tiempoTranscurrido = tiempoActual - timeStartFade;

  if (tiempoTranscurrido >= fadeDurarionMs)
  {
    // Fade completado
    currentSpeed = setSpeed(targetSpeed);
    ledcWrite(0, currentSpeed);
    isFade = false;

    // Si estábamos esperando cambio de dirección y llegamos a velocidad 0
    if (isChangeDirection && currentSpeed == 0)
    {
      // Configurar nueva dirección
      if (pendingDirection == DIRECTION_FORWARD)
      {
        digitalWrite(PIN_MOTOR_A, HIGH);
        digitalWrite(PIN_MOTOR_B, LOW);
      }
      else
      {
        digitalWrite(PIN_MOTOR_A, LOW);
        digitalWrite(PIN_MOTOR_B, HIGH);
      }

      // Iniciar fade hacia la velocidad pendiente
      initialSpeed = 0;
      targetSpeed = pendingSpeed;
      timeStartFade = millis();
      isFade = true;
      isChangeDirection = false;
      isOnMovement = true;
    }
    return;
  }

  // Calcular velocidad intermedia
  int velocidadTemp = map(tiempoTranscurrido, 0, fadeDurarionMs,
                          initialSpeed, targetSpeed);
  velocidadTemp = setSpeed(velocidadTemp);
  ledcWrite(0, velocidadTemp);
  currentSpeed = velocidadTemp;
}