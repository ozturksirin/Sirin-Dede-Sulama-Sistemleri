#include <SoftwareSerial.h>
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define WIFI_SSID "Mese5"         // wifi ağ adını " " içine yazınız.
#define WIFI_PASSWORD "mese1597"  // wifi şifresini " " içine yazınız.

// #define API_KEY "YourFirebaseApiKey"  
// #define DATABASE_URL "YourDatabaseUrl"

#define API_KEY "YourFirebaseApiKey"  // firebase api keyi " " içine yazınız.
#define DATABASE_URL "YourDatabaseUrl"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
///////////////////////////////


int nemSensorPin = A0;
int nemVeri;
int yuzdeNemVeri;

int suMotorRole = D1;

int suYuzdePin = D2;
int suYuzde;

int sinirDeger = 1000;


bool boolValue;
bool DamlaBool;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Ağa bağlanıyor");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Bağlandı. IP Adresi: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.signUp(&config, &auth, "", "");
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);


  ///////////////
  pinMode(nemSensorPin, INPUT);
  pinMode(suMotorRole, OUTPUT);
  pinMode(suYuzdePin, INPUT);

  digitalWrite(suMotorRole, HIGH);
}

void loop() {
  Firebase.RTDB.setInt(&fbdo, "yuzdeNemVeri", yuzdeNemVeri);
  // Firebase.RTDB.setInt(&fbdo, "nemVeri", nemVeri);

  if (Firebase.RTDB.getBool(&fbdo, "/useStateWater")) {
    if (fbdo.dataType() == "boolean") {
      boolValue = fbdo.boolData();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }
  if (Firebase.RTDB.getBool(&fbdo, "/Sulama_Damla")) {
    if (fbdo.dataType() == "boolean") {
      DamlaBool = fbdo.boolData();
    }
  } else {
    Serial.println(fbdo.errorReason());
  }


  ///////////7
  nemVeri = analogRead(nemSensorPin);
  Serial.println("Toprak Nem Orani=");
  Serial.println(nemVeri);
  // Serial.println("Yuzde Nem Orani=");
  // Serial.println(yuzdeNemVeri);
  delay(1000);
  // suYuzde = analogRead(suYuzdePin);
  // Serial.println("Su Yuzde Orani=");
  // Serial.println(suYuzde);
  // delay(1000);

  if (nemVeri >= sinirDeger) {
    digitalWrite(suMotorRole, LOW);
    delay(2000);
    digitalWrite(suMotorRole, HIGH);
    delay(5000);

  } else {
    digitalWrite(suMotorRole, HIGH);
  }


  if (DamlaBool == 1) {
    digitalWrite(suMotorRole, LOW);
    delay(350);
    digitalWrite(suMotorRole, HIGH);
    delay(350);

  } else {
    digitalWrite(suMotorRole, HIGH);
  }


  if (boolValue == 1) {
    digitalWrite(suMotorRole, LOW);

  } else {
    digitalWrite(suMotorRole, HIGH);
  }

  if (nemVeri > 950) {
    yuzdeNemVeri = 10;
  } else if (nemVeri > 850) {
    yuzdeNemVeri = 20;
  } else if (nemVeri > 750) {
    yuzdeNemVeri = 30;
  } else if (nemVeri > 650) {
    yuzdeNemVeri = 40;
  } else if (nemVeri > 550) {
    yuzdeNemVeri = 50;
  } else if (nemVeri > 450) {
    yuzdeNemVeri = 60;
  } else if (nemVeri > 350) {
    yuzdeNemVeri = 70;
  } else if (nemVeri > 250) {
    yuzdeNemVeri = 80;
  } else if (nemVeri > 150) {
    yuzdeNemVeri = 90;
  } else if (nemVeri > 50) {
    yuzdeNemVeri = 100;
  } else {
    yuzdeNemVeri = 101;
  }
}
