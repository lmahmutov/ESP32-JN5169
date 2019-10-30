#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define UseOled // Есть или нет Экран

#include <FS.h>
#include "time.h"
#include "SPIFFS.h"
#include <sqlite3.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "HardwareSerial.h"
HardwareSerial jnSerial(2);

#ifdef UseOled
//OLED Section
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int displayEnabled = 60;
#endif
unsigned long timing; // Переменная для хранения точки отсчета
//Web server
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
const char* PARAM_MESSAGE = "message";
//---------------------------------
//SQlite
sqlite3 *db;
int rc;
sqlite3_stmt *res;
int rec_count = 0;
const char *tail;

//NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;
char dateStringBuff[50]; //50 chars should be enough
char timeStringBuff[50]; //50 chars should be enough

//---------------------------------------
String print_string = "";
String attr_response = "";
bool oledidle = true;
bool joinStarted = false;
int joinSecCounter = 0;
// ------task del from database---------
bool DelCall = false;
uint64_t deletedDevLongAddr  = 0;
// ------task get full info ------------
bool new_device_connected = false;
bool connectGood = true;
bool needBind = false;
uint8_t bindEp = 0;
uint16_t new_device_ShortAddr = 0;
uint64_t new_device_LongAddr  = 0;
String NewDevName = "";
int counter = 0;
bool EpResponse = false;
bool ClResponse = false;
bool BindResponse = false;
bool DnResponse = false;
byte rxMessageData_newDevice[1000];
byte ClDataNewDevice[1000];
String NewDevComplete = "";
//---------------------------------------
uint64_t u64ExtendedAddr_coord;
// --------------------------------------
uint64_t au64ExtAddr[16];
byte rxMessageData[1024];
byte rxMessageChecksum = 0;
uint16_t rxMessageLength = 0;
uint8_t rxMessageState    = 0;
uint16_t rxMessageType = 0;
uint8_t rxMessageCount    = 0;
bool rxMessageInEscape = false;
byte rxByte;


#define RXD2 16
#define TXD2 17

void serialEvent() {
  webSocket.loop();
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

void TaskDecode( void *pvParameters );
void TaskGetFullInfo( void *pvParameters );
void TaskDelDevice( void *pvParameters );
void TaskOled( void *pvParameters );

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
#ifdef UseOled
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Zigbee Gateway v0.1"));
  display.setCursor(0, 10);
  display.println(F("https://nrf52840.ru"));
  display.setCursor(0, 20);
  display.println(F("Wifi config mode"));
  display.setCursor(10, 30);
  display.println(F("Connect to AP"));
  display.setCursor(20, 40);
  display.println(F("ZigBeeGW"));
  display.setCursor(20, 50);
  display.println(F("and Setup WiFi"));
  display.display();
#endif
}

void UpdateLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(dateStringBuff, sizeof(dateStringBuff), "%d %B %Y", &timeinfo); //"%A, %B %d %Y"
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
}

uint32_t getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start");

  jnSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);
  //SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //SQlite
  File root = SPIFFS.open("/");
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }

  sqlite3_initialize();
  if (db_open("/spiffs/data.db", &db))
    return;
  //-------------------------------------------------------------------------

