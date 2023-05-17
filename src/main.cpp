#include <Arduino.h>
// #include "CAN.h"

void sendATCommand(const char* command) {
  Serial1.println(command);
  delay(1000); // Wait for the response
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}

void setup()
{
  Serial.begin(9600);
  Serial1.begin(115200);

  delay(2000); // Wait for the module to initialize

  sendATCommand("AT+CWMODE=1"); // Set ESP32 to station mode
  delay(2000); // Wait for the mode change

  sendATCommand("AT+PASSCHANNEL=1"); // Reset the module
  delay(2000); // Wait for the reset

  sendATCommand("AT+CIPETH_DEF=\"192.168.1.254\",\"192.168.1.1\",\"255.255.255.0\""); // Reset the module
  delay(2000); // Wait for the reset

  // sendATCommand("AT+CWDHCP_DEF=3,0"); //! Giving error to this code (This code sets up DHCP)
  // delay(2000); // Wait for the reset

  sendATCommand("AT+CIFSR");
  delay(4000);

  sendATCommand("AT+CIPETH_DEF?");
  delay(2000);

  sendATCommand("AT+CIPSTART=\"TCPC\",\"192.168.1.50\",1234");
  delay(2000);

  sendATCommand("AT+CIPSEND=4");
  sendATCommand("cu12");
  delay(2000);
}

void loop()
{
    delay(1000);
    sendATCommand("AT+CIPSEND=4");
    sendATCommand("qwer");
    //String message = "APSJDUIAFJH"; // Değişken, seri porttan gelen mesajı tutacak
    //String sendCommand = "AT+CIPSEND=" + String(message.length() + 2) + "\r\n" + message + "\r\n"; // CIPSEND komutunu oluştur
    //Serial1.println(sendCommand); // CIPSEND komutunu gönder
    //Serial.println("sendCommand"); // CIPSEND komutunu gönder
}



// #include "globals.h"
// #include "serialParserTask.h"
// #include "readInputPinsTask.h"
// #include "pinOperations.h"
// #include "motorDriver.h"
// #include "waterPumpDriver.h"
// #include "eepromOperations.h"
// #include "CAN.h"

// void setup()
//{
//     InitializePins();
//     InitializeMotors();
//     InitializeEEPROM();
//     WaterPumpOff();
//     SerialBegin(9600);
//
//     BaseType_t xReturned;
//     xReturned = xTaskCreate(vSerialParserTask,
//                             "vSerialParserTask",
//                             configMINIMAL_STACK_SIZE,
//                             (void *)1,
//                             tskIDLE_PRIORITY,
//                             &xSerialParserTaskHandle);
//     if (xReturned == pdPASS)
//     {
//         SerialPrintln("vSerialParserTask Created Successfully!");
//     }
//     xReturned = xTaskCreate(vReadInputPinsTask,
//                         "vReadInputPinsTask",
//                         configMINIMAL_STACK_SIZE,
//                         (void *)1,
//                         tskIDLE_PRIORITY,
//                         &xSecondTaskHandle);
//     if (xReturned == pdPASS)
//     {
//         SerialPrintln("vSecondTask Created Successfully!");
//     }
//     vTaskStartScheduler();
// }
//
// void loop()
//{
//     /* Leave Empty! */
// }