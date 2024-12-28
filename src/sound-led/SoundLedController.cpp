#include "SoundLedController.h"
#include <Arduino.h>

SoundLedController::SoundLedController()
{
}

void SoundLedController::initialize()
{
  pinMode(PIN_SOUND, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // Iniciar todo apagado
  deactivateSound();
  turnOffLed();
}

void SoundLedController::activateSound()
{
  digitalWrite(PIN_SOUND, HIGH);
}

void SoundLedController::deactivateSound()
{
  digitalWrite(PIN_SOUND, LOW);
}

void SoundLedController::turnOnLed()
{
  digitalWrite(PIN_LED, HIGH);
}

void SoundLedController::turnOffLed()
{
  digitalWrite(PIN_LED, LOW);
}
