#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager()
{
}

bool WiFiManager::inicializar()
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