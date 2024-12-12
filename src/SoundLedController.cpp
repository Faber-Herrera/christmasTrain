#include "SoundLedController.h"
#include <Arduino.h>

SoundLedController::SoundLedController()
{
}

void SoundLedController::inicializar()
{
  pinMode(PIN_SONIDO, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // Iniciar todo apagado
  desactivarSonido();
  apagarLed();
}

void SoundLedController::activarSonido()
{
  digitalWrite(PIN_SONIDO, HIGH);
}

void SoundLedController::desactivarSonido()
{
  digitalWrite(PIN_SONIDO, LOW);
}

void SoundLedController::encenderLed()
{
  digitalWrite(PIN_LED, HIGH);
}

void SoundLedController::apagarLed()
{
  digitalWrite(PIN_LED, LOW);
}
