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
uint64_t u64ExtAddrArray[255];
uint16_t u16ShortAddrArray[255];
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
      displayDecodedCommand(rxMessageType, rxMessageLength, rxMessageData);
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
  //transmitCommand(0x0010, 0, 0);
  transmitCommand(0x0024, 0, 0);
  //delay(1000);
  //transmitCommand(0x0015, 0, 0);
  //network state
  transmitCommand(0x0009, 0, 0);
  DiscoverDevices();
  sendMgmtLqiRequest(0x0617, 0);
  //setPermitJoin(0x0000, 0xFE, 0x00);

}

void loop() {

  serialEvent();
}
