const int interrupt0 = 2;
const int num_of_pins = 5;
const int photo_relay = 12;
const int button = 11;
const int thermostat_sensor = 10;
const int voltage_sensor = 9;
const int current_sensor = 8;
uint8_t sensorsArray[num_of_pins] = { current_sensor, voltage_sensor, thermostat_sensor, button, photo_relay};
char stateArrayValues[6];
volatile int shutdown_button = LOW;
int controller;

void setup() {
  pinMode(photo_relay, OUTPUT);
  pinMode(button, OUTPUT);
  pinMode(thermostat_sensor, OUTPUT);
  pinMode(voltage_sensor, OUTPUT);
  pinMode(current_sensor, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt0), send_data, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0)
  {
     Serial.readBytes(stateArrayValues, 6);
     if(stateArrayValues[0] == '1')
        controller = 1;
     else
        controller = 0;
     
     if(controller == 0)
     {
        digitalWrite(current_sensor, stateArrayValues[1]);
        digitalWrite(voltage_sensor, stateArrayValues[2]);
        digitalWrite(thermostat_sensor, stateArrayValues[3]);
        digitalWrite(button, stateArrayValues[4]);
        digitalWrite(photo_relay, stateArrayValues[5]);
     }
     if(controller == 1)
     {
        digitalWrite(A0, stateArrayValues[1]);
        digitalWrite(A1, stateArrayValues[2]);
        digitalWrite(A2, stateArrayValues[3]);
        digitalWrite(A3, stateArrayValues[4]);
        digitalWrite(A4, stateArrayValues[5]);
     }
  }
  
  digitalWrite(13, shutdown_button);
  Serial.write(shutdown_button);
}

void send_data() {
  shutdown_button = !shutdown_button;
  digitalWrite(13, shutdown_button);
}
