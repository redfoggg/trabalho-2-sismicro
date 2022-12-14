#include <Wire.h>

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
char stateArrayValues[num_of_pins] = { LOW, LOW, LOW, LOW, LOW };
const int preLoaderTimeValue = 34285;
volatile int ledValue = HIGH;
char receivedData = LOW;
float voltage_mv = 0.0F;
char voltBuff[7];

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
   Wire.begin(2);
   Wire.onReceive(receiveEvent);  //Recepção de dados (chama função auxiliar)
   Serial.begin(9600);
}

void loop() {
    digitalWrite(led, ledValue);
    
    for(uint8_t i=0; i<num_of_pins; i++)
    {
      stateArrayValues[i] = digitalRead(sensorsArray[i]);
    }
      
}

void switch_led() {
   if(any_sensor_is_set() || receivedData == HIGH)
      ledValue = LOW;
   else if(ledValue == LOW && receivedData == LOW)
      ledValue = HIGH;
}

bool any_sensor_is_set(){
  return digitalRead(button) == HIGH || digitalRead(photo_relay) == HIGH
   || digitalRead(thermostat_sensor) == HIGH || digitalRead(voltage_sensor) == HIGH
   || digitalRead(current_sensor) == HIGH;
}

//Função auxiliar para processar os dados recebidos do Escravo
void receiveEvent()
{
  receivedData = Wire.read();         //recebe um byte do tipo char
  switch_led();
}
void send_data(char b1) {
  //uint16_t adc_value = 0;
  //adc_value = 3.2F;
  //voltage_mv = adc_value * 5/1023;
  //dtostrf(adc_value,7,4, voltBuff);
  Wire.beginTransmission(1);       // Transmite para o mestre
  if(b1 == HIGH) {Wire.write("1");}  //Caso a ultima interrupcao tenha sido para on
  if(b1 == LOW) {Wire.write("0");}  //Caso a ultima interrupcao tenha sido para off
  Wire.endTransmission();           // Para a transmissão

}
ISR(TIMER1_OVF_vect){
  sei();
  TCNT1 = preLoaderTimeValue;
  volatile char b1 = digitalRead(sensorsArray[1]);
  send_data(b1);
}
