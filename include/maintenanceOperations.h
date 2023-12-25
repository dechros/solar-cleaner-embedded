#ifndef _MAINTENANCE_OPERATIONS_H_
#define _MAINTENANCE_OPERATIONS_H_

#define MESSAGE_TIMEOUT_MS                      1000

#define MAINTENANCE_MODE_ACTIVE_MESSAGE         "M_A*"
#define MAINTENANCE_MODE_DEACTIVE_MESSAGE       "M_D*"

#define GET_PARAMETERS_MESSAGE                  "GET"
#define SET_PARAMETERS_MESSAGE                  "SET"
#define GET_PARAMETERS_ACK                      "GET_ACK"
#define SET_PARAMETERS_ACK                      "SET_ACK"

uint8_t CheckMaintenancePin(void);
bool IsRouterMaintenanceActive(void);
void ActivateRouterMaintenanceMode(void);
void DeactivateRouterMaintenanceMode(void);
void CheckMaintenanceMessages(void);
void PrintParameters(void);

#endif /* _MAINTENANCE_OPERATIONS_H_ */