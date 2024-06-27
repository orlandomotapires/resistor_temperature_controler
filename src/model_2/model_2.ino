#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1.h>

// Pins
#define TEMP_SENSOR_PIN 8  // Temperature sensor pin
#define MOSFET_PWM_PIN 5 // Mosfet PWM pin

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(TEMP_SENSOR_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Global variables
float desiredTemperature = 28.0;  // Initial desired temperature
int pwmValue = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the temperature sensor library
  sensors.begin();
  
  // Configure mosfet PWM pin as output
  pinMode(MOSFET_PWM_PIN, OUTPUT);
}

void loop() {
  float temperatureC = getTemperature();
  int pwmValueMosfet = calculatePwmValueMosfet(temperatureC);

  analogWrite(MOSFET_PWM_PIN, pwmValueMosfet);

  // Send data to Serial Plotter
  Serial.print("PWM Mosfet: ");
  Serial.println(pwmValueMosfet);

  Serial.print("Sensor Temperature: ");
  Serial.println(temperatureC);

  delay(1000);
}

float getTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

int calculatePwmValueMosfet(float temperatureC) {
  float tempDifference = desiredTemperature - temperatureC;
  return constrain(map(tempDifference * 10, -100, 100, 0, 255), 0, 255);
}