#include "TrainConfig.h"

uint8_t TrainConfig::velocidadMinima = 0;
uint8_t TrainConfig::velocidadMaxima = 255;
uint8_t TrainConfig::velocidadActual = 0;

bool TrainConfig::setVelocidadMinima(uint8_t velocidad)
{
  if (velocidad > velocidadMaxima || velocidad < VELOCIDAD_MIN_ABSOLUTA)
  {
    return false;
  }
  velocidadMinima = velocidad;
  return true;
}

bool TrainConfig::setVelocidadMaxima(uint8_t velocidad)
{
  if (velocidad < velocidadMinima || velocidad > VELOCIDAD_MAX_ABSOLUTA)
  {
    return false;
  }
  velocidadMaxima = velocidad;
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