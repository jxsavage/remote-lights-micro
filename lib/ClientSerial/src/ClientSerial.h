#ifndef CLIENT_SERIAL_H
#define CLIENT_SERIAL_H

template <class T>
class ClientSerial {
  public:
    T serial;
    ClientSerial(T serial) {
      this->serial = serial;
    };
};

#endif