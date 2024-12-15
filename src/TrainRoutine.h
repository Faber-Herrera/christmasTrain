#ifndef TRAIN_ROUTINE_H
#define TRAIN_ROUTINE_H

#include "TrainController.h"
#include "SoundLedController.h"
#include <Arduino.h>

enum class TipoRutina
{
  RUTINA_DEMO,
  RUTINA_AVANZAR,
};

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
  TipoRutina rutinaActual = TipoRutina::RUTINA_DEMO;
  unsigned long tiempoLedInicio;
  bool ledEncendido;

  void siguientePaso();
  void procesarPasoActual();
  void actualizarRutinaBasica(unsigned long tiempoTranscurrido);

public:
  TrainRoutine(TrainController &trenController, SoundLedController &soundLedController);
  void iniciarRutina(TipoRutina tipo);
  void iniciarRutinaDemo();
  void iniciarRutinaAvanzar();
  void actualizar();
  bool estaEnRutina() const { return enRutina; }
  void detenerRutina();
  void resetearDetencionManual() { detencionManual = false; }
  void actualizarRutinaAvanzar(unsigned long tiempoTranscurrido);
  void setRutina(TipoRutina tipo)
  {
    rutinaActual = tipo;
  }
};

#endif