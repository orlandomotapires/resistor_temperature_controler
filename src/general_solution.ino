#include <OneWire.h>
#include <DallasTemperature.h>

// Pins
#define FAN_PWM_OUTPUT_PIN 11  // Pin where the PWM signal for the fan will be sent
#define FAN_TACH_INPUT_PIN 3  // Tachometer pin
#define TEMP_SENSOR_PIN 8  // Temperature sensor pin
#define BUTTON_PIN 2  // Button pin
#define MOSFET_PWM_PIN 5 // Mosfet PWM pin

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(TEMP_SENSOR_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Global variables
unsigned long lastTime = 0;
unsigned long lastPulseCount = 0;
unsigned long rpm = 0;
volatile int controlMode = 0;  // Initial control mode
float desiredTemperature = 28.0;  // Initial desired temperature
unsigned long lastDebounceTime = 0; // For button debounce
const unsigned long debounceDelay = 50; // Debounce time in milliseconds
int pwmValue = 250;
volatile unsigned long lastPulseTime = 0;

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

  // Configure button pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
}

void loop() {
  checkSerialInput();
  float temperatureC = getTemperature();
  int pwmValueFan = calculatePwmValueFan(temperatureC);
  int pwmValueMosfet = calculatePwmValueMosfet(temperatureC);

  controlModeHandler(pwmValueFan, pwmValueMosfet);

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
  Serial.print(28.5);
  Serial.print(",");
  Serial.print(rpm);
  Serial.print(",");
  Serial.print(26);
  Serial.println(",");

  Serial.println();
  Serial.print("Current Mode: ");
  Serial.println(controlMode);

  Serial.print("PWM Fan: ");
  Serial.println(pwmValueFan);

  Serial.print("PWM Mosfet: ");
  Serial.println(pwmValueMosfet);

  Serial.print("Estimated RPM: ");
  Serial.println(map(pwmValue, 0, 255, 0, 2700));

  Serial.print("Sensor Temperature: ");
  Serial.println(temperatureC);

  delay(1);
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

void checkSerialInput() {
  if (Serial.available() > 0) {
    int newValue = Serial.parseFloat();
    if (newValue > 0 && newValue <= 255) {
      pwmValue = newValue;
      Serial.print("New PWM value: ");
      Serial.println(pwmValue);
    } else {
      Serial.println("Invalid value for fan PWM.");
    }
  }
}

float getTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

int calculatePwmValueMosfet(float temperatureC) {
  float tempDifference = desiredTemperature - temperatureC;
  return constrain(map(tempDifference * 10, -100, 100, 0, 255), 0, 255);
}

int calculatePwmValueFan(float temperatureC) {
  float tempDifference = temperatureC - desiredTemperature;
  return constrain(map(tempDifference * 10, -100, 100, 0, 255), 0, 255);
}

void controlModeHandler(int pwmValueFan, int pwmValueMosfet) {
  switch (controlMode) {
    case 0: // Manually vary the fan speed, ignoring the resistor
      analogWrite(FAN_PWM_OUTPUT_PIN, pwmValue);
      break;

    case 1: // Turn off the fan and vary the voltage delivered to the resistor
      pwmValue = 0;
      analogWrite(MOSFET_PWM_PIN, pwmValueMosfet);
      break;

    case 2: // Maintain constant voltage and vary the fan rotation speed
      analogWrite(MOSFET_PWM_PIN, 255);
      analogWrite(FAN_PWM_OUTPUT_PIN, 255);
      break;
  }
}

void buttonISR() {
  unsigned long currentTime = millis();
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    controlMode = (controlMode + 1) % 3;
    Serial.print("Control mode changed to: ");
    Serial.println(controlMode);
    lastDebounceTime = currentTime;
  }
}
