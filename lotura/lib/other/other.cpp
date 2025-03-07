#include "other.h"

void SetDefaultVal() {
  apSsid = preferences.getString("ap_ssid", "");
  apPasswd = preferences.getString("ap_passwd", "");
  serialNo = preferences.getString("serial_no", "0");
  authId = preferences.getString("AUTH_ID", "");
  authPasswd = preferences.getString("AUTH_PASSWD", "");

  CH1_DeviceNo = preferences.getString("CH1_DeviceNo", "1");
  CH2_DeviceNo = preferences.getString("CH2_DeviceNo", "2");

  CH1_Curr_W = preferences.getFloat("CH1_Curr_W", 0.2);
  CH2_Curr_W = preferences.getFloat("CH2_Curr_W", 0.2);
  CH1_Flow_W = preferences.getUInt("CH1_Flow_W", 50);
  CH2_Flow_W = preferences.getUInt("CH2_Flow_W", 50);
  CH1_Curr_D = preferences.getFloat("CH1_Curr_D", 0.5);
  CH2_Curr_D = preferences.getFloat("CH2_Curr_D", 0.5);

  CH1_EndDelay_W = preferences.getUInt("CH1_EndDelay_W", 10);
  CH2_EndDelay_W = preferences.getUInt("CH2_EndDelay_W", 10);
  CH1_EndDelay_D = preferences.getUInt("CH1_EndDelay_D", 10);
  CH2_EndDelay_D = preferences.getUInt("CH2_EndDelay_D", 10);

  CH1_Live = preferences.getBool("CH1_Live", true);
  CH2_Live = preferences.getBool("CH2_Live", true);

  roomNo = preferences.getString("RoomNo", "0");

  deviceName = defaultDeviceName+serialNo;
  WiFi.setHostname(deviceName.c_str());
  Serial.print("My Name Is : ");
  Serial.println(deviceName);
  Serial.print("CH1 : ");
  Serial.print(CH1_DeviceNo);
  Serial.print(" CH2 : ");
  Serial.println(CH2_DeviceNo);
  if (authId == "" || authPasswd == "") {
    Serial.println("NO AUTH CODE!!! YOU NEED TO CONFIG SERVER AUTHENTICATION BY AT+SET_AUTH_ID AND AT+SET_AUTH_PASSWD IN DEBUG MODE!!!");
  }
  if (apSsid == "") {
    Serial.println("NO WIFI SSID!!! YOU NEED TO CONFIG WIFI BY AT+SETAP_SSID AND AT+SETAP_PASSWD IN DEBUG MODE!!!");
  }
  CH1_EndDelay_W *= 10000;
  CH2_EndDelay_W *= 10000;
  CH1_EndDelay_D *= 1000;
  CH2_EndDelay_D *= 1000;
  Serial.print("CH1_Curr_Wash : ");
  Serial.print(CH1_Curr_W);
  Serial.print(" CH2_Curr_Wash : ");
  Serial.println(CH2_Curr_W);

  Serial.print("CH1_Flow_Wash : ");
  Serial.print(CH1_Flow_W);
  Serial.print(" CH2_Flow_Wash : ");
  Serial.println(CH2_Flow_W);

  Serial.print("CH1_Delay_Wash : ");
  Serial.print(CH1_EndDelay_W);
  Serial.print(" CH2_Delay_Wash : ");
  Serial.println(CH2_EndDelay_W);

  Serial.print("CH1_Curr_Dry : ");
  Serial.print(CH1_Curr_D);
  Serial.print(" CH2_Curr_Dry : ");
  Serial.println(CH2_Curr_D);

  Serial.print("CH1_Delay_Dry : ");
  Serial.print(CH1_EndDelay_D);
  Serial.print(" CH2_Delay_Dry : ");
  Serial.println(CH2_EndDelay_D);

  Serial.print("CH1_Enable : ");
  Serial.print(CH1_Live);
  Serial.print(" CH2_Enable : ");
  Serial.println(CH2_Live);
}

void NETWORK_INFO() {
  Serial.print("Name = ");
  Serial.println(deviceName);
  Serial.println(WiFiStatusCode(WiFi.status()));
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("RSSI = ");
    Serial.println(WiFi.RSSI());
    String ip = WiFi.localIP().toString();
    Serial.printf("Local IP = % s\r\n", ip.c_str());
  }
  Serial.print("MAC = ");
  Serial.println(WiFi.macAddress());
  Serial.print("SSID = ");
  Serial.println(apSsid);
  Serial.print("PASSWORD = ");
  Serial.print(apPasswd);
}