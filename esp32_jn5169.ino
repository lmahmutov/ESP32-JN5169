#include "HardwareSerial.h"
HardwareSerial jnSerial(2);
//
bool start_decode = false;

String print_string = "";
String attr_response = "";
byte rxMessageData[1024];
byte rxMessageChecksum = 0;
uint16_t rxMessageLength = 0;
uint8_t rxMessageState    = 0;
uint16_t rxMessageType = 0;
uint8_t rxMessageCount    = 0;
bool rxMessageInEscape = false;
byte rxByte;
//
bool showAdditionalDebug = false;
uint16_t u16ShortAddr = 0;
uint64_t u64ExtendedAddr_coord = 0;
uint64_t u64ExtAddr = 0;
//
uint64_t au64ExtAddr[16];
byte au8OTAFile[262144]; // 256k max file size
byte u8OtaInProgress;
byte u8OTAWaitForDataParamsPending;
uint16_t u16OTAWaitForDataParamsTargetAddr;
byte u8OTAWaitForDataParamsSrcEndPoint;
uint32_t u32OTAWaitForDataParamsCurrentTime;
uint32_t u32OTAWaitForDataParamsRequestTime;
uint16_t u16OTAWaitForDataParamsBlockDelay;
uint32_t u32OtaFileIdentifier;
uint16_t u16OtaFileHeaderVersion;
uint16_t u16OtaFileHeaderLength;
uint16_t u16OtaFileHeaderControlField;
uint16_t u16OtaFileManufacturerCode;
uint16_t u16OtaFileImageType;
uint32_t u32OtaFileVersion;
uint16_t u16OtaFileStackVersion;
uint32_t u32OtaFileTotalImage;
byte u8OtaFileSecurityCredVersion;
uint64_t u64OtaFileUpgradeFileDest;
uint16_t u16OtaFileMinimumHwVersion;
uint16_t u16OtaFileMaxHwVersion;
//


#define RXD2 16
#define TXD2 17

void serialEvent() {
  while (jnSerial.available())
  {
    byte rxByte = (byte)jnSerial.read();

    if (rxByte == 0x01)
    {
      // Start character received
      rxMessageChecksum = 0;
      rxMessageLength   = 0;
      rxMessageType     = 0;
      rxMessageState    = 0;
      rxMessageCount    = 0;
      rxMessageInEscape = false;
    }
    else if (rxByte == 0x02)
    {
      rxMessageInEscape = true;
    }
    else if (rxByte == 0x03)
    {
      start_decode = true;
      //displayDecodedCommand(rxMessageType, rxMessageLength, rxMessageData);
    }
    else
    {
      if (rxMessageInEscape == true)
      {
        rxByte ^= 0x10;
        rxMessageInEscape = false;
      }

      // Parse character
      switch (rxMessageState)
      {
        case 0:
          {
            rxMessageType = rxByte;
            rxMessageType <<= 8;
            rxMessageState++;
          }
          break;

        case 1:
          {
            rxMessageType |= rxByte;
            rxMessageState++;
          }
          break;

        case 2:
          {
            rxMessageLength = rxByte;
            rxMessageLength <<= 8;
            rxMessageState++;
          }
          break;

        case 3:
          {
            rxMessageLength |= rxByte;
            rxMessageState++;
          }
          break;

        case 4:
          {
            rxMessageChecksum = rxByte;
            rxMessageState++;
          }
          break;

        default:
          {
            rxMessageData[rxMessageCount++] = rxByte;
          }
          break;
      }
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  jnSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);
  //transmitCommand(0x0011, 0, 0);
  // transmitCommand(0x0012, 0, 0);
  //delay(5000);
  //Check version of firmware on JN5169
  transmitCommand(0x0010, 0, 0);
  //transmitCommand(0x0024, 0, 0);
  //delay(1000);
  //network state
  transmitCommand(0x0009, 0, 0);
  DiscoverDevices();
  sendMgmtLqiRequest(0x0617, 0);
  //setPermitJoin(0x0000, 0xFE, 0x00);

}

void loop() {
  if (start_decode)
  {
    displayDecodedCommand(rxMessageType, rxMessageLength, rxMessageData);
    start_decode = false;
  }
  serialEvent();
}

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
