char buffer[67];
byte Pino07 = 7;
  
void setup() {
  Serial.begin(9600);
  Serial.flush();
  pinMode(Pino07, OUTPUT);  
}

void loop(){
  if (Serial.available()>0){
    int index=0;
    delay(100);//deixe o buffer encher
    int numChar = Serial.available();
    if(numChar>65){
      numChar=65;
    }
    while(numChar--){
      buffer[index++] = Serial.read();
    }
    splitString(buffer);
  }
}

void splitString(char* data){
  Serial.print("Dados inseridos: ");
  Serial.println(data);
  char* parameter;
  parameter= strtok (data, " ,");
  while(parameter != NULL){
    setLED(parameter);
    parameter = strtok (NULL, " ,");
  }
  //Limpa o texto e os buffers seriais
  for (int x=0; x<66; x++){
    buffer[x]='\0';
  }
  Serial.flush();
}

void setLED(char* data){
  if (data[0] == 'l' || data[0] == 'L'){
    digitalWrite(Pino07, HIGH);
  } else if (data[0] == 'd' || data[0] == 'D'){
    digitalWrite(Pino07, LOW);
  }
}
