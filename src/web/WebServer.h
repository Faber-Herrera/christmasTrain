#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP32WebServer.h>
#include "train/TrainController.h"
#include "sound-led/SoundLedController.h"
#include "routine/TrainRoutine.h"
#include "config/TrainConfig.h"

class TrainWebServer
{
private:
  ESP32WebServer server;
  TrainController &train;
  SoundLedController &soundLed;
  TrainRoutine &routine;
  const int port = 80;

  // Documentación de endpoints
  static constexpr const char *ENDPOINT_SPEED_MIN = "/velocidad/minima";           // POST - param: valor
  static constexpr const char *ENDPOINT_SPEED_MAX = "/velocidad/maxima";           // POST - param: valor
  static constexpr const char *ENDPOINT_ADVANCE = "/avanzar";                      // GET - param: velocidad
  static constexpr const char *ENDPOINT_BACKWARD = "/retroceder";                  // GET - param: velocidad
  static constexpr const char *ENDPOINT_STOP = "/detener";                         // GET
  static constexpr const char *ENDPOINT_ROUTINE_START = "/routine/iniciar";        // GET - param: tipo
  static constexpr const char *ENDPOINT_ROUTINE_STOP = "/routine/detener";         // GET
  static constexpr const char *ENDPOINT_CONFIG_SPEED = "/configuracion/velocidad"; // GET
  static constexpr const char *ENDPOINT_SOUND_ON = "/sonido/activar";              // GET
  static constexpr const char *ENDPOINT_SOUND_OFF = "/sonido/desactivar";          // GET
  static constexpr const char *ENDPOINT_LED_ON = "/led/encender";                  // GET
  static constexpr const char *ENDPOINT_LED_OFF = "/led/apagar";                   // GET

public:
  TrainWebServer(TrainController &trenController, SoundLedController &soundLedController, TrainRoutine &trainRoutine);
  void initialize();
  void handleClients();
};

#endif