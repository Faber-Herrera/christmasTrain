#include "WebServer.h"
#include <Arduino.h>

TrainWebServer::TrainWebServer(TrainController &trenController, SoundLedController &soundLedController, TrainRoutine &trainRoutine)
    : server(80), train(trenController), soundLed(soundLedController), routine(trainRoutine)
{
}

void TrainWebServer::initialize()
{
  server.on(ENDPOINT_ADVANCE, HTTP_GET, [this]()
            {
        if (routine.isInRoutine()) {
            routine.stopRoutine();
        }
        String velocidadStr = server.arg("velocidad");
        int velocidad = velocidadStr.length() > 0 ? velocidadStr.toInt() : TrainConfig::getMaxSpeed();
        
        velocidad = constrain(velocidad,
                            TrainConfig::getMinSpeed(),
                            TrainConfig::getMaxSpeed());
        
        Serial.println("AVANZAR RECIBIDO - Velocidad: " + String(velocidad));
        train.forward(velocidad);
        server.send(200, "text/plain", "Avanzando a velocidad: " + String(velocidad)); });

  server.on(ENDPOINT_STOP, HTTP_GET, [this]()
            {
        if (routine.isInRoutine()) {
            routine.stopRoutine();
        }
        train.stop();
        server.send(200, "text/plain", "Detenido"); });

  server.on(ENDPOINT_BACKWARD, HTTP_GET, [this]()
            {
        if (routine.isInRoutine()) {
            routine.stopRoutine();
        }
        String velocidadStr = server.arg("velocidad");
        int velocidad = velocidadStr.length() > 0 ? velocidadStr.toInt() : TrainConfig::getMaxSpeed();

        velocidad = constrain(velocidad,
                            TrainConfig::getMinSpeed(),
                            TrainConfig::getMaxSpeed());

        Serial.println("RETROCEDER RECIBIDO - Velocidad: " + String(velocidad));
        train.backward(velocidad);
        server.send(200, "text/plain", "Retrocediendo a velocidad: " + String(velocidad)); });

  server.on(ENDPOINT_ROUTINE_STOP, HTTP_GET, [this]()
            {
        Serial.println("DETENIENDO RUTINA");
        routine.stopRoutine();
        server.send(200, "text/plain", "Rutina detenida"); });

  server.on(ENDPOINT_ROUTINE_START, HTTP_GET, [this]()
            {
        String tipoRutinaStr = server.arg("tipo");
        TypeRoutine tipoRutina = TypeRoutine::ROUTINE_DEMO;
        
        if (tipoRutinaStr == "demo") {
            tipoRutina = TypeRoutine::ROUTINE_DEMO;
        } else if (tipoRutinaStr == "avanzar") {
            tipoRutina = TypeRoutine::ROUTINE_AVANZAR;
        }
        
        Serial.println("INICIANDO RUTINA: " + tipoRutinaStr);
        routine.resetManualStop();
        routine.startRoutine(tipoRutina);
        server.send(200, "text/plain", "Rutina " + tipoRutinaStr + " iniciada"); });

  server.on(ENDPOINT_SPEED_MIN, HTTP_POST, [this]()
            {
        String velocidadStr = server.arg("valor");
        if (velocidadStr.length() == 0) {
            server.send(400, "text/plain", "Falta el parámetro 'valor'");
            return;
        }
        
        uint8_t velocidad = velocidadStr.toInt();
        if (TrainConfig::setMinSpeed(velocidad)) {
            server.send(200, "text/plain", "Velocidad mínima establecida: " + String(velocidad));
        } else {
            server.send(400, "text/plain", "Valor inválido para velocidad mínima");
        } });

  server.on(ENDPOINT_SPEED_MAX, HTTP_POST, [this]()
            {
        String velocidadStr = server.arg("valor");
        if (velocidadStr.length() == 0) {
            server.send(400, "text/plain", "Falta el parámetro 'valor'");
            return;
        }
        
        uint8_t velocidad = velocidadStr.toInt();
        if (TrainConfig::setMaxSpeed(velocidad)) {
            server.send(200, "text/plain", "Velocidad máxima establecida: " + String(velocidad));
        } else {
            server.send(400, "text/plain", "Valor inválido para velocidad máxima");
        } });

  server.on(ENDPOINT_CONFIG_SPEED, HTTP_GET, [this]()
            {
        uint8_t minSpeed = TrainConfig::getMinSpeed();
        uint8_t maxSpeed = TrainConfig::getMaxSpeed();
        uint8_t currentSpeed = TrainConfig::getCurrentSpeed();
        String response = "{\"minSpeed\": " + String(minSpeed) + 
                         ", \"maxSpeed\": " + String(maxSpeed) + 
                         ", \"currentSpeed\": " + String(currentSpeed) + "}";
        server.send(200, "application/json", response); });

  server.on(ENDPOINT_SOUND_ON, HTTP_GET, [this]()
            {
        if (routine.isInRoutine()) {
            routine.stopRoutine();
        }
        Serial.println("SONIDO ACTIVADO");
        soundLed.activateSound();
        server.send(200, "text/plain", "Sonido activado"); });

  server.on(ENDPOINT_SOUND_OFF, HTTP_GET, [this]()
            {
        if (routine.isInRoutine()) {
            routine.stopRoutine();
        }
        Serial.println("SONIDO DESACTIVADO");
        soundLed.deactivateSound();
        server.send(200, "text/plain", "Sonido desactivado"); });

  server.on(ENDPOINT_LED_ON, HTTP_GET, [this]()
            {
        if (routine.isInRoutine()) {
            routine.stopRoutine();
        }
        Serial.println("LED ENCENDIDO");
        soundLed.turnOnLed();
        server.send(200, "text/plain", "LED encendido"); });

  server.on(ENDPOINT_LED_OFF, HTTP_GET, [this]()
            {
        if (routine.isInRoutine()) {
            routine.stopRoutine();
        }
        Serial.println("LED APAGADO");
        soundLed.turnOffLed();
        server.send(200, "text/plain", "LED apagado"); });

  server.onNotFound([this]()
                    {
        Serial.println("Ruta no encontrada");
        server.send(404, "text/plain", "Ruta no encontrada"); });

  server.begin();
  Serial.println("Servidor iniciado en port 80");
}

void TrainWebServer::handleClients()
{
  server.handleClient();
  delay(2); // Pequeña pausa para estabilidad
}