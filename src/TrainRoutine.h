#ifndef TRAIN_ROUTINE_H
#define TRAIN_ROUTINE_H

#include "TrainController.h"
#include "SoundLedController.h"
#include <Arduino.h>

class TrainRoutine
{
private:
  TrainController &tren;
  SoundLedController &soundLed;

  unsigned long tiempoInicio = 0;
  unsigned long tiempoUltimoBlink = 0;
  int contadorBlinks = 0;
  bool enRutina = false;
  int pasoActual = 0;
  bool detencionManual = true;

  void siguientePaso();
  void procesarPasoActual();

public:
  TrainRoutine(TrainController &trenController, SoundLedController &soundLedController);
  void iniciarRutina();
  void actualizar();
  bool estaEnRutina() const { return enRutina; }
  void detenerRutina();
  void resetearDetencionManual() { detencionManual = false; }
};

#endif