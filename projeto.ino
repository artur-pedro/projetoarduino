#define TRUE 1
#define FALSE 0
#include <SoftwareSerial.h>
#include <DHT11.h>
#include <string.h>
#include <stdlib.h>
#include <IRremote.h>
#include "elbrus_ac.h"

SoftwareSerial HM10(10, 11); // RX, TX
IRsend irsend; // cria uma instancia do da classe IRsend e passa o pino 3 como padrão
DHT dht(DHTPIN, DHTTYPE);

float temperatura = 0;
float umidade = 0;
int leituraSensor = 0;
int sensorGas = A0;
int valorSeguro = 80; 
String comandoBLE = " ";
int comandoCompleto = FALSE;
int comandoTemperatura;


// Comando para desligar o ar
const uint16_t ACC_OF[] = {3160, 1540, 528, 1056, 528, 1056, 556, 264, 528, 292, 604, 220, 552, 
                           1060, 552, 264, 528, 292, 580, 1004, 552, 1036, 552, 292, 556, 1004, 
                           580, 264, 532, 288, 556, 1028, 532, 1056, 552, 268, 580, 1004, 552, 
                           1036, 552, 268, 580, 264, 552, 1036, 576, 216, 552, 316, 532, 1032, 
                           576, 268, 528, 292, 552, 268, 552, 292, 528, 292, 548, 272, 576, 268, 
                           532, 288, 552, 292, 528, 292, 528, 292, 552, 292, 532, 288, 528, 292, 
                           576, 268, 528, 292, 552, 268, 556, 288, 532, 288, 552, 268, 580, 1008, 
                           576, 268, 556, 264, 528, 1056, 528, 1056, 556, 268, 576, 268, 552, 264, 
                           556, 268, 552, 292, 528, 292, 524, 1088, 500, 292, 580, 264, 556, 1032, 
                           552, 264, 556, 264, 584, 224, 568, 292, 548, 272, 552, 292, 552, 268, 
                           528, 292, 552, 292, 528, 292, 528, 292, 552, 292, 528, 292, 552, 268,
                           556, 288, 532, 288, 552, 268, 556, 288, 528, 292, 552, 292, 532, 288,
                           528, 292, 556, 292, 528, 288, 528, 292, 556, 292, 528, 292, 528, 292, 
                           576, 268, 528, 292, 548, 272, 576, 268, 528, 292, 548, 272, 552, 292, 
                           528, 292, 576, 268, 528, 292, 552, 268, 552, 292, 528, 296, 524, 292, 
                           552, 292, 528, 292, 528, 1088, 496, 296, 576, 268, 528, 288, 528, 296, 
                           552, 292, 528, 1032, 552, 292, 552};





void setup() {

  Serial.begin(9600);
  delay(2000);
  Serial.println("Iniciando Monitoramento do Ambiente");
  Serial.println(" ");

  HM10.begin(9600); // Inicializa a comunicação com o módulo HM-10
  dht.begin(); // Inicializa sensor de temperatura e umidade
  pinMode(sensorGas, INPUT); //Detector de fumaça

 
}

void loop() {
  
  lerTemperatura();
  lerUmidade();
  lerSensorGas();
  Serial.println("");
  recebeComando();
  delay(2000);

  if(comandoCompleto){
    Serial.println("O COMANDO ESTA COMPLETO");
    Serial.print("Sinal de temperatura alterado para: ");
    Serial.print(comandoTemperatura);
    Serial.print(" °C ");
    comandoCompleto = FALSE;
    enviaComandoTemperatura(comandoTemperatura);
  }
  
}



void enviaComandoTemperatura(int comandoTemperatura){ // recebe o valor inteiro da temperatura via bluetooth da função recebeComando
  int khz = 38; // define a frequência de emissão
  byte temperatureBuff[MESSAGE_SIZE];  
  getTemperatureRepresentation(temperatureBuff, MESSAGE_SIZE, comandoTemperatura); // pega a representação do array da temperatura em bytes
  sendRawByBit(irsend, temperatureBuff, khz); // manda o array  de temperatura
}



void lerTemperatura(){ 
    temperatura = dht.readTemperature(); 
    if (isnan(temperatura)){
      Serial.println("Erro ao ler dados do sensor DHT!");
      }
    HM10.print(temperatura);
    HM10.print(";");
    Serial.print(temperatura);
    Serial.print(";");
}



void lerUmidade(){
    umidade = dht.readHumidity();
    if (isnan(umidade)){
      Serial.println("Erro ao ler dados do sensor DHT!");
      }
    HM10.print(umidade);
    HM10.print(";");
    Serial.print(umidade);
    Serial.print(";");

}

