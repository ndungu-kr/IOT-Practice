// This code measures the temp and humidity from the sensors attached to an 
// arduino and if above a threshold, activates a buzzer and then sends it to
// a raspberry pi via bluetooth to be displayed on an lcd screen and
// transmitted to thingsboard cloud.


// Include the DHT library so we can incorporate different sensors.
#include <DHT.h>
// Include the SoftwareSerial library so we can use digital pins for Bluetooth.
#include <SoftwareSerial.h>

#define DHT_PIN 4
#define DHT_TYPE DHT11
#define BUZZER_PIN 6
#define RxD 8
#define TxD 9            

// Create a DHT object using the pin and type we defined
DHT dht(DHT_PIN, DHT_TYPE);


SoftwareSerial blueToothSerial(RxD, TxD);

void setup()
{
  blueToothSerial.begin(9600);
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (humidity >= 80) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  blueToothSerial.print("Temp: ");
  blueToothSerial.print(temperature);
  blueToothSerial.print("C  Hum: ");
  blueToothSerial.print(humidity);
  blueToothSerial.println("%");

  delay(2000);
}