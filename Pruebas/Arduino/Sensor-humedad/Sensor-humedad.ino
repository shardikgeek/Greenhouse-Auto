int i;
double promedio=0;
int porcentaje;

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  for(i=0;i<10;i++){
    promedio += analogRead(A0);
    delay(100);
  }
  
  promedio = promedio/10;
  porcentaje = (promedio-1023)/-6.73; // Funcion en base a los limites testeados experimentalmente.
  
  if(porcentaje < 0){
    porcentaje = 0;
  }
  if(porcentaje > 100){
    porcentaje = 100;
  }
  
  
  Serial.print("Humdad: ");
  Serial.println(porcentaje);
  promedio = 0;
}
