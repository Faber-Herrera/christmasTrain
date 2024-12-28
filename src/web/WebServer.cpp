#include "WebServer.h"
#include <Arduino.h>

TrenWebServer::TrenWebServer(TrainController &trenController, SoundLedController &soundLedController, TrainRoutine &trainRoutine)
    : server(80), tren(trenController), soundLed(soundLedController), rutina(trainRoutine)
{
}

void TrenWebServer::inicializar()
{
  server.on(ENDPOINT_AVANZAR, HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        String velocidadStr = server.arg("velocidad");
        int velocidad = velocidadStr.length() > 0 ? velocidadStr.toInt() : TrainConfig::getVelocidadMaxima();
        
        velocidad = constrain(velocidad,
                            TrainConfig::getVelocidadMinima(),
                            TrainConfig::getVelocidadMaxima());
        
        Serial.println("AVANZAR RECIBIDO - Velocidad: " + String(velocidad));
        tren.avanzar(velocidad);
        server.send(200, "text/plain", "Avanzando a velocidad: " + String(velocidad)); });

  server.on(ENDPOINT_DETENER, HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        tren.detener();
        server.send(200, "text/plain", "Detenido"); });

  server.on(ENDPOINT_RETROCEDER, HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        String velocidadStr = server.arg("velocidad");
        int velocidad = velocidadStr.length() > 0 ? velocidadStr.toInt() : TrainConfig::getVelocidadMaxima();

        velocidad = constrain(velocidad,
                            TrainConfig::getVelocidadMinima(),
                            TrainConfig::getVelocidadMaxima());

        Serial.println("RETROCEDER RECIBIDO - Velocidad: " + String(velocidad));
        tren.retroceder(velocidad);
        server.send(200, "text/plain", "Retrocediendo a velocidad: " + String(velocidad)); });

  server.on(ENDPOINT_RUTINA_DETENER, HTTP_GET, [this]()
            {
        Serial.println("DETENIENDO RUTINA");
        rutina.detenerRutina();
        server.send(200, "text/plain", "Rutina detenida"); });

  server.on(ENDPOINT_RUTINA_INICIAR, HTTP_GET, [this]()
            {
        String tipoRutinaStr = server.arg("tipo");
        TipoRutina tipoRutina = TipoRutina::RUTINA_DEMO;
        
        if (tipoRutinaStr == "demo") {
            tipoRutina = TipoRutina::RUTINA_DEMO;
        } else if (tipoRutinaStr == "avanzar") {
            tipoRutina = TipoRutina::RUTINA_AVANZAR;
        }
        
        Serial.println("INICIANDO RUTINA: " + tipoRutinaStr);
        rutina.resetearDetencionManual();
        rutina.iniciarRutina(tipoRutina);
        server.send(200, "text/plain", "Rutina " + tipoRutinaStr + " iniciada"); });

  server.on(ENDPOINT_VELOCIDAD_MIN, HTTP_POST, [this]()
            {
        String velocidadStr = server.arg("valor");
        if (velocidadStr.length() == 0) {
            server.send(400, "text/plain", "Falta el parámetro 'valor'");
            return;
        }
        
        uint8_t velocidad = velocidadStr.toInt();
        if (TrainConfig::setVelocidadMinima(velocidad)) {
            server.send(200, "text/plain", "Velocidad mínima establecida: " + String(velocidad));
        } else {
            server.send(400, "text/plain", "Valor inválido para velocidad mínima");
        } });

  server.on(ENDPOINT_VELOCIDAD_MAX, HTTP_POST, [this]()
            {
        String velocidadStr = server.arg("valor");
        if (velocidadStr.length() == 0) {
            server.send(400, "text/plain", "Falta el parámetro 'valor'");
            return;
        }
        
        uint8_t velocidad = velocidadStr.toInt();
        if (TrainConfig::setVelocidadMaxima(velocidad)) {
            server.send(200, "text/plain", "Velocidad máxima establecida: " + String(velocidad));
        } else {
            server.send(400, "text/plain", "Valor inválido para velocidad máxima");
        } });

  server.on(ENDPOINT_CONFIGURACION_VELOCIDAD, HTTP_GET, [this]()
            {
        uint8_t velocidadMinima = TrainConfig::getVelocidadMinima();
        uint8_t velocidadMaxima = TrainConfig::getVelocidadMaxima();
        uint8_t velocidadActual = TrainConfig::getVelocidadActual();
        String response = "{\"velocidadMinima\": " + String(velocidadMinima) + 
                         ", \"velocidadMaxima\": " + String(velocidadMaxima) + 
                         ", \"velocidadActual\": " + String(velocidadActual) + "}";
        server.send(200, "application/json", response); });

  server.on(ENDPOINT_SONIDO_ACTIVAR, HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("SONIDO ACTIVADO");
        soundLed.activateSound();
        server.send(200, "text/plain", "Sonido activado"); });

  server.on(ENDPOINT_SONIDO_DESACTIVAR, HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("SONIDO DESACTIVADO");
        soundLed.deactivateSound();
        server.send(200, "text/plain", "Sonido desactivado"); });

  server.on(ENDPOINT_LED_ENCENDER, HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("LED ENCENDIDO");
        soundLed.turnOnLed();
        server.send(200, "text/plain", "LED encendido"); });

  server.on(ENDPOINT_LED_APAGAR, HTTP_GET, [this]()
            {
        if (rutina.estaEnRutina()) {
            rutina.detenerRutina();
        }
        Serial.println("LED APAGADO");
        soundLed.turnOffLed();
        server.send(200, "text/plain", "LED apagado"); });

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