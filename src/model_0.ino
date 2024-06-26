// Pins
#define FAN_PWM_OUTPUT_PIN 3  // Pin where the PWM signal for the fan will be sent
#define FAN_TACH_INPUT_PIN 2  // Tachometer pin

int pwmValue = 250;
volatile unsigned long pulseCount = 0; // Pulse counter
volatile unsigned long lastPulseTime = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Configure PWM output pin for the fan
  pinMode(FAN_PWM_OUTPUT_PIN, OUTPUT);

  // Configure fan tachometer pin
  pinMode(FAN_TACH_INPUT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FAN_TACH_INPUT_PIN), countPulse, RISING);
}

void loop() {
  checkSerialInput();

  delay(500);

  unsigned long rpm = pulseCount * 11;

  Serial.print("Measured RPM: ");
  Serial.println(rpm);
  
  Serial.print("Pulse count: ");
  Serial.println(pulseCount);

  Serial.print("PWM Value: ");
  Serial.println(pwmValue);

  Serial.print("Estimated RPM: ");
  Serial.println(map(pwmValue, 0, 255, 0, 2700));

  pulseCount = 0;

  analogWrite(FAN_PWM_OUTPUT_PIN, pwmValue);
}

void countPulse() {
  pulseCount++;
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
