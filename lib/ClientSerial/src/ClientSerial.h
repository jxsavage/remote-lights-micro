#include <EEPROMSettings.h>
#ifndef CLIENT_SERIAL_H
#define CLIENT_SERIAL_H
#ifdef USE_ESP32
#include <BluetoothSerial.h>
extern BluetoothSerial clientSerial;
#endif
#ifdef USE_TEENSY
#define clientSerial Serial
// extern HardwareSerial clientSerial;
#endif
// template <class T>
// class ClientSerial {
//   public:
//     T const clientSerial;
//     ClientSerial(const T s) {
//       clientSerial = s;
//     };
// };

#endif