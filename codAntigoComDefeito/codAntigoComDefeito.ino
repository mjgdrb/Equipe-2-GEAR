#include <Servo.h> 

Servo meuServo;

// sensor
#define trigPin 3           // Trig Pin Of HC-SR04
#define echoPin 4        // Echo Pin Of HC-SR04

// motores
#define MLa 8                   //left motor 1st pin
#define MLb 9                  //left motor 2nd pin
#define MRa 6               //right motor 1st pin
#define MRb 7               //right motor 2nd pin
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

  duration = pulseIn(echoPin, HIGH, 3000);

  distance = duration * 0.034 / 2;

  return distance;
}

void parar(){

  digitalWrite(MLa, LOW);
  digitalWrite(MLb, LOW);

  digitalWrite(MRa, LOW);
  digitalWrite(MRb, LOW);
}

void irFrente(){

  digitalWrite(MRa, HIGH);
  digitalWrite(MRb, LOW);

  digitalWrite(MLa, HIGH);
  digitalWrite(MLb, LOW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

}

void irDireita(){

  digitalWrite(MRa, LOW);
  digitalWrite(MRb, LOW);

  digitalWrite(MLa, HIGH);
  digitalWrite(MLb, LOW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed); 

}

void irEsquerda(){

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(MRa, HIGH);
  digitalWrite(MRb, LOW);

  digitalWrite(MLa, LOW);
  digitalWrite(MLb, LOW);

}

void darRe(){
  digitalWrite(MRa, LOW);
  digitalWrite(MRb, HIGH);

  digitalWrite(MLa, LOW);
  digitalWrite(MLb, HIGH);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

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
  pinMode(MLa, OUTPUT);     // Set Motor Pins As O/P
  pinMode(MLb, OUTPUT);
  pinMode(MRa, OUTPUT);
  pinMode(MRb, OUTPUT);

  pinMode(trigPin, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin, INPUT);        //Set Echo Pin As I/P To Receive Reflected Waves

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
      // fazer funcao irDireita();
      delay(2000);
    }
    else if(estadoDirecao == 2){
      parar();
      // fazer funcao irEsquerda()
      delay(2000);
    }
    else {
      Serial.println("estado direcao fodido lol no else if distance > 15");
    }
 
  }
}
