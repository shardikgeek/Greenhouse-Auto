#include <string.h>
char a = NULL;
String paquete = "";
String paquete_buffer = "";
bool flag_paquete = false;

String comando = "";
String cant_datos = "";
String datos = "";
bool flag_desempaquetado = false;

void recibir_paquete(void);
bool paquete_disponible(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
      recibir_paquete();
  }
  
  if(paquete_disponible() && !flag_desempaquetado){
    desempaquetar();
    flag_desempaquetado = true;
    procesar_paquete();
    comando = "";
    cant_datos = "";
    datos = "";
  }
  
  

}

void recibir_paquete(){
  
    a = Serial.read();
   
    if(a == ':' && !flag_paquete){ // Chequea si comenzo el paquete
      Serial.println("Comienzo del paquete");
      flag_paquete = true;
    }
    
    else if(a == ':' && flag_paquete){ // Chequea si se evia otro paquete en medio de la recepcion.
      Serial.println("Comienzo del paquete en medio de transmision. Paquete abortado.");
      flag_paquete = false;
      paquete_buffer = "";
    }

    if(flag_paquete && a != '$'){
      paquete_buffer += a;
    }

    if(a == '$' && flag_paquete){ // Termino de recibirse el paquete.
       paquete_buffer += '$'; // Agrega la terminacion.
       Serial.println("Fin del paquete");
       Serial.print("Paquete -> ");
       Serial.println(paquete_buffer);
       paquete  = paquete_buffer;
       flag_desempaquetado = false;
       paquete_buffer = ""; // Se limpia el buffer.
       
       if(flag_paquete)
        flag_paquete = false;
    }
}

bool paquete_disponible(){
  if(paquete[0] == ':' && !flag_paquete){
    return true;
  }
  else
    return false;
}

void desempaquetar(){
  int n = 1;
   
  while(paquete[n] != ','){
    comando += paquete[n];
    n++;
  }
  
  n++;
 
  while(paquete[n] != ','){
    cant_datos += paquete[n];
    n++;
  }

  n++;
  
  while(paquete[n] != '$'){
    datos += paquete[n];
    n++;
  }
}

void procesar_paquete(){
  if(comando == "HELLO"){
    Serial.println("Hola humano.");
  }
  else if(comando == "TEMP"){
    Serial.println("La temperatura es:");
  }
  else if(comando == "HUM"){
    Serial.println("La humedad es:");
  }
  else
    Serial.println("Comando incorrecto");
  
}




