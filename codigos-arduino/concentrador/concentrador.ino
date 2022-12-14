#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

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
int controller;
byte contador = 0;                    // Ajudar a debugar
volatile float value = 0.0F;
String dataString = "";
LiquidCrystal_I2C lcd (0x27, 16,2);

void setup() {
  attachInterrupt(digitalPinToInterrupt(interrupt0), sendData, CHANGE);
  lcd.init();
  lcd.backlight();

  //Comunicação i2c
  Wire.begin(1);                //Barramento I2C do endereço 16
  Wire.onReceive(receiveEvent);  //Recepção de dados (chama função auxiliar)
  Serial.begin(9600);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print(dataString);
  dataString = "";
  delay(100);
  digitalWrite(13, shutdown_button);
}

//Função auxiliar para processar os dados recebidos do Escravo
void receiveEvent()
{
  while (Wire.available()) //Loop para receber toda String de dados
  {
   char c = Wire.read();      //Recebe um byte caractere
    dataString = dataString + c;
  }
}

void sendData(){
  sei();
  int state_button = digitalRead(interrupt0);

  Wire.beginTransmission(2);              // Inicia transmissão para o escravo 2
  Wire.write(state_button);               // Envia o estado do do botão
  Wire.endTransmission();                 // Finaliza a transmissão
  Wire.beginTransmission(3);              // Inicia transmissão para o escravo 3
  Wire.write(state_button);               // Envia o estado do do botão
  Wire.endTransmission();                 // Finaliza a transmissão
}
