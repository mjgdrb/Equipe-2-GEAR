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
int contadorViradas = 0;
int speed = 100;


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

  digitalWrite(MRa, HIGH);
  digitalWrite(MRb, LOW);

  digitalWrite(MLa, LOW);
  digitalWrite(MLb, LOW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

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

  meuServo.write(90); //FRENTE
  Serial.println("90 GRAUS");
  // cudelay(7000);
}

void olharDireita(){
  meuServo.write(0); //nao sei a direção lol faz o L
  Serial.println("0 GRAUS");
  //cudelay(7000);
}

void olharEsquerda(){
  meuServo.write(180); //nao sei a direção lol faz o L
  Serial.println("180 GRAUS");
  //cudelay(7000);
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

  // para e faz o L -- parado no começo

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
      delay(2000);
    }
    else if(estadoDirecao == 1){
      olharEsquerda();
      estadoDirecao = 2;
      delay(2000);
    }
    else if(estadoDirecao == 2){
      olharFrente();
      estadoDirecao = 0;
      delay(2000);
    }
    else if(estadoDirecao == 3){ // fazer modo ré scr emoji chorando

    }
    else {
      Serial.println("estado direcao fodido lol no if distance <= distmin");
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
