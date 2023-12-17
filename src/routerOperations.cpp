#include "routerOperations.h"
#include "globals.h"
#include "waterPump.h"

uint8_t bufferClearCounter = 0;
bool messageCameFlag = false;

typedef struct
{
    uint8_t rightTrackSpeed;
    uint8_t leftTrackSpeed;
    MotorDirectionType_t rightTrackDirection;
    MotorDirectionType_t leftTrackDirection;
}TrackInfo_t;

static TrackInfo_t JoystickAlgorithm(uint8_t joystickX, uint8_t joystickY);

void InitRouterCommunication()
{
    ROUTER_SERIAL.begin(9600);
}

static bool ControlChecksum(uint8_t *dataPointer, uint8_t size)
{
	uint8_t checksum = 0;
	for (uint8_t i = 0; i < size - 1; i++)
	{
		checksum ^= dataPointer[i];
	}
	checksum ^= 255;
    return (dataPointer[size - 1] == checksum);
}

static void ParseTCPMessage(TCPMessage_t readTCPMessage)
{
    messageCameFlag = true;
    if (readTCPMessage.emergencyButton == 0)
    {
        SystemStop();
    }
    else
    {
        SystemResume();
        TrackInfo_t trackInfo = JoystickAlgorithm(readTCPMessage.joystickX, readTCPMessage.joystickY);
        RightTrackMotor.SetTargetSpeed((uint8_t)((trackInfo.rightTrackSpeed / 3) * ((double)readTCPMessage.brushSpeed / MAX_SPEED)));
        RightTrackMotor.SetTargetDirection(trackInfo.rightTrackDirection);
        LeftTrackMotor.SetTargetSpeed((uint8_t)((trackInfo.leftTrackSpeed / 3) * ((double)readTCPMessage.brushSpeed / MAX_SPEED)));
        LeftTrackMotor.SetTargetDirection(trackInfo.leftTrackDirection);
        BrushesMotor.SetTargetSpeed(readTCPMessage.driveSpeed);
        if (readTCPMessage.brushFrontCW == 1)
        {
            BrushesMotor.SetTargetDirection(FORWARD);
        }
        else
        {
            BrushesMotor.SetTargetDirection(REVERSE);
        }
        if (readTCPMessage.waterButton == 1)
        {
            WaterPumpHandler.Request(TURN_ON);
        }
        else
        {
           WaterPumpHandler.Request(TURN_OFF);
        }
    }
}

void CheckTCPMessage()
{
    int tcpBufferSize = ROUTER_SERIAL.available();
    if (tcpBufferSize > 0)
    {
        bufferClearCounter = 0;
        int readData = ROUTER_SERIAL.peek();
        if (readData == TCP_MESSAGE_FIRST_BYTE && tcpBufferSize >= sizeof(TCPMessage_t))
        {
            TCPMessage_t readTCPMessage;
            ROUTER_SERIAL.readBytes((uint8_t*)&readTCPMessage, sizeof(TCPMessage_t));
            /* Check the "TCP Message Header" */
            if (strncmp((const char*)&readTCPMessage, TCP_MESSAGE_HEADER, 3) == 0)
            {
                /* Control the checksum of message */
                bool result = ControlChecksum((uint8_t*)&readTCPMessage, sizeof(TCPMessage_t));
                if (result == true)
                {
                    ParseTCPMessage(readTCPMessage);
                    ROUTER_SERIAL.write(ACK_MESSAGE, 3);
                }
                else
                {
                    /* Checksum Error! Do not parse wrong message */
                }
            }
            else
            {
                /* Unknown header */
            }
        }
        else if (readData == 'T' && tcpBufferSize < sizeof(TCPMessage_t))
        {
            delay(10);
            bufferClearCounter++;
            if (bufferClearCounter == 5)
            {
                bufferClearCounter = 0;
                ROUTER_SERIAL.read();
                /* Half Message! Discarded! */
            }
        }
        else
        {
            ROUTER_SERIAL.read();
            /* Unknown header byte discard */
        }
    }
}

