#include "setvar.h"

int CH1_SETVAR(String SerialData, int dex1, int dexc, int end)
{
  dexc = SerialData.indexOf(',');
  String command = SerialData.substring(dex1+1, dexc);
  String Number = SerialData.substring(dexc+1, end - 1);
  if (command == "DeviceNo")
  {
    Serial.print("CH1_DeviceNo : ");
    Serial.println(Number);
    preferences.putString("CH1_DeviceNo", Number);
  }
  else if (command == "Current_Wash")
  {
    Serial.print("CH1_Curr_W : ");
    Serial.println(Number);
    float var = Number.toFloat();
    preferences.putFloat("CH1_Curr_W", var);
  }
  else if (command == "Flow_Wash")
  {
    Serial.print("CH1_Flow_W : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putUInt("CH1_Flow_W", var);
  }
  else if (command == "Current_Dry")
  {
    Serial.print("CH1_Curr_D : ");
    Serial.println(Number);
    float var = Number.toFloat();
    preferences.putFloat("CH1_Curr_D", var);
  }
  else if (command == "EndDelay_Wash")
  {
    Serial.print("CH1_EndDelay_W : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putUInt("CH1_EndDelay_W", var);
  }
  else if (command == "EndDelay_Dry")
  {
    Serial.print("CH1_EndDelay_D : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putUInt("CH1_EndDelay_D", var);
  }
  else if (command == "Enable")
  {
    Serial.print("CH1_Enable : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putBool("CH1_Live", var);
  }
  else
  {
    Serial.print("Command Not Found for : ");
    Serial.println(command);
  }
  return 0;
}

int CH2_SETVAR(String SerialData, int dex1, int dexc, int end)
{
  dexc = SerialData.indexOf(',');
  String command = SerialData.substring(dex1+1, dexc);
  String Number = SerialData.substring(dexc+1, end - 1);
  if (command == "DeviceNo")
  {
    Serial.print("CH2_DeviceNo : ");
    Serial.println(Number);
    preferences.putString("CH2_DeviceNo", Number);
  }
  else if (command == "Current_Wash")
  {
    Serial.print("CH2_Curr_W : ");
    Serial.println(Number);
    float var = Number.toFloat();
    preferences.putFloat("CH2_Curr_W", var);
  }
  else if (command == "Flow_Wash")
  {
    Serial.print("CH2_Flow_W : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putUInt("CH2_Flow_W", var);
  }
  else if (command == "Current_Dry")
  {
    Serial.print("CH2_Curr_D : ");
    Serial.println(Number);
    float var = Number.toFloat();
    preferences.putFloat("CH2_Curr_D", var);
  }
  else if (command == "EndDelay_Wash")
  {
    Serial.print("CH2_EndDelay_W : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putUInt("CH2_EndDelay_W", var);
  }
  else if (command == "EndDelay_Dry")
  {
    Serial.print("CH2_EndDelay_D : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putUInt("CH2_EndDelay_D", var);
  }
  else if (command == "Enable")
  {
    Serial.print("CH2_Enable : ");
    Serial.println(Number);
    int var = Number.toInt();
    preferences.putBool("CH2_Live", var);
  }
  else
  {
    Serial.print("Command Not Found for : ");
    Serial.println(command);
  }
  return 0;
}