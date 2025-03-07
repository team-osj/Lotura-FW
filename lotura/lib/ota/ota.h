#ifndef _OTA_H
#define _OTA_H

#include "variable.h"

String reset_reason(int reason);
String processor(const String& var);
void setupAsyncServer();
void notFound(AsyncWebServerRequest *request);
String convertFileSize(const size_t bytes);

#endif