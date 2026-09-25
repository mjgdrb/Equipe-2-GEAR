#include <Servo.h>

// SENSOR ULTRASSONICO
#define TRIG 2  // pino do trig
#define ECHO 4   // pino do echo

// SERVO
#define SERVO 3  // pino do servo
Servo meuServo;

// VELOCIDADE PWM
int velPWM = 120;

// ANGULOS
int anguloFrente = 90;
int anguloDireita = 0;
int anguloEsquerda = 180;

// DISTANCIAS USADAS NO COD PRINCIPAL
int distFrente = 0;   // distancia o objeto na frente do sensor
int distDireita = 0;  // distancia o objeto na direita do sensor
int distEsquerda = 0; // distancia o objeto na esquerda do sensor
int distMin = 15;     // distancia minima para ele mudar a direcao do sensor

// COISAS PARA O "DEBOUNCE" DO SENSOR

const int TAMANHO_FILTRO = 5; // número de leituras para fazer a media (tamanho da lista)
int leituras[TAMANHO_FILTRO];   // lista para armazenar as leituras
int indiceLeitura = 0;          // indice da leitura atual
long totalLeituras = 0;         // soma das leituras
int distanciaEstabilizada = 0;  // distancia final 

// MOTORES (TEM QUE VER QUAL É QUAL)
#define ENA 5           // pino ENA
#define pinMotorDireitaFrente 6     // pino motor 1 pra FRENTE (da DIREITA)
#define pinMotorDireitaTras 7       // pino motor 1 pra TRAS   (da DIREITA)
#define pinMotorEsquerdaFrente 8    // pino motor 2 pra FRENTE (da ESQUERDA)
#define pinMotorEsquerdaTras 9      // pino motor 2 pra TRAS   (da ESQUERDA)
#define ENB 10          // pino ENB

// FUNCAO LER DISTANCIA ----- RETORNA A DISTANCIA "LIMPA"
int lerDist(){

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long tempo = pulseIn(ECHO, HIGH, 30000); // Timeout de 30ms para não travar o código
  int distanciaAtual = tempo / 58;

  // Filtro 
  if (distanciaAtual > 2 && distanciaAtual < 100) {
    // Subtrai a leitura mais antiga
    totalLeituras = totalLeituras - leituras[indiceLeitura];
    // Armazena a nova leitura
    leituras[indiceLeitura] = distanciaAtual;
    // Adiciona a nova leitura ao total
    totalLeituras = totalLeituras + leituras[indiceLeitura];
    // Avança para a próxima posição da lista
    indiceLeitura = (indiceLeitura + 1) % TAMANHO_FILTRO;
    // Calcula a média 
    distanciaEstabilizada = totalLeituras / TAMANHO_FILTRO;
  }

  // Exibe o valor filtrado e "estavel"
  Serial.print("Distancia Filtrada: ");
  Serial.print(distanciaEstabilizada);
  Serial.println(" cm");
  
  delay(20); 

  return distanciaEstabilizada;
}

// FUNCAO PARAR O CARRO
void parar() {
  digitalWrite(pinMotorDireitaFrente, LOW);
  digitalWrite(pinMotorDireitaTras, LOW);
  digitalWrite(pinMotorEsquerdaFrente, LOW);
  digitalWrite(pinMotorEsquerdaTras, LOW);
}

// FUNCAO IR PARA FRENTE
void irFrente() {

  parar();

  analogWrite(ENA, velPWM); 
  analogWrite(ENB, velPWM);


  digitalWrite(pinMotorDireitaFrente, HIGH);
  digitalWrite(pinMotorDireitaTras, LOW);

  digitalWrite(pinMotorEsquerdaFrente, HIGH);
  digitalWrite(pinMotorEsquerdaTras, LOW);
  
}

//FUNCAO VIRAR 
void virarDireita() {

  parar();

  analogWrite(ENA, velPWM);
  analogWrite(ENB, velPWM);

  digitalWrite(pinMotorDireitaFrente, LOW);
  digitalWrite(pinMotorDireitaTras, LOW);

  digitalWrite(pinMotorEsquerdaFrente, HIGH);
  digitalWrite(pinMotorEsquerdaTras, LOW);

  delay(500);

  parar();
}

