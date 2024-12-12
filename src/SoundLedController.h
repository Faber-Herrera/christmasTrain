#ifndef SOUND_LED_CONTROLLER_H
#define SOUND_LED_CONTROLLER_H

class SoundLedController
{
private:
  static const int PIN_SONIDO = 5;
  static const int PIN_LED = 4;

public:
  SoundLedController();
  void inicializar();

  // Control de sonido
  void activarSonido();
  void desactivarSonido();

  // Control de LED
  void encenderLed();
  void apagarLed();
};

#endif
