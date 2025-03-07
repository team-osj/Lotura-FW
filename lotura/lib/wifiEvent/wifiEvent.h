#ifndef _WIFIEVENT_H
#define _WIFIEVENT_H

#include "variable.h"

void putString(const char *key, String value);
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
const char *WiFiStatusCode(wl_status_t status);

#endif