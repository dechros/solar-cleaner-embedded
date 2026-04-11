# solar-cleaner-embedded

Embedded firmware for the solar cleaner machine. Runs on an Arduino Mega 2560 and drives track motors, brushes and a water pump, reacts to sensors and error conditions, and communicates with a router board over TCP and with peripheral modules over CAN.

## Hardware

- Arduino Mega 2560 (ATmega2560)
- MCP2515 CAN controller
- DC motors for left/right tracks and brushes, water pump, limit/accelerometer sensors

## What it does

- Drives left track, right track, brushes and water pump with ramped motor control
- Reads sensors and stops the system on error or maintenance pin input
- Exchanges commands with the router board (TCP relay) and peripheral modules over CAN
- Supports a maintenance mode routed through the router board

## Layout

```
src/
  main.cpp                setup/loop, main scheduling
  motor.cpp               ramped motor driver
  waterPump.cpp           water pump control
  CANOperations.cpp       MCP_CAN messaging
  routerOperations.cpp    TCP link to router board
  maintenanceOperations.* maintenance mode
  sensorOperations.*      sensor handling
  errorOperations.*       error detection
  eepromOperations.*      EEPROM parameters
  pinOperations.*         pin init and reads
  timerOperations.*       timer ISRs
include/                  matching headers, pin and version definitions
platformio.ini            PlatformIO project config
```

## Build

PlatformIO project targeting the `megaatmega2560` board with the Arduino framework:

```
pio run
pio run -t upload
pio device monitor
```

## Dependencies

- feilipu/FreeRTOS
- coryjfowler/mcp_can 1.5.0
- greygnome/EnableInterrupt 1.1.0

## License

MIT
