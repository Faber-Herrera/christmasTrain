#ifndef TRAIN_CONTROLLER_H
#define TRAIN_CONTROLLER_H

#include "config/TrainConfig.h"

class TrainController
{
private:
  // Pines para el control del motor
  const int PIN_MOTOR_A = 26;   // Pin para dirección adelante
  const int PIN_MOTOR_B = 27;   // Pin para dirección atrás
  const int PIN_VELOCIDAD = 25; // Pin PWM para velocidad

  // Variables de estado
  int currentSpeed = 0;
  bool isOnMovement = false;

  // Variables para el control del fade
  unsigned long timeStartFade = 0;
  int initialSpeed = 0;
  int targetSpeed = 0;
  bool isFade = false;
  const int fadeDurarionMs = 3000;
  bool isChangeDirection = false;
  int pendingDirection;
  int pendingSpeed;
  static const int DIRECTION_FORWARD = 1;
  static const int DIRECTION_BACKWARD = 2;

  // Método privado para validar y ajustar velocidad
  int setSpeed(int speed);

public:
  TrainController();

  // Métodos básicos de control
  void initialize();
  void forward(int speed);  // speed: 0-255
  void backward(int speed); // speed: 0-255
  void stop();
  void updateFade(); // Nuevo método para manejar el fade

  // Métodos para obtener estado
  bool isMotion() const { return isOnMovement; }
  int getSpeed() const { return currentSpeed; }
};

#endif