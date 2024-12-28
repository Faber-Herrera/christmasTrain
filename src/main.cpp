#include "wifi/WiFiManager.h"
#include "TrainController.h"
#include "WebServer.h"
#include "SoundLedController.h"
#include "TrainRoutine.h"

WiFiManager wifi;
TrainController tren;
SoundLedController soundLed;
TrainRoutine rutina(tren, soundLed);
TrenWebServer servidor(tren, soundLed, rutina); // Actualizado con los tres controladores

void setup()
{
  Serial.begin(115200);
  TrainConfig::begin();

  // Inicializar controladores
  tren.inicializar();
  soundLed.inicializar();

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

  tren.actualizarFade();
  servidor.manejarClientes(); // Manejar peticiones web
  rutina.actualizar();
  delay(10);
}
