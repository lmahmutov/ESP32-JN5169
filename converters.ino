void displayAttribute(uint8_t SQN, uint16_t u16SrcAddr, uint16_t u16ClusterId, uint16_t u16AttribId, byte u8AttribType, byte* au8AttribData, byte u8AttribIndex, uint16_t u16AttrSize)
{

  attr_response += "{Date Time: ";
  attr_response += timeStringBuff;
  attr_response += "{SQN: 0x" + String(SQN, DEC);
  attr_response += ";Cluster ID: 0x" + String(u16ClusterId, HEX);
  attr_response += ";Attribute ID: 0x" + String(u16AttribId, HEX);
  //attr_response += "\n";
  attr_response += ";Attribute Size: 0x" + String(u16AttrSize, HEX);
  //attr_response += "\n";
  attr_response += ";Attribute Type: 0x" + String(u8AttribType, HEX);


  /*
     clusterList.Add(0x0000, " (General: Basic)");
     clusterList.Add(0x0001, " (General: Power Config)");
     clusterList.Add(0x0002, " (General: Temperature Config)");
     clusterList.Add(0x0003, " (General: Identify)");
     clusterList.Add(0x0004, " (General: Groups)");
     clusterList.Add(0x0005, " (General: Scenes)");
     clusterList.Add(0x0006, " (General: On/Off)");
     clusterList.Add(0x0007, " (General: On/Off Config)");
     clusterList.Add(0x0008, " (General: Level Control)");
     clusterList.Add(0x0009, " (General: Alarms)");
     clusterList.Add(0x000A, " (General: Time)");
     clusterList.Add(0x000F, " (General: Binary Input Basic)");
     clusterList.Add(0x0020, " (General: Poll Control)");
     clusterList.Add(0x0019, " (General: OTA)");
     clusterList.Add(0x0101, " (General: Door Lock");
     clusterList.Add(0x0201, " (HVAC: Thermostat)");
     clusterList.Add(0x0202, " (HVAC: Fan Control)");
     lusterList.Add(0x0204, " (HVAC: Thermostat UI Config)");
     clusterList.Add(0x0300, " (Lighting: Color Control)");
     clusterList.Add(0x0400, " (Measurement: Illuminance)");
     clusterList.Add(0x0402, " (Measurement: Temperature)");
     clusterList.Add(0x0403, " (Measurement: Pressure)");
     clusterList.Add(0x0405, " (Measurement: Relative Humidity)");
     clusterList.Add(0x0406, " (Measurement: Occupancy Sensing)");
     clusterList.Add(0x0500, " (Security & Safety: IAS Zone)");
     clusterList.Add(0x0502, " (Security & Safety: IAS WD)");
     clusterList.Add(0x0702, " (Smart Energy: Metering)");
     lusterList.Add(0x0B05, " (Misc: Diagnostics)");
     clusterList.Add(0x1000, " (ZLL: Commissioning)");
  */
  switch (u16ClusterId)
  {
    case 0x0000:
      switch (u8AttribType)
      {
        case 0x42:
          //attr_response += " (Character String)";
          // attr_response += "\n";
          NewDevName = "";
          attr_response += "  Attribute Data (Len - " + String(u16AttrSize) + "): ";
          for (int i = 0; i < u16AttrSize; i++)
          {
            char c = (char)au8AttribData[u8AttribIndex + i];
            attr_response += c;
            if (u16AttribId == 0x0005) {
              NewDevName += c;
            }
          }
          if (u16AttribId == 0x0005) {
            DnResponse = true;
          }
          //attr_response += "\n";
          break;
      }
      break;
    case 0x0006:
      switch (u8AttribType)
      {
        case 0x10:
          //attr_response += " (Boolean)";
          //attr_response += "\n";
          attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex], DEC);
          //attr_response += "\n";
          break;
      }
      break;
    case 0x0400:
      break;
    case 0x0402:
      switch (u8AttribType)
      {
        case 0x21:
          uint16_t u16Data;
          u16Data   = au8AttribData[u8AttribIndex];
          u16Data <<= 8;
          u16Data  |= au8AttribData[u8AttribIndex + 1];
          attr_response += " (UINT16)";
          attr_response += "\n";
          attr_response += "  Attribute Data: 0x" + String(u16Data, HEX);
          attr_response += "\n";
          break;
        case 0x29:
          uint16_t int16Data;
          int16Data   = au8AttribData[u8AttribIndex];
          int16Data <<= 8;
          int16Data  |= au8AttribData[u8AttribIndex + 1];
          attr_response += "Temperature = ";
          attr_response += int16Data;
          attr_response += "\n";
          break;
      }
      break;
    case 0x0403:
      switch (u8AttribType)
      {
        case 0x21:
          uint16_t u16Data;
          u16Data   = au8AttribData[u8AttribIndex];
          u16Data <<= 8;
          u16Data  |= au8AttribData[u8AttribIndex + 1];
          attr_response += " (UINT16)";
          attr_response += "\n";
          attr_response += "  Attribute Data: 0x" + String(u16Data, HEX);
          attr_response += "\n";
          break;
        case 0x29:
          uint16_t int16Data;
          int16Data   = au8AttribData[u8AttribIndex];
          int16Data <<= 8;
          int16Data  |= au8AttribData[u8AttribIndex + 1];
          attr_response += "Pressure = ";
          attr_response += int16Data;
          attr_response += "\n";
          break;
      }
      break;
    case 0x0405:
      switch (u8AttribType)
      {
        case 0x21:
          uint16_t u16Data;
          u16Data   = au8AttribData[u8AttribIndex];
          u16Data <<= 8;
          u16Data  |= au8AttribData[u8AttribIndex + 1];
          attr_response += "Humidity";
          attr_response += u16Data;
          attr_response += "\n";
          break;
      }
      break;
    default:
      //attr_response += " (Unknown)";
      //attr_response += "\n";
      break;
  }
  /*
    switch (u8AttribType)
    {
    case 0x10:
      //attr_response += " (Boolean)";
      //attr_response += "\n";
      //attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex], DEC);
      //attr_response += "\n";
      break;
    case 0x18:
      //attr_response += " (8-bit Bitmap)";
      //attr_response += "\n";
      //attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex],HEX);
      //attr_response += "\n";
      break;
    case 0x20:
      //attr_response += " (UINT8)";
      //attr_response += "\n";
      //attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex], HEX);
      //attr_response += "\n";
      break;
    case 0x21:
      uint16_t u16Data;
      u16Data   = au8AttribData[u8AttribIndex];
      u16Data <<= 8;
      u16Data  |= au8AttribData[u8AttribIndex + 1];
      attr_response += " (UINT16)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(u16Data, HEX);
      attr_response += "\n";
      break;
    case 0x23:
      uint32_t u32Data;
      u32Data = au8AttribData[u8AttribIndex];
      u32Data <<= 8;
      u32Data |= au8AttribData[u8AttribIndex + 1];
      u32Data <<= 8;
      u32Data |= au8AttribData[u8AttribIndex + 2];
      u32Data <<= 8;
      u32Data |= au8AttribData[u8AttribIndex + 3];
      attr_response += " (UINT32)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(u32Data, HEX);
      attr_response += "\n";
      break;
    case 0x29:
      uint16_t int16Data;
      int16Data   = au8AttribData[u8AttribIndex];
      int16Data <<= 8;
      int16Data  |= au8AttribData[u8AttribIndex + 1];
      attr_response += " (INT16)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(int16Data, HEX);
      attr_response += "\n";
      break;
    case 0x30:
      attr_response += " (8-bit Enumeration)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex], HEX);
      attr_response += "\n";
      break;
    case 0x42:
      attr_response += " (Character String)";
      attr_response += "\n";
      attr_response += "  Attribute Data (Len - " + String(u16AttrSize) + "): ";
      for (int i = 0; i < u16AttrSize; i++)
      {
        char c = (char)au8AttribData[u8AttribIndex + i];
        attr_response += c;
      }
      attr_response += "\n";
      break;
    case 0xF0:
      attr_response += " (IEEE Address)";
      attr_response += "\n";
      attr_response += "  Attribute Data: " + au8AttribData[u8AttribIndex];
      attr_response += ":" + au8AttribData[u8AttribIndex + 1];
      attr_response += ":" + au8AttribData[u8AttribIndex + 2];
      attr_response += ":" + au8AttribData[u8AttribIndex + 3];
      attr_response += ":" + au8AttribData[u8AttribIndex + 4];
      attr_response += ":" + au8AttribData[u8AttribIndex + 5];
      attr_response += ":" + au8AttribData[u8AttribIndex + 6];
      attr_response += ":" + au8AttribData[u8AttribIndex + 7];
      attr_response += "\n";
      break;
    default:
      attr_response += " (Unknown)";
      attr_response += "\n";
      break;
    }
  */
  attr_response += "}";
  Serial.print(attr_response);

  if (globalClient != NULL && globalClient->status() == WS_CONNECTED) {
    globalClient->text(attr_response);
  }
  attr_response = "";
}
