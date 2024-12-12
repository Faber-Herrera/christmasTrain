#include "TrainController.h"
#include <Arduino.h>

TrainController::TrainController()
{
}

void TrainController::inicializar()
{
  // Configurar pines como salidas
  pinMode(PIN_MOTOR_A, OUTPUT);
  pinMode(PIN_MOTOR_B, OUTPUT);
  pinMode(PIN_VELOCIDAD, OUTPUT);

  // Configurar canal PWM (ESP32)
  ledcSetup(0, 5000, 8); // Canal 0, 5000Hz, 8 bits de resolución
  ledcAttachPin(PIN_VELOCIDAD, 0);

  // Iniciar detenido
  detener();
}

void TrainController::avanzar(int velocidadFinal)
{
  velocidadFinal = abs(constrain(velocidadFinal, 0, 255));

  // Si está retrocediendo o en fade de retroceso, primero hacer fade hasta detenerse
  if ((enMovimiento || enFade) && digitalRead(PIN_MOTOR_B) == HIGH)
  {
    velocidadObjetivo = 0;
    velocidadInicial = abs(velocidadActual);
    tiempoInicioFade = millis();
    enFade = true;
    esperandoCambioDireccion = true;
    direccionPendiente = DIRECCION_ADELANTE;
    velocidadPendiente = velocidadFinal;
    return;
  }

  // Comportamiento normal si no estaba retrocediendo
  velocidadInicial = abs(velocidadActual);
  velocidadObjetivo = velocidadFinal;
  digitalWrite(PIN_MOTOR_A, HIGH);
  digitalWrite(PIN_MOTOR_B, LOW);
  tiempoInicioFade = millis();
  enFade = true;
  enMovimiento = true;
}

void TrainController::retroceder(int velocidadFinal)
{
  velocidadFinal = abs(constrain(velocidadFinal, 0, 255));

  // Si está avanzando o en fade de avance, primero hacer fade hasta detenerse
  if ((enMovimiento || enFade) && digitalRead(PIN_MOTOR_A) == HIGH)
  {
    velocidadObjetivo = 0;
    velocidadInicial = abs(velocidadActual);
    tiempoInicioFade = millis();
    enFade = true;
    esperandoCambioDireccion = true;
    direccionPendiente = DIRECCION_ATRAS;
    velocidadPendiente = velocidadFinal;
    return;
  }

  // Comportamiento normal si no estaba avanzando
  velocidadInicial = abs(velocidadActual);
  velocidadObjetivo = velocidadFinal;
  digitalWrite(PIN_MOTOR_A, LOW);
  digitalWrite(PIN_MOTOR_B, HIGH);
  tiempoInicioFade = millis();
  enFade = true;
  enMovimiento = true;
}

void TrainController::detener()
{
  // Si ya está detenido, no hacer nada
  if (!enMovimiento && velocidadActual == 0)
    return;

  // Solo cancelar cambio de dirección si no estamos en medio de uno
  if (!enFade)
  {
    esperandoCambioDireccion = false;
  }

  // Iniciar fade hasta velocidad 0
  velocidadInicial = abs(velocidadActual);
  velocidadObjetivo = 0;
  tiempoInicioFade = millis();
  enFade = true;
  enMovimiento = false; // Marcamos como no en movimiento inmediatamente
}

void TrainController::actualizarFade()
{
  if (!enFade)
    return;

  unsigned long tiempoActual = millis();
  unsigned long tiempoTranscurrido = tiempoActual - tiempoInicioFade;

  if (tiempoTranscurrido >= duracionFadeMs)
  {
    // Fade completado
    ledcWrite(0, velocidadObjetivo);
    velocidadActual = velocidadObjetivo;
    enFade = false;

    // Si estábamos esperando cambio de dirección y llegamos a velocidad 0
    if (esperandoCambioDireccion && velocidadActual == 0)
    {
      // Configurar nueva dirección
      if (direccionPendiente == DIRECCION_ADELANTE)
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
      velocidadInicial = 0;
      velocidadObjetivo = velocidadPendiente;
      tiempoInicioFade = millis();
      enFade = true;
      esperandoCambioDireccion = false;
      enMovimiento = true;
    }
    return;
  }

  // Calcular velocidad intermedia
  int velocidadTemp = map(tiempoTranscurrido, 0, duracionFadeMs,
                          velocidadInicial, velocidadObjetivo);
  velocidadTemp = abs(constrain(velocidadTemp, 0, 255));
  ledcWrite(0, velocidadTemp);
  velocidadActual = velocidadTemp;
}