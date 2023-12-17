#ifndef _WATER_PUMP_H_
#define _WATER_PUMP_H_

typedef enum
{
    DO_NOTHING,
    TURN_ON,
    TURN_OFF,
    TOGGLE
}RequestType_t;

class WaterPump
{
public:
    WaterPump();
    ~WaterPump();
    void Init();
    void ProcessRequests();
    void Request(RequestType_t);

private:
    RequestType_t currentRequest = DO_NOTHING;

    void TurnOn();
    void TurnOff();
    void Toggle();
};


#endif /* _WATER_PUMP_H_ */