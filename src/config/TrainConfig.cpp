#include "TrainConfig.h"

Preferences TrainConfig::preferences;
uint8_t TrainConfig::minSpeed = 0;
uint8_t TrainConfig::maxSpeed = 255;
uint8_t TrainConfig::currentSpeed = 0;

// Definición de constantes para almacenamiento
const char *TrainConfig::NAMESPACE = "train";
const char *TrainConfig::KEY_MIN_SPEED = "min_speed";
const char *TrainConfig::KEY_MAX_SPEED = "max_speed";

void TrainConfig::begin()
{
  preferences.begin(NAMESPACE, false);

  // Cargar valores almacenados o usar valores por defecto
  minSpeed = preferences.getUChar(KEY_MIN_SPEED, 0);
  maxSpeed = preferences.getUChar(KEY_MAX_SPEED, 255);
}

bool TrainConfig::setMinSpeed(uint8_t velocidad)
{
  if (velocidad > maxSpeed || velocidad < MIN_SPEED_ABSOLUTE)
  {
    return false;
  }
  minSpeed = velocidad;
  preferences.putUChar(KEY_MIN_SPEED, velocidad);
  return true;
}

bool TrainConfig::setMaxSpeed(uint8_t velocidad)
{
  if (velocidad < minSpeed || velocidad > MAX_SPEED_ABSOLUTE)
  {
    return false;
  }
  maxSpeed = velocidad;
  preferences.putUChar(KEY_MAX_SPEED, velocidad);
  return true;
}

bool TrainConfig::setCurrentSpeed(uint8_t velocidad)
{
  if (velocidad < minSpeed || velocidad > maxSpeed)
  {
    return false;
  }
  currentSpeed = velocidad;
  return true;
}