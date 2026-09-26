#include <Servo.h> 

Servo meuServo;

// sensor
#define trigPin 3      
#define echoPin 4    

// motores
#define MEf 8                 //esquerda motor frente
#define MEt 9                 //esquerda motor tras
#define MDf 6                 //direita motor frente
#define MDt 7                 //direita motor tras
#define ENA 10
#define ENB 5

long duration;
int distance, distAtual, estadoDirecao;
int distMin = 15;
int speed = 100;

int anguloFrente = 0;
int anguloDireita = 90;
int anguloEsquerda = 180;


int medeDist(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  distance = duration * 0.034 / 2;

  return distance;
}

void parar(){

  digitalWrite(MEf, LOW);
  digitalWrite(MEt, LOW);

  digitalWrite(MDf, LOW);
  digitalWrite(MDt, LOW);
}

void irFrente(){

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(MDf, HIGH);
  digitalWrite(MDt, LOW);

  digitalWrite(MEf, HIGH);
  digitalWrite(MEt, LOW);

}

void irDireita(){

  analogWrite(ENA, speed);
  analogWrite(ENB, speed); 

  digitalWrite(MDf, LOW);
  digitalWrite(MDt, HIGH);

  digitalWrite(MEf, HIGH);
  digitalWrite(MEt, LOW);

}

void irEsquerda(){

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(MDf, HIGH);
  digitalWrite(MDt, LOW);

  digitalWrite(MEf, LOW);
  digitalWrite(MEt, HIGH);

}

void darRe(){

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  
  digitalWrite(MDf, LOW);
  digitalWrite(MDt, HIGH);

  digitalWrite(MEf, LOW);
  digitalWrite(MEt, HIGH);

}

void olharFrente(){

  meuServo.write(anguloFrente); 
  delay(300);
}

void olharDireita(){
  meuServo.write(anguloDireita); 
  delay(300);
}

void olharEsquerda(){
  meuServo.write(anguloEsquerda); 
  delay(300);
}

void setup() {
  Serial.begin(9600);
  pinMode(MLa, OUTPUT);    
  pinMode(MLb, OUTPUT);
  pinMode(MRa, OUTPUT);
  pinMode(MRb, OUTPUT);

  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);      

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  meuServo.attach(2);

  //  parado no começo

  estadoDirecao = 0 ;// frente

  parar();

  olharFrente();

}
void loop() {

  distAtual = medeDist();

  if(estadoDirecao == 0){
    Serial.println("Direcao Frente");
  }
  else if(estadoDirecao == 1){
    Serial.println("Direcao Direita");
  }
  else if(estadoDirecao == 2){
    Serial.println("Direcao Esquerda");
  }
  else {
    Serial.println("Fudeus  primeira verificacao estadoDirecao rs");
  }

  Serial.println(distance);
  
  if(distAtual <= distMin){

    if(estadoDirecao == 0){
      olharDireita();
      estadoDirecao = 1;
      delay(200);
    }
    else if(estadoDirecao == 1){
      olharEsquerda();
      estadoDirecao = 2;
      delay(200);
    }
    else if(estadoDirecao == 2){
      olharFrente();
      estadoDirecao = 0;
      delay(200);
    }
    else {
      Serial.println("estado direcao com erro no if distance <= distmin");
    }
 
  }

  else if(distance > 15){

    if(estadoDirecao == 0){
      irFrente();
      delay(2000);
    }
    else if(estadoDirecao == 1){
      parar();
      irDireita();
      delay(2000);
    }
    else if(estadoDirecao == 2){
      parar();
      irEsquerda();
      delay(2000);
    }
    else {
      Serial.println("estado direcao com erro no else if distance > 15");
    }
 
  }
}
