//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int led = 2; //led Azul de prueba de conexión

float voltageValue[4] = {0,0,0,0};
char inbyte = 0; //Char para leer el led
 
void setup() {
  // initialise serial communications at 9600 bps:
  Serial.begin(115200);
  
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
 
}
 
void loop() {
  getVoltageValue();
  //when serial values have been received this will be true
  
  if (SerialBT.available() > 0)
  {
    inbyte = SerialBT.read();
    
    if (inbyte == '2')
    {
      digitalWrite(led, LOW); //LED off
      voltageValue[0] = 0;
    }
    if (inbyte == '1')
    {
      digitalWrite(led, HIGH); //LED on
      voltageValue[0] = 1;
    }
  }
  //sendAndroidValues();
  delay(2000); 
}
 
void getVoltageValue()
{
  voltageValue[0] = 1; //led
  voltageValue[1] = 2;
  voltageValue[2] = 3;
  voltageValue[3] = 4;
  
}

//enviar los valores por el dipositivo android por el modulo Bluetooth
void sendAndroidValues()
 {
  Serial.print('#'); //hay que poner # para el comienzo de los datos, así Android sabe que empieza el String de datos
  for(int k=0; k<4; k++)
  {
    Serial.print(voltageValue[k]);
    Serial.print('+'); //separamos los datos con el +, así no es más fácil debuggear la información que enviamos
  }
 Serial.print('~'); //con esto damos a conocer la finalización del String de datos
 Serial.println();
 delay(10);        //agregamos este delay para eliminar tramisiones faltantes
}
