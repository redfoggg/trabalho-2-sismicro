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
char stateArrayValues[6];
volatile int shutdown_button = LOW;
int controller;
byte contador = 0;                    // Ajudar a debugar
volatile float value = 0.0F;
String dataString = "";
LiquidCrystal_I2C lcd (0x27, 16,2);

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
  attachInterrupt(digitalPinToInterrupt(interrupt0), sendData, CHANGE);
  lcd.init();
  lcd.backlight();
  //lcd.setCursor(0,0);
  //lcd.print("triste");

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
void receiveEvent(int howMany)
{
  while (Wire.available()) //Loop para receber toda String de dados
  {
    char c = Wire.read();      //Recebe um byte caractere
    dataString = dataString + c;
  }
  //value = dataString.toFloat();         //recebe um byte do tipo inteiro

  Serial.println(dataString);           //Imprime na Serial
}

void sendData(){
  sei();
  int state_button = digitalRead(interrupt0);

  Wire.beginTransmission(2);              // Inicia transmissão para o escravo 2
  Wire.beginTransmission(3);              // Inicia transmissão para o escravo 3
  Wire.write(state_button);               // Envia o estado do do botão
  Wire.endTransmission();                 // Finaliza a transmissão
}