#ifdef UseOled
  //OLED
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
#endif
  // Wifi Section
  WiFiManager wm;
  //wm.resetSettings();

  wm.setAPCallback(configModeCallback);
  // id/name, placeholder/prompt, default, length
  if (!wm.autoConnect("ZigBeeGW")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  UpdateLocalTime();
#ifdef UseOled
  //Oled Show WIFI
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Zigbee Gateway v0.1"));
  display.setCursor(0, 15);
  display.println(F("https://nrf52840.ru"));
  display.setCursor(0, 30);
  display.println(F("WIFI Connected"));
  display.setCursor(0, 45);
  display.println(F("IP:"));
  display.setCursor(20, 45);
  display.println(WiFi.localIP());
  display.display();
#endif
  //Web Server setup
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/ws.html", "text/html");
  });
  server.on ( "/devices", [](AsyncWebServerRequest * request) {
    devicesWebpage(request);
  } );
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/", "text/html");
  });
  server.begin();
  delay(1000);
  webSocket.begin(); webSocket.onEvent(onWebSocketEvent); // Start WebSocket server and assign callback

  ////
  xTaskCreatePinnedToCore(
    TaskDecode
    ,  "TaskDecodeUart"
    ,  32768
    ,  NULL
    ,  1
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskGetFullInfo
    ,  "GetFullInfoFromConnectedDevice"
    ,  10000
    ,  NULL
    ,  1
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskOled
    ,  "Oled and join data"
    ,  10000
    ,  NULL
    ,  1
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskDelDevice
    ,  "Delete from database"
    ,  10000
    ,  NULL
    ,  1
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  //Hard RESET
  //transmitCommand(0x0012, 0, 0);
  //delay(2000);
  //SOFT RESET
  //transmitCommand(0x0011, 0, 0);
  //Check version of firmware on JN5169
  transmitCommand(0x0010, 0, 0);
  delay(50);
  //Set device type Coordinator
  setDeviceType(0);
  delay(50);
  //Set ZigBee Channel
  setChannel(11);
  delay(50);
  transmitCommand(0x0024, 0, 0);
  delay(50);
  //transmitCommand(0x0015, 0, 0);
  //network state
  transmitCommand(0x0009, 0, 0);
  delay(50);
  DiscoverDevices();
  delay(50);
  //sendMgmtLqiRequest(0x2FE8, 0);
  //delay(50);
  setPermitJoin(0xFFFC, 0x1E, 0x00);
  delay(50);
  transmitCommand(0x0014, 0, 0);
  delay(50);
  setTime(getTime());
  delay(50);
  transmitCommand(0x0017, 0, 0);
  delay(50);
  //sendReadAttribRequest(0x5465, 1, 1 , 0 , 0, 0, 0, 1, 0x0005);
  //void sendReadAttribRequest(uint16_t u16ShortAddr, byte u8SrcEndPoint, byte u8DstEndPoint, uint16_t u16ClusterID, byte u8Direction, byte u8ManuSpecific, uint16_t u16ManuID, byte u8AttribCount, uint16_t u16AttribID1)
  //sqlite_select_answer("0xf0c8fc06");
}

void OledTimeIP()
{
  UpdateLocalTime();
#ifdef UseOled
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 0);
  display.println(F("Zigbee Gateway v0.1"));
  display.setCursor(10, 12);
  display.println(F("IP:"));
  display.setCursor(40, 12);
  display.println(WiFi.localIP());
  display.setCursor(20, 24);
  display.println(dateStringBuff);
  display.setCursor(44, 36);
  display.println(timeStringBuff);
  if (joinStarted) {
    display.setCursor(20, 48);
    display.println("Join enabled " + String(joinSecCounter));
  }
  else {
    display.setCursor(10, 48);
    String freeRam = String(ESP.getFreeHeap(), DEC);
    display.println("Free RAM : " + freeRam);
  }
  display.display();
#endif
}

