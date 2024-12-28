#ifndef TRAIN_ROUTINE_H
#define TRAIN_ROUTINE_H

#include "train/TrainController.h"
#include "sound-led/SoundLedController.h"
#include <Arduino.h>

enum class TypeRoutine
{
  ROUTINE_DEMO,
  ROUTINE_AVANZAR,
};

class TrainRoutine
{
private:
  TrainController &tren;
  SoundLedController &soundLed;

  bool isLedOn;
  bool isRoutine = false;
  bool manualStop = true;
  int counterBlinks = 0;
  int currentStep = 0;
  TypeRoutine routineActual = TypeRoutine::ROUTINE_DEMO;
  unsigned long timeLastBlink = 0;
  unsigned long timeLedStart;
  unsigned long timeStart = 0;

  void nextStep();
  void processCurrentStep();
  void updateBasicRoutine(unsigned long timeElapsed);

public:
  TrainRoutine(TrainController &trenController, SoundLedController &soundLedController);
  void startRoutine(TypeRoutine type);
  void startDemoRoutine();
  void startAdvanceRoutine();
  void update();
  bool isInRoutine() const { return isRoutine; }
  void stopRoutine();
  void resetManualStop() { manualStop = false; }
  void updateAdvanceRoutine(unsigned long timeElapsed);
  void setRoutine(TypeRoutine type)
  {
    routineActual = type;
  }
};

#endif