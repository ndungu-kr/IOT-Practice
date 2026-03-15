/*
  Home Intrusion System
  
  The Arduino reads a PIR motion sensor, sound sensor and ultrasonic distance sensor.
  It also monitors a button to toggle the system between armed and disarmed mode.
  All sensor readings and the armed state are sent to a Raspberry Pi via Bluetooth.
  The Raspberry Pi (Node-RED) processes the data and determines if the alarm should
  trigger based on configurable thresholds. If the system is armed and thresholds are
  breached, the Pi sends an ALARM command back to the Arduino which activates the buzzer.

*/

#include <SoftwareSerial.h>
#include <Ultrasonic.h>

#define SOUND_SENSOR A3
#define BUTTON_PIN 4
#define PIR_PIN 2
#define BUZZER_PIN 6
#define RxD 8
#define TxD 9
#define ULTRASONIC_PIN 7

// Create a Bluetooth serial connection using pins 8 and 9
SoftwareSerial blueToothSerial(RxD, TxD);

// Create an ultrasonic object using pin 7
Ultrasonic ultrasonic(ULTRASONIC_PIN);

bool armed = false;

void setup()
{
  blueToothSerial.begin(9600);
  
  // Set sensor and button pins as INPUT, buzzer as OUTPUT
  pinMode(PIR_PIN, INPUT);
  pinMode(SOUND_SENSOR, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  // Toggle armed state when button is pressed for 1 second
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    armed = !armed;
    //  waits until you release the button before continuing, so it can only toggle once per press
    while(digitalRead(BUTTON_PIN) == LOW);
    delay(50);
  }

  // Read all sensor values
  int pirValue = digitalRead(PIR_PIN);
  int soundValue = analogRead(SOUND_SENSOR);
  // function from the Grove library that returns the distance in cm
  long distance = ultrasonic.MeasureInCentimeters();


  // Send all sensor readings and armed state to the Pi in JSON format
  blueToothSerial.print("{\"Armed\":");
  blueToothSerial.print(armed);
  blueToothSerial.print(",\"Motion\":");
  blueToothSerial.print(pirValue);
  blueToothSerial.print(",\"Sound\":");
  blueToothSerial.print(soundValue);
  blueToothSerial.print(",\"Distance\":");
  blueToothSerial.print(distance);
  blueToothSerial.println("}");

  // Listen for commands from the Pi
  if (blueToothSerial.available() > 0) {
    blueToothSerial.setTimeout(50);
    String command = blueToothSerial.readStringUntil('\n');
    Serial.println(command);
    
    if (command == "ALARM") {
      digitalWrite(BUZZER_PIN, HIGH);
    } else if (command == "OFF") {
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  delay(1000);
}