#ifndef SOUND_LED_CONTROLLER_H
#define SOUND_LED_CONTROLLER_H

class SoundLedController
{
private:
  static const int PIN_SOUND = 5;
  static const int PIN_LED = 4;

public:
  SoundLedController();
  void initialize();

  // Control de sonido
  void activateSound();
  void deactivateSound();

  // Control de LED
  void turnOnLed();
  void turnOffLed();
};

#endif
