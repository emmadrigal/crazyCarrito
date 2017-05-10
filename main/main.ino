#define SensorD1 A0
#define SensorC1 A1
#define SensorI1 A2

int SensorVal [3];

void setup() {
  pinMode(SensorD1,INPUT);
  pinMode(SensorC2,INPUT);
  pinMode(SensorI3,INPUT);
  
  Serial.begin(9600);   // Initialize serial communications with PC
}

void loop(){
  if(SensorD1>512 and SensorI1<512 and SensorC1<512){
    Serial.println("Barra a la derecha");
  }else if(SensorI1>512 and SensorD1<512 and SensorC1<512){
    Serial.println("Barra a la izquierda");
  }else if(SensorI1< 512 and SensorD1<512 and SensorC1>512){
    Serial.println("Barra al centro");
  }else {
    Serial.println("Ni puta idea donde está la barra");
  }
  delay(1000);
}
