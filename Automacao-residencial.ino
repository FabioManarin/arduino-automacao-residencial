#include <Ultrasonic.h>
#include <Servo.h>
#include "thermistor.h"

#define pino_trigger 4
#define pino_echo 5
#define tempo 10

int pinNTC = A2;
float temperatura = 0;

THERMISTOR thermistor(pinNTC, 10000, 3950, 10000);

const int pinoServo1 = 11;
const int pinoServo2 = 12;
int frequencia = 0;
char buffer[67];
byte Pino07 = 7;
byte Pino08 = 8;
byte Pino09falante = 9;
const int sensorPin = 0;
String sequencia;
int porta_rele13 = 13;

int ldrPin = 0;
int ldrValor = 0;

Ultrasonic ultrasonic(pino_trigger, pino_echo);

Servo servo1;
Servo servo2;
int pos;
int pos2;
int tempRecebidaUsuario;
  
void setup() 
{
  Serial.begin(9600); 
  Serial.flush();
  pinMode(Pino07, OUTPUT);
  pinMode(Pino08, OUTPUT);
  pinMode(porta_rele13, OUTPUT);
  servo1.attach(pinoServo1);
  servo1.write(0);
  servo2.attach(pinoServo2);
  servo2.write(0);
  pinMode(Pino09falante, OUTPUT);
}

void loop()
{
  informaTemperatura();
  digitalWrite(porta_rele13, HIGH);
  if (Serial.available() > 0) {
    sequencia = Serial.readString();
//    Serial.println(sequencia);
  }
  if (sequencia != "") {
    selectOperation(sequencia);
  }
}

void informaTemperatura()
{
  if (temperatura == 0)
  {
    temperatura = thermistor.read();
    Serial.print(temperatura);
    Serial.print(" C°"); 
  }
}

void selectOperation(String data) 
{
  switch(data[0])
  {
    case 'L':
      setIluminacao(data);
      break;
    case 'S':
      setSensor(data);
      break;
    case 'P':
      setPortao2(data);
      break;
    case 'A':
      setIluminacaoAutomatica(data);
      break;
    case 'V':
      setVentilador(data);
      break;
    case 'X':
      setVentiladorAutomatico(data);
      break;
    case 'T':
      tempEnviada(data);
      break;
  }
}

void setIluminacaoAutomatica(String data)
{
  ldrValor = analogRead(ldrPin);

  if (data == "A1")
  {
    if (ldrValor>= 800)
    {
      digitalWrite(Pino08, HIGH);
      digitalWrite(Pino07, HIGH);
    } else
    {
      digitalWrite(Pino08,LOW);
      digitalWrite(Pino07,LOW);
    }
  } else if (data == "A0")
  {
    sequencia = "";
    digitalWrite(Pino08,LOW);
    digitalWrite(Pino07,LOW);
  }
}

void setPortao2(String data)
{
  if (data == "P1") 
  {
    pos2 = 100;
    for(pos = 0; pos < 100; pos++){
      servo1.write(pos);
      delay(50);
      for(pos2; pos2 >= 0; pos2--){
        servo2.write(pos2);
        delay(50);
        break;
      }
      pos2--;
    }
    sequencia = "";
  }

  if (data == "P0")
  {
    pos2 = 0;
    for(pos = 100; pos >= 0; pos--){
      servo1.write(pos);
      delay(50);
      for(pos2; pos2 < 100; pos2++){
        servo2.write(pos2);
        delay(50);
        break;
      }
      pos2++;
    }
    sequencia = "";
  }
}

void setIluminacao(String data)
{
  setLampadaQuarto(data);
  setLampadaSuite(data);
}

void setSensor(String data)
{
  sensorDistancia(data);
  setPanico(data);
}

void setVentilador(String data)
{
  if (data == "V1")
  {
    digitalWrite(porta_rele13, LOW);
  } else if (data == "V0")
  {
    digitalWrite(porta_rele13, HIGH);
  }
}

void tempEnviada(String data)
{
  // FALTA PEGAR A TEMPERATURA E CONVERTER PARA COMPARAR NA FUNÇÃO ABAIXO
  // Serial.println(strcat(data[1], data[2]));
}

void setVentiladorAutomatico(String data)
{
  if (data == "XA1")
  {
    temperatura = thermistor.read();
//    Serial.println(tempRecebidaUsuario);
    if (temperatura >= 10)
    {
      digitalWrite(porta_rele13, LOW);
    } else
    {
      digitalWrite(porta_rele13, HIGH);
    }
  } else if (data == "XA0")
  {
    sequencia = "";
    digitalWrite(porta_rele13, HIGH);
  }
}

void setLampadaQuarto(String data)
{
  if (data == "LQ1")
  {
    digitalWrite(Pino07, HIGH);
  } else if (data == "LQ0")
  {
    digitalWrite(Pino07, LOW);
  }
}

void setLampadaSuite(String data){
  if (data == "LS1")
  {
    digitalWrite(Pino08, HIGH);
  } else if (data == "LS0")
  {
    digitalWrite(Pino08, LOW);
  }
}

void autoFalante() {
  for (frequencia = 150; frequencia < 1800; frequencia += 1) 
  {
    tone(Pino09falante, frequencia, tempo); 
    delay(1);
  }
  for (frequencia = 1800; frequencia > 150; frequencia -= 1) 
  {
    tone(Pino09falante, frequencia, tempo); 
    delay(1);
  }
}

void sensorDistancia(String data) 
{
  if (data == "S1")
  {
    float cmMsec, inMsec;
    long microsec = ultrasonic.timing();
    cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
    inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
//    Serial.print("Distancia em cm: ");
//    Serial.println(cmMsec);
    delay(1000);
    if (cmMsec < 10) 
    {
      autoFalante();
    }
  } else if (data == "S0")
  {
    sequencia = "";
  }
}

void setPanico(String data)
{
  if (data == "SP1")
  {
    autoFalante();
  }
}

void temperaturaAtual() {
  temperatura = thermistor.read();
  delay(1000);
}
