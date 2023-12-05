/**
 * @file pinDefinitions.h
 * @author Talha Cetin (talhacetin96@hotmail.com)
 * @brief A header file that contains pin definitions of the project.
 * @version 0.1
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _PIN_DEFINITIONS_H_
#define _PIN_DEFINITIONS_H_

/**
 * @note Pinmap for EFC39 (Arduino Mega 2560)
 *
 * Cable    Pin Mode	            Arduino Pin     Explanation
 *
 * A1	    PWM_Output_1	        13	            Brush speed
 * A2	    PWM_Output_2	        12	            Brush forward
 * A3	    PWM_Output_3	        11	            Brush stop
 * A4	    PWM_Output_4	        10	            Brush backward
 * A5	    PWM_Output_5	        9	            Left track forward
 * A6	    PWM_Output_6	        8	            Left track stop
 * A8	    PWM_Output_7	        7	            Left track backward
 * A9	    PWM_Output_8	        6	            Right track forward
 * A10	    PWM_Output_9	        4	            Right track stop
 * A11	    PWM_Output_10	        3	            Right track backward
 * A12	    PWM_Output_11	        2	            _
 * A13	    PWM_Output_12	        5	            Water pump
 *
 * B2	    Analog_Input_1	        A0	            Front sensor
 * B3	    Analog_Input_2	        A1	            Rear sensor
 * B4	    Analog_Input_3	        A2	            Left sensor
 * B5	    Analog_Input_4	        A3	            Right sensor
 * B6	    Analog_Input_5	        A8	            _
 * B7	    Analog_Input_6	        A9	            _
 * B8	    Analog_Input_7	        A10	            _
 * B9	    Analog_Input_8	        A11	            _
 * B10	    Analog_Input_9	        A12	            _
 * B11	    Analog_Input_10	        A13	            _
 *
 * C2	    Digital_Input_1	        49	            Brush error
 * C3	    Digital_Input_2	        47	            Left track error
 * C4	    Digital_Input_3	        43	            Right track error
 * C5	    Digital_Input_4	        48	            _
 *
 * C7	    Analogical_Output_1     45 (PWM)	    Left track speed
 * C8	    Analogical_Output_2	    46 (PWM)	    Right track speed
 *
 * A7	    Sensor Supply	        _	            +5V
 * B1	    GND	                    _	            Eksi besleme Aküden
 * B12	    VDD_System	            _	            Kontrol Kartı + Besleme
 * B13	    GND	                    _	            Eksi besleme Aküden
 * C1	    OUTPUT_SUPPLY_1	        _	            Çıkışların + Beslemesi(1.grup)
 * C6	    Programming_Reset       _	            Yazılım Güncellerken kullanılacak.
 * C9	    CAN0_H	                _	            Can H
 * C10	    CAN0_L	                _	            Can L
 * C11	    UART_TX	                1	            Tx
 * C12	    UART_RX	                0	            Rx
 * C13	    OUTPUT_SUPPLY_2	        _	            Çıkışların + Beslemesi(2.grup)
 */

/* Fake pin numbers for "EnableInterrupt.h" library */
#define CAN_INTERRUPT_PIN (70)
#define PJ3_PCINT12_PIN (71)
#define PJ4_PCINT13_PIN (72)
#define PJ5_PCINT14_PIN (73)
#define PJ6_PCINT15_PIN (74)

#define BRUSHES_SPEED_CONTROL_PIN (13)
#define BRUSHES_FORWARD_TURN_PIN (12)
#define BRUSHES_STOP_PIN (11)
#define BRUSHES_REVERSE_TURN_PIN (10)
#define BRUSHES_ERROR_PIN (49)

#define WATER_ON_OFF_PIN (5)

#define TRACK_LEFT_SPEED_CONTROL_PIN (46)
#define TRACK_LEFT_FORWARD_TURN_PIN (9)
#define TRACK_LEFT_STOP_PIN (8)
#define TRACK_LEFT_REVERSE_TURN_PIN (7)
#define TRACK_LEFT_ERROR_PIN (47)

#define TRACK_RIGHT_SPEED_CONTROL_PIN (45)
#define TRACK_RIGHT_FORWARD_TURN_PIN (6)
#define TRACK_RIGHT_STOP_PIN (4)
#define TRACK_RIGHT_REVERSE_TURN_PIN (3)
#define TRACK_RIGHT_ERROR_PIN (43)

#define PC_CONNECTION_UART_TX_PIN (1)
#define PC_CONNECTION_UART_RX_PIN (0)

#define MCP_2515_SPI_MOSI_PIN (51)
#define MCP_2515_SPI_MISO_PIN (50)
#define MCP_2515_SPI_CLK_PIN (52)
#define MCP_2515_SPI_CS_PIN (36)
#define MCP_2515_SPI_INTERRUPT_PIN PJ2_PCINT11_PIN

#define SENSOR_FRONT PIN_A0
#define SENSOR_REAR PIN_A1
#define SENSOR_LEFT PIN_A2
#define SENSOR_RIGHT PIN_A3

#define RESERVED_PIN_2 (2)
#define RESERVED_PIN_48 (48)
#define RESERVED_PIN_A8 PIN_A8
#define RESERVED_PIN_A9 PIN_A9
#define RESERVED_PIN_A10 PIN_A10
#define RESERVED_PIN_A11 PIN_A11
#define RESERVED_PIN_A12 PIN_A12
#define RESERVED_PIN_A13 PIN_A13

#endif /* _PIN_DEFINITIONS_H_ */