/* Names: B. Vukajlovic; M. Bellari; I. Ouhamcha
 * UvAnetIDs: 15225054; 14675218; 13690930
 * Study: BSc Informatica
 *
 * This file is used to test sensor data of multiple connected ultrasonic
 * sensors to an Arduino. Prints the distance of each sensor.
 *
 * Reference: https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
 */

const int trigPin1 = 9;
const int echoPin1 = 10;
const int trigPin2 = 11;
const int echoPin2 = 12;
const int trigPin3 = 6;
const int echoPin3 = 7;

float duration, distance;

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  Serial.begin(9600);
}

float detect_distance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343) / 2;
  return distance;
}

void loop() {
  Serial.print("Distance1: ");
  Serial.print(detect_distance(trigPin1, echoPin1));
  delay(100);
  Serial.print(" Distance2: ");
  Serial.print(detect_distance(trigPin2, echoPin2));
  delay(100);
  Serial.print(" Distance3: ");
  Serial.println(detect_distance(trigPin3, echoPin3));
  delay(100);
}
