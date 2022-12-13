const int interrupt0 = 2; //define interrupt pin to 2
const int interrupt1 = 3; //define interrupt pin to 3
const int led = 13;
const int photo_relay = 12;
const int button = 11;
const int thermostat_sensor = 10;
const int voltage_sensor = 9;
const int current_sensor = 8;
const int num_of_pins = 5;
uint8_t sensorsArray[num_of_pins] = { current_sensor, voltage_sensor, thermostat_sensor, button, photo_relay};
char stateArrayValues[6] = { '2', LOW, LOW, LOW, LOW, LOW };
const int preLoaderTimeValue = 34285;
volatile int ledValue = HIGH;
char receivedData = LOW;

void setup() {
   pinMode(led, OUTPUT);
   pinMode(photo_relay, INPUT);
   pinMode(button, INPUT);
   pinMode(thermostat_sensor, INPUT);
   pinMode(voltage_sensor, INPUT);
   pinMode(current_sensor, INPUT);
   noInterrupts();                       // disable all interrupts
   TCCR1A = 0;                           // Timer 1 initialization
   TCCR1B = 0;                           // Timer 1 initialization
   TCNT1 = preLoaderTimeValue;           // preload timer
   TCCR1B |= (1 << CS10)|(1 << CS12);    // 1024 prescaler 
   TIMSK1 |= (1 << TOIE1);               // enable timer overflow interrupt ISR
   interrupts();                         // enable all interrupts
   attachInterrupt(digitalPinToInterrupt(interrupt1), switch_led, CHANGE);
   attachInterrupt(digitalPinToInterrupt(interrupt0), switch_led, CHANGE);
   Serial.begin(9600);
}

void loop() {
    if(Serial.available() > 0) {
      receivedData = Serial.read();
      if(receivedData == LOW && !any_sensor_is_set())
        ledValue = HIGH;
      if(receivedData == HIGH)
        ledValue = LOW;
    }
    digitalWrite(led, ledValue);
    for(uint8_t i=1; i<6; i++)
      stateArrayValues[i] = digitalRead(sensorsArray[i]);
}

void switch_led() {
   if(any_sensor_is_set())
      ledValue = LOW;
   else if(ledValue == LOW && receivedData == LOW)
      ledValue = HIGH;
}

bool any_sensor_is_set(){
  return digitalRead(button) == HIGH || digitalRead(photo_relay) == HIGH
   || digitalRead(thermostat_sensor) == HIGH || digitalRead(voltage_sensor) == HIGH
   || digitalRead(current_sensor) == HIGH;
}

ISR(TIMER1_OVF_vect){
  TCNT1 = preLoaderTimeValue;
  send_data();
}

void send_data() {
  Serial.write(stateArrayValues, 6);
}
