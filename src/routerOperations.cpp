#include "routerOperations.h"
#include "globals.h"
#include "waterPump.h"

uint8_t bufferClearCounter = 0;

typedef struct
{
    uint8_t rightTrackSpeed;
    uint8_t leftTrackSpeed;
    MotorDirectionType_t rightTrackDirection;
    MotorDirectionType_t leftTrackDirection;
} TrackInfo_t;

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
    messageTimeoutCounter = 0;
    controllerError = false;
    firstMessageCame = true;

    if (readTCPMessage.emergencyButton == 0)
    {
        SystemStop();
    }
    else
    {
        TrackInfo_t trackInfo = JoystickAlgorithm(readTCPMessage.joystickX, readTCPMessage.joystickY);
        uint16_t rightMappedValue = map(trackInfo.rightTrackSpeed, 0, 255, 100 * SystemParameters.rightMinSpeed, 100 * SystemParameters.rightMaxSpeed);
        RightTrackMotor.SetTargetSpeed((uint16_t)(rightMappedValue * ((double)readTCPMessage.driveSpeed / MAX_SPEED)));
        RightTrackMotor.SetTargetDirection(trackInfo.rightTrackDirection);
        uint16_t leftMappedValue = map(trackInfo.rightTrackSpeed, 0, 255, 100 * SystemParameters.leftMinSpeed, 100 * SystemParameters.leftMaxSpeed);
        LeftTrackMotor.SetTargetSpeed((uint16_t)(leftMappedValue * ((double)readTCPMessage.driveSpeed / MAX_SPEED)));
        LeftTrackMotor.SetTargetDirection(trackInfo.leftTrackDirection);
        uint16_t brushMappedValue = map(readTCPMessage.brushSpeed, 0, 255, 100 * SystemParameters.brushMinSpeed, 100 * SystemParameters.brushMaxSpeed);
        BrushesMotor.SetTargetSpeed(brushMappedValue);
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
        if (readData == TCP_MESSAGE_FIRST_BYTE && tcpBufferSize >= (int)sizeof(TCPMessage_t))
        {
            TCPMessage_t readTCPMessage;
            ROUTER_SERIAL.readBytes((uint8_t *)&readTCPMessage, sizeof(TCPMessage_t));

            if (strncmp((const char *)&readTCPMessage, TCP_MESSAGE_HEADER, 3) == 0)
            {
                bool result = ControlChecksum((uint8_t *)&readTCPMessage, sizeof(TCPMessage_t));
                if (result == true)
                {
                    ParseTCPMessage(readTCPMessage);
                    ROUTER_SERIAL.write(ACK_MESSAGE, 3);

                }
                else
                {
                }
            }
            else
            {
            }
        }
        else if (readData == 'T' && tcpBufferSize < (int)sizeof(TCPMessage_t))
        {
            delay(10);
            bufferClearCounter++;
            if (bufferClearCounter == 5)
            {
                bufferClearCounter = 0;
                ROUTER_SERIAL.read();

            }
        }
        else
        {
            ROUTER_SERIAL.read();

        }
    }
}

