#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

#define SS_PIN D4
#define RST_PIN D2
WiFiClient wifiClient;
SoftwareSerial mySerial2(D0, D1);
const int buzzer = D8;
int datastatus = 0;
int statusrfid;


const char *ssid = "arjeyeyey"; //WIFI NAME OR HOTSPOT
const char *password = "Tmp12345*"; //WIFI PASSWORD POR MOBILE HOTSPOT PASSWORD

MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup()
{
  delay(1000);
  Serial.begin(9600);
  mySerial2.begin(9600);
  pinMode(buzzer, OUTPUT);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop()
{
    if ( mfrc522.PICC_IsNewCardPresent())
    {
      if ( mfrc522.PICC_ReadCardSerial())
      {
        long code = 0;
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
          code = ((code + mfrc522.uid.uidByte[i]) * 10);
          Serial.println(code);
        }
        tone(buzzer, 1000);
        delay(200);
        noTone(buzzer);
        delay(200);
        mySerial2.println("/open");
        sendRfidLog(code);
      }
    }
    delay (2000);
}

void sendRfidLog(long cardid)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String postData = "cardid=" + String(cardid) + "&action=insertRecord";
    http.begin(wifiClient, "http://192.168.43.111/RFIDandPHP/process.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(postData);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    http.end();
  }
}
