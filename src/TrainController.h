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
  int velocidadActual = 0;
  bool enMovimiento = false;

  // Variables para el control del fade
  unsigned long tiempoInicioFade = 0;
  int velocidadInicial = 0;
  int velocidadObjetivo = 0;
  bool enFade = false;
  const int duracionFadeMs = 3000;

  bool esperandoCambioDireccion = false;
  int direccionPendiente;
  int velocidadPendiente;
  static const int DIRECCION_ADELANTE = 1;
  static const int DIRECCION_ATRAS = 2;

  // Método privado para validar y ajustar velocidad
  int ajustarVelocidad(int velocidad);

public:
  TrainController();

  // Métodos básicos de control
  void inicializar();
  void avanzar(int velocidad);    // velocidad: 0-255
  void retroceder(int velocidad); // velocidad: 0-255
  void detener();
  void actualizarFade(); // Nuevo método para manejar el fade

  // Métodos para obtener estado
  bool estaEnMovimiento() const { return enMovimiento; }
  int obtenerVelocidad() const { return velocidadActual; }
};

#endif