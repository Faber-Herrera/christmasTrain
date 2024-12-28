Estoy creando un tren de juguete controlado por wifi.

El tren tiene un motor, una luz y sonido.

El tren debe tener un modo de control remoto por wifi con las siguientes funcionalidades:

- Avanzar
- Retroceder
- Detener
- Parpadeo
- Sonido
- Luces

El tren debe tener rutinas en donde el tren integre una o varias de las funcionalidades anteriores.

Todo el código debe ser modular, en ingles y con comentarios en ingles.

Debe tener buenas practicas de programación, como evitar el uso de delay(), usar millis() para el tiempo, etc.

la arquitectura debe ser la siguiente:

├── src/
│ ├── main.cpp
│ ├── config/
│ │ ├── TrainConfig.h
│ │ ├── TrainConfig.cpp
│ ├── routine/
│ │ ├── TrainRoutine.h
│ │ ├── TrainRoutine.cpp
│ ├── sound-led/
│ │ ├── SoundLedController.h
│ │ ├── SoundLedController.cpp
│ ├── train/
│ │ ├── TrainController.h
│ │ ├── TrainController.cpp
│ ├── web/
│ │ ├── WebServer.h
│ │ ├── WebServer.cpp
│ ├── wifi/
│ │ ├── WiFiManager.h
│ │ ├── WiFiManager.cpp
