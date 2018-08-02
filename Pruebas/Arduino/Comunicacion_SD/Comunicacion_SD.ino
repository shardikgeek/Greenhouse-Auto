#include <SPI.h>
#include <SD.h>
#include "DHT.h"


/*Pines usados
 * SD:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 * 
 * DHT11:
 * Señal - pin 2
  */

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

File fp;
unsigned long tiempo;
DHT dht(DHTPIN, DHTTYPE);
int temp;

void carga_archivos();
void leer_dht();

void setup() {
  Serial.begin(9600);
  dht.begin();

  if(!SD.begin(4)){
    Serial.println("Error al abrir SD.");
    return;
  }
  SD.remove("Hola.csv");
  carga_archivos();
}

void loop() {
  bool flag = false;
  if(Serial.available() > 0){
    if(!flag){
      flag = true;
    }
    else{
      flag = false;
    }
  }

  if(!flag){
    tiempo = millis()/1000;
    leer_dht();
    carga_archivos();
    delay(1000);
  }
}

void carga_archivos(){
    if(SD.exists("Hola.csv") == 0){
      fp = SD.open("Hola.csv",FILE_WRITE);

      fp.println("X;Y");
  }
  
  else{
     fp = SD.open("Hola.csv",FILE_WRITE);
    
     fp.print(tiempo);
     fp.print(";");
     fp.println(temp);
     Serial.println(temp);
  }
fp.close();
}

void leer_dht(){
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  temp = t;
}
