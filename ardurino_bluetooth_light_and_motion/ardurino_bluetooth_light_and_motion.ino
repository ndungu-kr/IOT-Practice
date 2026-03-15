// 1. Include the SoftwareSerial library so we can use digital pins for Bluetooth.
#include <SoftwareSerial.h>

// 2. Assign pins for Bluetooth, PIR sensor and light sensor.
#define RxD 8                // Arduino RX pin for Bluetooth
#define TxD 9                // Arduino TX pin for Bluetooth
#define PIR_MOTION_SENSOR 2  // PIR sensor input pin
#define LIGHT_SENSOR A3      // Light sensor pin

// 3. Create a Bluetooth serial connection using pins 8 and 9.
SoftwareSerial blueToothSerial(RxD, TxD);

void setup()
{
  // 4. Start the Bluetooth connection at 9600 baud.
  blueToothSerial.begin(9600);

  // 5. Set the PIR sensor pin as an input.
  pinMode(PIR_MOTION_SENSOR, INPUT);
}

void loop()
{
  // 6. Check if the PIR sensor detects motion. If yes, send "Movement" over Bluetooth.
  if (digitalRead(PIR_MOTION_SENSOR)) {
    blueToothSerial.println("Movement");
  }

  // 7. Read the light sensor value (0 to 1023).
  int lightValue = analogRead(LIGHT_SENSOR);

  // 8. If the value is low it is dark, if high it is bright. Send the result over Bluetooth.
  if (lightValue < 200) {
    blueToothSerial.println("Dark");
  } else {
    blueToothSerial.println("Bright");
  }

  // 9. Wait 2 seconds before checking again.
  delay(2000);
}