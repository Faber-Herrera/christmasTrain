#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager()
{
}

bool WiFiManager::inicializar()
{
  Serial.println("Iniciando conexión WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Esperar a que se conecte
  int intento = 0;
  while (WiFi.status() != WL_CONNECTED && intento < maxIntentos)
  {
    delay(500);
    Serial.print(".");
    intento++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    conectado = true;
    Serial.println("\nConectado al WiFi!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
  {
    conectado = false;
    Serial.println("\nFallo en la conexión WiFi!");
    return false;
  }
}

bool WiFiManager::reconectar()
{
  if (estaConectado())
    return true;

  intentosReconexion++;
  if (intentosReconexion > maxIntentos)
  {
    Serial.println("Demasiados intentos de reconexión. Reiniciando...");
    ESP.restart();
  }

  return inicializar();
}

void WiFiManager::desconectar()
{
  WiFi.disconnect();
  conectado = false;
  Serial.println("WiFi desconectado");
}

bool WiFiManager::estaConectado() const
{
  return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::obtenerIP() const
{
  return WiFi.localIP().toString();
}

String WiFiManager::obtenerSSID() const
{
  return String(ssid);
}

int WiFiManager::obtenerRSSI() const
{
  return WiFi.RSSI();
}