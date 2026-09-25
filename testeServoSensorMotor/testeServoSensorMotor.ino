#include <Servo.h>

// SENSOR ULTRASSONICO
#define TRIG 2  // pino do trig
#define ECHO 4   // pino do echo

// SERVO
#define SERVO 3  // pino do servo
Servo meuServo;

// ESTADO DA DIRECAO QUE O SENSOR TA APONTADO 0=frente 1=direita 2=esquerda
int estadoDirecao; // para onde ele esta olhando (frente/direita/esquerda)

// DISTANCIAS USADAS NO COD PRINCIPAL
int distAtual; // distancia o objeto na frente do sensor
int distMin = 15; // distancia minima para ele mudar a direcao do sensor

// COISAS PARA O "DEBOUNCE" DO SENSOR

const int TAMANHO_FILTRO = 5; // número de leituras para fazer a media (tamanho da lista)
int leituras[TAMANHO_FILTRO];   // lista para armazenar as leituras
int indiceLeitura = 0;          // indice da leitura atual
long totalLeituras = 0;         // soma das leituras
int distanciaEstabilizada = 0;  // distancia final 

// MOTORES (TEM QUE VER QUAL É QUAL LOL)
#define ENA 5           // pino ENA
#define pinMotorDireitaFrente 6    // pino motor 1 pra FRENTE (da direita)
#define pinMotorDireitaTras 7    // pino motor 1 pra TRAS   (da direita)
#define pinMotorEsquerdaFrente 8    // pino motor 2 pra FRENTE (da esquerda)
#define pinMotorEsquerdaTras 9    // pino motor 2 pra TRAS   (da esquerda)
#define ENB 10          // pino ENB

// FUNCAO LER DISTANCIA ----- RETORNA A DISTANCIA "LIMPA"
int lerDist(){

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long tempo = pulseIn(ECHO, HIGH, 30000); // Timeout de 30ms para não travar o código
  int distanciaAtual = tempo * 0.034 / 2;

  // Filtro 
  if (distanciaAtual > 2 && distanciaAtual < 400) {
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
  
  delay(100); // Leituras estaveis a cada 100ms

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
  for (int velocidade = 100; velocidade >= 45; velocidade--) {
    analogWrite(pinMotorDireitaFrente, velocidade);
    analogWrite(pinMotorEsquerdaFrente, velocidade);
    delay(1);
  }
}

//FUNCAO VIRAR 
void virarDireita() {
  parar();
  analogWrite(pinMotorDireitaTras, 100);
  analogWrite(pinMotorEsquerdaFrente, 100);
  delay(100);
  parar();
}

//FUNCAO VIRAR ESQUERDA
void virarEsquerda() {
  parar();
  analogWrite(pinMotorDireitaFrente, 100);
  analogWrite(pinMotorEsquerdaTras, 100);
  delay(100);
  parar();
}

//FUNCAO IR PARA TRAS
void darRe() {
  parar();
  for (int velocidade = 100; velocidade >= 30; velocidade--) {
    analogWrite(pinMotorDireitaTras, velocidade);
    analogWrite(pinMotorEsquerdaFrente, velocidade);
    delay(1);
  }
}

void setup() {

  // SENSOR ULTRASSONICO
  pinMode(TRIG, OUTPUT); // coloca trigg como saida
  pinMode(ECHO, INPUT); // coloca echo como entrada
  
  // SERVO
  meuServo.attach(SERVO); // informa o pino do servo

  // MOTORES
  pinMode(pinMotorDireitaFrente, OUTPUT);
  pinMode(pinMotorDireitaTras, OUTPUT);
  pinMode(pinMotorEsquerdaFrente, OUTPUT);
  pinMode(pinMotorEsquerdaTras, OUTPUT);

  Serial.begin(9600);   // liga o terminal serial

   // Inicializa o array com zeros
  for (int i = 0; i < TAMANHO_FILTRO; i++) {
    leituras[i] = 0;
  }

  estadoDirecao = 0; // 0 = olhando pra frente
  meuServo.write(0);
  Serial.println("--- ROBÔ INICIADO (Olhando Frente) ---");
}

void loop() {
  distAtual = lerDist(); // le a distancia atual


  Serial.print("Estado atual: "); //printa a o estado atual
  if(estadoDirecao == 0) {
    Serial.print("Frente (0)");
  }
  else if(estadoDirecao == 1) {
    Serial.print("Direita (180)");
  }
  else if(estadoDirecao == 2) {
    Serial.print("Esquerda (90)");
  }

  // PARTE PRINCIPAL

  // COM OBSTACULO

  if(distAtual < distMin){  // se estiver um obstaculo na frente do sensor
   
    if (estadoDirecao == 0){  // se está no estado = 0 (se o sensor esta olhando para frente)
      Serial.println("[!] Algo na FRENTE -> Virando sensor para DIREITA...");
      meuServo.write(0);
      estadoDirecao = 1; // muda o estado para estado == 1 (olhando direita)
      delay(2000); 
      distAtual = lerDist();
    }
 
    else if (estadoDirecao == 1){
      // se esta no estado = 1 (se o sensor esta olhando para a direita)
      Serial.println("[!] Algo na DIREITA -> Virando sensor para ESQUERDA...");
      meuServo.write(180);
      estadoDirecao = 2; // muda o estado para o estado == 2 (olhando para esquerda)
      delay(2000);
      distAtual = lerDist();
    }

    else if (estadoDirecao == 2){
        // se esta no estado = 2 (se o sensor esta olhando para a esquerda)
      Serial.println("[!] Algo na ESQUERDA -> Voltando sensor para FRENTE...");
      meuServo.write(90);
      estadoDirecao = 0; // muda o estado para o estado == 0 (olhando para frente)
      delay(2000);
      distAtual = lerDist();
    }
    else {
      Serial.println("Estado direçao com problema na parte principal com obstaculo");
    }
  }

  // SEM OBSTACULO

  else {
    
    if (estadoDirecao == 0){  
      Serial.println("[I] INDO FRENTE");
      irFrente();
    }
 
    else if (estadoDirecao == 1){

      Serial.println("[->] VIRANDO DIREITA");
      virarDireita();
    }

    else if (estadoDirecao == 2){
      Serial.println("[<-] VIRANDO ESQUERDA");
      virarEsquerda();
    }
    else{
      Serial.println("PEstado direcao com problema na parte principal sem obstaculo");
    }
  
  delay(500); 
}