void loop() {
  //transmitCommand(0x0017, 0, 0);
  delay(1000);
  //sendClusterOnOff(2,0x5465,1,1,2);
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDecode(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    serialEvent();
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskOled(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
#ifdef UseOled
    OledTimeIP();
#endif
    if (joinStarted) {
      joinSecCounter--;
      if (joinSecCounter < 0) {
        Serial.println("Join disabled");
        joinStarted = false;
        webSocket.broadcastTXT("Join disabled");
      }
    }
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskGetFullInfo(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    if (new_device_connected && joinStarted) {
      new_device_connected = false;  // получаем флаг что девайс подключился, и сразу его сбросим
      // дальше начнем обработку, проверим есть ли железка в базе
      if (sqlite_select_answer(u64toStr(new_device_LongAddr)) == 0) {
        NewDevComplete = "";           // очищаем стринг вывода
        activeEndpointDescriptorRequest(new_device_ShortAddr); // посылаем в сеть запрос эндпоинтов (0x0045)
        counter = 5000;                // задерка на 5 сек
        while (!EpResponse) {          // ждем ответа 0x8045 (копируем данные в новый массив, и флаг ставим true)
          delay(1);
          if (counter-- == 0) {
            connectGood = false;
            break;
          }
        }                             // получили ответ или закончилось время
        delay(50);                    // На всякий случай подождем, чтобы слишком быстро не слать команду
        //Эндпоинты получили, пытаемся узнать имя железки
        sendReadAttribRequest(new_device_ShortAddr, 1, rxMessageData_newDevice[1] , 0 , 0, 0, 0, 1, 0x0005); // Запрос атрибута как зовут железку
        counter = 5000;               // задержка 5 сек
        while (!DnResponse) {         // ждем ответа 0x8102 (почему то ответчает атрибут репорт.... имя ложим в переменную NewDevName)
          delay(1);
          if (counter-- == 0) {
            connectGood = false;
            break;
          }
        }
        delay(50);                   //На всякий случай подождем, чтобы слишком быстро не слать команду
        //
        NewDevComplete += "{" + u64toStr(new_device_LongAddr) + ": ";
        NewDevComplete += NewDevName + " ; ";
        NewDevComplete += u16toStr(new_device_ShortAddr) + " ; ";
        for (int i = 0; i < rxMessageData_newDevice[0]; i++)
        {
          NewDevComplete += " Ep";
          NewDevComplete += String(i, DEC) + ":";
          NewDevComplete += String(rxMessageData_newDevice[i + 1], DEC) + " ; ";
          bindEp = rxMessageData_newDevice[i + 1];
          simpleDescriptorRequest(new_device_ShortAddr, bindEp);
          counter = 5000;
          while (!ClResponse) {
            delay(1);
            if (counter-- == 0) {
              connectGood = false;
              break;
            }
          }
          ///Get clusters
          byte u8Length = 0;
          u8Length = ClDataNewDevice[0];
          if (u8Length > 0)
          {
            byte u8InputClusterCount = 0;
            u8InputClusterCount = ClDataNewDevice[7];
            NewDevComplete += "Clusters ";
            for (int i = 0; i < u8InputClusterCount; i++)
            {
              uint16_t u16ClusterId = 0;
              u16ClusterId = ClDataNewDevice[(i * 2) + 8];
              u16ClusterId <<= 8;
              u16ClusterId |= ClDataNewDevice[(i * 2) + 9];
              NewDevComplete += ": " + u16toStr(u16ClusterId);
              //Тест механики биндда, если сработает будет круто ) Биндим все кластеры по очереди. и биндим только на новые устройства.
              if (connectGood && needBind) {
                Serial.println(u64toStr(new_device_LongAddr) + ", " +  String(bindEp, DEC) + ", " + u16toStr(u16ClusterId) + ", 3, " + u64toStr(u64ExtendedAddr_coord) + ", 1");
                sendBindRequest(new_device_LongAddr, bindEp, u16ClusterId, 3, u64ExtendedAddr_coord, 1 );
                counter = 5000;
                while (!BindResponse) {
                  delay(1);
                  if (counter-- == 0) {
                    break;
                  }
                }
                BindResponse = false;
              }
              delay(50); //На всякий случай подождем, чтобы слишком быстро не слать команду
              // sqlite_insertnewdev(u64toStr(new_device_LongAddr), NewDevName, u16toStr(new_device_ShortAddr));
            }
          }
          delay(50); //На всякий случай подождем, чтобы слишком быстро не слать команду
        }
        NewDevComplete += " }";
        if (connectGood == true) {
          webSocket.broadcastTXT(NewDevComplete);
          sqlite_insertnewdev(u64toStr(new_device_LongAddr), NewDevName, u16toStr(new_device_ShortAddr));
        }
        else
        {
          NewDevComplete = "!!!!!!Add device fail, please try again!!!!!!";
          webSocket.broadcastTXT(NewDevComplete);
        }
        Serial.println(NewDevComplete);

        connectGood = true; EpResponse = false; BindResponse = false; bindEp = 0; DnResponse = false; ClResponse = false; NewDevName = ""; //needBind = false;
        memset(rxMessageData_newDevice, 0, sizeof(rxMessageData_newDevice));
        memset(ClDataNewDevice, 0, sizeof(ClDataNewDevice));
      }
      else {
        Serial.println("Device in base");
      }
    }
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskDelDevice(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    if(DelCall){
      sqliteDeleteDevice(u64toStr(deletedDevLongAddr));
      Serial.println("Delete device from database");
      DelCall = false;
    }
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}
