#ifndef ESP32_LIB_H
#define ESP32_LIB_H

void sendATCommand(char const* command);
void startESP32Client();
void startESP32Server();
void sendString(char const* sentence);

#endif  // ESP32_LIB_H
