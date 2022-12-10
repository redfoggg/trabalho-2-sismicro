const int interrupt0 = 2;
const int num_of_pins = 5;
const int photo_relay = 12;
const int button = 11;
const int thermostat_sensor = 10;
const int voltage_sensor = 9;
const int current_sensor = 8;
uint8_t sensorsArray[num_of_pins] = { current_sensor, voltage_sensor, thermostat_sensor, button, photo_relay};
char stateArrayValues[num_of_pins];
volatile int shutdown_button = LOW;


void setup() {
  pinMode(photo_relay, OUTPUT);
  pinMode(button, OUTPUT);
  pinMode(thermostat_sensor, OUTPUT);
  pinMode(voltage_sensor, OUTPUT);
  pinMode(current_sensor, OUTPUT);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt0), send_data, CHANGE);
  Serial.begin(9600);
}

void loop() {
  Serial.readBytes(stateArrayValues, num_of_pins);
  digitalWrite(current_sensor, stateArrayValues[0]);
  digitalWrite(voltage_sensor, stateArrayValues[1]);
  digitalWrite(thermostat_sensor, stateArrayValues[2]);
  digitalWrite(button, stateArrayValues[3]);
  digitalWrite(photo_relay, stateArrayValues[4]);
  digitalWrite(13, shutdown_button);
  digitalWrite(14, HIGH);
  Serial.write(shutdown_button);
}

void send_data() {
  shutdown_button = !shutdown_button;
  digitalWrite(13, shutdown_button);
}
