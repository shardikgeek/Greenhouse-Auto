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

unsigned long time_timeout = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
 unsigned long currentMillis = millis();
const long interval = 1;           // interval at which to blink (milliseconds)

struct{
  int contador;
  bool flag_contador;
  int estado;
  int valor_contador;
}led;

struct{
  unsigned int estado;
}menu_serial;



struct{
  unsigned int estado;
}comunicacion_serial;

bool ledState = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  menu_serial.estado = 0;
  led.valor_contador = 1000;
}

void loop() {
  // put your main code here, to run repeatedly:

  currentMillis = millis();

   if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

  // COntadores.
   if(led.contador >= 1){
    led.contador--;
   }
   else{
      led.contador = led.valor_contador;
      led.flag_contador = HIGH;
      led.estado = (led.estado) ? LOW : HIGH;
   }

    // Tareas.
    if(paquete_disponible() && !flag_desempaquetado){
        time_timeout = millis() + 1000;
        desempaquetar();
       flag_desempaquetado = true;
       procesar_paquete();
        comando = "";
      cant_datos = "";
       datos = "";
     }

      // Tarea led.
     if(led.flag_contador && !menu_serial.estado){
      digitalWrite(13, led.estado);
      led.flag_contador = LOW;
     }
  }
  
  if(Serial.available() > 0){
       recibir_paquete() ;
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
   
  while(paquete[n] != ','|| millis() >= time_timeout){
    comando += paquete[n];
    n++;
  }
  
  n++;
 
  while(paquete[n] != ',' || millis() >= time_timeout){
    cant_datos += paquete[n];
    n++;
  }

  n++;
  
  while(paquete[n] != '$' || millis() >= time_timeout){
    datos += paquete[n];
    n++;
  }
  
}

void procesar_paquete(){
  if(comando == "INI"){
    // Si se envio el comando INIT se pasa a modo recepcion.
    Serial.write(":OK,-,$");
    menu_serial.estado = 1;
  }
  else if(comando == "RVA"){
    // Se envian las variables del sistema si este esta en modo recepcion.
    if(menu_serial.estado == 1){
      Serial.write(":OK,-,$\n");
      Serial.write(":DAT,123 123 4,$");
    }
  }
  else if(comando == "EXT"){
    // Si se recibe el comando salir, se sale del modo recepcion.
    if(menu_serial.estado != 0){
    menu_serial.estado = 0;
    Serial.println("Fin de comunicacion");
    }
    else{
      Serial.println("Comuniacion no iniciada");
    }
  }
  else if(comando == "MVA"){
    // Si se recibe el comando para modificar datos, se esperan los mismos.
    if(menu_serial.estado == 1){
      Serial.write(":OK,-,$\n");
      menu_serial.estado = 2;
    }
  }
  else if(comando == "RCV"){
    // Se reciben los datos si se los estan esperando.
    if(menu_serial.estado == 2){
      Serial.write("Datos recividos:");
      Serial.println(datos);
      modificar_variables();
      menu_serial.estado = 1;
    }
  }
  else{
    Serial.println("Comando incorrecto");
  }
}

void modificar_variables(){
  char *pch; 
  char temp[10];
  datos.toCharArray(temp,10);
  pch = strtok (temp," ,.-");
  led.valor_contador = atoi(pch);
}
