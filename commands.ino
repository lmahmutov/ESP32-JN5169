void transmitCommand(int iCommand, int iLength, byte data[])
{
  int i;
  uint8_t specialCharacter[1];
  uint8_t message[256];

  // Build message payload, starting with the type field
  message[0] = (byte)(iCommand >> 8);
  message[1] = (byte)iCommand;

  // Add message length
  message[2] = (byte)(iLength >> 8);
  message[3] = (byte)iLength;

  // Calculate checksum of header
  byte csum = 0;
  csum ^= message[0];
  csum ^= message[1];
  csum ^= message[2];
  csum ^= message[3];

  // Add message data and update checksum
  if (iLength != 0)
  {
    for (i = 0; i < iLength; i++)
    {
      message[5 + i] = data[i];
      csum ^= data[i];
    }
  }

  // Add checksum
  message[4] = csum;

  // Transmit the message, send start character first
  specialCharacter[0] = 1;
  writeByte(specialCharacter[0]);

  // Transmit message payload with byte stuffing as required
  for (i = 0; i < iLength + 5; i++)
  {
    // Check if stuffing is required
    if (message[i] < 0x10)
    {
      // First send escape character then message byte XOR'd with 0x10
      specialCharacter[0] = 2;
      writeByte(specialCharacter[0]);
      int msg = message[i];
      msg = msg ^ 0x10;
      message[i] = (byte)msg;
      writeByte(message[i]);

    }
    else
    {
      // Send the character with no modification
      writeByte(message[i]);
    }
  }

  // Send end character
  specialCharacter[0] = 3;
  writeByte(specialCharacter[0]);
}


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

void setChannel(int channel)
{
  if (channel >= 11 && channel <= 26)
  {
    // User is specifying a single channel, we must create the 32-bit mask from this
    uint32_t u32ChannelMaskTemp = 1;

    for (int i = 0; i < channel; i++)
    {
      u32ChannelMaskTemp <<= 1;
    }

    // Set channel mask
    setChannelMask(u32ChannelMaskTemp);
  }
}

void setChannelMask(uint32_t uiMask)
{
  byte commandData[4];

  // Build command payload
  commandData[0] = (byte)(uiMask >> 24);
  commandData[1] = (byte)(uiMask >> 16);
  commandData[2] = (byte)(uiMask >> 8);
  commandData[3] = (byte)uiMask;

  // Transmit command
  transmitCommand(0x0021, 4, commandData);
}

void setDeviceType(byte deviceType)  // 0 - Coordinator, 1 - router, 2 end device
{
  byte commandData[1];

  // Build command payload
  commandData[0] = deviceType;

  // Transmit command
  transmitCommand(0x0023, 1, commandData);
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
