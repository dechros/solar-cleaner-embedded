#ifndef _PIN_DEFINITIONS_H_
#define _PIN_DEFINITIONS_H_

#define PJ2_PCINT11_PIN                     (70)
#define PJ3_PCINT12_PIN                     (71)
#define PJ4_PCINT13_PIN                     (72)
#define PJ5_PCINT14_PIN                     (73)
#define PJ6_PCINT15_PIN                     (74)

#define BRUSHES_SPEED_CONTROL_PIN           (13)
#define BRUSHES_FORWARD_TURN_PIN            (12)
#define BRUSHES_STOP_PIN                    (11)
#define BRUSHES_BACKWARD_TURN_PIN           (10)
#define BRUSHES_ERROR_PIN                   (49)

#define WATER_ON_OFF_PIN                    (5)

#define PALLET_LEFT_SPEED_CONTROL_PIN       (45)
#define PALLET_LEFT_FORWARD_TURN_PIN        (9)
#define PALLET_LEFT_STOP_PIN                (8)
#define PALLET_LEFT_BACKWARD_TURN_PIN       (7)
#define PALLET_LEFT_ERROR_PIN               (47)

#define PALLET_RIGHT_SPEED_CONTROL_PIN      (46)
#define PALLET_RIGHT_FORWARD_TURN_PIN       (6)
#define PALLET_RIGHT_STOP_PIN               (4)
#define PALLET_RIGHT_BACKWARD_TURN_PIN      (3)
#define PALLET_RIGHT_ERROR_PIN              (43)

#define PC_CONNECTION_UART_TX_PIN           (1)
#define PC_CONNECTION_UART_RX_PIN           (0)

#define MCP_2515_SPI_MOSI_PIN               (51)
#define MCP_2515_SPI_MISO_PIN               (50)
#define MCP_2515_SPI_CLK_PIN                (52)
#define MCP_2515_SPI_CS_PIN                 (36)
#define MCP_2515_SPI_INTERRUPT_PIN          PJ2_PCINT11_PIN

#define RESERVED_PIN_2                      (2)
#define RESERVED_PIN_A0                     PIN_A0 
#define RESERVED_PIN_A1                     PIN_A1 
#define RESERVED_PIN_A2                     PIN_A2 
#define RESERVED_PIN_A3                     PIN_A3 
#define RESERVED_PIN_A8                     PIN_A8 
#define RESERVED_PIN_A9                     PIN_A9 
#define RESERVED_PIN_A10                    PIN_A10
#define RESERVED_PIN_A11                    PIN_A11
#define RESERVED_PIN_A12                    PIN_A12
#define RESERVED_PIN_A13                    PIN_A13

#endif /* _PIN_DEFINITIONS_H_ */