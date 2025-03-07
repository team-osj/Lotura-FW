#include "state.h"

DynamicJsonDocument jsonLog1(1024);
DynamicJsonDocument jsonLog2(1024);

void Dryer_Status_Judgment(float Amps_TRMS, int cnt, int m, unsigned long previousMillisEnd, int ChannelNum) {
  if (ChannelNum == 1 && Amps_TRMS < CH1_Curr_D && jsonLogFlag1) {
    if (jsonLogFlag1_c == 1) {
      jsonLogFlag1_c = 0;
      String jsonLogCnt1String = String(jsonLogCnt1);
      jsonLog1[jsonLogCnt1String]["t"] = millis() - jsonLogMillis1;
      jsonLog1[jsonLogCnt1String]["n"] = "C";
      jsonLog1[jsonLogCnt1String]["s"] = 0;
      if (jsonLogCnt1 % 100 == 0) {
        String jsonLogData1 = "";
        serializeJson(jsonLog1, jsonLogData1);
        jsonLog1.clear();
        SendLog(1, jsonLogData1);
      }
      jsonLogCnt1++;
    }
  }
  if (ChannelNum == 2 && Amps_TRMS < CH2_Curr_D && jsonLogFlag2) {
    if (jsonLogFlag2_c == 1) {
      jsonLogFlag2_c = 0;
      String jsonLogCnt2String = String(jsonLogCnt2);
      jsonLog2[jsonLogCnt2String]["t"] = millis() - jsonLogMillis2;
      jsonLog2[jsonLogCnt2String]["n"] = "C";
      jsonLog2[jsonLogCnt2String]["s"] = 0;
      if (jsonLogCnt2 % 100 == 0) {
        String jsonLogData2 = "";
        serializeJson(jsonLog2, jsonLogData2);
        jsonLog2.clear();
        SendLog(2, jsonLogData2);
      }
      jsonLogCnt2++;
    }
  }
  if (ChannelNum == 1 && Amps_TRMS > CH1_Curr_D) {
    if (jsonLogFlag1) {
      if (jsonLogFlag1_c == 0) {
        jsonLogFlag1_c = 1;
        String jsonLogCnt1String = String(jsonLogCnt1);
        jsonLog1[jsonLogCnt1String]["t"] = millis() - jsonLogMillis1;
        jsonLog1[jsonLogCnt1String]["n"] = "C";
        jsonLog1[jsonLogCnt1String]["s"] = 1;
        if (jsonLogCnt1 % 100 == 0) {
          String jsonLogData1 = "";
          serializeJson(jsonLog1, jsonLogData1);
          jsonLog1.clear();
          SendLog(1, jsonLogData1);
        }
        jsonLogCnt1++;
      }
    }
    // CH1 건조기 동작 시작
    if (cnt == 1) {
      timeSendFlag1 = 1;
      jsonLogFlag1 = 1;
      jsonLogCnt1 = 1;
      jsonLogMillis1 = millis();
      String localTime = "";
      jsonLog1["START"]["local_time"] = localTime;

      CH1_Cnt = 0;
      digitalWrite(PIN_CH1_LED, HIGH);
      CH1_CurrStatus = 0;
      Serial.print("CH");
      Serial.print(ChannelNum);
      Serial.println(" Dryer Started");
      SendStatus(ChannelNum, 0);
    }
    m1 = 1;
  } else if (ChannelNum == 2 && Amps_TRMS > CH2_Curr_D) {
    if (jsonLogFlag2) {
      if (jsonLogFlag2_c == 0) {
        jsonLogFlag2_c = 1;
        String json_log_cnt2_string = String(jsonLogCnt2);
        jsonLog2[json_log_cnt2_string]["t"] = millis() - jsonLogMillis2;
        jsonLog2[json_log_cnt2_string]["n"] = "C";
        jsonLog2[json_log_cnt2_string]["s"] = 1;
        if (jsonLogCnt2 % 100 == 0) {
          String jsonLogData2 = "";
          serializeJson(jsonLog2, jsonLogData2);
          jsonLog2.clear();
          SendLog(2, jsonLogData2);
        }
        jsonLogCnt2++;
      }
    }
    // CH2 건조기 동작 시작
    if (cnt == 1) {
      timeSendFlag2 = 1;

      jsonLogFlag2 = 1;
      jsonLogCnt2 = 1;
      jsonLogMillis2 = millis();

      String localTime = "";
      jsonLog2["START"]["local_time"] = localTime;

      CH2_Cnt = 0;
      digitalWrite(PIN_CH2_LED, HIGH);
      CH2_CurrStatus = 0;
      Serial.print("CH");
      Serial.print(ChannelNum);
      Serial.println(" Dryer Started");
      SendStatus(ChannelNum, 0);
    }
    m2 = 1;
  } else {
    if (previousMillisEnd > millis())
      previousMillisEnd = millis();
    if (m) {
      if (ChannelNum == 1)
        previousMillisEnd = millis();
      if (ChannelNum == 2)
        previousMillisEnd = millis();
      if (ChannelNum == 1)
        m1 = 0;
      if (ChannelNum == 2)
        m2 = 0;
    } else if (cnt)
      ;
    // CH1 건조기 동작 종료 
    else if (ChannelNum == 1 && millis() - previousMillisEnd >= CH1_EndDelay_D) {
      timeSendFlag1 = 1;

      jsonLogFlag1_c = 0;
      jsonLogFlag1 = 0;


      String localTime = "";
      jsonLog1["END"]["local_time"] = localTime;

      String jsonLogData1 = "";
      serializeJson(jsonLog1, jsonLogData1);
      jsonLog1.clear();
      Serial.println("CH1 Dryer Ended");
      SendStatus(1, 1);
      SendLog(1, jsonLogData1);
      CH1_Cnt = 1;
      digitalWrite(PIN_CH1_LED, LOW);
      CH1_CurrStatus = 1;
    }
    // CH2 건조기 동작 종료
    else if (ChannelNum == 2 && millis() - previousMillisEnd >= CH2_EndDelay_D) {
      timeSendFlag2 = 1;

      jsonLogFlag2_c = 0;
      jsonLogFlag2 = 0;


      String localTime = "";
      jsonLog2["END"]["local_time"] = localTime;

      String jsonLogData2 = "";
      serializeJson(jsonLog2, jsonLogData2);
      jsonLog2.clear();
      Serial.println("CH2 Dryer Ended");
      SendStatus(2, 1);
      SendLog(2, jsonLogData2);
      CH2_Cnt = 1;
      digitalWrite(PIN_CH2_LED, LOW);
      CH2_CurrStatus = 1;
    }
  }
}

