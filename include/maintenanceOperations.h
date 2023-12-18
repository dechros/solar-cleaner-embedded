#ifndef _MAINTENANCE_OPERATIONS_H_
#define _MAINTENANCE_OPERATIONS_H_

#define MAINTENANCE_MODE_ACTIVE_MESSAGE         "M_A*"
#define MAINTENANCE_MODE_DEACTIVE_MESSAGE       "M_D*"

#define GET_MACHINE_NAME_MESSAGE                "GET0*"
#define GET_PARAMETERS_MESSAGE                  "GET1*"
#define SET_PARAMETERS_MESSAGE                  "SET1*"

uint8_t CheckMaintenanceMode(void);

bool SendMaintenanceModeMessage(bool mode);

bool ReceiveACK(void);

void CheckMaintenanceMessages(void);

#endif /* _MAINTENANCE_OPERATIONS_H_ */