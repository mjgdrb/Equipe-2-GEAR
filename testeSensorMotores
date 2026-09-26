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

void setup() {
  // put your setup code here, to run once:

  pinMode(ENA, OUTPUT);                     //
  pinMode(MotorDireitaFrente, OUTPUT);      //    MOTOR DA DIREITA
  pinMode(MotorDireitaTras, OUTPUT);        //

  pinMode(MotorEsquerdaFrente, OUTPUT);     //
  pinMode(MotorEsquerdaTras, OUTPUT);       //    MOTOR DA ESQUERDA
  pinMode(ENB, OUTPUT);                     //

  pinMode(TRIG, OUTPUT);                    //
  pinMode(ECHO, INPUT);                     //    SENSOR

}


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);

  long tempo = pulseIn(ECHO, HIGH, 30000);
  int distancia = tempo * 0.034 / 2;

  if(distancia > 20){
    analogWrite(ENA, velocidade);                             
    analogWrite(ENB, velocidade);                             
    
    digitalWrite(MotorDireitaFrente, HIGH);
    digitalWrite(MotorDireitaTras, LOW);

    digitalWrite(MotorEsquerdaFrente, HIGH);                  
    digitalWrite(MotorEsquerdaTras, LOW);  
  }
  else{
    analogWrite(ENA, velocidade);
    analogWrite(ENB, velocidade);

    digitalWrite(MotorDireitaFrente, LOW);
    digitalWrite(MotorDireitaTras, LOW);

    digitalWrite(MotorEsquerdaFrente, LOW);
    digitalWrite(MotorEsquerdaTras, LOW);
  }

}
