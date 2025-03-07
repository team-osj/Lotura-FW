#include "main.h"

bool rebooting = false;
bool pingFlag = false;

uint64_t previousMillis = 0;
uint64_t previousMillisEnd1 = 0;
uint64_t previousMillisEnd2 = 0;
uint64_t ledMillisPrev;
uint64_t currMillis;
uint64_t serverRetryMillis;
uint64_t lastPingMillis = 0;

int m1 = 0;
int m2 = 0;

uint32_t timeSendFlag1 = 0;
uint32_t timeSendFlag2 = 0;

bool modeDebug = false;

bool ledStatus = 0;

// 작동 시작 조건
float CH1_Curr_W;
float CH2_Curr_W;
uint32_t CH1_Flow_W;
uint32_t CH2_Flow_W;
float CH1_Curr_D;
float CH2_Curr_D;

// 작동 종료 조건
uint32_t CH1_EndDelay_W;
uint32_t CH2_EndDelay_W;
uint32_t CH1_EndDelay_D;
uint32_t CH2_EndDelay_D;

bool CH1_Mode = false;
bool CH2_Mode = false;
bool CH1_CurrStatus = 1;
bool CH2_CurrStatus = 1;
int CH1_Cnt = 1;
int CH2_Cnt = 1;
bool CH1_Live = true;
bool CH2_Live = true;

String defaultDeviceName = "OSJ_";
String deviceName;
String apSsid;
String apPasswd;
String serialNo;
String authId;
String authPasswd;
String CH1_DeviceNo;
String CH2_DeviceNo;
String roomNo = "0";

bool wifiFail = 1;

// 전류
float Amps_TRMS1;
float Amps_TRMS2;

// 배수
int WaterSensorData1 = 0;
int WaterSensorData2 = 0;

// 유량
int flowFrequency1;
int flowFrequency2;
uint32_t lHour1;
uint32_t lHour2;

char jsonLogFlag1 = 0;
char jsonLogFlag2 = 0;
char jsonLogFlag1_c = 0;
char jsonLogFlag2_c = 0;
char jsonLogFlag1_f = 0;
char jsonLogFlag2_f = 0;
char jsonLogFlag1_w = 0;
char jsonLogFlag2_w = 0;
int jsonLogMillis1 = 0;
int jsonLogMillis2 = 0;
int jsonLogCnt1 = 1;
int jsonLogCnt2 = 1;

int sePrevMillis1 = 0;
int sePrevMillis2 = 0;
int seCnt1 = 0;
int seCnt2 = 0;

