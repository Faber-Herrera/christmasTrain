#include "TrainConfig.h"

Preferences TrainConfig::preferences;
uint8_t TrainConfig::velocidadMinima = 0;
uint8_t TrainConfig::velocidadMaxima = 255;
uint8_t TrainConfig::velocidadActual = 0;

// Definición de constantes para almacenamiento
const char *TrainConfig::NAMESPACE = "train";
const char *TrainConfig::KEY_VEL_MIN = "vel_min";
const char *TrainConfig::KEY_VEL_MAX = "vel_max";

void TrainConfig::begin()
{
  preferences.begin(NAMESPACE, false);

  // Cargar valores almacenados o usar valores por defecto
  velocidadMinima = preferences.getUChar(KEY_VEL_MIN, 0);
  velocidadMaxima = preferences.getUChar(KEY_VEL_MAX, 255);
}

bool TrainConfig::setVelocidadMinima(uint8_t velocidad)
{
  if (velocidad > velocidadMaxima || velocidad < VELOCIDAD_MIN_ABSOLUTA)
  {
    return false;
  }
  velocidadMinima = velocidad;
  preferences.putUChar(KEY_VEL_MIN, velocidad);
  return true;
}

bool TrainConfig::setVelocidadMaxima(uint8_t velocidad)
{
  if (velocidad < velocidadMinima || velocidad > VELOCIDAD_MAX_ABSOLUTA)
  {
    return false;
  }
  velocidadMaxima = velocidad;
  preferences.putUChar(KEY_VEL_MAX, velocidad);
  return true;
}

bool TrainConfig::setVelocidadActual(uint8_t velocidad)
{
  if (velocidad < velocidadMinima || velocidad > velocidadMaxima)
  {
    return false;
  }
  velocidadActual = velocidad;
  return true;
}