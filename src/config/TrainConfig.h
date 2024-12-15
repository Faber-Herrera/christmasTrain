#ifndef TRAIN_CONFIG_H
#define TRAIN_CONFIG_H

#include <Arduino.h>

class TrainConfig
{
private:
  static uint8_t velocidadMinima;
  static uint8_t velocidadMaxima;
  static uint8_t velocidadActual;

  // Constantes para validación
  static const uint8_t VELOCIDAD_MIN_ABSOLUTA = 0;
  static const uint8_t VELOCIDAD_MAX_ABSOLUTA = 255;

public:
  static bool setVelocidadMinima(uint8_t velocidad);
  static bool setVelocidadMaxima(uint8_t velocidad);
  static bool setVelocidadActual(uint8_t velocidad);
  static uint8_t getVelocidadMinima() { return velocidadMinima; }
  static uint8_t getVelocidadMaxima() { return velocidadMaxima; }
  static uint8_t getVelocidadActual() { return velocidadActual; }
};

#endif