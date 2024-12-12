#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP32WebServer.h>
#include "TrainController.h"
#include "SoundLedController.h"
#include "TrainRoutine.h"

class TrenWebServer
{
private:
  ESP32WebServer server;
  TrainController &tren;
  SoundLedController &soundLed;
  TrainRoutine &rutina;
  const int puerto = 80;

public:
  TrenWebServer(TrainController &trenController, SoundLedController &soundLedController, TrainRoutine &trainRoutine);
  void inicializar();
  void manejarClientes();
};

#endif