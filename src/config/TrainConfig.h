#ifndef TRAIN_CONFIG_H
#define TRAIN_CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

class TrainConfig
{
private:
  static uint8_t minSpeed;
  static uint8_t maxSpeed;
  static uint8_t currentSpeed;
  static Preferences preferences;

  // Constantes para validación
  static const uint8_t MIN_SPEED_ABSOLUTE = 0;
  static const uint8_t MAX_SPEED_ABSOLUTE = 255;

  // Claves para almacenamiento en flash
  static const char *NAMESPACE;
  static const char *KEY_MIN_SPEED;
  static const char *KEY_MAX_SPEED;

public:
  static void begin();
  static bool setMinSpeed(uint8_t speed);
  static bool setMaxSpeed(uint8_t speed);
  static bool setCurrentSpeed(uint8_t speed);
  static uint8_t getMinSpeed() { return minSpeed; }
  static uint8_t getMaxSpeed() { return maxSpeed; }
  static uint8_t getCurrentSpeed() { return currentSpeed; }
};

#endif