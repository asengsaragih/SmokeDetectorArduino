#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

int redLed = D3;
int greenLed = D2;
int buzzer = D1;
int smoke = D0;
// Your threshold value
int sensorThres = 500;

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

#define WIFI_SSID "jelly"
#define WIFI_PASSWORD "punyaolin."
#define FIREBASE_HOST "just-java-ee9d8.firebaseio.com"
#define FIREBASE_AUTH "vfV4rC8tB5Xw3vih5tFwasADZpU8lFK6pM7Bcc9I"

void setup() {
  Serial.begin(115200);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smoke, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
}

void loop() {
  int analogSensor = analogRead(smoke);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    Serial.println ("Status Asap : Berbahaya");
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(buzzer, HIGH);
    if (WiFi.status() == WL_CONNECTED) {
      Firebase.setDouble(firebaseData, "/SmokeDetector/isThere", 1);
    }
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(buzzer, LOW);
    if (WiFi.status() == WL_CONNECTED) {
      Firebase.setDouble(firebaseData, "/SmokeDetector/isThere", 0);
    }
  }
  delay(300);

}