void Status_Judgment(float Amps_TRMS, int WaterSensorData, unsigned int lHour, int cnt, int m, unsigned long previousMillisEnd, int ChannelNum) {
  if (ChannelNum == 1 && (Amps_TRMS > CH1_Curr_W || WaterSensorData || lHour > CH1_Flow_W) && seCnt1 == 0) {  //세탁기가 동작하면 millis시작
    seCnt1 = 1;
    sePrevMillis1 = millis();
  }
  if (ChannelNum == 1 && (Amps_TRMS < CH1_Curr_W && !WaterSensorData && lHour < CH1_Flow_W) && seCnt1 == 1) { // 전부 멈추면 시작 취소
    seCnt1 = 0;
  }
  if (ChannelNum == 2 && (Amps_TRMS > CH2_Curr_W || WaterSensorData || lHour > CH2_Flow_W) && seCnt2 == 0) {
    seCnt1 = 1;
    sePrevMillis2 = millis();
  }
  if (ChannelNum == 2 && (Amps_TRMS < CH2_Curr_W && !WaterSensorData && lHour < CH2_Flow_W) && seCnt2 == 1) {
    seCnt1 = 0;
  }

  if (ChannelNum == 1) {
    if (jsonLogFlag1) {
      if (Amps_TRMS > CH1_Curr_W && jsonLogFlag1_c == 0) {
        jsonLogFlag1_c = 1;
        String jsonLogCnt1String = String(jsonLogCnt1);
        jsonLog1[jsonLogCnt1String]["t"] = millis() - jsonLogMillis1;
        jsonLog1[jsonLogCnt1String]["n"] = "C";
        jsonLog1[jsonLogCnt1String]["s"] = 1;
        if (jsonLogCnt1 % 100 == 0) {
          String jsonLogData1 = "";
          serializeJson(jsonLog1, jsonLogData1);
          jsonLog1.clear();
          SendLog(1, jsonLogData1);
        }
        jsonLogCnt1++;
      }
      if (Amps_TRMS < CH1_Curr_W && jsonLogFlag1_c == 1) {
        jsonLogFlag1_c = 0;
        String json_log_cnt1_string = String(jsonLogCnt1);
        jsonLog1[json_log_cnt1_string]["t"] = millis() - jsonLogMillis1;
        jsonLog1[json_log_cnt1_string]["n"] = "C";
        jsonLog1[json_log_cnt1_string]["s"] = 0;
        if (jsonLogCnt1 % 100 == 0) {
          String jsonLogData1 = "";
          serializeJson(jsonLog1, jsonLogData1);
          jsonLog1.clear();
          SendLog(1, jsonLogData1);
        }
        jsonLogCnt1++;
      }

      if (lHour > CH1_Flow_W && jsonLogFlag1_f == 0) {
        jsonLogFlag1_f = 1;
        String jsonLogCnt1String = String(jsonLogCnt1);
        jsonLog1[jsonLogCnt1String]["t"] = millis() - jsonLogMillis1;
        jsonLog1[jsonLogCnt1String]["n"] = "F";
        jsonLog1[jsonLogCnt1String]["s"] = 1;
        if (jsonLogCnt1 % 100 == 0) {
          String jsonLogData1 = "";
          serializeJson(jsonLog1, jsonLogData1);
          jsonLog1.clear();
          SendLog(1, jsonLogData1);
        }
        jsonLogCnt1++;
      }
      if (lHour < CH1_Flow_W && jsonLogFlag1_f == 1) {
        jsonLogFlag1_f = 0;
        String jsonLogCnt1String = String(jsonLogCnt1);
        jsonLog1[jsonLogCnt1String]["t"] = millis() - jsonLogMillis1;
        jsonLog1[jsonLogCnt1String]["n"] = "F";
        jsonLog1[jsonLogCnt1String]["s"] = 0;
        if (jsonLogCnt1 % 100 == 0) {
          String jsonLogData1 = "";
          serializeJson(jsonLog1, jsonLogData1);
          jsonLog1.clear();
          SendLog(1, jsonLogData1);
        }
        jsonLogCnt1++;
      }

      if (WaterSensorData && jsonLogFlag1_w == 0) {
        jsonLogFlag1_w = 1;
        String jsonLogCnt1String = String(jsonLogCnt1);
        jsonLog1[jsonLogCnt1String]["t"] = millis() - jsonLogMillis1;
        jsonLog1[jsonLogCnt1String]["n"] = "W";
        jsonLog1[jsonLogCnt1String]["s"] = 1;
        if (jsonLogCnt1 % 100 == 0) {
          String jsonLogData1 = "";
          serializeJson(jsonLog1, jsonLogData1);
          jsonLog1.clear();
          SendLog(1, jsonLogData1);
        }
        jsonLogCnt1++;
      }
      if (!WaterSensorData && jsonLogFlag1_w == 1) {
        jsonLogFlag1_w = 0;
        String jsonLogCnt1String = String(jsonLogCnt1);
        jsonLog1[jsonLogCnt1String]["t"] = millis() - jsonLogMillis1;
        jsonLog1[jsonLogCnt1String]["n"] = "W";
        jsonLog1[jsonLogCnt1String]["s"] = 0;
        if (jsonLogCnt1 % 100 == 0) {
          String jsonLogData1 = "";
          serializeJson(jsonLog1, jsonLogData1);
          jsonLog1.clear();
          SendLog(1, jsonLogData1);
        }
        jsonLogCnt1++;
      }
    }
  }

  if (ChannelNum == 2) {
    if (jsonLogFlag2) {
      if (Amps_TRMS > CH2_Curr_W && jsonLogFlag2_c == 0) {
        jsonLogFlag2_c = 1;
        String jsonLogCnt2String = String(jsonLogCnt2);
        jsonLog2[jsonLogCnt2String]["t"] = millis() - jsonLogMillis2;
        jsonLog2[jsonLogCnt2String]["n"] = "C";
        jsonLog2[jsonLogCnt2String]["s"] = 1;
        if (jsonLogCnt2 % 100 == 0) {
          String jsonLogData2 = "";
          serializeJson(jsonLog2, jsonLogData2);
          jsonLog2.clear();
          SendLog(2, jsonLogData2);
        }
        jsonLogCnt2++;
      }
      if (Amps_TRMS < CH2_Curr_W && jsonLogFlag2_c == 1) {
        jsonLogFlag2_c = 0;
        String jsonLogCnt2String = String(jsonLogCnt2);
        jsonLog2[jsonLogCnt2String]["t"] = millis() - jsonLogMillis2;
        jsonLog2[jsonLogCnt2String]["n"] = "C";
        jsonLog2[jsonLogCnt2String]["s"] = 0;
        if (jsonLogCnt2 % 100 == 0) {
          String jsonLogData2 = "";
          serializeJson(jsonLog2, jsonLogData2);
          jsonLog2.clear();
          SendLog(2, jsonLogData2);
        }
        jsonLogCnt2++;
      }

      if (lHour > CH2_Flow_W && jsonLogFlag2_f == 0) {
        jsonLogFlag2_f = 1;
        String jsonLogCnt2String = String(jsonLogCnt2);
        jsonLog2[jsonLogCnt2String]["t"] = millis() - jsonLogMillis2;
        jsonLog2[jsonLogCnt2String]["n"] = "F";
        jsonLog2[jsonLogCnt2String]["s"] = 1;
        if (jsonLogCnt2 % 100 == 0) {
          String jsonLogData2 = "";
          serializeJson(jsonLog2, jsonLogData2);
          jsonLog2.clear();
          SendLog(2, jsonLogData2);
        }
        jsonLogCnt2++;
      }
      if (lHour < CH2_Flow_W && jsonLogFlag2_f == 1) {
        jsonLogFlag2_f = 0;
        String jsonLogCnt2String = String(jsonLogCnt2);
        jsonLog2[jsonLogCnt2String]["t"] = millis() - jsonLogMillis2;
        jsonLog2[jsonLogCnt2String]["n"] = "F";
        jsonLog2[jsonLogCnt2String]["s"] = 0;
        if (jsonLogCnt2 % 100 == 0) {
          String jsonLogData2 = "";
          serializeJson(jsonLog2, jsonLogData2);
          jsonLog2.clear();
          SendLog(2, jsonLogData2);
        }
        jsonLogCnt2++;
      }

      if (WaterSensorData && jsonLogFlag2_w == 0) {
        jsonLogFlag2_w = 1;
        String jsonLogCnt2String = String(jsonLogCnt2);
        jsonLog2[jsonLogCnt2String]["t"] = millis() - jsonLogMillis2;
        jsonLog2[jsonLogCnt2String]["n"] = "W";
        jsonLog2[jsonLogCnt2String]["s"] = 1;
        if (jsonLogCnt2 % 100 == 0) {
          String jsonLogData2 = "";
          serializeJson(jsonLog2, jsonLogData2);
          jsonLog2.clear();
          SendLog(2, jsonLogData2);
        }
        jsonLogCnt2++;
      }
      if (!WaterSensorData && jsonLogFlag2_w == 1) {
        jsonLogFlag2_w = 0;
        String jsonLogCnt2String = String(jsonLogCnt2);
        jsonLog2[jsonLogCnt2String]["t"] = millis() - jsonLogMillis2;
        jsonLog2[jsonLogCnt2String]["n"] = "W";
        jsonLog2[jsonLogCnt2String]["s"] = 0;
        if (jsonLogCnt2 % 100 == 0) {
          String jsonLogData2 = "";
          serializeJson(jsonLog2, jsonLogData2);
          jsonLog2.clear();
          SendLog(2, jsonLogData2);
        }
        jsonLogCnt2++;
      }
    }
  }

  if (ChannelNum == 1 && millis() - sePrevMillis1 >= 500 && seCnt1 == 1) {
    if (cnt == 1) { // CH1 세탁기 동작 시작 
      timeSendFlag1 = 1;

      jsonLogFlag1 = 1;
      jsonLogCnt1 = 1;
      jsonLogMillis1 = millis();

      String localTime = "";
      jsonLog2["START"]["local_time"] = localTime;

      seCnt1 = 0;
      CH1_Cnt = 0;
      digitalWrite(PIN_CH1_LED, HIGH);
      CH1_CurrStatus = 0;
      Serial.print("CH");
      Serial.print(ChannelNum);
      Serial.println(" Washer Started");
      SendStatus(ChannelNum, 0);
    }
    m1 = 1;
  }
  if (ChannelNum == 2 && millis() - sePrevMillis2 >= 500 && seCnt2 == 1) {
    if (cnt == 1) { // CH2 세탁기 동작 시작
      timeSendFlag2 = 1;

      jsonLogFlag2 = 1;
      jsonLogCnt2 = 1;
      jsonLogMillis2 = millis();

      String localTime = "";
      jsonLog2["START"]["local_time"] = localTime;

      seCnt2 = 0;
      CH2_Cnt = 0;
      digitalWrite(PIN_CH2_LED, HIGH);
      CH2_CurrStatus = 0;
      Serial.print("CH");
      Serial.print(ChannelNum);
      Serial.println(" Washer Started");
      SendStatus(ChannelNum, 0);
    }
    m2 = 1;
  } else {
    if (previousMillisEnd > millis())
      previousMillisEnd = millis();
    if (m) {
      if (ChannelNum == 1)
        previousMillisEnd1 = millis();
      if (ChannelNum == 2)
        previousMillisEnd2 = millis();
      if (ChannelNum == 1)
        m1 = 0;
      if (ChannelNum == 2)
        m2 = 0;
    } else if (cnt){
    } else if (ChannelNum == 1 && millis() - previousMillisEnd >= CH1_EndDelay_W) { // CH1 세탁기 동작 종료
      timeSendFlag1 = 1;
      jsonLogFlag1_c = 0;
      jsonLogFlag1_f = 0;
      jsonLogFlag1_w = 0;
      jsonLogFlag1 = 0;
      String localTime = "";
      jsonLog1["END"]["local_time"] = localTime;

      String jsonLogData1 = "";
      serializeJson(jsonLog1, jsonLogData1);
      //Serial.println(json_log_data1);
      jsonLog1.clear();
      Serial.println("CH1 Washer Ended");
      SendStatus(1, 1);
      SendLog(1, jsonLogData1);
      CH1_Cnt = 1;
      digitalWrite(PIN_CH1_LED, LOW);
      CH1_CurrStatus = 1;
    } else if (ChannelNum == 2 && millis() - previousMillisEnd >= CH2_EndDelay_W) { // CH2 세탁기 동작 종료 
      timeSendFlag2 = 1;

      jsonLogFlag2_c = 0;
      jsonLogFlag2_f = 0;
      jsonLogFlag2_w = 0;
      jsonLogFlag2 = 0;
      String localTime = "";
      jsonLog2["END"]["local_time"] = localTime;

      String jsonLogData2 = "";
      serializeJson(jsonLog2, jsonLogData2);
      jsonLog2.clear();
      Serial.println("CH2 Washer Ended");
      SendStatus(2, 1);
      SendLog(2, jsonLogData2);
      CH2_Cnt = 1;
      digitalWrite(PIN_CH2_LED, LOW);
      CH2_CurrStatus = 1;
    }
  }
}