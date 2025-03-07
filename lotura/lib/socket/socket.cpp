#include "socket.h"

StaticJsonDocument<200> doc;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        lastPingMillis = millis();
        pingFlag = true;
        if (modeDebug)
        {
          if (CH1_Live == true) {
            SendStatus(1, CH1_CurrStatus);
          }
          if (CH2_Live == true) {
            SendStatus(2, CH2_CurrStatus);
          }
        }
      }
      break;
    case WStype_TEXT:
      {
        Serial.printf("[WSc] get text: %s\n", payload);
        deserializeJson(doc, payload);
        String title = doc["title"];
        if (title == "GetData")
        {
          StaticJsonDocument<600> MyStatus;
          MyStatus["title"] = "GetData";
          if (modeDebug)
          {
            MyStatus["debug"] = "No";
          }
          else
          {
            MyStatus["debug"] = "Yes";
          }
          if (CH1_Mode)
          {
            MyStatus["ch1_mode"] = "Wash";
          }
          else
          {
            MyStatus["ch1_mode"] = "Dry";
          }
          if (CH2_Mode)
          {
            MyStatus["ch2_mode"] = "Wash";
          }
          else
          {
            MyStatus["ch2_mode"] = "Dry";
          }
          if (CH1_CurrStatus)
          {
            MyStatus["ch1_status"] = "Not Working";
          }
          else
          {
            MyStatus["ch1_status"] = "Working";
          }
          if (CH2_CurrStatus)
          {
            MyStatus["ch2_status"] = "Not Working";
          }
          else
          {
            MyStatus["ch2_status"] = "Working";
          }
          MyStatus["CH1_Curr_W"] = CH1_Curr_W;
          MyStatus["CH2_Curr_W"] = CH2_Curr_W;
          MyStatus["CH1_Flow_W"] = CH1_Flow_W;
          MyStatus["CH2_Flow_W"] = CH2_Flow_W;
          MyStatus["CH1_Curr_D"] = CH1_Curr_D;
          MyStatus["CH2_Curr_D"] = CH2_Curr_D;
          MyStatus["CH1_EndDelay_W"] = CH1_EndDelay_W;
          MyStatus["CH2_EndDelay_W"] = CH2_EndDelay_W;
          MyStatus["CH1_EndDelay_D"] = CH1_EndDelay_D;
          MyStatus["CH2_EndDelay_D"] = CH2_EndDelay_D;
          MyStatus["ch1_deviceno"] = CH1_DeviceNo;
          MyStatus["ch2_deviceno"] = CH2_DeviceNo;
          MyStatus["ch1_current"] = Amps_TRMS1;
          MyStatus["ch2_current"] = Amps_TRMS2;
          MyStatus["ch1_flow"] = lHour1;
          MyStatus["ch2_flow"] = lHour2;
          MyStatus["ch1_drain"] = WaterSensorData1;
          MyStatus["ch2_drain"] = WaterSensorData2;
          MyStatus["wifi_ssid"] = apSsid;
          MyStatus["wifi_rssi"] = WiFi.RSSI();
          MyStatus["wifi_ip"] = WiFi.localIP().toString();
          MyStatus["mac"] = WiFi.macAddress();
          MyStatus["fw_ver"] = BUILD_DATE;
          String MyStatus_String;
          serializeJson(MyStatus, MyStatus_String);
          webSocket.sendTXT(MyStatus_String);
        }
      }
      break;
    case WStype_PING:
      lastPingMillis = millis();
      break;
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}

int SendStatus(int ch, bool status) {
  if (ch == 1 && CH1_Live == false)
    return 1;
  if (ch == 2 && CH2_Live == false)
    return 1;
  if (WiFi.status() == WL_CONNECTED && webSocket.isConnected() == true) {
    StaticJsonDocument<100> CurrStatus;
    CurrStatus["title"] = "Update";
    if (ch == 1) {
      CurrStatus["id"] = CH1_DeviceNo;
      CurrStatus["type"] = timeSendFlag1; 
      timeSendFlag1 = 0;
    }
    if (ch == 2) {
      CurrStatus["id"] = CH2_DeviceNo;
      CurrStatus["type"] = timeSendFlag2;
      timeSendFlag2 = 0;
    }
    CurrStatus["state"] = status;
    String CurrStatus_String;
    serializeJson(CurrStatus, CurrStatus_String);
    webSocket.sendTXT(CurrStatus_String);
    return 0;
  } else {
    Serial.println("SendStatus Fail - No Server Connection");
    return 1;
  }
}

int SendLog(int ch, String log) {
  if (ch == 1 && CH1_Live == false)
    return 1;
  if (ch == 2 && CH2_Live == false)
    return 1;
  if (WiFi.status() == WL_CONNECTED && webSocket.isConnected() == true) {
    DynamicJsonDocument LogData(1024);
    LogData["title"] = "Log";
    if (ch == 1)
      LogData["id"] = CH1_DeviceNo;
    if (ch == 2)
      LogData["id"] = CH2_DeviceNo;
    LogData["log"] = log;
    String LogData_String;
    serializeJson(LogData, LogData_String);
    webSocket.sendTXT(LogData_String);
    return 0;
  }
  else
  {
    Serial.println("SendLog Fail - No Server Connection");
    return 1;
  }
}