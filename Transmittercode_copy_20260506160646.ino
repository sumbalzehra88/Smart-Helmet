#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver(2000,11,12);

// Sensors
int alcoholSensor = A0;
int irSensor = A1;

int threshold = 500;

void setup()
{
  rf_driver.init();

  pinMode(irSensor, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop()
{
  int alcoholValue = analogRead(alcoholSensor);
  int irState = digitalRead(irSensor);

  int alcoholDetected = 0;
  int obstacleDetected = 0;

  // Alcohol detection
  if (alcoholValue > threshold)
  {
    alcoholDetected = 1;
  }

  // IR LOW = obstacle
  if (irState == LOW)
  {
    obstacleDetected = 1;
  }

  // Create data
  String data = String(alcoholDetected) + "," + String(obstacleDetected);

  char msg[20];
  data.toCharArray(msg, 20);

  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();

  Serial.println(data);

  delay(200);
}