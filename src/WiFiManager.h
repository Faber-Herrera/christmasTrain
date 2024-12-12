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
  bool conectado = false;
  int intentosReconexion = 0;
  const int maxIntentos = 20;

public:
  WiFiManager();

  // Métodos principales
  bool inicializar();
  bool reconectar();
  void desconectar();

  // Métodos de estado
  bool estaConectado() const;
  String obtenerIP() const;
  String obtenerSSID() const;
  int obtenerRSSI() const; // Fuerza de la señal
};

#endif