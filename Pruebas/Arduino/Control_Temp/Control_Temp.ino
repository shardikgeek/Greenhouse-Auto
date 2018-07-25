// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


#define MAX_TEMP 22
#define MIN_TEMP 12
#define TEMP_OBJETIVO 17

int hum_ext;
int temp_ext;
int hum_int;
int temp_int;

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(4,DHTTYPE);

void leer_dht();
void leer_dht2();
void imprimir_ht();

void setup() {
  pinMode(A5,INPUT);
  Serial.begin(9600);

  dht.begin();
  dht2.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  leer_dht();
  leer_dht2();
  imprimir_ht();

  if(temp_int == TEMP_OBJETIVO)
    Serial.println("Todo apagado");
  else if(temp_int < TEMP_OBJETIVO){
    Serial.println("Prender Calentador");
  }
  else if(temp_int > TEMP_OBJETIVO && temp_ext > MIN_TEMP)
    Serial.println("Encender ventilador");
  else if(temp_int > TEMP_OBJETIVO && temp_ext <= MIN_TEMP)
    Serial.println("Todo apagado");

  

  

 
   delay(2000);

}

void leer_dht(){
  
  hum_ext = dht.readHumidity();
  temp_ext = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(hum_ext) || isnan(temp_ext)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void leer_dht2(){

  hum_int = dht2.readHumidity();
  temp_int = dht2.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(hum_int) || isnan(temp_int)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void imprimir_ht(){
  Serial.print("temperatura interna: ");
  Serial.println(temp_int);
  Serial.print("temperatura externa: ");
  Serial.println(temp_ext);
  Serial.println("-----------");
  
}


