void displayDecodedCommand(uint16_t u16Type, uint16_t u16Length, byte* au8Data)
{

  if ( (u16Type != 0x8011 && u16Type != 0x8012))
  {
    print_string += "Type: ";
    print_string += u16toStr(u16Type);
    print_string += "\n";
  }

  switch (u16Type)
  {

    case 0x8000:
      {
        print_string += " (Status)";
        print_string += "\n";
        print_string += "  Length: ";
        print_string += u16toStr(u16Length);
        print_string += "\n";
        print_string += "  Status: ";
        print_string += u8toStr(au8Data[0]);

        switch (au8Data[0])
        {
          case 0:
            {
              print_string += " (Success)";
            }
            break;

          case 1:
            {
              print_string += " (Incorrect Parameters)";
            }
            break;

          case 2:
            {
              print_string += " (Unhandled Command)";
            }
            break;

          case 3:
            {
              print_string += " (Command Failed)";
            }
            break;

          case 4:
            {
              print_string += " (Busy)";
            }
            break;

          case 5:
            {
              print_string += " (Stack Already Started)";
            }
            break;

          default:
            {
              print_string += " (ZigBee Error Code)";
            }
            break;
        }
        print_string += "\n";
        /*
                print_string += "\n";
                print_string += "  SQN: 0x";
                print_string += au8Data[1];

                if (u16Length > 2)
                {
                  print_string += "\n";
                  print_string += "  Message: ";
                  //string errorMessage = System.Text.Encoding.Default.GetString(au8Data);
                  //print_string += errorMessage.Substring(2, (u16Length - 2));
                }
                print_string += "\n";
                //Serial.print(print_string);
        */
      }
      break;

    case 0x8011:
      {

        {
          uint16_t u16ProfileID = 0;
          uint16_t u16ClusterID = 0;

          u16ProfileID = au8Data[4];
          u16ProfileID <<= 8;
          u16ProfileID |= au8Data[5];

          u16ClusterID = au8Data[6];
          u16ClusterID <<= 8;
          u16ClusterID |= au8Data[7];

          print_string += " (APS Data ACK)";
          print_string += "\n";
          print_string += "   Status: 0x" + au8Data[0];
          print_string += "\n";
          print_string += "   SQN: 0x" + au8Data[1];
          print_string += "\n";
          print_string += "   Source EndPoint: 0x" + au8Data[2];
          print_string += "\n";
          print_string += "   Destination EndPoint: 0x" + au8Data[3];
          print_string += "\n   ";
          //displayProfileId(u16ProfileID);
          print_string += "\n   ";
          ////displayClusterId(u16ClusterID);
          print_string += "\n";
        }
      }
      break;

    case 0x8012:
      {

        {
          print_string += "  (APS Data Confirm)";
          print_string += "\n";
          print_string += "   Status: 0x" + au8Data[0];
          print_string += "\n";
          print_string += "   SQN: 0x" + au8Data[1];
          print_string += "\n";
          print_string += "   Source EndPoint: 0x" + au8Data[2];
          print_string += "\n";
          print_string += "   Destination EndPoint: 0x" + au8Data[3];
          print_string += "\n";
        }
      }
      break;

    case 0x8001:
      {
        print_string += " (Log)";
        print_string += "\n";
        print_string += "  Level: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Message: ";

        //string logMessage = System.Text.Encoding.Default.GetString(au8Data);
        //print_string += logMessage.Substring(1, (u16Length - 1));
        print_string += "\n";
      }
      break;

    case 0x8002:
      {
        uint16_t u16ProfileID = 0;
        uint16_t u16ClusterID = 0;

        u16ProfileID = au8Data[1];
        u16ProfileID <<= 8;
        u16ProfileID |= au8Data[2];

        u16ClusterID = au8Data[3];
        u16ClusterID <<= 8;
        u16ClusterID |= au8Data[4];

        print_string += " (Data Indication)";
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[0];
        print_string += "\n";
        //displayProfileId(u16ProfileID);
        print_string += "\n";
        // //displayClusterId(u16ClusterID);
        print_string += "\n";
        print_string += "  Source EndPoint: 0x" + au8Data[5];
        print_string += "\n";
        print_string += "  Destination EndPoint: 0x" + au8Data[6];
        print_string += "\n";
        print_string += "  Source Address Mode: 0x" + au8Data[7];
        print_string += "\n";
        print_string += "  Source Address: ";

        byte nextIndex = 0;

        if (au8Data[9] == 0)
        {
          //0x00 = DstAddress and DstEndpoint not present
          print_string += "Not Present";
          print_string += "\n";

          nextIndex = 10;
        }
        else if (au8Data[9] == 1)
        {
          uint16_t u16GroupAddr = 0;

          u16GroupAddr   = au8Data[10];
          u16GroupAddr <<= 8;
          u16GroupAddr  |= au8Data[11];

          //0x01 = 16-bit group address for DstAddress; DstEndpoint not present
          print_string += u16GroupAddr;
          print_string += "\n";

          nextIndex = 12;
        }
        else if (au8Data[9] == 2)
        {
          uint16_t u16DstAddress = 0;
          uint16_t u16DstEndPoint1 = 0;

          u16DstAddress = au8Data[10];
          u16DstAddress <<= 8;
          u16DstAddress |= au8Data[11];

          u16DstEndPoint1 = au8Data[12];
          u16DstEndPoint1 <<= 8;
          u16DstEndPoint1 |= au8Data[13];

          //0x02 = 16-bit address for DstAddress and DstEndpoint present
          print_string += u16DstAddress;
          print_string += "  EndPoint: 0x" + u16DstEndPoint1;
          print_string += "\n";

          nextIndex = 14;
        }
        else if (au8Data[9] == 3)
        {
          //0x03 = 64-bit extended address for DstAddress and DstEndpoint present
        }
        else
        {
          //0x04 - 0xff = reserved
          nextIndex = 10;
          print_string += "Not Valid";
          print_string += "\n";
        }

        print_string += "  Destination Address Mode: 0x" + au8Data[nextIndex];
        print_string += "\n";
      }
      break;

    case 0x8003:
      {
        uint16_t u16Entries = 0;
        uint16_t u16ProfileID = 0;

        u16ProfileID = au8Data[1];
        u16ProfileID <<= 8;
        u16ProfileID |= au8Data[2];

        u16Entries = (uint16_t)((u16Length - 3) / 2);

        print_string += " (Cluster List - Entries: ";
        print_string += u16Entries;
        print_string += ")\n";
        print_string += "  Source EndPoint: 0x";
        print_string += au8Data[0];
        print_string += "\n";
        //displayProfileId(u16ProfileID);

        for (int i = 3; i < u16Length; i += 2)
        {
          uint16_t u16ClusterID;

          u16ClusterID = au8Data[i];
          u16ClusterID <<= 8;
          u16ClusterID |= au8Data[i + 1];

          //displayClusterId(u16ClusterID);
          //print_string += "\n";
        }
      }
      break;

    case 0x8004:
      {
        uint16_t u16Entries = 0;
        uint16_t u16ProfileID = 0;
        uint16_t u16ClusterID = 0;

        u16ProfileID = au8Data[1];
        u16ProfileID <<= 8;
        u16ProfileID |= au8Data[2];

        u16ClusterID = au8Data[3];
        u16ClusterID <<= 8;
        u16ClusterID |= au8Data[4];

        u16Entries = (uint16_t)((u16Length - 5) / 2);

        print_string += " (Cluster Attributes - Entries: ";
        print_string += u16Entries;
        print_string += ")\n";
        print_string += " Source EndPoint: 0x";
        print_string += au8Data[0];
        print_string += "\n";
        //displayProfileId(u16ProfileID);
        print_string += "\n";
        ////displayClusterId(u16ClusterID);
        print_string += "\n";

        for (int i = 5; i < u16Length; i += 2)
        {
          uint16_t u16AttributeID = 0;

          u16AttributeID = au8Data[i];
          u16AttributeID <<= 8;
          u16AttributeID |= au8Data[i + 1];

          print_string += " Attribute ID: 0x";
          print_string += u16AttributeID;
          print_string += "\n";
        }
      }
      break;

    case 0x8005:
      {
        uint16_t u16Entries = 0;
        uint16_t u16ProfileID = 0;
        uint16_t u16ClusterID = 0;

        u16ProfileID = au8Data[1];
        u16ProfileID <<= 8;
        u16ProfileID |= au8Data[2];

        u16ClusterID = au8Data[3];
        u16ClusterID <<= 8;
        u16ClusterID |= au8Data[4];

        u16Entries = (uint16_t)(u16Length - 5);

        print_string += " (Command IDs - Entries: ";
        print_string += u16Entries;
        print_string += ")\n";
        print_string += " Source EndPoint: 0x";
        print_string += au8Data[0];
        print_string += "\n";
        //displayProfileId(u16ProfileID);
        print_string += "\n";
        ////displayClusterId(u16ClusterID);
        print_string += "\n";

        for (int i = 5; i < u16Length; i++)
        {
          print_string += " Command ID: 0x";
          print_string += au8Data[i];
          print_string += "\n";
        }
      }
      break;

    case 0x8009:
      {

        uint16_t u16PanId = 0;
        uint64_t u64ExtendedPANID = 0;
        uint16_t u16ShortAddr = 0;
        uint64_t u64ExtendedAddr = 0;

        u16ShortAddr = au8Data[0];
        u16ShortAddr <<= 8;
        u16ShortAddr |= au8Data[1];

        for (int i = 0; i < 8; i++)
        {
          u64ExtendedAddr <<= 8;
          u64ExtendedAddr |= au8Data[2 + i];
        }

        u16PanId = au8Data[10];
        u16PanId <<= 8;
        u16PanId |= au8Data[11];

        for (int i = 0; i < 8; i++)
        {
          u64ExtendedPANID <<= 8;
          u64ExtendedPANID |= au8Data[12 + i];
        }


        print_string += " (Network State Response)";
        print_string += "\n";
        print_string += "  Short Address: ";
        print_string += u16toStr(u16ShortAddr);
        print_string += "\n";
        print_string += "  Extended Address: ";
        print_string += u64toStr(u64ExtendedAddr);
        print_string += "\n";
        print_string += "  PAN ID: ";
        print_string += u16toStr(u16PanId);
        print_string += "\n";
        print_string += "  Ext PAN ID: ";
        print_string += u64toStr(u64ExtendedPANID);
        print_string += "\n";
        print_string += "  Channel: ";
        print_string += String(au8Data[20], DEC);
        print_string += "\n";
      }
      break;

    case 0x8010:
      {
        uint16_t u16Major = 0;
        uint16_t u16Installer = 0;

        u16Major   = au8Data[0];
        u16Major <<= 8;
        u16Major  |= au8Data[1];

        u16Installer = au8Data[2];
        u16Installer <<= 8;
        u16Installer |= au8Data[3];

        print_string += " (Version)";
        print_string += "\n";
        print_string += "  Length: ";
        print_string += u16toStr(u16Length);
        print_string += "\n";
        print_string += "  Application: ";
        print_string +=  u16toStr(u16Major);
        print_string += "\n";
        print_string += "  SDK: ";
        print_string +=  u16toStr(u16Installer);
        print_string += "\n";
      }
      break;

    case 0x8024:
      {
        uint16_t u16ShortAddr = 0;
        uint64_t u64ExtAddr = 0;

        u16ShortAddr   = au8Data[1];
        u16ShortAddr <<= 8;
        u16ShortAddr  |= au8Data[2];

        for (int i = 0; i < 8; i++)
        {
          u64ExtAddr <<= 8;
          u64ExtAddr  |= au8Data[3 + i];
        }

        print_string += " (Network Up)";
        print_string += "\n";
        print_string += "  Status: " + u8toStr(au8Data[0]);
        print_string += "\n";
        print_string += "  Short Address: " + u16toStr(u16ShortAddr);
        print_string += "\n";
        print_string += "  Extended Address: " + u64toStr(u64ExtAddr);
        print_string += "\n";
        print_string += "  Channel: " + String(au8Data[11], DEC);
        print_string += "\n";
      }
      break;

    case 0x8014:
      {
        print_string += " (Permit Join State)";
        print_string += "\n";
        print_string += "Permit Join: ";
        if (au8Data[0] == 1)
        {
          print_string += "TRUE";
        }
        else
        {
          print_string += "FALSE";
        }
        print_string += "\n";
      }
      break;

    case 0x8015:
      {
        uint16_t u16PanId = 0;
        uint16_t u16ShortAddr = 0;
        uint16_t u16SuperframeSpec = 0;
        uint32_t u32TimeStamp = 0;
        uint64_t u64ExtendedPANID = 0;

        u16PanId = au8Data[1];
        u16PanId <<= 8;
        u16PanId |= au8Data[2];

        u16ShortAddr = au8Data[3];
        u16ShortAddr <<= 8;
        u16ShortAddr |= au8Data[4];

        u16SuperframeSpec = au8Data[11];
        u16SuperframeSpec <<= 8;
        u16SuperframeSpec |= au8Data[12];

        for (int i = 0; i < 4; i++)
        {
          u32TimeStamp <<= 8;
          u32TimeStamp |= au8Data[13 + i];
        }

        for (int i = 0; i < 8; i++)
        {
          u64ExtendedPANID <<= 8;
          u64ExtendedPANID |= au8Data[17 + i];
        }

        print_string += " (Discovery Only Scan Response)";
        print_string += "\n";
        print_string += "  Address Mode: " + au8Data[0];
        print_string += "\n";
        print_string += "  PAN ID: " + u16PanId;
        print_string += "\n";
        print_string += "  Short Address: 0x" + u16ShortAddr;
        print_string += "\n";
        print_string += "  Channel: " + au8Data[5];
        print_string += "\n";
        print_string += "  GTS Permit: " + au8Data[6];
        print_string += "\n";
        print_string += "  Link Quality: " + au8Data[7];
        print_string += "\n";
        print_string += "  Security Use: " + au8Data[8];
        print_string += "\n";
        print_string += "  ACL Entry: " + au8Data[9];
        print_string += "\n";
        print_string += "  Security Failure: " + au8Data[10];
        print_string += "\n";
        print_string += "  Superframe Specification: " + u16SuperframeSpec;
        print_string += "\n";
        print_string += "  Time Stamp: " + u32TimeStamp;
        print_string += "\n";
        print_string += "  Ext PAN ID: 0x" + u64ExtendedPANID;

      }
      break;
      
    case 0x8017:
      {
        uint32_t u32TimeStamp = 0;
        for (int i = 0; i < 4; i++)
        {
          u32TimeStamp <<= 8;
          u32TimeStamp |= au8Data[i];
        }
        print_string += "  Time Stamp: " + String(long(u32TimeStamp));
        print_string += "\n";
      }
      break;
      
    case 0x8029:
      {
        uint64_t u64AddrData = 0;
        uint64_t u64Key = 0;
        uint64_t u64HostAddrData = 0;
        uint64_t u64ExtPANID = 0;
        uint32_t u32Mic = 0;
        uint16_t u16PANID = 0;
        uint16_t u16ShortAddr = 0;
        uint16_t u16DeviceId = 0;

        for (int i = 0; i < 8; i++)
        {
          u64AddrData <<= 8;
          u64AddrData  |= au8Data[0 + i];
        }

        for (int i = 0; i < 16; i++)
        {
          u64Key <<= 8;
          u64Key |= au8Data[8 + i];
        }

        for (int i = 0; i < 4; i++)
        {
          u32Mic <<= 8;
          u32Mic |= au8Data[24 + i];
        }

        for (int i = 0; i < 8; i++)
        {
          u64HostAddrData  <<= 8;
          u64HostAddrData |= au8Data[28 + i];
        }

        u16PANID = au8Data[38];
        u16PANID <<= 8;
        u16PANID |= au8Data[39];

        for (int i = 0; i < 8; i++)
        {
          u64ExtPANID <<= 8;
          u64ExtPANID |= au8Data[40 + i];
        }

        u16ShortAddr = au8Data[48];
        u16ShortAddr <<= 8;
        u16ShortAddr |= au8Data[49];

        u16DeviceId = au8Data[50];
        u16DeviceId <<= 8;
        u16DeviceId |= au8Data[51];

        print_string += " (Encrypted Data)";
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[52];
        print_string += "\n";
        print_string += "  Device Extended Address: " + u64AddrData;
        print_string += "\n";
        print_string += "  Key: " + u64Key;
        print_string += "\n";
        print_string += "  Mic: " + u32Mic;
        print_string += "\n";
        print_string += "  Host Extended Address: " + u64HostAddrData;
        print_string += "\n";
        print_string += "  Active Key Sequence Number: " + au8Data[36];
        print_string += "\n";
        print_string += "  Channel: " + au8Data[37];
        print_string += "\n";
        print_string += "  PAN ID: " + u16PANID;
        print_string += "\n";
        print_string += "  Extended PAN ID: " + u64ExtPANID;
        print_string += "\n";
        print_string += "  Short Address: " + u16ShortAddr;
        print_string += "\n";
        print_string += "  Device ID: " + u16DeviceId;
        print_string += "\n";

      }
      break;

    case 0x8030:
      {
        print_string += " (Bind Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + String(au8Data[0], HEX);
        print_string += "\n";
        print_string += "  Status: 0x" + String(au8Data[1], HEX);
        print_string += "\n";
      }
      break;

    case 0x8031:
      {
        print_string += " (UnBind Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";
      }
      break;

    case 0x8041:
      {
        uint64_t u64ExtAddr = 0;
        uint16_t u16ShortAddr = 0;

        for (int i = 0; i < 8; i++)
        {
          u64ExtAddr <<= 8;
          u64ExtAddr |= au8Data[2 + i];
        }

        u16ShortAddr = au8Data[10];
        u16ShortAddr <<= 8;
        u16ShortAddr |= au8Data[11];

        print_string += " (IEEE Address Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Extended Address: 0x" + u64ExtAddr;
        print_string += "\n";
        print_string += "  Short Address: 0x" + u16ShortAddr;
        print_string += "\n";

        if (u16Length > 14)
        {
          print_string += "  Associated End Devices: " + au8Data[12];
          print_string += "\n";
        }
      }
      break;

    case 0x8042:
      {
        /*
        uint16_t u16ShortAddr = 0;
        uint16_t u16ManufacturerCode = 0;
        uint16_t u16RxSize = 0;
        uint16_t u16TxSize = 0;
        uint16_t u16ServerMask = 0;
        uint16_t u16BitFields = 0;
        byte u8DescriptorCapability = 0;
        byte u8MacCapability = 0;
        byte u8MaxBufferSize = 0;

        u16ShortAddr   = au8Data[2];
        u16ShortAddr <<= 8;
        u16ShortAddr  |= au8Data[3];

        u16ManufacturerCode   = au8Data[4];
        u16ManufacturerCode <<= 8;
        u16ManufacturerCode  |= au8Data[5];

        u16RxSize = au8Data[6];
        u16RxSize <<= 8;
        u16RxSize |= au8Data[7];

        u16TxSize = au8Data[8];
        u16TxSize <<= 8;
        u16TxSize |= au8Data[9];

        u16ServerMask = au8Data[10];
        u16ServerMask <<= 8;
        u16ServerMask |= au8Data[11];

        u8DescriptorCapability = au8Data[12];
        u8MacCapability = au8Data[13];
        u8MaxBufferSize = au8Data[14];

        u16BitFields = au8Data[15];
        u16BitFields <<= 8;
        u16BitFields |= au8Data[16];

        print_string += " (Node Descriptor Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Short Address: 0x" + u16ShortAddr;
        print_string += "\n";
        print_string += "  Manufacturer Code: 0x" + u16ManufacturerCode;
        print_string += "\n";
        print_string += "  Max Rx Size: " + u16RxSize;
        print_string += "\n";
        print_string += "  Max Tx Size: " + u16TxSize;
        print_string += "\n";
        print_string += "  Server Mask: 0x" + u16ServerMask;
        print_string += "\n";
        //displayDescriptorCapability(u8DescriptorCapability);
        //displayMACcapability(u8MacCapability);
        print_string += "  Max Buffer Size: " + u8MaxBufferSize;
        print_string += "\n";
        print_string += "  Bit Fields: 0x" + u16BitFields;
        print_string += "\n";
        */
      }
      break;

    case 0x8043:
      {
        memcpy(ClDataNewDevice, &(au8Data[4]), 1000);
        /*
          byte u8Length = 0;
          uint16_t u16ShortAddr = 0;
          u16ShortAddr = au8Data[2];
          u16ShortAddr <<= 8;
          u16ShortAddr |= au8Data[3];
          u8Length = au8Data[4];

          print_string += " (Simple Descriptor Response)";
          print_string += "\n";
          print_string += "  SQN: 0x" + String(au8Data[0], HEX);
          print_string += "\n";
          print_string += "  Status: 0x" + String(au8Data[1], HEX);
          print_string += "\n";
          print_string += "  Short Address: 0x" + String(u16ShortAddr, HEX);
          print_string += "\n";
          print_string += "  Length: " + String(u8Length, HEX);
          print_string += "\n";

          if (u8Length > 0)
          {
          byte u8InputClusterCount = 0;
          uint16_t u16ProfileId = 0;
          uint16_t u16DeviceId = 0;

          u16ProfileId = au8Data[6];
          u16ProfileId <<= 8;
          u16ProfileId |= au8Data[7];
          u16DeviceId = au8Data[8];
          u16DeviceId <<= 8;
          u16DeviceId |= au8Data[9];
          u8InputClusterCount = au8Data[11];

          print_string += "  EndPoint: 0x" + String(au8Data[5], HEX);
          print_string += "\n";
          //displayProfileId(u16ProfileId);
          print_string += "\n";
          //displayDeviceId(u16DeviceId);
          print_string += "\n";
          print_string += "  Input Cluster Count: " + String(u8InputClusterCount, HEX);
          print_string += "\n";

          uint16_t u16Index = 12;
          for (int i = 0; i < u8InputClusterCount; i++)
          {
            uint16_t u16ClusterId = 0;

            u16ClusterId = au8Data[(i * 2) + 12];
            u16ClusterId <<= 8;
            u16ClusterId |= au8Data[(i * 2) + 13];

            print_string += "    Cluster " + i;
            print_string += ":" + String(u16ClusterId, HEX);

            //displayClusterId(u16ClusterId);
            print_string += "\n";
            u16Index += 2;
          }

          byte u8OutputClusterCount = au8Data[u16Index];
          u16Index++;

          print_string += "  Output Cluster Count: " + String(u8OutputClusterCount, HEX);
          print_string += "\n";

          for (int i = 0; i < u8OutputClusterCount; i++)
          {
            uint16_t u16ClusterId = 0;

            u16ClusterId = au8Data[u16Index];
            u16ClusterId <<= 8;
            u16ClusterId |= au8Data[u16Index + 1];

            print_string += "    Cluster " + i;
            print_string += ":";
            print_string += ":" + String(u16ClusterId, HEX);
            //displayClusterId(u16ClusterId);
            print_string += "\n";
            u16Index += 2;
          }
          }
        */
        ClResponse = true;
      }
      break;
    /*
      case 0x8044:
      {
      print_string += " (Power Descriptor Response)";
      print_string += "\n";
      print_string += "  SQN: 0x" + au8Data[0];
      print_string += "\n";
      print_string += "  Status: 0x" + au8Data[1];
      print_string += "\n";
      }
      break;
    */
    case 0x8045:
      {
        memcpy(rxMessageData_newDevice, &(au8Data[4]), 1000);
        /*
          uint16_t u16ShortAddr = 0;
          u16ShortAddr = au8Data[2];
          u16ShortAddr <<= 8;
          u16ShortAddr |= au8Data[3];

          print_string += " (Active Endpoints Response)";
          print_string += "\n";
          print_string += "  SQN: 0x" + String(au8Data[0], HEX);
          print_string += "\n";
          print_string += "  Status: 0x" + String(au8Data[1], HEX);
          print_string += "\n";
          print_string += "  Short Address: 0x" + String(u16ShortAddr, HEX);
          print_string += "\n";
          print_string += "  Endpoint Count: " + String(au8Data[4], HEX);
          print_string += "\n";
          print_string += "  Endpoint List: ";
          print_string += "\n";
          //rxMessageData_newDevice[0] = au8Data[4];
          for (int i = 0; i < au8Data[4]; i++)
          {
          //rxMessageData_newDevice[1+i] = au8Data[i + 5];
          print_string += "    Endpoint " + i;
          print_string += ": ";
          print_string += "0x" + String(au8Data[i + 5], HEX);
          //simpleDescriptorRequest(u16ShortAddr, au8Data[i + 5]);
          print_string += "\n";
          }
        */
        EpResponse = true;
      }
      break;

    case 0x8047:
      {
        print_string += " (Leave Confirmation)";
        print_string += "\n";

        if (u16Length == 2)
        {
          print_string += "  SQN: 0x" + au8Data[0];
          print_string += "\n";
        }

        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";

        if (u16Length == 9)
        {

          uint64_t u64ExtAddr = 0;
          for (int i = 0; i < 8; i++)
          {
            u64ExtAddr <<= 8;
            u64ExtAddr |= au8Data[1 + i];
          }

          print_string += "  Extended Address: 0x" + u64ExtAddr;
          print_string += "\n";
        }
      }
      break;

    case 0x8048:
      {
        uint64_t u64ExtAddr = 0;

        for (int i = 0; i < 8; i++)
        {
          u64ExtAddr <<= 8;
          u64ExtAddr |= au8Data[i];
        }

        print_string += " (Leave Indication)";
        print_string += "\n";
        print_string += "  Extended Address: 0x" + String(long(u64ExtAddr), HEX);
        print_string += "\n";
        print_string += "  Rejoin Status: 0x" + String(au8Data[8]);
        print_string += "\n";
      }
      break;

    case 0x804A:
      {
        byte u8ScannedChannelsListCount;
        uint16_t u16TotalTx = 0;
        uint16_t u16TxFailures = 0;
        uint32_t u32ScannedChannels = 0;

        u16TotalTx = au8Data[2];
        u16TotalTx <<= 8;
        u16TotalTx |= au8Data[3];

        u16TxFailures = au8Data[4];
        u16TxFailures <<= 8;
        u16TxFailures |= au8Data[5];

        u32ScannedChannels = au8Data[6];
        u32ScannedChannels <<= 8;
        u32ScannedChannels |= au8Data[7];
        u32ScannedChannels <<= 8;
        u32ScannedChannels |= au8Data[8];
        u32ScannedChannels <<= 8;
        u32ScannedChannels |= au8Data[9];

        u8ScannedChannelsListCount = au8Data[10];

        print_string += " (Mgmt Nwk Update Notify)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Total Tx: " + u16TotalTx;
        print_string += "\n";
        print_string += "  Tx Failures: " + u16TxFailures;
        print_string += "\n";
        print_string += "  Scanned Channels: 0x" + u32ScannedChannels;
        print_string += "\n";
        print_string += "  Scanned Channels List Count: " + u8ScannedChannelsListCount;
        print_string += "\n";

        for (int x = 0; x < u8ScannedChannelsListCount; x++)
        {
          print_string += "  Value " + x;
          print_string += ":  0x" + au8Data[11 + x];
          print_string += "\n";
        }
      }
      break;

    case 0x804E:
      {
        byte u8NbTableEntries = 0;
        byte u8StartIx = 0;
        byte u8NbTableListCount = 0;

        uint16_t au16NwkAddr[16];

        u8NbTableEntries = au8Data[2];
        u8NbTableListCount = au8Data[3];
        u8StartIx = au8Data[4];

        print_string += " (Mgmt LQI Response)";
        print_string += "\n";
        print_string += "  SQN: " + u8toStr(au8Data[0]);
        print_string += "\n";
        print_string += "  Status: " + u8toStr(au8Data[1]);
        print_string += "\n";
        print_string += "  Nb Table Entries: " + u8toStr(u8NbTableEntries);
        print_string += "\n";
        print_string += "  Start Index: " + u8toStr(u8StartIx);
        print_string += "\n";
        print_string += "  Nb Table List Count: " + u8toStr(u8NbTableListCount);
        print_string += "\n";

        ////comboBoxAddressList.Items.Clear();

        if (u8NbTableListCount > 0)
        {
          byte i;
          uint64_t u64PanID = 0;
          uint64_t u64ExtAddr = 0;
          uint16_t u16NwkAddr = 0;
          byte u8Lqi = 0;
          byte u8Depth = 0;
          byte u8Flags = 0;
          byte u8PayloadIndex = 5;

          for (i = 0; i < u8NbTableListCount; i++)
          {
            u16NwkAddr = 0;
            for (int x = 0; x < 2; x++, u8PayloadIndex++)
            {
              u16NwkAddr <<= 8;
              u16NwkAddr |= au8Data[u8PayloadIndex];
            }

            u64PanID = 0;
            for (int x = 0; x < 8; x++, u8PayloadIndex++)
            {
              u64PanID <<= 8;
              u64PanID |= au8Data[u8PayloadIndex];
            }

            u64ExtAddr = 0;
            for (int x = 0; x < 8; x++, u8PayloadIndex++)
            {
              u64ExtAddr <<= 8;
              u64ExtAddr |= au8Data[u8PayloadIndex];
            }

            au16NwkAddr[i] = u16NwkAddr;

            au64ExtAddr[i] = u64ExtAddr;

            u8Depth = au8Data[u8PayloadIndex++];
            u8Lqi = au8Data[u8PayloadIndex++];
            u8Flags = au8Data[u8PayloadIndex++];

            print_string += "  Neighbor " + i;
            print_string += ":";
            print_string += "\n";
            print_string += "    Extended Pan ID: " + u64toStr(u64PanID);
            print_string += "\n";
            print_string += "    Extended Address: " + u64toStr(u64ExtAddr);
            print_string += "\n";
            print_string += "    Nwk Address: " + u16toStr(u16NwkAddr);
            print_string += "\n";
            print_string += "    LQI: " + String(u8Lqi, DEC);
            print_string += "\n";
            print_string += "    Depth: " + String(u8Depth, HEX);
            print_string += "\n";
            print_string += "    Flags: 0x" + String(u8Flags, HEX);
            print_string += "\n";

            byte u8DeviceType = (byte)(u8Flags & 0x03);
            print_string += "    Device Type: ";

            if (u8DeviceType == 0)
            {
              print_string += "Coordinator";
            }
            else if (u8DeviceType == 1)
            {
              print_string += "Router";
            }
            else if (u8DeviceType == 2)
            {
              print_string += "End Device";
            }
            else
            {
              print_string += "Unknown";
            }
            print_string += "\n";

            byte u8PermitJoin = (byte)((u8Flags & 0x0C) >> 2);
            print_string += "    Permit Joining: ";

            if (u8PermitJoin == 0)
            {
              print_string += "Off";
            }
            else if (u8PermitJoin == 1)
            {
              print_string += "On";
            }
            else
            {
              print_string += "Unknown";
            }
            print_string += "\n";

            byte u8Relationship = (byte)((u8Flags & 0x30) >> 4);
            print_string += "    Relationship: ";

            if (u8Relationship == 0)
            {
              print_string += "Parent";
            }
            else if (u8Relationship == 1)
            {
              print_string += "Child";
            }
            else if (u8Relationship == 2)
            {
              print_string += "Sibling";
            }
            else if (u8Relationship == 4)
            {
              print_string += "Previous Child";
            }
            else
            {
              print_string += "Unknown";
            }
            print_string += "\n";

            byte u8RxOnWhenIdle = (byte)((u8Flags & 0xC0) >> 6);
            print_string += "    RxOnWhenIdle: ";

            if (u8RxOnWhenIdle == 0)
            {
              print_string += "No";
            }
            else if (u8RxOnWhenIdle == 1)
            {
              print_string += "Yes";
            }
            else
            {
              print_string += "Unknown";
            }
            print_string += "\n";
          }
          for (i = 0; i < u8NbTableListCount; i++)
          {
            //comboBoxAddressList.Items.Add(au16NwkAddr[i]);
          }
        }
      }
      break;

    case 0x8050:
      {
        uint16_t u16TableSize;
        uint16_t u16TableEntries;

        u16TableSize = au8Data[1];
        u16TableSize <<= 8;
        u16TableSize |= au8Data[2];

        u16TableEntries = au8Data[3];
        u16TableEntries <<= 8;
        u16TableEntries |= au8Data[4];

        print_string += " (Addr Map Table Response)";
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Table Size: " + u16TableSize;
        print_string += "\n";
        print_string += "  Entries: " + u16TableEntries;
        print_string += "\n";

        byte i;
        for (i = 0; i < u16TableEntries; i++)
        {
          uint16_t u16Addr;
          uint64_t u64Addr;

          u16Addr = au8Data[5 + (i * 8)];
          u16Addr <<= 8;
          u16Addr |= au8Data[6 + (i * 8)];

          u64Addr = au8Data[7 + (i * 8)];
          u64Addr <<= 8;
          u64Addr |= au8Data[8 + (i * 8)];
          u64Addr <<= 8;
          u64Addr |= au8Data[9 + (i * 8)];
          u64Addr <<= 8;
          u64Addr |= au8Data[10 + (i * 8)];
          u64Addr <<= 8;
          u64Addr |= au8Data[11 + (i * 8)];
          u64Addr <<= 8;
          u64Addr |= au8Data[12 + (i * 8)];
          u64Addr <<= 8;
          u64Addr |= au8Data[13 + (i * 8)];
          u64Addr <<= 8;
          u64Addr |= au8Data[14 + (i * 8)];

          print_string += "  Entry " + i;
          print_string += ": 0x" + u16Addr;
          print_string += " 0x" + u64Addr;

          print_string += "\n";
        }

      }
      break;

    case 0x8060:
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16GroupId = 0;

        u16ClusterId   = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId  |= au8Data[3];

        u16GroupId   = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId  |= au8Data[6];

        print_string += " (Add Group Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Group: 0x" + u16GroupId;
        print_string += "\n";
      }
      break;

    case 0x8061:
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16GroupId = 0;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u16GroupId = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[6];

        print_string += " (View Group Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Group: 0x" + u16GroupId;
        print_string += "\n";
      }
      break;

    case 0x8062:
      {
        uint16_t u16ClusterId = 0;
        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        print_string += " (Get Group Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Capacity: " + au8Data[4];
        print_string += "\n";
        print_string += "  Count: " + au8Data[5];
        print_string += "\n";

        byte i;
        for (i = 0; i < au8Data[5]; i++)
        {
          uint16_t u16GroupId;

          u16GroupId = au8Data[6 + (i * 2)];
          u16GroupId <<= 8;
          u16GroupId |= au8Data[7 + (i * 2)];

          print_string += "  Group " + i;
          print_string += ": 0x" + u16GroupId;
          print_string += "\n";
        }
      }
      break;

    case 0x8063:
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16GroupId = 0;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u16GroupId = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[6];

        print_string += " (Remove Group Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Group: 0x" + u16GroupId;
        print_string += "\n";
      }
      break;

    case 0x807A:
      {
        print_string += " (Identify Local Active)";
        print_string += "\n";
        if (au8Data[0] == 1)
        {
          print_string += "  Status: Start Identifying";
          print_string += "\n";
        }
        else if (au8Data[0] != 1)
        {
          print_string += "  Status: Stop Identifying";
          print_string += "\n";
        }
      }
      break;

    case 0x8095:
      {
        uint16_t u16ClusterId = 0;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        print_string += " (On/Off Update)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Src Addr Mode: 0x" + au8Data[4];
        print_string += "\n";

        if (au8Data[4] == 0x03)
        {
        }
        else
        {
          uint16_t u16Addr = 0;

          u16Addr = au8Data[5];
          u16Addr <<= 8;
          u16Addr |= au8Data[6];

          print_string += "  Src Addr Mode: 0x" + u16Addr;
          print_string += "\n";
          print_string += "  Status: 0x" + au8Data[7];
          print_string += "\n";
        }
      }
      break;

    case 0x80A0:
      {
        uint16_t u16ClusterId = 0, u16GroupId = 0, u16TransTime = 0, u16SceneLength = 0;
        byte u8Status;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u8Status = au8Data[4];

        u16GroupId = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[6];


        print_string += " (View Scene)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Cluster ID: 0x" + u16ClusterId;
        print_string += "\n";
        print_string += "  Status: 0x" + u8Status;
        print_string += "\n";
        print_string += "  Group ID: 0x" + u16GroupId;
        print_string += "\n";
        print_string += "  Scene Id: 0x" + au8Data[7];

        if (0 == u8Status)
        {
          u16TransTime = au8Data[8];
          u16TransTime <<= 8;
          u16TransTime |= au8Data[9];

          print_string += "\n";
          print_string += "  Transition Time: 0x" + u16TransTime;
          print_string += "\n";
          print_string += "  Scene Name Length: 0x" + au8Data[10];
          print_string += "\n";
          print_string += "  Scene Name Max Length: 0x" + au8Data[11];
          print_string += "\n";

          print_string += "  Scene Name: ";

          byte i = 0;
          for (i = 0; i < au8Data[10]; i++)
          {
            //print_string += Convert.ToChar(au8Data[12 + i]);
          }

          u16SceneLength = au8Data[12 + i];
          u16SceneLength <<= 8;
          u16SceneLength |= au8Data[13 + i];

          print_string += "\n";
          print_string += "  Ext Scene Length: 0x" + u16SceneLength;
          print_string += "\n";
          print_string += "  Ext Max Length: 0x" + au8Data[14 + i];
          print_string += "\n";
          print_string += "  Scene Data: ";
          print_string += "\n      ";

          for (byte c = 0; i < u16SceneLength; i++)
          {
            //print_string += "0x" + au8Data[15 + i + c] + " ";
          }
        }

      }
      break;

    case 0x80A3:
      {
        uint16_t u16ClusterId, u16GroupId;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u16GroupId = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[6];

        print_string += " (Remove All Scenes)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Cluster ID: 0x" + u16ClusterId;
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Group ID: 0x" + u16GroupId;
      }
      break;

    case 0x80A2:
      {
        uint16_t u16ClusterId, u16GroupId;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u16GroupId = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[6];

        print_string += " (Remove Scene)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Cluster ID: 0x" + u16ClusterId;
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Group ID: 0x" + u16GroupId;
        print_string += "\n";
        print_string += "  Scene ID: 0x" + au8Data[7];
      }
      break;

    case 0x8100: // Read attribute response
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16AttribId = 0;
        uint16_t u16SrcAddr = 0;
        uint16_t u16AttributeSize = 0;

        u16SrcAddr = au8Data[1];
        u16SrcAddr <<= 8;
        u16SrcAddr |= au8Data[2];

        u16ClusterId = au8Data[4];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[5];

        u16AttribId = au8Data[6];
        u16AttribId <<= 8;
        u16AttribId |= au8Data[7];

        u16AttributeSize = au8Data[10];
        u16AttributeSize <<= 8;
        u16AttributeSize |= au8Data[11];

        print_string += " (Read Attrib Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + String(au8Data[0], HEX);
        print_string += "\n";
        print_string += "  Src Addr: 0x" + String(u16SrcAddr, HEX);
        print_string += "\n";
        print_string += "  EndPoint: 0x" + String(au8Data[3], HEX);
        print_string += "\n";
        print_string += "  Status: 0x" + String(au8Data[8], HEX);
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        //displayAttribute(u16AttribId, au8Data[9], au8Data, 12, u16AttributeSize);
      }
      break;

    case 0x8101:
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16DstAddr = 0;

        u16ClusterId = au8Data[13];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[14];

        u16DstAddr = au8Data[1];
        u16DstAddr <<= 8;
        u16DstAddr |= au8Data[2];

        print_string += " (Default Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Short Address: 0x" + u16DstAddr;
        print_string += "\n";
        print_string += "  Source EndPoint: 0x" + au8Data[11];
        print_string += "\n";
        print_string += "  Destination EndPoint: 0x" + au8Data[12];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Command: 0x" + au8Data[15];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[16];
        print_string += "\n";
      }
      break;

    case 0x8120:
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16SrcAddr = 0;

        u16ClusterId = au8Data[4];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[5];

        u16SrcAddr = au8Data[1];
        u16SrcAddr <<= 8;
        u16SrcAddr |= au8Data[2];

        print_string += " (Report Config Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Src Addr: 0x" + u16SrcAddr;
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[3];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[6];
        print_string += "\n";
      }
      break;

    case 0x8102:
      {
        uint16_t u16SrcAddr = 0;
        uint16_t u16ClusterId = 0;
        uint16_t u16AttribId = 0;
        uint16_t u16AttributeSize = 0;

        u16SrcAddr = au8Data[1];
        u16SrcAddr <<= 8;
        u16SrcAddr |= au8Data[2];

        u16ClusterId = au8Data[4];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[5];

        u16AttribId = au8Data[6];
        u16AttribId <<= 8;
        u16AttribId |= au8Data[7];

        u16AttributeSize = au8Data[10];
        u16AttributeSize <<= 8;
        u16AttributeSize |= au8Data[11];

        print_string += " (Attribute Report)";
        print_string += "\n";
        //print_string += "  SQN: 0x" + String(au8Data[0], HEX);
        //print_string += "\n";
        //print_string += "  Src Addr: 0x" + String(u16SrcAddr, HEX);
        //print_string += "\n";
        //print_string += "  Src Ep: 0x" + String(au8Data[3], DEC);
        //print_string += "\n";
        //print_string += "  Cluster: 0x" + String(u16ClusterId, HEX);
        //displayClusterId(u16ClusterId);
        //print_string += "\n";
        displayAttribute(au8Data[0], u16SrcAddr, u16ClusterId, u16AttribId, au8Data[9], au8Data, 12, u16AttributeSize);
      }
      break;

    case 0x8122:
      {
        uint16_t u16SrcAddr = 0;
        uint16_t u16ClusterId = 0;
        uint16_t u16AttribId = 0;
        uint16_t u16MaxInterval = 0;
        uint16_t u16MinInterval = 0;

        u16SrcAddr = au8Data[1];
        u16SrcAddr <<= 8;
        u16SrcAddr |= au8Data[2];

        u16ClusterId = au8Data[4];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[5];

        u16AttribId = au8Data[8];
        u16AttribId <<= 8;
        u16AttribId |= au8Data[9];

        u16MaxInterval = au8Data[10];
        u16MaxInterval <<= 8;
        u16MaxInterval |= au8Data[11];

        u16MinInterval = au8Data[12];
        u16MinInterval <<= 8;
        u16MinInterval |= au8Data[13];

        print_string += " (Attribute Config Report)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Src Addr: 0x" + u16SrcAddr;
        print_string += "\n";
        print_string += "  Src Ep: 0x" + au8Data[3];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[6];
        print_string += "\n";
        //displayAttribute(au8Data[7]);
        print_string += "\n";
        print_string += "  Attribute: 0x" + u16AttribId;
        print_string += "\n";
        print_string += "  Min Interval: " + u16MinInterval;
        print_string += "\n";
        print_string += "  Max Interval: " + u16MaxInterval;
        print_string += "\n";
      }
      break;

    case 0x8103: // Read local attribute response
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16AttribId = 0;
        uint16_t u16SrcAddr = 0;
        uint16_t u16AttributeSize = 0;

        u16SrcAddr = au8Data[1];
        u16SrcAddr <<= 8;
        u16SrcAddr |= au8Data[2];

        u16ClusterId = au8Data[4];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[5];

        u16AttribId = au8Data[6];
        u16AttribId <<= 8;
        u16AttribId |= au8Data[7];

        u16AttributeSize = au8Data[10];
        u16AttributeSize <<= 8;
        u16AttributeSize |= au8Data[11];

        print_string += " (Read Local Attrib Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        //print_string += "  Src Addr: 0x" + u16SrcAddr;
        //print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[3];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[8];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        ////displayAttribute(u16AttribId, au8Data[9], au8Data, 12, u16AttributeSize);
      }
      break;

    case 0x8140: // Discover attribute response
      {
        uint16_t u16AttribId = 0;

        u16AttribId = au8Data[2];
        u16AttribId <<= 8;
        u16AttribId |= au8Data[3];

        print_string += " (Discover Attrib Response)";
        print_string += "\n";
        print_string += "  Complete: 0x" + au8Data[0];
        print_string += "\n";
        //displayAttribute(au8Data[1]);
        print_string += "\n";
        print_string += "  Attribute: 0x" + u16AttribId;
        print_string += "\n";
      }
      break;

    case 0x8141: // Discover extended attribute response
      {
        uint16_t u16AttribId = 0;

        u16AttribId = au8Data[2];
        u16AttribId <<= 8;
        u16AttribId |= au8Data[3];

        print_string += " (Discover Attrib Response)";
        print_string += "\n";
        print_string += "  Complete: 0x" + au8Data[0];
        print_string += "\n";
        //displayDataType(au8Data[1]);
        print_string += "\n";
        print_string += "  Attribute: 0x" + u16AttribId;
        print_string += "\n";
        print_string += "  Flags: 0x" + au8Data[4];
        print_string += "\n";
      }
      break;

    case 0x8150: // Discover command received individual response
      {
        print_string += " (Discover Command Received Individual Response)";
        print_string += "\n";
        print_string += "  Command: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Index: 0x" + au8Data[1];
        print_string += "\n";
      }
      break;

    case 0x8151: // Discover command received response
      {
        print_string += " (Discover Command Received Response)";
        print_string += "\n";
        print_string += "  Complete: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Commands: " + au8Data[1];
        print_string += "\n";
      }
      break;

    case 0x8160: // Discover command generated individual response
      {
        print_string += " (Discover Command Generated Individual Response)";
        print_string += "\n";
        print_string += "  Command: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Index: 0x" + au8Data[1];
        print_string += "\n";
      }
      break;

    case 0x8161: // Discover command generated response
      {
        print_string += " (Discover Command Generated Response)";
        print_string += "\n";
        print_string += "  Complete: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Commands: " + au8Data[1];
        print_string += "\n";
      }
      break;

    case 0x8401:
      {
        uint16_t u16ClusterId = 0;
        uint16_t u16ZoneStatus = 0;
        uint16_t u16Delay = 0;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        print_string += " (IAS Zone Status Change)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  EndPoint: 0x" + au8Data[1];
        print_string += "\n";
        //displayClusterId(u16ClusterId);
        print_string += "\n";
        print_string += "  Src Addr Mode: 0x" + au8Data[4];
        print_string += "\n";

        if (au8Data[4] == 0x03)
        {
        }
        else
        {
          uint16_t u16Addr = 0;

          u16Addr = au8Data[5];
          u16Addr <<= 8;
          u16Addr |= au8Data[6];

          print_string += "  Src Addr Mode: 0x" + u16Addr;
          print_string += "\n";
        }

        u16ZoneStatus   = au8Data[7];
        u16ZoneStatus <<= 8;
        u16ZoneStatus  |= au8Data[8];

        print_string += "  Zone Status: 0x" + u16ZoneStatus;
        print_string += "\n";
        print_string += "  Ext Status: 0x" + au8Data[9];
        print_string += "\n";
        print_string += "  Zone ID: 0x" + au8Data[10];
        print_string += "\n";

        u16Delay   = au8Data[11];
        u16Delay <<= 8;
        u16Delay  |= au8Data[12];

        print_string += "  Delay: 0x" + u16Delay;
        print_string += "\n";
      }
      break;

    case 0x004D:
      {

        uint16_t u16ShortAddr = 0;
        uint64_t u64ExtAddr = 0;

        u16ShortAddr = au8Data[0];
        u16ShortAddr <<= 8;
        u16ShortAddr |= au8Data[1];

        for (int i = 0; i < 8; i++)
        {
          u64ExtAddr <<= 8;
          u64ExtAddr |= au8Data[2 + i];
        }
        /*
          unsigned long u16ShortAddr_long = u16ShortAddr;
          unsigned long u64ExtAddr_long = u64ExtAddr;

          print_string += " (End Device Announce)";
          print_string += "\n";
          print_string += "  Short Address: 0x" + String(u16ShortAddr_long, HEX);
          print_string += "\n";
          print_string += "  Extended Address: 0x" + String(u64ExtAddr_long, HEX);
          print_string += "\n";
        */
        new_device_LongAddr = u64ExtAddr;
        new_device_ShortAddr = u16ShortAddr;
        new_device_connected = true;

        //activeEndpointDescriptorRequest(u16ShortAddr);
      }
      break;

    case 0x8501:
      {

      }
      break;
    case 0x8503:
      {


      }
      break;

    case 0x8110:
      {
        uint16_t u16SrcAddr = 0;
        uint16_t u16ClusterId = 0;
        uint16_t u16AttribId = 0;
        uint16_t u16AttributeSize = 0;

        u16SrcAddr = au8Data[1];
        u16SrcAddr <<= 8;
        u16SrcAddr |= au8Data[2];

        u16ClusterId = au8Data[4];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[5];

        u16AttribId = au8Data[6];
        u16AttribId <<= 8;
        u16AttribId |= au8Data[7];

        u16AttributeSize = au8Data[10];
        u16AttributeSize <<= 8;
        u16AttributeSize |= au8Data[11];

        print_string += " (Write Attrib Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Src Addr: 0x" + u16SrcAddr;
        print_string += "\n";
        print_string += "  Src Ep: 0x" + au8Data[3];
        print_string += "\n";
        ////displayClusterId(u16ClusterId);
        print_string += "\n";
        ////displayAttribute(u16AttribId, au8Data[9], au8Data, 12, u16AttributeSize);
        print_string += "  Status: 0x" + au8Data[8];
        print_string += "\n";
      }
      break;

    case 0x8600:
      {
        //nwkRecovery.NetworkRecoveryParseBuffer(au8Data);
        print_string += " (Retrieve Network Recovery Response)";
        print_string += "\n";
        //print_string += nwkRecovery;
        print_string += "\n";
      }
      break;

    case 0x8601:
      {
        print_string += " (Restore Network Recovery Response)";
        print_string += "\n";
        print_string += "  Success = " + au8Data[0];
        print_string += "\n";
      }
      break;

    case 0x80A4:
      {
        uint16_t u16GroupId;
        uint16_t u16ClusterId;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u16GroupId = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[6];

        print_string += " (Store Scene Response)";
        print_string += "\n";
        print_string += "  Tx Num: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Source Endpoint: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Cluster ID: 0x" + u16ClusterId;
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Group ID: 0x" + u16GroupId;
        print_string += "\n";
        print_string += "  Scene ID: 0x" + au8Data[7];
        print_string += "\n";
      }
      break;

    case 0x80A1:
      {
        uint16_t u16GroupId;
        uint16_t u16ClusterId;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u16GroupId = au8Data[5];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[6];

        print_string += " (Add Scene Response)";
        print_string += "\n";
        print_string += "  Tx Num: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Source Endpoint: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Cluster ID: 0x" + u16ClusterId;
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Group ID: 0x" + u16GroupId;
        print_string += "\n";
        print_string += "  Scene ID: 0x" + au8Data[7];
        print_string += "\n";
      }
      break;

    case 0x80A6:
      {
        uint16_t u16GroupId;
        uint16_t u16ClusterId;

        u16ClusterId = au8Data[2];
        u16ClusterId <<= 8;
        u16ClusterId |= au8Data[3];

        u16GroupId = au8Data[6];
        u16GroupId <<= 8;
        u16GroupId |= au8Data[7];

        print_string += " (Get Scene Membership Response)";
        print_string += "\n";
        print_string += "  Tx Num: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Source Endpoint: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Cluster ID: 0x" + u16ClusterId;
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[4];
        print_string += "\n";
        print_string += "  Capacity: 0x" + au8Data[5];
        print_string += "\n";
        print_string += "  Group ID: 0x" + u16GroupId;
        print_string += "\n";
        print_string += "  Scene Count: 0x" + au8Data[8];

        if (au8Data[8] != 0)
        {
          print_string += "\n";
          print_string += "  Scene List: ";
        }

        byte i;

        for (i = 0; i < au8Data[8]; i++)
        {

          print_string += "\n";
          print_string += "    Scene: 0x" + au8Data[i + 9];
        }
        print_string += "\n";
      }
      break;
    case 0x8046:
      {
        uint16_t u16AddrOfInterest;

        u16AddrOfInterest = au8Data[2];
        u16AddrOfInterest <<= 8;
        u16AddrOfInterest |= au8Data[3];

        print_string += " (Match Descriptor Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Address Of Interest: 0x" + u16AddrOfInterest;
        print_string += "\n";
        print_string += "  Match Length: " + au8Data[4];

        if (au8Data[4] != 0)
        {
          print_string += "  Matched Endpoints: ";
        }

        byte i;
        for (i = 0; i < au8Data[4]; i++)
        {
          print_string += "\n";
          print_string += "    Endpoint " + au8Data[5 + i];
          print_string += "\n";
        }
      }
      break;
    case 0x8044:
      {
        print_string += " (Power Descriptor Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Power Source Level: "; // + Convert.ToString(au8Data[2] & 0x7, 2).PadLeft(4, '0');
        print_string += "\n";
        print_string += "  Current Power Source: "; // + Convert.ToString((au8Data[2] >> 4) & 0x7, 2).PadLeft(4, '0');
        print_string += "\n";
        print_string += "  Available Power Source: "; // + Convert.ToString((au8Data[3]) & 0x7, 2).PadLeft(4, '0');
        print_string += "\n";
        print_string += "  Current Power Mode: "; // + Convert.ToString((au8Data[3] >> 4) & 0x7, 2).PadLeft(4, '0');
        print_string += "\n";
      }
      break;

    case 0x8701:
      {
        print_string += " (Route Discovery Confirm)";
        print_string += "\n";
        print_string += "  SQN: 0x" + String(au8Data[0], HEX);
        print_string += "\n";
        print_string += "  Status: 0x" + String(au8Data[1], HEX);
        print_string += "\n";
        print_string += "  Network Status: 0x" + String(au8Data[2], HEX);
        print_string += "\n";
      }
      break;

    case 0x8702:
      {
        uint16_t u16DestAddr;

        u16DestAddr = au8Data[4];
        u16DestAddr <<= 8;
        u16DestAddr |= au8Data[5];

        print_string += " (APS Data Confirm Fail)";
        print_string += "\n";
        print_string += "  Status: 0x" + String(au8Data[0], HEX);
        print_string += "\n";
        print_string += "  Source Endpoint: 0x" + String(au8Data[1], HEX);
        print_string += "\n";
        print_string += "  Destination Endpoint: 0x" + String(au8Data[2], HEX);
        print_string += "\n";
        print_string += "  Destination Mode: 0x" + String(au8Data[3], HEX);
        print_string += "\n";
        print_string += "  Destination Address: 0x" + String(u16DestAddr, HEX);
        print_string += "\n";
        print_string += "  SQN: 0x" + String(au8Data[6], HEX);
        print_string += "\n";
      }
      break;

    case 0x8531:
      {
        uint16_t u16AddressOfInterest;

        u16AddressOfInterest = au8Data[2];
        u16AddressOfInterest <<= 8;
        u16AddressOfInterest |= au8Data[3];

        print_string += " (Complex Descriptor Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + String(au8Data[0], HEX);
        print_string += "\n";
        print_string += "  Status: 0x" + String(au8Data[1], HEX);
        print_string += "\n";
        print_string += "  Address of Interest: 0x" + String(u16AddressOfInterest, HEX);
        print_string += "\n";
        print_string += "  Length: " + String(au8Data[4], HEX);
        print_string += "\n";

        if (au8Data[1] == 0)
        {
          print_string += "        XML Tag: " + au8Data[5];
          print_string += "\n";
          print_string += "        Field Count: " + au8Data[6];
          print_string += "\n";
          print_string += "        Complex Description: ";
          for (int i = 0; i < au8Data[6]; i++)
          {
            char c = (char)au8Data[6 + i + 1];
            print_string += c;
          }
          print_string += "\n";
        }
      }
      break;

    case 0x8532:
      {
        byte u8StrLen;
        uint16_t u16NwkAddr = 0;

        u16NwkAddr = au8Data[2];
        u16NwkAddr <<= 8;
        u16NwkAddr |= au8Data[3];
        u8StrLen = au8Data[4];

        print_string += " (User Descriptor Request Response)";
        print_string += "\n";
        print_string += "  SQN: 0x" + au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x" + au8Data[1];
        print_string += "\n";
        print_string += "  Nwk Address: 0x" + u16NwkAddr;
        print_string += "\n";

        if (au8Data[1] == 0)
        {
          print_string += "  Length: " + u8StrLen;
          print_string += "\n";
          print_string += "  Descriptor: ";

          for (int i = 0; i < u8StrLen; i++)
          {
            char c = (char)au8Data[5 + i];
            print_string += c;
          }
          print_string += "\n";
        }
      }
      break;

    case 0x8533:
      {
        byte u8StrLen;
        uint16_t u16NwkAddr = 0;

        u16NwkAddr = au8Data[2];
        u16NwkAddr <<= 8;
        u16NwkAddr |= au8Data[3];
        u8StrLen = au8Data[4];

        print_string += " (User Descriptor Set Confirm)";
        print_string += "\n";
        print_string += "  SQN: 0x";
        print_string += au8Data[0];
        print_string += "\n";
        print_string += "  Status: 0x";
        print_string += au8Data[1];
        print_string += "\n";
        print_string += "  Nwk Address: 0x";
        print_string += u16NwkAddr;
        print_string += "\n";
      }
      break;


    default:
      {
        print_string += " (Unrecognized)";
        print_string += "\n";
      }
      break;
  }
  Serial.print(print_string);
  print_string = "";
}
