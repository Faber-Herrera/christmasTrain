#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager()
{
}

bool WiFiManager::initialize()
{
  Serial.println("Iniciando conexión WiFi...");

  WiFi.mode(WIFI_STA);

  // Configurar IP estática
  IPAddress ip(192, 168, 0, 254);     // Cambia a la IP deseada
  IPAddress gateway(192, 168, 0, 1);  // Cambia a la puerta de enlace deseada
  IPAddress subnet(255, 255, 255, 0); // Cambia a la máscara de subred deseada
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);

  // Esperar a que se conecte
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < maxAttempts)
  {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    connected = true;
    Serial.println("\nConectado al WiFi!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
  {
    connected = false;
    Serial.println("\nFallo en la conexión WiFi!");
    return false;
  }
}

bool WiFiManager::reconnect()
{
  if (isConnected())
    return true;

  reconnectionAttempts++;
  if (reconnectionAttempts > maxAttempts)
  {
    Serial.println("Demasiados intentos de reconexión. Reiniciando...");
    ESP.restart();
  }

  return initialize();
}

void WiFiManager::disconnect()
{
  WiFi.disconnect();
  connected = false;
  Serial.println("WiFi desconectado");
}

bool WiFiManager::isConnected() const
{
  return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIP() const
{
  return WiFi.localIP().toString();
}

String WiFiManager::getSSID() const
{
  return String(ssid);
}

int WiFiManager::getRSSI() const
{
  return WiFi.RSSI();
}