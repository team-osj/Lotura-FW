#include "wifiEvent.h"

void putString(const char *key, String value)
{
  Serial.print(key);
  Serial.print(" = ");
  Serial.println(value);
  preferences.putString(key, value);
} 
 
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("AP Connected");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.print("WiFi connected ");
  Serial.println(WiFi.localIP());
  webSocket.disconnect();
  serverRetryMillis = currMillis;
  String ip = WiFi.localIP().toString();
  // 코드 진행 순서 변경 금지
  webSocket.beginSSL(SERVER_DOMAIN, SERVER_PORT, SERVER_URL);
  char HeaderData[35];
  sprintf(HeaderData, "HWID: %s\r\nCH1: %s\r\nCH2: %s\r\nROOM: %s", serialNo.c_str(), CH1_DeviceNo.c_str(), CH2_DeviceNo.c_str(), roomNo.c_str());
  webSocket.setExtraHeaders(HeaderData);
  webSocket.setAuthorization(authId.c_str(), authPasswd.c_str());
  webSocket.onEvent(webSocketEvent);
  MDNS.begin(deviceName);
  // 코드 진행 순서 변경 금지
  Serial.printf("Host: http://%s.local/\n", deviceName);
  setupAsyncServer();
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  WiFi.disconnect(true);
  Serial.print("WiFi Lost. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  wifiFail = 1;
  WiFi.begin(apSsid, apPasswd);
}

const char *WiFiStatusCode(wl_status_t status)
{
  switch (status)
  {
    case WL_NO_SHIELD:
      return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:
      return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:
      return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:
      return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:
      return "WL_CONNECTED";
    case WL_CONNECT_FAILED:
      return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:
      return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:
      return "WL_DISCONNECTED";
  }
}