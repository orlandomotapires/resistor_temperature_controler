#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1.h>

#define MOSFET 6  // Pino onde o sinal PWM para o ventilador será enviado
#define SENSOR_TEMPERATURA 8  // Pino do tacômetro

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(SENSOR_TEMPERATURA);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);// Pinos

double Setpoint, Input, Output;
int kp = 1000, ki = 20, kd = 6000;

PID myPID(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);

void setup() {
  // Inicialização da comunicação serial para monitoramento
  Serial.begin(9600);
  
  // Define o setpoint desejado
  Setpoint = 2500; // Por exemplo, 100 graus Celsius
  
  // Configurar pino de saída PWM para o ventilador
  pinMode(MOSFET, OUTPUT);

  sensors.begin();
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  Input = obterTemperatura();
  myPID.Compute();
  Serial.println(Output);
}

float obterTemperatura() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}
