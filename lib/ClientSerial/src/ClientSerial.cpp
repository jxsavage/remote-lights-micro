#include <EEPROMSettings.h>
#include <ClientSerial.h>
#ifdef USE_TEENSY
typedef HardwareSerial SerialType;
#endif
#ifdef USE_ESP32
#include <BluetoothSerial.h>
typedef BluetoothSerial SerialType;
#endif