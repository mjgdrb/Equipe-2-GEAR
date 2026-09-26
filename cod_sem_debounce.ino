#include <Servo.h>

Servo meuServo;
#define SERVO 3

#define TRIG 2
#define ECHO 4

#define ENA 5
#define MotorDireitaFrente 6
#define MotorDireitaTras 7
#define MotorEsquerdaFrente 8
#define MotorEsquerdaTras 9
#define ENB 10


//    VELOCIDADE
int velocidade = 120;

//    DISTANCIAS
int distanciaFrente, distanciaDireita, distanciaEsquerda;

//    TEMPOS
int tempoServo = 500;
int tempoCurva = 1000;
int tempoGiro180 = 500;
int tempoFrente = 1000;
int tempoRe = 300;

//    ANGULOS
int anguloFrente = 90;
int anguloDireita = 0;
int anguloEsquerda = 180;

void irFrente(){                                            

  analogWrite(ENA, velocidade);                             
  analogWrite(ENB, velocidade);                             
  
  digitalWrite(MotorDireitaFrente, HIGH);
  digitalWrite(MotorDireitaTras, LOW);

  digitalWrite(MotorEsquerdaFrente, HIGH);                  
  digitalWrite(MotorEsquerdaTras, LOW);    
}

void parar(){

  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);

  digitalWrite(MotorDireitaFrente, LOW);
  digitalWrite(MotorDireitaTras, LOW);

  digitalWrite(MotorEsquerdaFrente, LOW);
  digitalWrite(MotorEsquerdaTras, LOW);
}


void virarDireita(){

  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);

  digitalWrite(MotorDireitaFrente, LOW);
  digitalWrite(MotorDireitaTras, LOW);

  digitalWrite(MotorEsquerdaFrente, HIGH);
  digitalWrite(MotorEsquerdaTras, LOW);
}

void virarEsquerda(){

  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);

  digitalWrite(MotorDireitaFrente, HIGH);
  digitalWrite(MotorDireitaTras, LOW);

  digitalWrite(MotorEsquerdaFrente, LOW);
  digitalWrite(MotorEsquerdaTras, LOW);
}

void girar180(){

  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);

  digitalWrite(MotorDireitaFrente, LOW);
  digitalWrite(MotorDireitaTras, HIGH);

  digitalWrite(MotorEsquerdaFrente, HIGH);
  digitalWrite(MotorEsquerdaTras, LOW);
}

void darRe(){

  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);

  digitalWrite(MotorDireitaFrente, LOW);
  digitalWrite(MotorDireitaTras, HIGH);

  digitalWrite(MotorEsquerdaFrente, LOW);
  digitalWrite(MotorEsquerdaTras, HIGH);
}


void olharFrente(){
  meuServo.write(anguloFrente);
}

void olharDireita(){
  meuServo.write(anguloDireita);
}

void olharEsquerda(){
  meuServo.write(anguloEsquerda);
}

int medirDistancia(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);

  long tempo = pulseIn(ECHO, HIGH, 30000);
  int distancia = tempo * 0.034 / 2;

  return distancia;
}

void setup() {
  // put your setup code here, to run once:

  pinMode(ENA, OUTPUT);                     //
  pinMode(MotorDireitaFrente, OUTPUT);      //    MOTOR DA DIREITA
  pinMode(MotorDireitaTras, OUTPUT);        //

  pinMode(MotorEsquerdaFrente, OUTPUT);     //
  pinMode(MotorEsquerdaTras, OUTPUT);       //    MOTOR DA ESQUERDA
  pinMode(ENB, OUTPUT);                     //

  pinMode(TRIG, OUTPUT);                    //
  pinMode(ECHO, INPUT);                     //    SERVO

  meuServo.attach(SERVO);                   //    SERVO
}


void loop() {
  // put your main code here, to run repeatedly:

  parar();
  olharFrente();
  delay(tempoServo);

  distanciaFrente = medirDistancia();

  if(distanciaFrente < 20 && distanciaFrente > 2){
    // FRENTE FECHADA
    olharDireita();
    delay(tempoServo);
    distanciaDireita = medirDistancia();

    if(distanciaDireita < 20 && distanciaDireita > 2){
      // DIREITA FECHADA
      olharEsquerda();
      delay(tempoServo);
      distanciaEsquerda = medirDistancia();

      if(distanciaEsquerda < 20 && distanciaEsquerda > 2) {
        // ESQUERDA FECHADA
        olharFrente();
        delay(tempoServo);
        girar180();
        delay(tempoGiro180);

      }
      else{
        // ESQUERDA LIVRE
        virarEsquerda();
        delay(tempoCurva);
      }
    }
    else{
      // DIREITA LIVRE
      virarDireita();
      delay(tempoCurva);
    }
  } 
  else{
    // FRENTE LIVRE
    irFrente();
    delay(tempoFrente);
  }

}
