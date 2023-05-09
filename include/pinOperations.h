#include <Arduino.h>
#include "pinDefinitions.h"
#include "math.h"

#define PORT_J_FIRST_PIN    14

enum
{
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_F,
    PORT_G,
    PORT_H,
    PORT_J,
    PORT_K,
    PORT_L,
};

/**
 * @brief This function initializes all pins as INPUT or OUTPUT
 *        
 * @note UART and SPI pins are pre-configured in Arduino libraries.
 */
void InitializePins(void);

/**
 * @brief 
 * 
 * @param pin 
 * @return int 
 */
int digitalReadExtended(uint8_t pin);

/**
 * @brief 
 * 
 * @param port 
 * @param pullupRegister 
 * @param pin 
 * @param ioMode 
 */
void pinModeExtended(uint8_t port, uint8_t pin, uint8_t ioMode);

/**
 * @brief 
 * 
 * @param pin 
 * @param val 
 */
void digitalWriteExtended(uint8_t pin, uint8_t val);

/**
 * @brief 
 * 
 */
void MCP2515InterruptCallback(void);

/**
 * @brief 
 * 
 */
void Encoder1InterruptCallback(void);

/**
 * @brief 
 * 
 */
void Encoder2InterruptCallback(void);