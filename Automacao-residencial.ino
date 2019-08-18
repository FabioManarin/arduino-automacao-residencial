#include <Ultrasonic.h>
#include <Servo.h>
#include <Thermistor.h>

#define pino_trigger 4
#define pino_echo 5
#define tempo 10

const int pinoServo1 = 13;
const int pinoServo2 = 12;
int frequencia = 0;
int LM35 = A5;
float temperatura = 0;
char buffer[67];
byte Pino07 = 7;
byte Pino08 = 8;
byte Pino09falante = 9;
const int sensorPin = 0;
String sequencia;

int ldrPin = 0;
int ldrValor = 0;

Ultrasonic ultrasonic(pino_trigger, pino_echo);

Servo servo1;
Servo servo2;
int pos;
  
void setup() 
{
  Serial.begin(9600); 
  Serial.flush();
  pinMode(Pino07, OUTPUT);
  pinMode(Pino08, OUTPUT);
  servo1.attach(pinoServo1);
  servo2.attach(pinoServo2);
//  servo1.write(10);
//  servo2.write(10);
  pinMode(Pino09falante, OUTPUT);
}

void loop()
{
//  ldrValor = analogRead(ldrPin);
//
//  if (ldrValor>= 800)
//  {
//    digitalWrite(Pino08, HIGH);
//    digitalWrite(Pino07, HIGH);
//  } else
//  {
//    digitalWrite(Pino08,LOW);
//    digitalWrite(Pino07,LOW);
//  }
  
  if (Serial.available() > 0) {
    sequencia = Serial.readString();
  }
  if (sequencia != "") {
    selectOperation(sequencia);
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
      setPortao(data);
      break;
    case 'A':
      setIluminacaoAutomatica(data);
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

void setPortao(String data)
{
  if (data == "P1") 
  {
    for(pos = 0; pos < 180; pos++){
      servo1.write(pos);
      servo2.write(pos);
      sequencia = "";
    }
  }
  if (data == "P0")
  {
    for(pos = 180; pos >= 0; pos--){
      servo1.write(pos);
      servo2.write(pos);
      sequencia = ""; 
    }
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

void setLampadaQuarto(String data)
{
  if (data == "LQ1")
  {
    digitalWrite(Pino07, HIGH);
    //Serial.println("Acesa");
  } else if (data == "LQ0")
  {
    digitalWrite(Pino07, LOW);
    //Serial.println("Apagada");
  }
}

void setLampadaSuite(String data){
  if (data == "LS1")
  {
    digitalWrite(Pino08, HIGH);
    //Serial.println("Acesa 1");
  } else if (data == "LS0")
  {
    digitalWrite(Pino08, LOW);
    //Serial.println("Apagada 1");
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
    //Serial.println("Liga sensor");
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
    //Serial.println("desliga sensor");
    sequencia = "";
  }
}

void setPanico(String data)
{
  if (data == "SP1")
  {
    //Serial.println("Liga auto falante");
    autoFalante();
  }
}

void temperaturaAtual() {
//  //temperatura = (analogRead(sensorPin) * 0.0048828125 * 100);
//  temperatura = (float(analogRead(sensorPin))*5/(1023))/0.01;
//  //   temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
//  temperatura = analogRead(sensorPin);
//  Serial.print("Temperatura = ");
//  Serial.print(temperatura);
//  Serial.println(" C");
//  delay(2000);
  // temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
  temperatura = float(analogRead(LM35));
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  delay(2000);
}
