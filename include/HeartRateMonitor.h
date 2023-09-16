#ifndef HEARTRATEMONITOR_H
#define HEARTRATEMONITOR_H

#include "MAX30100_PulseOximeter.h"

extern PulseOximeter pox;

void initHeartRateMonitor();
void updateHeartRateSensor();

#endif