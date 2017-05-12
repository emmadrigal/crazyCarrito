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

int SensorVal [3];
int Mode;

byte whereIsTheLine(){
  return 0;
}



void movimiento(byte modo){
  switch(modo){
    case 0://Hacia Atrás en circulo
      servoDer.write(180);
      servoIzq.write(80);
    break;
    case 1://ZigZag
      switch(estadoZigZag){
        case 0://izquerda
          if(tiempoPasado < 2000){
            servoDer.write(0);
            servoIzq.write(180);
          }
          else{
            estadoZigZag = 1;//gire a la derecha
            //
            ultimaLlamada = millis();
          }
        break;
        case 1://Giro derecha
          if(tiempoPasado < 500){
            servoDer.write(0);
            servoIzq.write(90);
          }
          else{
            estadoZigZag = 2;//muevase a la derecha
            //
            ultimaLlamada = millis();
          }
        break;
        case 2://derercha
          if(tiempoPasado < 2000){
            servoDer.write(0);
            servoIzq.write(180);
          }
          else{
            estadoZigZag = 3;//gire a la izquierda
            //Pare
            servoDer.write(180);
            servoIzq.write(0);
            //
            ultimaLlamada = millis();
          }
        break;
        case 3://Giro izquierda
          if(tiempoPasado < 500){
            servoDer.write(90);
            servoIzq.write(180);
          }
          else{
            estadoZigZag = 0;//muevase a la izquierda
            //
            ultimaLlamada = millis();
          }
        break;
        default://No se mueva
          servoDer.write(90);
          servoIzq.write(90);
        break;
      }
    break;
    case 3://Seguidor de Linea
      //TODO
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
  pinMode(S3,INPUT);
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
  SensorVal[2] = analogRead(S3);
  
  for(int i=0;i<5;i++){
    Mode += digitalRead(i+1);
  }

  //SensorVal[0]: Izq
  //SensorVal[1]: -
  //SensorVal[2]: Der
  //0 Negro
  //1 Blanco
  if(SensorVal[0]<512 and SensorVal[2]<512){
    Serial.println("Barra en el centro");
  }else if(SensorVal[0]<512 and SensorVal[2]>512 ){
    Serial.println("Barra a la derecha");
  }else if(SensorVal[0]>512 and SensorVal[2]<512 ){
    Serial.println("Barra a la izquierda");
  }else if(SensorVal[0]>512 and SensorVal[2]>512){
    Serial.println("Ni puta idea donde está la barra");
  } else {
    Serial.println("Condición no considerada");
  }
  Serial.println("S Izquierdo");
  Serial.println(SensorVal[0]);
  Serial.println("S Derecho");
  Serial.println(SensorVal[2]);
  delay(1000);
  
  tiempoPasado= millis() - ultimaLlamada;

  movimiento(15);
}