void CheckMessageTimeout()
{
    /* No message came in 1 sec */
    if (messageCameFlag == false)
    {
        SystemStop();
    }
    messageCameFlag = false;
}

TrackInfo_t JoystickAlgorithm(uint8_t joystickX, uint8_t joystickY)
{
    TrackInfo_t trackInfo;
    int8_t joystickXSigned = joystickX - JOYSTICK_MIDDLE_VALUE; 
    int8_t joystickYSigned = joystickY - JOYSTICK_MIDDLE_VALUE; 

    if (((joystickXSigned < JOYSTICK_DEAD_ZONE_ROUTER) && (joystickXSigned > -JOYSTICK_DEAD_ZONE_ROUTER)) && 
        ((joystickYSigned < JOYSTICK_DEAD_ZONE_ROUTER) && (joystickYSigned > -JOYSTICK_DEAD_ZONE_ROUTER)))
    {
        /* No Movement */
        trackInfo.rightTrackDirection = REVERSE;
        trackInfo.leftTrackDirection = FORWARD;
        trackInfo.rightTrackSpeed = 0;
        trackInfo.leftTrackSpeed = 0;
    }
    else if ((joystickXSigned > JOYSTICK_DEAD_ZONE_ROUTER) && 
        ((joystickYSigned < JOYSTICK_DEAD_ZONE_ROUTER) && (joystickYSigned > -JOYSTICK_DEAD_ZONE_ROUTER)))
    {
        /* Rotate machine clockwise (Both Tracks Maximum) */
        trackInfo.rightTrackDirection = FORWARD;
        trackInfo.leftTrackDirection = FORWARD;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickXSigned / JOYSTICK_MAX_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickXSigned / JOYSTICK_MAX_VALUE));
    }
    else if ((joystickXSigned < -JOYSTICK_DEAD_ZONE_ROUTER) && 
        ((joystickYSigned < JOYSTICK_DEAD_ZONE_ROUTER) && (joystickYSigned > -JOYSTICK_DEAD_ZONE_ROUTER)))
    {
        /* Rotate machine counter-clockwise (Both Tracks Maximum) */
        trackInfo.rightTrackDirection = REVERSE;
        trackInfo.leftTrackDirection = REVERSE;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickXSigned) / JOYSTICK_MAX_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickXSigned) / JOYSTICK_MAX_VALUE));
    }
    else if ((joystickYSigned > JOYSTICK_DEAD_ZONE_ROUTER) && 
        ((joystickXSigned < JOYSTICK_DEAD_ZONE_ROUTER) && (joystickXSigned > -JOYSTICK_DEAD_ZONE_ROUTER)))
    {
        /* Move machine forward (Both Tracks Maximum) */
        trackInfo.rightTrackDirection = REVERSE;
        trackInfo.leftTrackDirection = FORWARD;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickYSigned / JOYSTICK_MAX_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickYSigned / JOYSTICK_MAX_VALUE));
    }
    else if ((joystickYSigned < -JOYSTICK_DEAD_ZONE_ROUTER) && 
        ((joystickXSigned < JOYSTICK_DEAD_ZONE_ROUTER) && (joystickXSigned > -JOYSTICK_DEAD_ZONE_ROUTER)))
    {
        /* Move machine reverse (Both Tracks Maximum) */
        trackInfo.rightTrackDirection = FORWARD;
        trackInfo.leftTrackDirection = REVERSE;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickYSigned) / JOYSTICK_MAX_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickYSigned) / JOYSTICK_MAX_VALUE));
    }
    else if ((joystickXSigned > JOYSTICK_DEAD_ZONE_ROUTER) && (joystickYSigned > JOYSTICK_DEAD_ZONE_ROUTER))
    {
        /** 
         * 1. Quadrant of coordinate system 
         * 
         * @brief   In the first quadrant of coordinate system
         *          "right track" will make movement in both directions
         *          depending on the angle of joystick to X axis.
         *        
         * @example If you move joystick all the way up both tracks
         *          will move forward. Which means closer to 90 degree
         *          right track will start moving forward up until maximum.
         *          
         *          But if you move joystick all the way right side,
         *          left track will still go forward but right track will
         *          go reverse for clockwise rotation. Which means closer to
         *          0 degree right track will start moving reverse up until maximum
         */

        double radian = atan((double)joystickYSigned/joystickXSigned);
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
        }
        else if (degree < 45 )
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
        }
        else if(degree == 45)
        {
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            trackInfo.rightTrackSpeed = 0;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
        }
    }
    else if ((joystickXSigned < -JOYSTICK_DEAD_ZONE_ROUTER) && (joystickYSigned > JOYSTICK_DEAD_ZONE_ROUTER))
    {
        /** 
         * 2. Quadrant of coordinate system 
         * 
         * @brief   In the second quadrant of coordinate system
         *          "left track" will make movement in both directions
         *          depending on the angle of joystick to X axis.
         *        
         * @example If you move joystick all the way up both tracks
         *          will move forward. Which means closer to 90 degree
         *          left track will start moving forward up until maximum.
         *          
         *          But if you move joystick all the way left side,
         *          right track will still go forward but left track will
         *          go reverse for counter-clockwise rotation. Which means closer to
         *          0 degree left track will start moving reverse up until maximum
         */

        double radian = atan((double)joystickYSigned/abs(joystickXSigned));
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
        }
        else if (degree < 45 )
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
        }
        else if(degree == 45)
        {
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
            trackInfo.leftTrackSpeed = 0;
        }
    }
    else if ((joystickXSigned < -JOYSTICK_DEAD_ZONE_ROUTER) && (joystickYSigned < -JOYSTICK_DEAD_ZONE_ROUTER))
    {
        /** 
         * 3. Quadrant of coordinate system 
         * 
         * @brief   In the third quadrant of coordinate system
         *          "right track" will make movement in both directions
         *          depending on the angle of joystick to X axis.
         *        
         * @example If you move joystick all the way down both tracks
         *          will move reverse. Which means closer to 90 degree
         *          right track will start moving reverse up until maximum.
         *          
         *          But if you move joystick all the way left side,
         *          left track will still go reverse but right track will
         *          go forward for counter-clockwise rotation. Which means closer to
         *          0 degree right track will start moving forward up until maximum
         */

        double radian = atan((double)abs(joystickYSigned)/abs(joystickXSigned));
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
        }
        else if (degree < 45 )
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
        }
        else if(degree == 45)
        {
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = REVERSE;
            trackInfo.rightTrackSpeed = 0;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
        }
    }
    else if ((joystickXSigned > JOYSTICK_DEAD_ZONE_ROUTER) && (joystickYSigned < -JOYSTICK_DEAD_ZONE_ROUTER))
    {
        /** 
         * 4. Quadrant of coordinate system 
         * 
         * @brief   In the forth quadrant of coordinate system
         *          "left track" will make movement in both directions
         *          depending on the angle of joystick to X axis.
         *        
         * @example If you move joystick all the way down both tracks
         *          will move reverse. Which means closer to 90 degree
         *          left track will start moving reverse up until maximum.
         *          
         *          But if you move joystick all the way right side,
         *          right track will still go reverse but left track will
         *          go forward for clockwise rotation. Which means closer to
         *          0 degree left track will start moving forward up until maximum
         */

        double radian = atan((double)abs(joystickYSigned)/joystickXSigned);
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
        }
        else if (degree < 45 )
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE) * angleMultiplier);
        }
        else if(degree == 45)
        {
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned*joystickXSigned) + (joystickYSigned*joystickYSigned));
            hipotenus = (hipotenus > JOYSTICK_MAX_VALUE) ? JOYSTICK_MAX_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / JOYSTICK_MAX_VALUE));
            trackInfo.leftTrackSpeed = 0;
        }
    }
    return trackInfo;
}

