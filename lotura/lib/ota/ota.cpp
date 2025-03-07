#include "ota.h"

AsyncWebServer server(80);
WiFiClient client;

String reset_reason(int reason) {
  switch (reason) {
    case 1 : return "POWERON_RESET";            /**<1,  Vbat power on reset*/
    case 3 : return "SW_RESET";                 /**<3,  Software reset digital core*/
    case 4 : return "OWDT_RESET";               /**<4,  Legacy watch dog reset digital core*/
    case 5 : return "DEEPSLEEP_RESET";          /**<5,  Deep Sleep reset digital core*/
    case 6 : return "SDIO_RESET";               /**<6,  Reset by SLC module, reset digital core*/
    case 7 : return "TG0WDT_SYS_RESET";         /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : return "TG1WDT_SYS_RESET";         /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9 : return "RTCWDT_SYS_RESET";         /**<9,  RTC Watch dog Reset digital core*/
    case 10 : return "INTRUSION_RESET";         /**<10, Instrusion tested to reset CPU*/
    case 11 : return "TGWDT_CPU_RESET";         /**<11, Time Group reset CPU*/
    case 12 : return "SW_CPU_RESET";            /**<12, Software reset CPU*/
    case 13 : return "RTCWDT_CPU_RESET";        /**<13, RTC Watch dog Reset CPU*/
    case 14 : return "EXT_CPU_RESET";           /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : return "RTCWDT_BROWN_OUT_RESET";  /**<15, Reset when the vdd voltage is not stable*/
    case 16 : return "RTCWDT_RTC_RESET";        /**<16, RTC Watch dog reset digital core and rtc module*/
    default : return "NO_MEAN";
  }
}

String processor(const String& var) {
  if(var == "DEVICE_NAME") {
    return deviceName;
  }
  if(var == "SSID") {
    return apSsid;
  }
  if(var == "PASS") {
    return apPasswd;
  }
  if(var == "RSSI") {
    return String(WiFi.RSSI());
  }
  if(var == "WIFI_QUALITY") {
    if (WiFi.status() != WL_CONNECTED) {
      return "WiFi Not Connected";
    }
    int rssi = WiFi.RSSI();
    if(rssi > -40) {
      return "Very Good";
    } else if(rssi > -60) {
      return "Good";
    } else if(rssi > -70) {
      return "Weak";
    } else {
      return "Poor";
    }
  }
  if(var == "IP") {
    return WiFi.localIP().toString();
  }
  if(var == "MAC") {
    return WiFi.macAddress();
  }
  if(var == "RoomNo") {
    return roomNo;
  }
  if(var == "TCP_STATUS") {
    if(client.connected()) {
      return "Connected";
    } else {
      return "Disconnected";
    }
  }
  if(var == "FlashSize") {
    return String(ESP.getFlashChipSize()/1024);
  }
  if(var == "Heap") {
    return String(ESP.getFreeHeap()/1024);
  }
  if(var == "BUILD_VER") {
    return BUILD_DATE;
  }
  // CH1
  if(var == "CH1_DeviceNo") {
    return CH1_DeviceNo;
  }
  if(var == "CH1_Live") {
    if(CH1_Live) {
      return "Yes";
    }
    else {
      return "NO";
    }
  }
  if(var == "CH1_Mode") {
    if(CH1_Mode) {
      return "Wash";
    } else {
      return "Dry";
    }
  }
  if(var == "CH1_Curr_W") {
    return String(CH1_Curr_W);
  }
  if(var == "CH1_Flow_W") {
    return String(CH1_Flow_W);
  }
  if(var == "CH1_Curr_D") {
    return String(CH1_Curr_D);
  }
  if(var == "CH1_EndDelay_W") {
    return String(CH1_EndDelay_W);
  }
  if(var == "CH1_EndDelay_D") {
    return String(CH1_EndDelay_D);
  }
  if(var == "Amps_TRMS1") {
    return String(Amps_TRMS1);
  }
  if(var == "WaterSensorData1") {
    return String(WaterSensorData1);
  }
  if(var == "l_hour1") {
    return String(lHour1);
  }
  // CH2
  if(var == "CH2_DeviceNo") {
    return CH2_DeviceNo;
  }
  if(var == "CH2_Live") {
    if(CH2_Live) {
      return "Yes";
    } else {
      return "NO";
    }
  }
  if(var == "CH2_Mode") {
    if(CH2_Mode) {
      return "Wash";
    } else {
      return "Dry";
    }
  }
  if(var == "CH2_Curr_W") {
    return String(CH2_Curr_W);
  }
  if(var == "CH2_Flow_W") {
    return String(CH2_Flow_W);
  }
  if(var == "CH2_Curr_D") {
    return String(CH2_Curr_D);
  }
  if(var == "CH2_EndDelay_W") {
    return String(CH2_EndDelay_W);
  }
  if(var == "CH2_EndDelay_D") {
    return String(CH2_EndDelay_D);
  }
  if(var == "Amps_TRMS2") {
    return String(Amps_TRMS2);
  }
  if(var == "WaterSensorData2") {
    return String(WaterSensorData2);
  }
  if(var == "l_hour2") {
    return String(lHour2);
  }
  return String();
}

void setupAsyncServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(!request->authenticate(authId.c_str(), authPasswd.c_str())) {
      return request->requestAuthentication();
    }
    request->send_P(200, "text/html", managerHtml, processor);
  });

  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    for(int i=0;i<params;i++) {
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()) {
        if (p->name() == "WiFi_SSID") {
          String ssid = p->value().c_str();
          Serial.print("SSID set to : ");
          Serial.println(ssid);
          putString("ap_ssid", ssid);
        }
        if (p->name() == "WiFi_PASS") {
          String pass = p->value().c_str();
          Serial.print("Password set to : ");
          Serial.println(pass);
          putString("ap_passwd", pass);
        }
      }
    }
    request->redirect("/");
  });

  server.on("/auth", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()){
        if (p->name() == "AUTH_ID") {
          authId = p->value().c_str();
          Serial.print("AUTH_ID : ");
          Serial.println(authId);
          if(authId != "") {
            putString("AUTH_ID", authId);
          }
        }
        if (p->name() == "AUTH_PASSWD") {
          authPasswd = p->value().c_str();
          Serial.print("AUTH_PASSWD : ");
          Serial.println(authPasswd);
          if(authPasswd != "") {
            putString("AUTH_PASSWD", authPasswd);
          }
        }
      }
    }
    request->redirect("/");
  });

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    rebooting = !Update.hasError();
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", rebooting ? okHtml : failedHtml);

    response->addHeader("Connection", "close");
    request->send(response);
  },
  [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if(!index) {
      Serial.print("Updating : ");
      Serial.println(filename.c_str());

      if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
        Update.printError(Serial);
      }
    }
    if(!Update.hasError()) {
      if(Update.write(data, len) != len) {
        Update.printError(Serial);
      }
    }
    if(final) {
      if(Update.end(true)) {
        Serial.print("The update is finished : ");
        Serial.println(convertFileSize(index+len));
      } else {
        Update.printError(Serial);
      }
    }
  });
  
  server.on("/CH1", HTTP_POST, [](AsyncWebServerRequest *request) {
    String Command;
    String Value;
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()) {
        if (p->name() == "CH1") {
          Command = p->value().c_str();
          Serial.print("CH1_Commend : ");
          Serial.println(Command);
        }
        if (p->name() == "value") {
          Value = p->value().c_str();
          Serial.print("CH1_Value : ");
          Serial.println(Value);
          if(Value != ""){
            if (Command == "DeviceNo")
            {
              Serial.print("CH1_DeviceNo : ");
              Serial.println(Value);
              preferences.putString("CH1_DeviceNo", Value);
            } else if (Command == "Current_Wash")
            {
              Serial.print("CH1_Curr_W : ");
              Serial.println(Value);
              float var = Value.toFloat();
              preferences.putFloat("CH1_Curr_W", var);
            } else if (Command == "Flow_Wash") {
              Serial.print("CH1_Flow_W : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putUInt("CH1_Flow_W", var);
            } else if (Command == "Current_Dry") {
              Serial.print("CH1_Curr_D : ");
              Serial.println(Value);
              float var = Value.toFloat();
              preferences.putFloat("CH1_Curr_D", var);
            } else if (Command == "EndDelay_Wash") {
              Serial.print("CH1_EndDelay_W : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putUInt("CH1_EndDelay_W", var);
            } else if (Command == "EndDelay_Dry") {
              Serial.print("CH1_EndDelay_D : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putUInt("CH1_EndDelay_D", var);
            } else if (Command == "Enable") {
              Serial.print("CH1_Enable : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putBool("CH1_Live", var);
            }
          }
        }
      }
    }
    request->redirect("/");
  });
  
  server.on("/CH2", HTTP_POST, [](AsyncWebServerRequest *request) {
    String Command;
    String Value;
    int params = request->params();
    for(int i=0;i<params;i++) {
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()) {
        if (p->name() == "CH2") {
          Command = p->value().c_str();
          Serial.print("CH2_Commend : ");
          Serial.println(Command);
        }
        if (p->name() == "value") {
          Value = p->value().c_str();
          Serial.print("CH2_Value : ");
          Serial.println(Value);
          if(Value != "") {
            if (Command == "DeviceNo") {
              Serial.print("CH2_DeviceNo : ");
              Serial.println(Value);
              preferences.putString("CH2_DeviceNo", Value);
            } else if (Command == "Current_Wash") {
              Serial.print("CH2_Curr_W : ");
              Serial.println(Value);
              float var = Value.toFloat();
              preferences.putFloat("CH2_Curr_W", var);
            } else if (Command == "Flow_Wash") {
              Serial.print("CH2_Flow_W : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putUInt("CH2_Flow_W", var);
            } else if (Command == "Current_Dry") {
              Serial.print("CH2_Curr_D : ");
              Serial.println(Value);
              float var = Value.toFloat();
              preferences.putFloat("CH2_Curr_D", var);
            } else if (Command == "EndDelay_Wash") {
              Serial.print("CH2_EndDelay_W : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putUInt("CH2_EndDelay_W", var);
            } else if (Command == "EndDelay_Dry") {
              Serial.print("CH2_EndDelay_D : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putUInt("CH2_EndDelay_D", var);
            } else if (Command == "Enable") {
              Serial.print("CH2_Enable : ");
              Serial.println(Value);
              int var = Value.toInt();
              preferences.putBool("CH2_Live", var);
            }
          }
        }
      }
    }
    request->redirect("/");
  });

  server.on("/roomno", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    String Value;
    for(int i=0;i<params;i++) {
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()) {
        if (p->name() == "RoomNo") {
          Value = p->value().c_str();
          Serial.print("RoomNo : ");
          Serial.println(Value);
          if(Value != "") {
            preferences.putString("RoomNo", Value);
          }
        }
      }
    }
    request->redirect("/");
  });

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(!request->authenticate(authId.c_str(), authPasswd.c_str())) {
      return request->requestAuthentication();
    }
    request->redirect("/");
    delay(200);
    ESP.restart();
  });

  server.on("/SetDefaultVal", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(!request->authenticate(authId.c_str(), authPasswd.c_str())) {
      return request->requestAuthentication();
    }
    request->redirect("/");
    delay(200);
    SetDefaultVal();
  });
  
  server.onNotFound(notFound);

  server.begin();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404);
}

String convertFileSize(const size_t bytes) {
  if(bytes < 1024) {
    return String(bytes)+" B";
  }
  else if (bytes < 1048576) {
    return String(bytes/1024.0)+" kB";
  }
  else if (bytes < 1073741824) {
    return String(bytes/1048576.0)+" MB";
  }
}