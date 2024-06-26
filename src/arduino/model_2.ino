#include <OneWire.h>
#include <DallasTemperature.h>

// Pins
#define FAN_PWM_OUTPUT_PIN 11  // Pin where the PWM signal for the fan will be sent
#define FAN_TACH_INPUT_PIN 3  // Tachometer pin
#define TEMP_SENSOR_PIN 8  // Temperature sensor pin
#define MOSFET_PWM_PIN 5 // Mosfet PWM pin

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(TEMP_SENSOR_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Global variables
unsigned long lastTime = 0;
unsigned long lastPulseCount = 0;
unsigned long rpm = 0;
float desiredTemperature = 28.0;  // Initial desired temperature
int pwmValue = 250;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the temperature sensor library
  sensors.begin();
  
  // Configure PWM output pin for the fan
  pinMode(FAN_PWM_OUTPUT_PIN, OUTPUT);

  // Configure mosfet PWM pin as output
  pinMode(MOSFET_PWM_PIN, OUTPUT);

  // Configure fan tachometer pin
  pinMode(FAN_TACH_INPUT_PIN, INPUT_PULLUP);

  // Set MOSFET to provide constant power
  analogWrite(MOSFET_PWM_PIN, 255);
}

void loop() {
  float temperatureC = getTemperature();
  int pwmValueFan = calculatePwmValueFan(temperatureC);

  analogWrite(FAN_PWM_OUTPUT_PIN, pwmValueFan);

  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastTime;

  if (deltaTime >= 1000) { // Calculate RPM every second
      unsigned long currentPulseCount = pulseCount();
      rpm = (currentPulseCount - lastPulseCount) * 60; // Calculate RPM based on received pulses
      lastPulseCount = currentPulseCount; // Update pulse count
      lastTime = currentTime; // Update reference time

      Serial.print("RPM: ");
      Serial.println(rpm);
  }

  // Send data to Serial Plotter
  Serial.print("PWM Fan: ");
  Serial.println(pwmValueFan);

  Serial.print("Sensor Temperature: ");
  Serial.println(temperatureC);

  delay(1000);
}

unsigned long pulseCount() {
   static unsigned long lastPulseCount = 0;
   static bool lastState = LOW; // Pin state in the last cycle
   unsigned long count = lastPulseCount;

   // Check for a transition from LOW to HIGH (pulse)
   if (digitalRead(FAN_TACH_INPUT_PIN) == HIGH && lastState == LOW) {
       count++;
   }
   lastState = digitalRead(FAN_TACH_INPUT_PIN); // Update last pin state
   lastPulseCount = count; // Update pulse count

   return count;
}

float getTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

int calculatePwmValueFan(float temperatureC) {
  float tempDifference = temperatureC - desiredTemperature;
  return constrain(map(tempDifference * 10, -100, 100, 0, 255), 0, 255);
}