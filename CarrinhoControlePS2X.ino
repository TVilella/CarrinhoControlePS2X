/*
 	AUTOR:			    Thiago Vilella
 	YOUTUBE:		    https://www.youtube.com/channel/UCpg7_v1Wtvp-XPj-vjh8igQ
 	CURSO ARDUINO:	http://bit.ly/2ZhypC9
 	CURSO ROBOTICA:	http://bit.ly/2zjK9Eo
 	COMPRE:			    https://www.arducore.com.br/
 	SKETCH:			    Carro Controle PS2X + MotorShield
 	VERSÃO:			    1.0.0
 	DATA:			      10/10/2020
*/
// INCLUSÃO DE BIBLIOTECAS
#include <PS2X_lib.h>
#include <AFMotor.h>

// DEFINIÇÕES DE PINOS
#define pinClock      A0
#define pinCommand    A1
#define pinAttention  A2
#define pinData       A3

// DEFINIÇÕES
#define velMax 255
#define velMin 180

#define DEBUG
// INSTANCIANDO OBJETOS
PS2X controle;

AF_DCMotor volante(1);  // motor do volante em M1
AF_DCMotor motor(2);    // motor da roda em M2

// DECLARAÇÃO DE FUNÇÕES
void confereControle();
void testaMotor(int tempoDelay = 1000);

// DECLARAÇÃO DE VARIÁVEIS
int error = 0;
byte tipoControle = 0;
byte vibrate = 0;

// ************* INÍCIO DO SETUP *************
void setup() {
  error = controle.config_gamepad(pinClock, pinCommand, pinAttention, pinData, true, true);
  controle.enableRumble();
  
  volante.setSpeed(velMax);
  volante.run(RELEASE);  
  motor.setSpeed(velMax);
  motor.run(RELEASE);
  
  #ifdef DEBUG
    Serial.begin(9600);
    
    confereControle();
    testaMotor();
    
    Serial.println("Fim Setup");
  #endif
}
// ************** FIM DO SETUP ***************

// ************** INÍCIO DO LOOP *************
void loop() {
  static unsigned long ultimaLeitura = millis();
  if(millis() - ultimaLeitura > 50){
    controle.read_gamepad(false, vibrate);
    ultimaLeitura = millis();
  }

  if (controle.Button(PSB_PAD_UP)) { // MUDA PARA velMax
    motor.setSpeed(velMax);
    #ifdef DEBUG
      Serial.println("cima");
    #endif
  }else if(controle.Button(PSB_PAD_DOWN)){
    motor.setSpeed(velMin);
    #ifdef DEBUG
      Serial.println("baixo");
    #endif
  }

  if (controle.Button(PSB_PAD_RIGHT)) {
    volante.run(FORWARD);
    #ifdef DEBUG
      Serial.println("Direita");
    #endif
  } else if (controle.Button(PSB_PAD_LEFT)) {
    volante.run(BACKWARD);
    #ifdef DEBUG
      Serial.println("Esquerda");
    #endif
  } else {
    volante.run(RELEASE);
  }

  if (controle.Button(PSB_BLUE)) { // ACELERA
    motor.run(FORWARD);
    #ifdef DEBUG
      Serial.println("Acelera");
    #endif
  }else if (controle.Button(PSB_PINK)) { // RÉ
    motor.run(BACKWARD);
    #ifdef DEBUG
      Serial.println("Ré");
    #endif
  }else{ // DESLIGA O MOTOR
    motor.run(RELEASE);   
  }

  vibrate = controle.Analog(PSAB_BLUE);// VIBRAÇÃO DO CONTROLE
  vibrate = controle.Analog(PSAB_PINK);// VIBRAÇÃO DO CONTROLE
}
// *************** FIM DO LOOP ***************

// IMPLEMENTO DE FUNÇÕES

void confereControle() {
    
  if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  tipoControle = controle.readType();
  switch (tipoControle) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }

  while(error != 0){}
}

void testaMotor(int tempoDelay = 1000){
  Serial.println("Iniciando teste Motor");
  Serial.println("Mover para Frente");
  delay(tempoDelay);
  motor.run(FORWARD);
  delay(tempoDelay);

  Serial.println("Parado");
  delay(tempoDelay);
  motor.run(RELEASE);
  delay(tempoDelay); 

  Serial.println("Mover para Tras");
  delay(tempoDelay);
  motor.run(BACKWARD);
  delay(tempoDelay);

  Serial.println("Parado");
  delay(tempoDelay);
  motor.run(RELEASE);
  delay(tempoDelay);

  Serial.println("Iniciando teste Volante");
  Serial.println("Volante direita");
  delay(tempoDelay);
  volante.run(FORWARD);
  delay(tempoDelay);

  Serial.println("Volante centro");
  delay(tempoDelay);
  volante.run(RELEASE);
  delay(tempoDelay); 

  Serial.println("Volante esquerda");
  delay(tempoDelay);
  volante.run(BACKWARD);
  delay(tempoDelay);

  Serial.println("Volante centro");
  delay(tempoDelay);
  volante.run(RELEASE);
  delay(tempoDelay);
}
