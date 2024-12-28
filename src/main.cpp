#include "wifi/WiFiManager.h"
#include "train/TrainController.h"
#include "web/WebServer.h"
#include "sound-led/SoundLedController.h"
#include "routine/TrainRoutine.h"

WiFiManager wifi;
TrainController train;
SoundLedController soundLed;
TrainRoutine rutina(train, soundLed);
TrenWebServer servidor(train, soundLed, rutina); // Actualizado con los tres controladores

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
  servidor.inicializar();

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
  servidor.manejarClientes(); // Manejar peticiones web
  rutina.actualizar();
  delay(10);
}
