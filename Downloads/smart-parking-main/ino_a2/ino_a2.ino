/* Names: B. Vukajlovic; M. Bellari; I. Ouhamcha
 * UvAnetIDs: 15225054; 14675218; 13690930
 * Study: BSc Informatica
 *
 * This file contains the code for an Arduino(a2) that is used to send a
 * message to a WiFi network when a button is pressed. This message is send to
 * an Arduino(a1) that is connected to the same WiFi network.
 */
#include <WiFiNINA.h>

const char* ssid = "boriss20";
const char* password = "ru6tns3Ybpnp";
const char* arduino1_ip = "192.168.38.100";
const int arduino1_port = 80;

WiFiClient client;

#define ENTRY_BUTTON_PIN 2
#define EXIT_BUTTON_PIN 3

void setup() {
  pinMode(ENTRY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(EXIT_BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (digitalRead(ENTRY_BUTTON_PIN) == LOW) {
    sendCommand("ENTRY_PRESSED");
    delay(500);
  }
  if (digitalRead(EXIT_BUTTON_PIN) == LOW) {
    sendCommand("EXIT_PRESSED");
    delay(500);
  }
}

void sendCommand(const char* command) {
  if (client.connect(arduino1_ip, arduino1_port)) {
    client.println(command);
    client.stop();
  } else {
    Serial.println("Connection to Arduino 1 failed.");
  }
}
