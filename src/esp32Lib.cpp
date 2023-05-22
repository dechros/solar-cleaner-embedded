/**
 * @file esp32Setup.cpp
 * @author Oguz Selman Cetin (oguzselmancetin@hotmail.com)
 * @brief This file Setups the wt32-eth01 esp32 module
 * @version 0.1
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include "esp32Lib.h"

void sendATCommand(const char *command)
{
    Serial1.println(command);
    delay(1000);
    while (Serial1.available())
    {
        Serial.write(Serial1.read());
    }
}

void startESP32Client()
{

    Serial.begin(9600);
    Serial1.begin(115200);

    sendATCommand("AT");
    delay(2000);

    sendATCommand("AT+CWMODE=1"); // Set ESP32 to station mode
    delay(2000);                  // Wait for the mode change

    sendATCommand("AT+PASSCHANNEL=1"); // Serial to Ethernet mode (default mode)
    delay(2000);                       // Wait for the mode change

    sendATCommand("AT+CIPETH_DEF=\"192.168.1.254\",\"192.168.1.1\",\"255.255.255.0\""); // Reset the module
    delay(2000);                                                                        // Wait for the ip change

    sendATCommand("AT+CIPSTART=\"TCPC\",\"192.168.1.50\",1234"); // Connect to the TCPC server
    delay(2000);                                                 // Wait for connect

    sendATCommand("ATE0");
    delay(2000);
}

void startESP32Server()
{
    Serial.begin(9600);
    Serial1.begin(115200);

    sendATCommand("AT");
    delay(2000);
    
    //sendATCommand("AT+CWMODE=1"); // Set ESP32 to station mode
    //delay(2000);                  // Wait for the mode change

    sendATCommand("AT+PASSCHANNEL=1"); // Serial to Ethernet mode (default mode)
    delay(2000);                       // Wait for the mode change

    sendATCommand("AT+CIPETH_DEF=\"192.168.1.254\",\"192.168.1.1\",\"255.255.255.0\""); // Reset the module
    delay(2000);                                                                        // Wait for the ip change

    sendATCommand("AT+CIPSTART=\"TCPS\",\"192.168.1.50\",1234,5678"); // Connect to the TCPC server
    delay(2000);                                                 // Wait for connect

    sendATCommand("ATE0");
    delay(2000);
}

void sendString(char const *sentence)
{
    sendATCommand("AT+CIPSEND=4");
    sendATCommand(sentence)
}