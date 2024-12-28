#include "TrainRoutine.h"

TrainRoutine::TrainRoutine(TrainController &trenController, SoundLedController &soundLedController)
    : tren(trenController), soundLed(soundLedController)
{
}

void TrainRoutine::startRoutine(TypeRoutine type)
{
  routineActual = type;
  switch (type)
  {
  case TypeRoutine::ROUTINE_DEMO:
    startDemoRoutine();
    break;
  case TypeRoutine::ROUTINE_AVANZAR:
    startAdvanceRoutine();
    break;
  default:
    break;
  }
}

void TrainRoutine::startDemoRoutine()
{
  counterBlinks = 0;
  currentStep = 0;
  isRoutine = true;
  timeStart = millis();

  Serial.println("Iniciando rutina demo del tren");
  Serial.println("Paso 0: Espera inicial de 2 segundos");

  // Paso 1: encender led y sonido
  soundLed.activateSound();
  soundLed.turnOnLed();
}

void TrainRoutine::startAdvanceRoutine()
{
  startDemoRoutine();
  timeLedStart = millis();
  isLedOn = true;
}

void TrainRoutine::stopRoutine()
{
  manualStop = true;
  isRoutine = false;
  tren.stop();
  soundLed.turnOffLed();
  soundLed.deactivateSound();
  Serial.println("Rutina detenida manualmente");
}

void TrainRoutine::update()
{
  if (!isRoutine)
  {
    if (!manualStop)
    {
      startRoutine(routineActual);
    }
    return;
  }

  unsigned long tiempoActual = millis();
  unsigned long tiempoTranscurrido = tiempoActual - timeStart;

  switch (routineActual)
  {
  case TypeRoutine::ROUTINE_DEMO:
    updateBasicRoutine(tiempoTranscurrido);
    break;

  case TypeRoutine::ROUTINE_AVANZAR:
    updateAdvanceRoutine(tiempoTranscurrido);
    break;
  }
}

void TrainRoutine::updateBasicRoutine(unsigned long tiempoTranscurrido)
{

  // Agregamos mensajes de debug para ver los tiempos
  Serial.print("Tiempo transcurrido: ");
  Serial.print(tiempoTranscurrido);
  Serial.print("ms - Paso actual: ");
  Serial.println(currentStep);

  switch (currentStep)
  {
  case 0:
    if (tiempoTranscurrido >= 2000) // 0-2000ms: espera inicial
    {
      Serial.println("Paso 1: Avanzando por 5 segundos (con fade de 3s)");
      tren.forward(TrainConfig::getCurrentSpeed());
      currentStep++;
    }
    break;

  case 1:
    if (tiempoTranscurrido >= 10000) // 2000-10000ms: 5s avance + 3s fade
    {
      Serial.println("Paso 2: Retrocediendo por 5 segundos (con fade de 3s + 3s)");
      tren.backward(TrainConfig::getCurrentSpeed()); // Esto implica 3s fade a 0 + 3s fade a velocidad final
      currentStep++;
    }
    break;

  case 2:
    if (tiempoTranscurrido >= 21000) // 10000-21000ms: 5s retroceso + 6s fades totales
    {
      Serial.println("Paso 3: Deteniendo con fade (3s)");
      tren.stop();
      currentStep++;
    }
    break;

  case 3:
    if (tiempoTranscurrido >= 24000) // 21000-24000ms: 3s fade de detención
    {
      Serial.println("Paso 4: Iniciando secuencia de parpadeo");
      timeLastBlink = millis();
      soundLed.turnOffLed();
      currentStep++;
    }
    break;

  case 4:
    // Para el parpadeo sí necesitamos un control de tiempo independiente
    if (tiempoTranscurrido - (24000 + (counterBlinks * 1000)) >= 1000)
    {
      if (counterBlinks < 6)
      {
        if (counterBlinks % 2 == 0)
        {
          Serial.print("LED encendido - Parpadeo ");
          Serial.println((counterBlinks / 2) + 1);
          soundLed.turnOnLed();
        }
        else
        {
          Serial.print("LED apagado - Parpadeo ");
          Serial.println((counterBlinks / 2) + 1);
          soundLed.turnOffLed();
        }
        counterBlinks++;
      }
      else
      {
        Serial.println("Ciclo de rutina completado");
        soundLed.turnOffLed();
        soundLed.deactivateSound();
        isRoutine = false;
        // No establecemos manualStop aquí para permitir el reinicio
      }
    }
    break;
  }
}

void TrainRoutine::updateAdvanceRoutine(unsigned long tiempoTranscurrido)
{
  // Control del parpadeo continuo
  if (millis() - timeLedStart >= 500)
  {
    timeLedStart = millis(); // Reiniciamos el contador
    isLedOn = !isLedOn;      // Invertimos el estado
    if (isLedOn)
    {
      soundLed.turnOnLed();
    }
    else
    {
      soundLed.turnOffLed();
    }
  }

  Serial.print("Tiempo transcurrido: ");
  Serial.print(tiempoTranscurrido);
  Serial.print("ms - Paso actual: ");
  Serial.println(currentStep);

  switch (currentStep)
  {
  case 0:
    if (tiempoTranscurrido >= 2000) // Espera inicial de 2 segundos
    {
      Serial.println("Paso 1: Avanzando");
      tren.forward(TrainConfig::getCurrentSpeed());
      currentStep++;
    }
    break;

  case 1:
    if (tiempoTranscurrido >= 15000) // 2000+3000+10000: 02s espera + 03s fade + 10s de avance
    {
      Serial.println("Paso 2: Deteniendo");
      tren.stop();
      currentStep++;
    }
    break;

  case 2:
    if (tiempoTranscurrido >= 20000) // 15000-20000ms: 15s de avance + 3s de fade + 2s de espera
    {
      Serial.println("Rutina de avance completada");
      soundLed.turnOffLed();
      soundLed.deactivateSound();
      isRoutine = false;
    }
    break;
  }
}