//FUNCAO VIRAR ESQUERDA
void virarEsquerda() {

  parar();

  analogWrite(ENA, velPWM);
  analogWrite(ENB, velPWM);

  digitalWrite(pinMotorDireitaFrente, HIGH);
  digitalWrite(pinMotorDireitaTras, LOW);

  digitalWrite(pinMotorEsquerdaFrente, LOW);
  digitalWrite(pinMotorEsquerdaTras, LOW);

  delay(500);

  parar();
}

//FUNCAO IR PARA TRAS
void darRe() {

  parar();

  analogWrite(ENA, velPWM);
  analogWrite(ENB, velPWM);

  digitalWrite(pinMotorDireitaFrente, LOW);
  digitalWrite(pinMotorDireitaTras, HIGH);
  
  digitalWrite(pinMotorEsquerdaFrente, LOW);
  digitalWrite(pinMotorEsquerdaTras, HIGH);

  delay(500);

  parar();
}

void olharFrente(){
  meuServo.write(anguloFrente);
  delay(1000);
}

void olharDireita(){
  meuServo.write(anguloDireita);
  delay(1000);
}

void olharEsquerda(){
  meuServo.write(anguloEsquerda);
  delay(1000);
}

void setup() {

  // SENSOR ULTRASSONICO
  pinMode(TRIG, OUTPUT); // coloca trigg como saida
  pinMode(ECHO, INPUT); // coloca echo como entrada
  
  // SERVO
  meuServo.attach(SERVO); // informa o pino do servo

  // MOTORES
  pinMode(ENA, OUTPUT);
  pinMode(pinMotorDireitaFrente, OUTPUT);
  pinMode(pinMotorDireitaTras, OUTPUT);
  pinMode(pinMotorEsquerdaFrente, OUTPUT);
  pinMode(pinMotorEsquerdaTras, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);   // liga o terminal serial

   // Inicializa o array com zeros
  for (int i = 0; i < TAMANHO_FILTRO; i++) {
    leituras[i] = 0;
  }

  parar();
  olharFrente();
  Serial.println("--- ROBÔ INICIADO (Olhando Frente) ---");
}

void loop() {
  olharFrente();
  distFrente = lerDist(); // le a distancia atual
  delay(500);
  // PARTE PRINCIPAL

  // COM OBSTACULO

  if(distFrente < distMin && distFrente > 5){  // se estiver um obstaculo na frente do sensor
   
    parar();

    Serial.println("Obstaculo na frente! CHECANDO DIREITA");

    olharDireita();
    distDireita = lerDist();
    Serial.println("Distancia Direita: ");
    Serial.print(distDireita);

    Serial.println("Obstaculo na frente! CHECANDO ESQUERDA");

    olharEsquerda();
    distEsquerda = lerDist();
    Serial.print("Distancia Esquerda: ");
    Serial.print(distEsquerda);

    olharFrente();

    if (distDireita >= distMin) {
      // LIVRE DIREITA
      Serial.println("DIREITA LIVRE -> VIRANDO DIREITA");
      olharFrente();
      virarDireita();
    }
    else if (distEsquerda >= distMin){
      // LIVRE ESQUERDA
      Serial.println("ESQUERDA LIVRE -> VIRANDO ESQUERDA");
      olharFrente();
      virarEsquerda();

    }
    else {
      Serial.println("[!] BLOQUEADO EM TUDO! Iniciando manobra de RÉ...");

      bool achouSaida = false;

      while (!achouSaida) {
        Serial.println("Dando ré...");
        darRe();
        parar();

        // Checa Direita
        Serial.println("Checando Direita...");
        olharDireita();
        distDireita = lerDist();

        if (distDireita >= distMin) {
          Serial.println("[->] Saída encontrada na Direita!");
          olharFrente();
          virarDireita();
          achouSaida = true;
          break;
        }

        // Checa Esquerda
        Serial.println("Checando Esquerda...");
        olharEsquerda();
        distEsquerda = lerDist();

        if (distEsquerda >= distMin || distEsquerda == 0) {
          Serial.println("[<-] Saída encontrada na Esquerda!");
          olharFrente();
          virarEsquerda();
          achouSaida = true;
          break;
        }

        Serial.println("[!] LATERAIS BLOQUEADAS. Dando ré novamente...");
      }

    }
   
  }

  // SEM OBSTACULO

  else {
    
    Serial.println("Frente livre");
    olharFrente();
    irFrente();
    delay(1000);
    parar();

  }
  
}
