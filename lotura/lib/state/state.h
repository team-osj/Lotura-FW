#ifndef _STATE_H
#define _STATE_H

#include "variable.h"

void Dryer_Status_Judgment(float Amps_TRMS, int cnt, int m, unsigned long previousMillisEnd, int ChannelNum);
void Status_Judgment(float Amps_TRMS, int WaterSensorData, unsigned int lHour, int cnt, int m, unsigned long previousMillisEnd, int ChannelNum);

#endif