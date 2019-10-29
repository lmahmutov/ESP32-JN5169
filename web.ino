
//Websocket
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
    String jsond = "";
  String spayload;
  // Figure out the type of WebSocket event
  switch (type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] WS from ", num);
        Serial.println(ip.toString());
      }
      break;

    // Echo text message back to client
    case WStype_TEXT:
      Serial.printf("[%u] Text: %s\n", num, payload);
      spayload = String((char*) payload);

      if (spayload == "getDeviceList") {
        webSocket.sendTXT(num, jsond);jsond="";
      }

      if (spayload == "addDevice") {
        Serial.println("set_permit_joining_req");
          setPermitJoin(0x0000, 0xFE, 0x00);
          delay(50);
      
      }

      if (spayload == "getConfig") {
        Serial.println("getConfig");
        //String StringConfig = "{\"ssid\":\"" + ConfigSettings.ssid + "\",\"pass\":\"" + ConfigSettings.password + "\",\"ip\":\"" + ConfigSettings.ipAddress + "\",\"mask\":\"" + ConfigSettings.ipMask + "\",\"gw\":\"" + ConfigSettings.ipGW + "\",\"mqtt\":\"" + ConfigSettings.ipMQTT + "\",\"mqttPort\":\"" + ConfigSettings.mqttPort + "\",\"mqttLogin\":\"" + ConfigSettings.mqttLogin + "\",\"mqttPassw\":\"" + ConfigSettings.mqttPassw + "\",\"mqttEnable\":\"" + ConfigSettings.mqttEnable + "\"}";
       
       // webSocket.sendTXT(num, "retConfig/" + StringConfig);
      }

      if (spayload.indexOf("ssid") > 1) {
        Serial.println("wsetConfig");
      }
      if (spayload == "initDevice") {
        Serial.println("INIT Zigbee");
      }
      break;



    case WStype_BIN:
      Serial.printf("[WSc] get binary lenght: %u\n", length); // hexdump(payload,length);
      if (payload[0] == 0xfe) {
        Serial.println("send_raw_data");  // отправка рав даты в юарт
      }
      if (payload[0] == 0x01) {
       Serial.println("send_cmd_OnOf");  // отправка on_off
       sendClusterOnOff(3,0xdb91,1,1,2);
       // send_on_off(BUILD_UINT16(payload[2], payload[1]), payload[3], payload[4]);
      }
      break;
    case WStype_ERROR: break;
    case WStype_FRAGMENT_TEXT_START: break;
    case WStype_FRAGMENT_BIN_START: break;
    case WStype_FRAGMENT: break;
    case WStype_FRAGMENT_FIN: break;
    default:
      break;

  }
}


//Generated page
void devices() {
  String sql = "select * from devices";
  rc = sqlite3_prepare_v2(db, sql.c_str(), 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    resp += sqlite3_errmsg(db);
    resp += "<br><br><a href='/'>back</a>";
    request->send(200, "text/html", resp);
    Serial.println(resp.c_str());
    return;
  }
  rec_count = 0;
  String resp = "<h2>List of connected devices:</h2><h3>";
  resp += sql;
  resp += "</h3><table cellspacing='1' cellpadding='1' border='1'>";
  bool first = true;
  while (sqlite3_step(res) == SQLITE_ROW) {
    //resp = "";
    if (first) {
      int count = sqlite3_column_count(res);
      if (count == 0) {
        resp += "<tr><td>Statement executed successfully</td></tr>";
        rec_count = sqlite3_changes(db);
        break;
      }
      resp += "<tr>";
      for (int i = 0; i < count; i++) {
        resp += "<td>";
        resp += sqlite3_column_name(res, i);
        resp += "</td>";
      }
      resp += "</tr>";
      first = false;
    }
    int count = sqlite3_column_count(res);
    resp += "<tr>";
    for (int i = 0; i < count; i++) {
      resp += "<td>";
      resp += (const char *) sqlite3_column_text(res, i);
      resp += "</td>";
    }
    resp += "</tr>";
    rec_count++;
  }
  resp += "</table><br><br>Number of records: ";
  resp += rec_count;
  resp += ".<br><br><input type=button onclick='location.href=\"/\"' value='back'/>";
  request->send(200, "text/html", resp);
  sqlite3_finalize(res);
}
