/* Names: B. Vukajlovic; M. Bellari; I. Ouhamcha
 * UvAnetIDs: 15225054; 14675218; 13690930
 * Study: BSc Informatica
 *
 * This file contains the code for the Arduino(a1) that is used to control
 * ultrasonic sensors and LEDs for a 'Smart Parking' system. When the distance
 * of an ultrasonic sensor is at a certain threshold, the corresponding LED
 * will turn off. This indicates that this parking space is now occupied.
 * The Arduino(a1) is connected to the WiFi network and listens for incoming
 * connections from a Raspberry Pi. The Arduino sends the distance readings
 * from the ultrasonic sensors and the status of the LEDs to the Raspberry Pi.
 * The Arduino(a1) does this after receiving a command from the Raspberry Pi.
 */
#include <WiFiNINA.h>

const char* ssid = "boriss20";
const char* password = "ru6tns3Ybpnp";
String ledStatus1, ledStatus2, ledStatus3;

IPAddress localIp(192, 168, 38, 100);
IPAddress gateway(192, 168, 38, 51);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);
WiFiClient client;

const char* raspberry_pi_ip = "192.168.38.44";
const int raspberry_pi_port = 5000;

const int TRIG_PIN1 = 9;
const int ECHO_PIN1 = 10;
const int GREEN_LED_PIN1 = 4;

const int TRIG_PIN2 = 11;
const int ECHO_PIN2 = 12;
const int GREEN_LED_PIN2 = 3;

const int TRIG_PIN3 = 6;
const int ECHO_PIN3 = 7;
const int GREEN_LED_PIN3 = 2;

const int initialFreeSpaces = 3;
int freeSpaces = initialFreeSpaces;

void setup() {
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  WiFi.config(localIp, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  server.begin();

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(GREEN_LED_PIN1, OUTPUT);
  digitalWrite(GREEN_LED_PIN1, HIGH);

  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(GREEN_LED_PIN2, OUTPUT);
  digitalWrite(GREEN_LED_PIN2, HIGH);

  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);
  pinMode(GREEN_LED_PIN3, OUTPUT);
  digitalWrite(GREEN_LED_PIN3, HIGH);
}

long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration * 0.0343) / 2;
  return distance;
}

void updateLEDs(long distance1, long distance2, long distance3) {
  if (distance1 < 3) {
    digitalWrite(GREEN_LED_PIN1, LOW);
    ledStatus1 = "OFF";
  } else {
    digitalWrite(GREEN_LED_PIN1, HIGH);
    ledStatus1 = "ON";
  }

  if (distance2 < 3) {
    digitalWrite(GREEN_LED_PIN2, LOW);
    ledStatus2 = "OFF";
  } else {
    digitalWrite(GREEN_LED_PIN2, HIGH);
    ledStatus2 = "ON";
  }

  if (distance3 < 3) {
    digitalWrite(GREEN_LED_PIN3, LOW);
    ledStatus3 = "OFF";
  } else {
    digitalWrite(GREEN_LED_PIN3, HIGH);
    ledStatus3 = "ON";
  }
}

void handleClient(WiFiClient client, long distance1, long distance2, long distance3) {
  String command = client.readStringUntil('\n');
  command.trim();
  Serial.println("Received command: " + command);

  if (command == "ENTRY_PRESSED") {
    if (freeSpaces > 0) {
      freeSpaces--;
      Serial.print("FreeSpaces: ");
      Serial.println(freeSpaces);

      sendStatusToRaspberryPi(distance1, distance2, distance3, ledStatus1, ledStatus2, ledStatus3);
    } else {
      Serial.println("ERROR: No Free Spaces");
      sendStatusToRaspberryPi(distance1, distance2, distance3, ledStatus1, ledStatus2, ledStatus3);
      sendErrorToRaspberryPi("No Free Spaces");
    }
  } else if (command == "EXIT_PRESSED") {
    freeSpaces++;
    if (freeSpaces <= initialFreeSpaces) {
      Serial.print("FreeSpaces: ");
      Serial.println(freeSpaces);

      sendStatusToRaspberryPi(distance1, distance2, distance3, ledStatus1, ledStatus2, ledStatus3);
    } else {
      Serial.println("ERROR: Free Spaces Exceeded");
      freeSpaces = initialFreeSpaces;
      sendStatusToRaspberryPi(distance1, distance2, distance3, ledStatus1, ledStatus2, ledStatus3);
      sendErrorToRaspberryPi("Free Spaces Exceeded");
    }
  }
}

void sendStatusToRaspberryPi(long distance1, long distance2, long distance3, String ledStatus1, String ledStatus2, String ledStatus3) {
  if (client.connect(raspberry_pi_ip, raspberry_pi_port)) {
    String postData = "distance1=" + String(distance1) + "&ledStatus1=" + ledStatus1 +
                      "&distance2=" + String(distance2) + "&ledStatus2=" + ledStatus2 +
                      "&distance3=" + String(distance3) + "&ledStatus3=" + ledStatus3 +
                      "&freeSpaces=" + String(freeSpaces);
    client.println("POST /update HTTP/1.1");
    client.println("Host: " + String(raspberry_pi_ip));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
    client.stop();
  } else {
    Serial.println("Connection to Raspberry Pi failed.");
  }
}

void sendErrorToRaspberryPi(String error) {
  if (client.connect(raspberry_pi_ip, raspberry_pi_port)) {
    String postData = "error=" + error;
    client.println("POST /error HTTP/1.1");
    client.println("Host: " + String(raspberry_pi_ip));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
    client.stop();
  } else {
    Serial.println("Connection to Raspberry Pi failed.");
  }
}

void loop() {
  long distance1 = readDistance(TRIG_PIN1, ECHO_PIN1);
  long distance2 = readDistance(TRIG_PIN2, ECHO_PIN2);
  long distance3 = readDistance(TRIG_PIN3, ECHO_PIN3);
  updateLEDs(distance1, distance2, distance3);

  WiFiClient client = server.available();
  if (client) {
    handleClient(client, distance1, distance2, distance3);
    client.stop();
  }

  delay(300);
}