void setup() {
  preferences.begin("config", false);
  Serial.begin(115200);
  Serial.print("FW_VER : ");
  Serial.println(BUILD_DATE);

  REG_WRITE(GPIO_ENABLE_W1TS_REG, (1ULL << PIN_STATUS) | (1ULL << PIN_CH1_LED) | (1ULL << PIN_CH2_LED)); // 17,18,19 OUTPUT SET
  REG_WRITE(GPIO_ENABLE_W1TC_REG, (1ULL << PIN_DEBUG) | (1ULL << PIN_FLOW1) | (1ULL << PIN_FLOW2) | (1ULL << PIN_DRAIN1) | (1ULL << PIN_DRAIN2)); // 14,27,26,23,25 INPUT SET
  REG_WRITE(GPIO_ENABLE1_W1TC_REG, (1ULL << (PIN_CH1_MODE % 32)) | (1ULL << (PIN_CH2_MODE % 32)) | (1ULL << (PIN_CT1 % 32)) | (1ULL << (PIN_CT2 % 32))); // 33,32,35,34 INPUT SET
  REG_WRITE(GPIO_PIN33_REG, GPIO_PIN33_REG | (1 << 10)); // 33 PULLUP SET
  REG_WRITE(GPIO_PIN32_REG, GPIO_PIN32_REG | (1 << 10)); // 32 PULLUP SET

  ct1.current(PIN_CT1, 30.7);
  ct2.current(PIN_CT2, 30.7);
  attachInterrupt(digitalPinToInterrupt(PIN_FLOW1), flow1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_FLOW2), flow2, FALLING);
  SetDefaultVal();
  modeDebug = digitalRead(PIN_DEBUG);
  CH1_Mode = digitalRead(PIN_CH1_MODE);
  CH2_Mode = digitalRead(PIN_CH2_MODE);
  if (modeDebug == 0) {
    Serial.println("YOU ARE IN THE DEBUG MODE !!!");
  }
  Serial.print("CH1_Mode : ");
  Serial.println(CH1_Mode);
  Serial.print("CH2_Mode : ");
  Serial.println(CH2_Mode);
  WiFi.disconnect(true);

  // 센서 안정화
  for (int i = 0; i < 30; i++) {
    ct1.calcIrms(1480);
    ct2.calcIrms(1480);
  }
  Serial.print("Boot Heap : ");
  Serial.println(ESP.getFreeHeap());
  digitalWrite(PIN_STATUS, HIGH);
  if (apSsid == "") {
    Serial.println("Skip WiFi Setting Due to No SSID");
  } else {
    Serial.print("Connecting to WiFi .. ");
    Serial.println(apSsid);
    if (apPasswd == "") {
      WiFi.begin(apSsid);
    } else {
      WiFi.begin(apSsid, apPasswd);
    }

    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    int wifiTimeout = 0;

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      digitalWrite(PIN_STATUS, LOW);
      delay(100);
      digitalWrite(PIN_STATUS, HIGH);
      delay(100);
      wifiTimeout++;
      if (wifiTimeout > 25) {
        Serial.println("Skip WiFi Connection Due to Timeout");
        break;
      } 
    }
  }

}

