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
 * 1    Hacia adelante
 * 2    No hay linea
 * 3    giro derecha 1
 * 4    giro derecha 90
 * 5    giro izquierda 1
 * 6    giro izquierda 90
 * 7    giro derecha 2
 * 8    giro izquierda 2
 * 9    linea perpendicular
 * 10   linea 45 izquierda   
 * 11   linea 45 derecha
 */
byte state = 2;
byte nextState = 2;

byte whereIsTheLine(){
  //Read sensors
  SensorVal[0] = analogRead(S1);
  SensorVal[1] = analogRead(S2);
  SensorVal[2] = analogRead(S3);

  //SensorVal[0]: Izq
  //SensorVal[1]: -
  //SensorVal[2]: Der
  //0 Negro
  //1 Blanco
  if(SensorVal[0]<512 and SensorVal[2]<512){
    //Serial.println("Barra en el centro");
    nextState=1;
  }else if(SensorVal[0]<512 and SensorVal[2]>512 ){
    //Serial.println("Barra a la derecha");
    nextState=2;
  }else if(SensorVal[0]>512 and SensorVal[2]<512 ){
    //Serial.println("Barra a la izquierda");
    nextState=0;
  }else if(SensorVal[0]>512 and SensorVal[2]>512){
    //Serial.println("Ni puta idea donde está la barra");
    nextState=3;
  } else {
    Serial.println("Condicion no considerada");
  }
  
  switch(nextState){
    case 0:
      if(state==0){
        state = 5;
      }else if (state==1){
        state = 7;
      }else if (state==2){
        state = 9;
      }else if (state==3){
        state = 2;
      }
    case 1:
      if(state==0){
        state = 5;
      }else if (state==1){
        state = 1;  
      }else if (state==2){
         state = 3;
      }else if (state==3){
        state = 2;
      }
    case 2:
      if(state==0){
        state = 9;
      }else if (state==1){
        state = 6;
      }else if (state==2){
        state = 1;
      }else if (state==3){
        state = 2;
      }
    case 3: // No habia leido la linea seguimos
      state = 1;
  }
  return state;
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
            adelante();
          }
          else{
            estadoZigZag = 1;//gire a la derecha
            //
            ultimaLlamada = millis();
          }
        break;
        case 1://Giro derecha
          if(tiempoPasado < 500){
            derecha();
          }
          else{
            estadoZigZag = 2;//muevase a la derecha
            //
            ultimaLlamada = millis();
          }
        break;
        case 2://derercha
          if(tiempoPasado < 2000){
            adelante();
          }
          else{
            estadoZigZag = 3;//gire a la izquierda
            ultimaLlamada = millis();
          }
        break;
        case 3://Giro izquierda
          if(tiempoPasado < 500){
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
        case 1:
          adelante();
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
  
  for(int i=0;i<5;i++){
    Mode += digitalRead(i+1);
  }
    
  tiempoPasado= millis() - ultimaLlamada;

  movimiento(15);
}

/**
 * 0:
 * 1:
 */
byte sensores(){
  
}
