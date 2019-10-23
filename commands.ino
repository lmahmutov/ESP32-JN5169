void setPermitJoin(uint16_t u16ShortAddr, byte u8Interval, byte u8TCsignificance)
{
  byte commandData[4];

  // Build command payload
  commandData[0] = (byte)(u16ShortAddr >> 8);
  commandData[1] = (byte)u16ShortAddr;
  commandData[2] = u8Interval;
  commandData[3] = u8TCsignificance;

  // Transmit command
  transmitCommand(0x0049, 4, commandData);
}

void activeEndpointDescriptorRequest(uint16_t u16ShortAddr)
{
  byte commandData[2];

  // Build command payload
  commandData[0] = (byte)(u16ShortAddr >> 8);
  commandData[1] = (byte)u16ShortAddr;

  // Transmit command
  transmitCommand(0x0045, 2, commandData);
}

void simpleDescriptorRequest(uint16_t u16ShortAddr, byte u8EndPoint)
{
  byte commandData[3];

  // Build command payload
  commandData[0] = (byte)(u16ShortAddr >> 8);
  commandData[1] = (byte)u16ShortAddr;
  commandData[2] = u8EndPoint;

  // Transmit command
  transmitCommand(0x0043, 3, commandData);
}

void sendBindRequest(uint64_t u64TargetExtAddr, byte u8TargetEndPoint, uint16_t u16ClusterID, byte u8DstAddrMode, uint64_t u64DstAddr, byte u8DstEndPoint)
{
  byte commandData[32];
  byte u8Len = 0;

  // Build command payload
  commandData[u8Len++] = (byte)(u64TargetExtAddr >> 56);
  commandData[u8Len++] = (byte)(u64TargetExtAddr >> 48);
  commandData[u8Len++] = (byte)(u64TargetExtAddr >> 40);
  commandData[u8Len++] = (byte)(u64TargetExtAddr >> 32);
  commandData[u8Len++] = (byte)(u64TargetExtAddr >> 24);
  commandData[u8Len++] = (byte)(u64TargetExtAddr >> 16);
  commandData[u8Len++] = (byte)(u64TargetExtAddr >> 8);
  commandData[u8Len++] = (byte)u64TargetExtAddr;
  commandData[u8Len++] = u8TargetEndPoint;
  commandData[u8Len++] = (byte)(u16ClusterID >> 8);
  commandData[u8Len++] = (byte)u16ClusterID;
  commandData[u8Len++] = u8DstAddrMode;

  if (u8DstAddrMode == 3)
  {
    commandData[u8Len++] = (byte)(u64DstAddr >> 56);
    commandData[u8Len++] = (byte)(u64DstAddr >> 48);
    commandData[u8Len++] = (byte)(u64DstAddr >> 40);
    commandData[u8Len++] = (byte)(u64DstAddr >> 32);
    commandData[u8Len++] = (byte)(u64DstAddr >> 24);
    commandData[u8Len++] = (byte)(u64DstAddr >> 16);
    commandData[u8Len++] = (byte)(u64DstAddr >> 8);
    commandData[u8Len++] = (byte)u64DstAddr;
    commandData[u8Len++] = u8DstEndPoint;
  }
  else
  {
    commandData[u8Len++] = (byte)(u64DstAddr >> 8);
    commandData[u8Len++] = (byte)u64DstAddr;
  }

  // Transmit command
  transmitCommand(0x0030, u8Len, commandData);
}

void displayAttribute(uint16_t u16AttribId, byte u8AttribType, byte* au8AttribData, byte u8AttribIndex, uint16_t u16AttrSize)
{
  attr_response += "  Attribute ID: 0x" + String(u16AttribId, HEX);
  attr_response += "\n";
  attr_response += "  Attribute Size: 0x" + String(u16AttrSize, HEX);
  attr_response += "\n";
  attr_response += "  Attribute Type: 0x" + String(u8AttribType,HEX);

  switch (u8AttribType)
  {
    case 0x10:
      attr_response += " (Boolean)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex], DEC);
      attr_response += "\n";
      break;
    case 0x18:
      attr_response += " (8-bit Bitmap)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex],HEX);
      attr_response += "\n";
      break;
    case 0x20:
      attr_response += " (UINT8)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(au8AttribData[u8AttribIndex], HEX);
      attr_response += "\n";
      break;
    case 0x21:
      uint16_t u16Data;
      u16Data   = au8AttribData[u8AttribIndex];
      u16Data <<= 8;
      u16Data  |= au8AttribData[u8AttribIndex + 1];
      attr_response += " (UINT16)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(u16Data,HEX);
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
      attr_response += "  Attribute Data: 0x" + String(u32Data,HEX);
      attr_response += "\n";
      break;
    case 0x29:
      uint16_t int16Data;
      int16Data   = au8AttribData[u8AttribIndex];
      int16Data <<= 8;
      int16Data  |= au8AttribData[u8AttribIndex + 1];
      attr_response += " (INT16)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + String(int16Data,HEX);
      attr_response += "\n";
      break;
    case 0x30:
      attr_response += " (8-bit Enumeration)";
      attr_response += "\n";
      attr_response += "  Attribute Data: 0x" + au8AttribData[u8AttribIndex];
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
  Serial.print(attr_response);
  attr_response="";
}

void sendMgmtLqiRequest(uint16_t u16ShortAddr, byte u8StartIndex)
{
  byte commandData[4];

  // Build command payload
  commandData[0] = (byte)(u16ShortAddr >> 8);
  commandData[1] = (byte)u16ShortAddr;
  commandData[2] = u8StartIndex;

  // Transmit command
  transmitCommand(0x004E, 3, commandData);
}


void DiscoverDevices()
{
  uint16_t u16ShortAddr = 0x0000;
  byte u8StartIndex = 0;
  sendMgmtLqiRequest(u16ShortAddr, u8StartIndex);
}

void writeByte(byte data)
{
  uint8_t dataArray[1];
  dataArray[0] = data;
  // Write data byte to serial port
  jnSerial.write(dataArray, 1);
}
