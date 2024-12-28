#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager
{
private:
  // Credenciales de WiFi
  const char *ssid = "MaxYMatilda";
  const char *password = "Qwerty123**";

  // Variables de estado
  bool connected = false;
  int reconnectionAttempts = 0;
  const int maxAttempts = 20;

public:
  WiFiManager();

  // Métodos principales
  bool initialize();
  bool reconnect();
  void disconnect();

  // Métodos de estado
  bool isConnected() const;
  String getIP() const;
  String getSSID() const;
  int getRSSI() const; // Fuerza de la señal
};

#endif