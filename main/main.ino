#include <Servo.h> 

#define SW1 1
#define SW2 2
#define SW3 3
#define SW4 4
#define SW4 5
#define S1 A0
#define S2 A1
#define S3 A2

Servo servoIzq;
Servo servoDer;

int SensorVal [2];

unsigned long ultimaLlamada;
unsigned long tiempoPasado;

byte estadoZigZag = 0;
/*
0 -> izquierda
1 -> giro derecha
2 -> derecha
3 -> giro izquierda
*/

bool estadoPareAvance = 0;
/*
0 -> pare
1 -> avance
*/

int Mode;

void adelante(){
  servoDer.write(0);
  servoIzq.write(180);
}

void atras(){
  servoDer.write(180);
  servoIzq.write(0);
}

void derecha(){
  servoDer.write(0);
  servoIzq.write(90);
}

void izquierda(){
  servoDer.write(90);
  servoIzq.write(180);
}

void quieto(){
  servoDer.write(90);
  servoIzq.write(90);
}


/*
 * 0    Sobre linea
 * 1    Sobre linea, linea a la derecha
 * 2    Sobre line, linea a la izquierda
 * 3    No hay linea
 * 4    No hay linea, linea a la derercha
 * 5    No hay linea, linea a la izquierda
 * 6    Linea de frente
 */
byte state = 3;
int umbral_sensor = 500;

byte whereIsTheLine(){
  switch(state){
    case 0:
      if((SensorVal[0] > umbral_sensor) && (SensorVal[1] < umbral_sensor) ){
        state = 1;
      }
      else if((SensorVal[0] < umbral_sensor) && (SensorVal[1] > umbral_sensor) ){
        state = 1;
      }
    break;
    case 1:
      if((SensorVal[0] < umbral_sensor) && (SensorVal[1] < umbral_sensor) ){
        state = 0;
      }
    break;
    case 2:
      if((SensorVal[0] < umbral_sensor) && (SensorVal[1] < umbral_sensor) ){
        state = 0;
      }
    break;
    case 3:
      if(SensorVal[0] > umbral_sensor){
        state = 5;
      }
      else if(SensorVal[1] > umbral_sensor ){
        state = 4;
      }
    break;
    case 4:
      if(SensorVal[1] < umbral_sensor ){
        state = 0;
      }
    break;
    case 5:
      if(SensorVal[0] < umbral_sensor ){
        state = 0;
      }
    break;
    default:
      state = 3;
    break;
  }
  return 0;
}



void movimiento(byte modo){
  switch(modo){
    case 0://Hacia Atrás en circulo
      servoDer.write(135);
      servoIzq.write(85);
    break;
    case 1://ZigZag
      switch(estadoZigZag){
        case 0://izquerda
          if(tiempoPasado < 1500){
            adelante();
          }
          else{
            estadoZigZag = 1;//gire a la derecha
            //
            ultimaLlamada = millis();
          }
        break;
        case 1://Giro derecha
          if(tiempoPasado < 1000){
            derecha();
          }
          else{
            estadoZigZag = 2;//muevase a la derecha
            //
            ultimaLlamada = millis();
          }
        break;
        case 2://derercha
          if(tiempoPasado < 1500){
            adelante();
          }
          else{
            estadoZigZag = 3;//gire a la izquierda
            ultimaLlamada = millis();
          }
        break;
        case 3://Giro izquierda
          if(tiempoPasado < 1000){
            izquierda();
          }
          else{
            estadoZigZag = 0;//muevase a la izquierda
            //
            ultimaLlamada = millis();
          }
        break;
        default://No se mueva
          quieto();
        break;
      }
    break;
    case 3://Seguidor de Linea
      switch(whereIsTheLine()){
        case 0:
          adelante();
        break;
        case 1:
          izquierda();
        break;
        case 2:
          derecha();
        break;
        case 3:
          adelante();
        break;
        case 4:
          derecha();
        break;
        case 5:
          izquierda();
        break;
        default:
          quieto();
        break;
      }
    break;
    case 7://Dos segundos y se detiene
      if(estadoPareAvance){
        servoDer.write(0);
        servoIzq.write(180); 
        if(tiempoPasado > 2000){
          ultimaLlamada = millis();
          estadoPareAvance = false;
        }
      }
      else{
        servoDer.write(90);
        servoIzq.write(90);  
        if(tiempoPasado > 2000){
          ultimaLlamada = millis();
          estadoPareAvance = true;
        }
      }
    break;
    case 15://En Espera
      servoDer.write(90);
      servoIzq.write(90);
    break;
    default:
      servoDer.write(0);
      servoIzq.write(180);
    break;
    
  }
}

void setup() {
  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  
  pinMode(SW1,INPUT_PULLUP);
  pinMode(SW2,INPUT_PULLUP);
  pinMode(SW3,INPUT_PULLUP);
  pinMode(SW4,INPUT_PULLUP);
  Serial.begin(9600);   // Initialize serial communications with PC
  Mode=0;
  
  servoIzq.attach(9);
  servoDer.attach(10);
  ultimaLlamada = millis();
}

void loop(){
  SensorVal[0] = analogRead(S1);
  SensorVal[1] = analogRead(S2);
  
  for(int i=0;i<5;i++){
    Mode += digitalRead(i+1);
  }
  
  tiempoPasado= millis() - ultimaLlamada;

  movimiento(0);
}


