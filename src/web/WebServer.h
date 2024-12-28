#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP32WebServer.h>
#include "train/TrainController.h"
#include "sound-led/SoundLedController.h"
#include "routine/TrainRoutine.h"
#include "config/TrainConfig.h"

class TrenWebServer
{
private:
  ESP32WebServer server;
  TrainController &tren;
  SoundLedController &soundLed;
  TrainRoutine &rutina;
  const int puerto = 80;

  // Documentación de endpoints
  static constexpr const char *ENDPOINT_VELOCIDAD_MIN = "/velocidad/minima";                  // POST - param: valor
  static constexpr const char *ENDPOINT_VELOCIDAD_MAX = "/velocidad/maxima";                  // POST - param: valor
  static constexpr const char *ENDPOINT_AVANZAR = "/avanzar";                                 // GET - param: velocidad
  static constexpr const char *ENDPOINT_RETROCEDER = "/retroceder";                           // GET - param: velocidad
  static constexpr const char *ENDPOINT_DETENER = "/detener";                                 // GET
  static constexpr const char *ENDPOINT_RUTINA_INICIAR = "/rutina/iniciar";                   // GET - param: tipo
  static constexpr const char *ENDPOINT_RUTINA_DETENER = "/rutina/detener";                   // GET
  static constexpr const char *ENDPOINT_CONFIGURACION_VELOCIDAD = "/configuracion/velocidad"; // GET
  static constexpr const char *ENDPOINT_SONIDO_ACTIVAR = "/sonido/activar";                   // GET
  static constexpr const char *ENDPOINT_SONIDO_DESACTIVAR = "/sonido/desactivar";             // GET
  static constexpr const char *ENDPOINT_LED_ENCENDER = "/led/encender";                       // GET
  static constexpr const char *ENDPOINT_LED_APAGAR = "/led/apagar";                           // GET

public:
  TrenWebServer(TrainController &trenController, SoundLedController &soundLedController, TrainRoutine &trainRoutine);
  void inicializar();
  void manejarClientes();
};

#endif