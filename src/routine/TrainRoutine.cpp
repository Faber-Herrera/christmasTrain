#include "TrainRoutine.h"

TrainRoutine::TrainRoutine(TrainController &trenController, SoundLedController &soundLedController)
    : tren(trenController), soundLed(soundLedController)
{
}

void TrainRoutine::iniciarRutina(TipoRutina tipo)
{
  rutinaActual = tipo;
  switch (tipo)
  {
  case TipoRutina::RUTINA_DEMO:
    iniciarRutinaDemo();
    break;
  case TipoRutina::RUTINA_AVANZAR:
    iniciarRutinaAvanzar();
    break;
  default:
    break;
  }
}

void TrainRoutine::iniciarRutinaDemo()
{
  pasoActual = 0;
  enRutina = true;
  tiempoInicio = millis();
  contadorBlinks = 0;

  Serial.println("Iniciando rutina demo del tren");
  Serial.println("Paso 0: Espera inicial de 2 segundos");

  // Paso 1: encender led y sonido
  soundLed.turnOnLed();
  soundLed.activateSound();
}

void TrainRoutine::iniciarRutinaAvanzar()
{
  iniciarRutinaDemo();
  tiempoLedInicio = millis();
  ledEncendido = true;
}

void TrainRoutine::detenerRutina()
{
  detencionManual = true;
  enRutina = false;
  tren.detener();
  soundLed.turnOffLed();
  soundLed.deactivateSound();
  Serial.println("Rutina detenida manualmente");
}

void TrainRoutine::actualizar()
{
  if (!enRutina)
  {
    if (!detencionManual)
    {
      iniciarRutina(rutinaActual);
    }
    return;
  }

  unsigned long tiempoActual = millis();
  unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;

  switch (rutinaActual)
  {
  case TipoRutina::RUTINA_DEMO:
    actualizarRutinaBasica(tiempoTranscurrido);
    break;

  case TipoRutina::RUTINA_AVANZAR:
    actualizarRutinaAvanzar(tiempoTranscurrido);
    break;
  }
}

void TrainRoutine::actualizarRutinaBasica(unsigned long tiempoTranscurrido)
{

  // Agregamos mensajes de debug para ver los tiempos
  Serial.print("Tiempo transcurrido: ");
  Serial.print(tiempoTranscurrido);
  Serial.print("ms - Paso actual: ");
  Serial.println(pasoActual);

  switch (pasoActual)
  {
  case 0:
    if (tiempoTranscurrido >= 2000) // 0-2000ms: espera inicial
    {
      Serial.println("Paso 1: Avanzando por 5 segundos (con fade de 3s)");
      tren.avanzar(TrainConfig::getVelocidadActual());
      pasoActual++;
    }
    break;

  case 1:
    if (tiempoTranscurrido >= 10000) // 2000-10000ms: 5s avance + 3s fade
    {
      Serial.println("Paso 2: Retrocediendo por 5 segundos (con fade de 3s + 3s)");
      tren.retroceder(TrainConfig::getVelocidadActual()); // Esto implica 3s fade a 0 + 3s fade a velocidad final
      pasoActual++;
    }
    break;

  case 2:
    if (tiempoTranscurrido >= 21000) // 10000-21000ms: 5s retroceso + 6s fades totales
    {
      Serial.println("Paso 3: Deteniendo con fade (3s)");
      tren.detener();
      pasoActual++;
    }
    break;

  case 3:
    if (tiempoTranscurrido >= 24000) // 21000-24000ms: 3s fade de detención
    {
      Serial.println("Paso 4: Iniciando secuencia de parpadeo");
      tiempoUltimoBlink = millis();
      soundLed.turnOffLed();
      pasoActual++;
    }
    break;

  case 4:
    // Para el parpadeo sí necesitamos un control de tiempo independiente
    if (tiempoTranscurrido - (24000 + (contadorBlinks * 1000)) >= 1000)
    {
      if (contadorBlinks < 6)
      {
        if (contadorBlinks % 2 == 0)
        {
          Serial.print("LED encendido - Parpadeo ");
          Serial.println((contadorBlinks / 2) + 1);
          soundLed.turnOnLed();
        }
        else
        {
          Serial.print("LED apagado - Parpadeo ");
          Serial.println((contadorBlinks / 2) + 1);
          soundLed.turnOffLed();
        }
        contadorBlinks++;
      }
      else
      {
        Serial.println("Ciclo de rutina completado");
        soundLed.turnOffLed();
        soundLed.deactivateSound();
        enRutina = false;
        // No establecemos detencionManual aquí para permitir el reinicio
      }
    }
    break;
  }
}

void TrainRoutine::actualizarRutinaAvanzar(unsigned long tiempoTranscurrido)
{
  // Control del parpadeo continuo
  if (millis() - tiempoLedInicio >= 500)
  {
    tiempoLedInicio = millis();   // Reiniciamos el contador
    ledEncendido = !ledEncendido; // Invertimos el estado
    if (ledEncendido)
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
  Serial.println(pasoActual);

  switch (pasoActual)
  {
  case 0:
    if (tiempoTranscurrido >= 2000) // Espera inicial de 2 segundos
    {
      Serial.println("Paso 1: Avanzando");
      tren.avanzar(TrainConfig::getVelocidadActual());
      pasoActual++;
    }
    break;

  case 1:
    if (tiempoTranscurrido >= 15000) // 2000+3000+10000: 02s espera + 03s fade + 10s de avance
    {
      Serial.println("Paso 2: Deteniendo");
      tren.detener();
      pasoActual++;
    }
    break;

  case 2:
    if (tiempoTranscurrido >= 20000) // 15000-20000ms: 15s de avance + 3s de fade + 2s de espera
    {
      Serial.println("Rutina de avance completada");
      soundLed.turnOffLed();
      soundLed.deactivateSound();
      enRutina = false;
    }
    break;
  }
}