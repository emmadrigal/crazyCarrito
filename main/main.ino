#define SW1 1
#define SW2 2
#define SW3 3
#define SW4 4
#define SW4 5
#define S1 A0
#define S2 A1
#define S3 A2

int SensorVal [3];
int Mode;

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
}

void loop(){
  SensorVal[0] = analogRead(S1);
  SensorVal[1] = analogRead(S2);
  SensorVal[2] = analogRead(S3);
  
  for(int i=0;i<5;i++){
    Mode += digitalRead(i+1);
  }
  
  switch(Mode){
    case 0: //Avanza en circulo reversa   0 0 0 0
      break;
    case 1: //Zig Zag                     0 0 0 1
      break;
    case 2: //Sigue la línea              0 0 1 1
      break;
    case 3: //Avanza 2s y detiene repite  0 1 1 1
      break;
    default: //Espera                      1 1 1 1
      //Serial.println("En espera"); 
      break; 
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
}

/**
 * 0:
 * 1:
 */
byte sensores(){
  
}
