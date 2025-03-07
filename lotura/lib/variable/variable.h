#ifndef _VARIABLE_H
#define _VARIABLE_H

// header file
#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h> 
#include <stdlib.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsClient.h>
#include <EmonLib.h>

#include "serverInfo.h"
#include "managerHtml.h"
#include "stateHtml.h"
#include "wifiEvent.h"
#include "socket.h"
#include "exti.h"
#include "setvar.h"
#include "state.h"
#include "other.h"
#include "ota.h"

// define
#define PIN_STATUS 17
#define PIN_CH1_LED 18
#define PIN_CH2_LED 19
#define PIN_CH1_MODE 33
#define PIN_CH2_MODE 32
#define PIN_DEBUG 14
#define PIN_CT1 35
#define PIN_CT2 34
#define PIN_FLOW1 27
#define PIN_FLOW2 26
#define PIN_DRAIN1 23
#define PIN_DRAIN2 25

#define PING_LATE_MILLIS 21500
#define sensPeriod 500

// variable
extern EnergyMonitor ct1;
extern EnergyMonitor ct2;
extern Preferences preferences;
extern WebSocketsClient webSocket;

extern bool rebooting;
extern bool pingFlag;

extern uint64_t previousMillis;
extern uint64_t previousMillisEnd1;
extern uint64_t previousMillisEnd2;
extern uint64_t ledMillisPrev;
extern uint64_t currMillis;
extern uint64_t serverRetryMillis;
extern uint64_t lastPingMillis;

extern int m1;
extern int m2;

extern uint32_t timeSendFlag1;
extern uint32_t timeSendFlag2;

extern bool modeDebug;

extern bool ledStatus;

// 작동 시작 조건
extern float CH1_Curr_W;
extern float CH2_Curr_W;
extern uint32_t CH1_Flow_W;
extern uint32_t CH2_Flow_W;
extern float CH1_Curr_D;
extern float CH2_Curr_D;

// 작동 종료 조건
extern uint32_t CH1_EndDelay_W;
extern uint32_t CH2_EndDelay_W;
extern uint32_t CH1_EndDelay_D;
extern uint32_t CH2_EndDelay_D;

extern bool CH1_Mode;
extern bool CH2_Mode;
extern bool CH1_CurrStatus;
extern bool CH2_CurrStatus;
extern int CH1_Cnt;
extern int CH2_Cnt;
extern bool CH1_Live;
extern bool CH2_Live;

extern String defaultDeviceName;
extern String deviceName;
extern String apSsid;
extern String apPasswd;
extern String serialNo;
extern String authId;
extern String authPasswd;
extern String CH1_DeviceNo;
extern String CH2_DeviceNo;
extern String roomNo;

extern bool wifiFail;

// 전류
extern float Amps_TRMS1;
extern float Amps_TRMS2;

// 배수
extern int WaterSensorData1;
extern int WaterSensorData2;

// 유량
extern int flowFrequency1;
extern int flowFrequency2;
extern uint32_t lHour1;
extern uint32_t lHour2;

extern char jsonLogFlag1;
extern char jsonLogFlag2;
extern char jsonLogFlag1_c;
extern char jsonLogFlag2_c;
extern char jsonLogFlag1_f;
extern char jsonLogFlag2_f;
extern char jsonLogFlag1_w;
extern char jsonLogFlag2_w;
extern int jsonLogMillis1;
extern int jsonLogMillis2;
extern int jsonLogCnt1;
extern int jsonLogCnt2;

extern int sePrevMillis1;
extern int sePrevMillis2;
extern int seCnt1;
extern int seCnt2;

#endif