#include "wifi/WiFiManager.h"
#include "train/TrainController.h"
#include "web/WebServer.h"
#include "sound-led/SoundLedController.h"
#include "routine/TrainRoutine.h"

WiFiManager wifi;
TrainController train;
SoundLedController soundLed;
TrainRoutine routine(train, soundLed);
TrainWebServer servidor(train, soundLed, routine); // Actualizado con los tres controladores

void setup()
{
  Serial.begin(115200);
  TrainConfig::begin();

  // Inicializar controladores
  train.initialize();
  soundLed.initialize();

  // Inicializar WiFi
  if (!wifi.initialize())
  {
    Serial.println("Error al iniciar WiFi");
    return;
  }

  // Inicializar servidor web
  servidor.initialize();

  Serial.print("Servidor web disponible en: http://");
  Serial.println(wifi.getIP());
}

void loop()
{
  // Verificar conexión WiFi
  if (!wifi.isConnected())
  {
    Serial.println("Reconectando WiFi...");
    wifi.reconnect();
    delay(1000);
    return;
  }

  train.updateFade();
  servidor.handleClients(); // Manejar peticiones web
  routine.update();
  delay(10);
}