TrackInfo_t JoystickAlgorithm(uint8_t joystickX, uint8_t joystickY)
{
    TrackInfo_t trackInfo;
    int8_t joystickXSigned = joystickX - SystemParameters.joystickMiddleValue;
    int8_t joystickYSigned = joystickY - SystemParameters.joystickMiddleValue;

    if (((joystickXSigned < SystemParameters.joystickDeadZone) && (joystickXSigned > -SystemParameters.joystickDeadZone)) &&
        ((joystickYSigned < SystemParameters.joystickDeadZone) && (joystickYSigned > -SystemParameters.joystickDeadZone)))
    {
        trackInfo.rightTrackDirection = REVERSE;
        trackInfo.leftTrackDirection = FORWARD;
        trackInfo.rightTrackSpeed = 0;
        trackInfo.leftTrackSpeed = 0;
    }
    else if ((joystickXSigned > SystemParameters.joystickDeadZone) &&
             ((joystickYSigned < SystemParameters.joystickDeadZone) && (joystickYSigned > -SystemParameters.joystickDeadZone)))
    {
        trackInfo.rightTrackDirection = FORWARD;
        trackInfo.leftTrackDirection = FORWARD;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickXSigned / MAX_HIPOTENUS_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickXSigned / MAX_HIPOTENUS_VALUE));
    }
    else if ((joystickXSigned < -SystemParameters.joystickDeadZone) &&
             ((joystickYSigned < SystemParameters.joystickDeadZone) && (joystickYSigned > -SystemParameters.joystickDeadZone)))
    {
        trackInfo.rightTrackDirection = REVERSE;
        trackInfo.leftTrackDirection = REVERSE;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickXSigned) / MAX_HIPOTENUS_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickXSigned) / MAX_HIPOTENUS_VALUE));
    }
    else if ((joystickYSigned > SystemParameters.joystickDeadZone) &&
             ((joystickXSigned < SystemParameters.joystickDeadZone) && (joystickXSigned > -SystemParameters.joystickDeadZone)))
    {
        trackInfo.rightTrackDirection = REVERSE;
        trackInfo.leftTrackDirection = FORWARD;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickYSigned / MAX_HIPOTENUS_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)joystickYSigned / MAX_HIPOTENUS_VALUE));
    }
    else if ((joystickYSigned < -SystemParameters.joystickDeadZone) &&
             ((joystickXSigned < SystemParameters.joystickDeadZone) && (joystickXSigned > -SystemParameters.joystickDeadZone)))
    {
        trackInfo.rightTrackDirection = FORWARD;
        trackInfo.leftTrackDirection = REVERSE;
        trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickYSigned) / MAX_HIPOTENUS_VALUE));
        trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * ((double)abs(joystickYSigned) / MAX_HIPOTENUS_VALUE));
    }
    else if ((joystickXSigned > SystemParameters.joystickDeadZone) && (joystickYSigned > SystemParameters.joystickDeadZone))
    {
        double radian = atan((double)joystickYSigned / joystickXSigned);
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
        }
        else if (degree < 45)
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
        }
        else if (degree == 45)
        {
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            trackInfo.rightTrackSpeed = 0;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
        }
    }
    else if ((joystickXSigned < -SystemParameters.joystickDeadZone) && (joystickYSigned > SystemParameters.joystickDeadZone))
    {
        double radian = atan((double)joystickYSigned / abs(joystickXSigned));
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
        }
        else if (degree < 45)
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
        }
        else if (degree == 45)
        {
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
            trackInfo.leftTrackSpeed = 0;
        }
    }
    else if ((joystickXSigned < -SystemParameters.joystickDeadZone) && (joystickYSigned < -SystemParameters.joystickDeadZone))
    {
        double radian = atan((double)abs(joystickYSigned) / abs(joystickXSigned));
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
        }
        else if (degree < 45)
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
        }
        else if (degree == 45)
        {
            trackInfo.rightTrackDirection = REVERSE;
            trackInfo.leftTrackDirection = REVERSE;
            trackInfo.rightTrackSpeed = 0;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
        }
    }
    else if ((joystickXSigned > SystemParameters.joystickDeadZone) && (joystickYSigned < -SystemParameters.joystickDeadZone))
    {
        double radian = atan((double)abs(joystickYSigned) / joystickXSigned);
        double degree = (180 * radian) / M_PI;
        if (degree > 45)
        {
            double angleMultiplier = ((degree - 45)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = REVERSE;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
        }
        else if (degree < 45)
        {
            double angleMultiplier = ((45 - degree)) / 45;
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
            trackInfo.leftTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE) * angleMultiplier);
        }
        else if (degree == 45)
        {
            trackInfo.rightTrackDirection = FORWARD;
            trackInfo.leftTrackDirection = FORWARD;
            float hipotenus = sqrtf((joystickXSigned * joystickXSigned) + (joystickYSigned * joystickYSigned));
            hipotenus = (hipotenus > MAX_HIPOTENUS_VALUE) ? MAX_HIPOTENUS_VALUE : hipotenus;
            trackInfo.rightTrackSpeed = (uint8_t)(MAX_SPEED * (hipotenus / MAX_HIPOTENUS_VALUE));
            trackInfo.leftTrackSpeed = 0;
        }
    }
    return trackInfo;
}
