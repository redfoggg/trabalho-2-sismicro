#include <Wire.h> 

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
int x = 1;

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
  //attachInterrupt(digitalPinToInterrupt(interrupt0), send_data, CHANGE);
  Serial.begin(9600);
  escutarS1();
}

void loop() {
  digitalWrite(13, shutdown_button);
  
  delay(300);
  if(x == 0) { escutarS1(); }
  if(x == 1) { escutarS2(); }
}

//Função auxiliar para processar os dados recebidos do Escravo
void receiveEvent(int howMany)
{
  Serial.println("teste");
  if(x == 0){ x = 1;}
  else {x = 0;}

  x=1; // teste
  while (1 < Wire.available()) //Loop para receber toda String de dados
  {
    char c = Wire.read();      //Recebe um byte caractere
    Serial.print(c);           //Imprime na Serial
  }
  int x = Wire.read();         //recebe um byte do tipo inteiro
  Serial.println(x);           //Imprime na Serial
  
}

void escutarS1(){
  Wire.begin(16);                //Barramento I2C do endereço 16
  Wire.onReceive(receiveEvent);  //Recepção de dados (chama função auxiliar)
}

void escutarS2(){
  Wire.begin(15);                //Barramento I2C do endereço 15
  Wire.onReceive(receiveEvent);  //Recepção de dados (chama função auxiliar)
}