void loop() {
  currMillis = millis();
  if(pingFlag == true && webSocket.isConnected() == true && currMillis - lastPingMillis >= PING_LATE_MILLIS) {
    pingFlag = false;
    webSocket.disconnect();
  }
  if(rebooting) {
    delay(100);
    ESP.restart();
  }
  if (WiFi.status() == WL_CONNECTED) {
    webSocket.loop();
    digitalWrite(PIN_STATUS, HIGH);
    wifiFail = 0;
  } else {
    wifiFail = 1;
    if (currMillis - ledMillisPrev >= 100) {
      ledMillisPrev = currMillis;
      digitalWrite(PIN_STATUS, !digitalRead(PIN_STATUS));
    }
  }

  if (modeDebug) {
    Amps_TRMS1 = ct1.calcIrms(1480);
    Amps_TRMS2 = ct2.calcIrms(1480);

    if (previousMillis > millis()) {
      previousMillis = millis();
    }

    if (millis() - previousMillis >= sensPeriod) {
      previousMillis = millis();

      WaterSensorData1 = digitalRead(PIN_DRAIN1);
      WaterSensorData2 = digitalRead(PIN_DRAIN2);

      lHour1 = (flowFrequency1 * 60/7.5); // L/hour계산
      lHour2 = (flowFrequency2 * 60/7.5);

      flowFrequency1 = 0;
      flowFrequency2 = 0;
    }

    if (CH1_Mode) {
      Status_Judgment(Amps_TRMS1, WaterSensorData1, lHour1, CH1_Cnt, m1, previousMillisEnd1, 1);
    } else {
      Dryer_Status_Judgment(Amps_TRMS1, CH1_Cnt, m1, previousMillisEnd1, 1);
    }

    if (CH2_Mode) {
      Status_Judgment(Amps_TRMS2, WaterSensorData2, lHour2, CH2_Cnt, m2, previousMillisEnd2, 2);
    } else {
      Dryer_Status_Judgment(Amps_TRMS2, CH2_Cnt, m2, previousMillisEnd2, 2);
    }
  }
  // debug mode
  else {
    currMillis = millis();
    if (currMillis - ledMillisPrev >= 100)
    {
      ledMillisPrev = currMillis;
      if (ledStatus == 1) {
        ledStatus = 0;
        digitalWrite(PIN_CH1_LED, HIGH);
        digitalWrite(PIN_CH2_LED, LOW);
      } else if (ledStatus == 0) {
        ledStatus = 1;
        digitalWrite(PIN_CH1_LED, LOW);
        digitalWrite(PIN_CH2_LED, HIGH);
      }
    }
    if (Serial.available()) {
      int dex, dex1, dexc, end;
      String SerialData = Serial.readStringUntil('\n');
      dex = SerialData.indexOf('+');
      dex1 = SerialData.indexOf('"');
      end = SerialData.length();
      String AT_Command = SerialData.substring(dex+1, dex1);
      if (!(AT_Command.compareTo("HELP"))) {
      } else if (!(AT_Command.compareTo("SENSDATA_START"))) {
        Serial.println("AT+OK SENSDATA_START");
      } else if (!(AT_Command.compareTo("SOCKET_SEND"))) {
      } else if (!(AT_Command.compareTo("UPDATE"))) {
        Serial.println("AT+OK UPDATE");
      } else if (!(AT_Command.compareTo("CH1_SETVAR"))) {
        Serial.println("AT+OK CH1_SETVAR");
        CH1_SETVAR(SerialData, dex1, dexc, end);
      } else if (!(AT_Command.compareTo("CH2_SETVAR"))) {
        Serial.println("AT+OK CH2_SETVAR");
        CH2_SETVAR(SerialData, dex1, dexc, end);
      } else if (!(AT_Command.compareTo("UPDATE"))) {
        Serial.println("AT+OK UPDATE");
      } else if (!(AT_Command.compareTo("NETWORK_INFO"))) {
        Serial.println("AT+OK NETWORK_INFO");
        NETWORK_INFO();
      } else if (!(AT_Command.compareTo("SETAP_SSID"))) {
        Serial.println("AT+OK SETAP_SSID");
        putString("ap_ssid", SerialData.substring(dex1+1, end - 1));
      } else if (!(AT_Command.compareTo("SETAP_PASSWD"))) {
        Serial.println("AT+OK SETAP_PASSWD");
        putString("ap_passwd", SerialData.substring(dex1+1, end - 1));
      } else if (!(AT_Command.compareTo("SET_SERIALNO"))) {
        Serial.println("AT+OK SET_SERIALNO");
        putString("serial_no", SerialData.substring(dex1+1, end - 1));
      } else if (!(AT_Command.compareTo("SET_AUTH_ID"))) {
        Serial.println("AT+OK SET_AUTH_ID");
        putString("AUTH_ID", SerialData.substring(dex1+1, end - 1));
      } else if (!(AT_Command.compareTo("SET_AUTH_PASSWD"))) {
        Serial.println("AT+OK SET_AUTH_PASSWD");
        putString("AUTH_PASSWD", SerialData.substring(dex1+1, end - 1));
      } else if (!(AT_Command.compareTo("FORMAT_NVS"))) {
        Serial.println("AT+OK FORMAT_NVS");
        nvs_flash_erase();
        nvs_flash_init();
        ESP.restart();
      } else if (!(AT_Command.compareTo("SHOWMETHEMONEY"))) {
        Serial.println("AT+OK SHOWMETHEMONEY");
        Serial.print(ESP.getFreeHeap());
        Serial.println("Byte");
      } else if (!(AT_Command.compareTo("WHATTIMEISIT"))) {
        Serial.println("AT+OK WHATTIMEISIT");
      } else if (!(AT_Command.compareTo("REBOOT"))) {
        Serial.println("AT+OK REBOOT");
        delay(500);
        ESP.restart();
      } else {
        Serial.println("ERROR: Unknown command");
      }
    }
  }
}