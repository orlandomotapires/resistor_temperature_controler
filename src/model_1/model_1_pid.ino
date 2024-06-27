// Pins
#define FAN_PWM_OUTPUT_PIN 3   // Pin where PWM signal for fan will be sent
#define FAN_TACH_INPUT_PIN 2   // Pin for fan tachometer

// Variables for PID control
double Setpoint, Input, Output;
double errors[3] = {0, 0, 0};   // Stores the last three errors
double outputs[3] = {0, 0, 0};  // Stores the last three outputs
int pwmValue = 250;

unsigned long lastTime;
double sampleTime = 100.0;  // Sampling time in ms

// Total capture time in milliseconds (800 seconds)
const unsigned long captureDuration = 800000;

void setup() {
  // Initialize serial communication for monitoring
  Serial.begin(9600);
  
  // Define the desired setpoint
  Setpoint = 2500;  // For example, 100 degrees Celsius
  
  lastTime = millis();
  
  // Configure PWM output pin for the fan
  pinMode(FAN_PWM_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // Convert sensor value to a measurement unit
  // Assuming sensor returns 0-1023 for 0-100 degrees Celsius
  Input = map(constrain(Output, 0, 255), 0, 255, 0, 2700);
  
  // Calculate elapsed time
  unsigned long now = millis();
  double timeChange = (double)(now - lastTime);
  
  if (timeChange >= sampleTime) {
    // Calculate error
    double error = Setpoint - Input;
    
    // Update error and output arrays
    errors[2] = errors[1];
    errors[1] = errors[0];
    errors[0] = error;
    
    outputs[2] = outputs[1];
    outputs[1] = outputs[0];
    
    // Apply the discretized control equation
    Output = outputs[2] - 99.84 * errors[2] + 190.5 * errors[1] - 90.68 * errors[0];
    
    // Update the output array
    outputs[0] = Output;
    
    // Control the actuator (fan)
    analogWrite(FAN_PWM_OUTPUT_PIN, constrain(Output, 0, 255));
    
    // Update last time for the next iteration
    lastTime = now;
    
    // Send data to Serial Plotter
    Serial.print(now / 1000.0);  // Time in seconds
    Serial.print(","); 
    Serial.print(Input);  // Sensor input
    Serial.print(",");
    Serial.println(Output);  // PID output
    
    delay(500);
  }
  
  // Small delay for stability
  delay(100);
  
  // Check if total capture time has been reached
  if (now >= captureDuration) {
    while (true) {
      // End of capture, stop loop
    }
  }
}
