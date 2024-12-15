#include "WebServer.h"
#include <Arduino.h>

TrenWebServer::TrenWebServer(TrainController &trenController, SoundLedController &soundLedController, TrainRoutine &trainRoutine)
    : server(80), tren(trenController), soundLed(soundLedController), rutina(trainRoutine)
{
}

void TrenWebServer::inicializar()
{
  server.on("/avanzar", HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        String velocidadStr = server.arg("velocidad");
        int velocidad = velocidadStr.length() > 0 ? velocidadStr.toInt() : 255;
        
        // Asegurar que la velocidad esté en el rango correcto
        velocidad = constrain(velocidad, 0, 255);
        
        Serial.println("AVANZAR RECIBIDO - Velocidad: " + String(velocidad));
        tren.avanzar(velocidad);
        server.send(200, "text/plain", "Avanzando a velocidad: " + String(velocidad)); });

  server.on("/detener", HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        tren.detener();
        server.send(200, "text/plain", "Detenido"); });

  server.on("/retroceder", HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        String velocidadStr = server.arg("velocidad");
        int velocidad = velocidadStr.length() > 0 ? velocidadStr.toInt() : 255;
        
        // Asegurar que la velocidad esté en el rango correcto
        velocidad = constrain(velocidad, 0, 255);
        
        Serial.println("RETROCEDER RECIBIDO - Velocidad: " + String(velocidad));
        tren.retroceder(velocidad);
        server.send(200, "text/plain", "Retrocediendo a velocidad: " + String(velocidad)); });

  server.on("/sonido/activar", HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("SONIDO ACTIVADO");
        soundLed.activarSonido();
        server.send(200, "text/plain", "Sonido activado"); });

  server.on("/sonido/desactivar", HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("SONIDO DESACTIVADO");
        soundLed.desactivarSonido();
        server.send(200, "text/plain", "Sonido desactivado"); });

  server.on("/led/encender", HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("LED ENCENDIDO");
        soundLed.encenderLed();
        server.send(200, "text/plain", "LED encendido"); });

  server.on("/led/apagar", HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("LED APAGADO");
        soundLed.apagarLed();
        server.send(200, "text/plain", "LED apagado"); });

  server.on("/rutina/detener", HTTP_GET, [this]()
            {
        Serial.println("DETENIENDO RUTINA");
        rutina.detenerRutina();
        server.send(200, "text/plain", "Rutina detenida"); });

  server.on("/rutina/iniciar", HTTP_GET, [this]()
            {
        String tipoRutinaStr = server.arg("tipo");
        TipoRutina tipoRutina = TipoRutina::RUTINA_DEMO; // valor por defecto
        
        if (tipoRutinaStr == "demo") {
            tipoRutina = TipoRutina::RUTINA_DEMO;
        } else if (tipoRutinaStr == "avanzar") {
            tipoRutina = TipoRutina::RUTINA_AVANZAR;
        }
        
        Serial.println("INICIANDO RUTINA: " + tipoRutinaStr);
        rutina.resetearDetencionManual();
        rutina.iniciarRutina(tipoRutina);
        server.send(200, "text/plain", "Rutina " + tipoRutinaStr + " iniciada"); });

  server.onNotFound([this]()
                    {
        Serial.println("Ruta no encontrada");
        server.send(404, "text/plain", "Ruta no encontrada"); });

  server.begin();
  Serial.println("Servidor iniciado en puerto 80");
}

void TrenWebServer::manejarClientes()
{
  server.handleClient();
  delay(2); // Pequeña pausa para estabilidad
}