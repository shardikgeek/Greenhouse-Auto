// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

#define R_FAN 7
#define R_CAL 8
#define HISTERESIS 1  

#define TEMP_OBJ_FAN 18
#define TEMP_MAX_FAN TEMP_OBJ_FAN+HISTERESIS
#define TEMP_MIN_FAN  TEMP_OBJ_FAN-HISTERESIS

#define TEMP_OBJ_CAL 12
#define TEMP_MAX_CAL TEMP_OBJ_CAL+HISTERESIS
#define TEMP_MIN_CAL  TEMP_OBJ_CAL-HISTERESIS

int hum_ext;
int temp_ext;
int hum_int;
int temp_int;

int temp;
bool fan_flag = false;
bool cal_flag = false;

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
void control_temp();

void setup() {
  pinMode(R_FAN,OUTPUT);
  pinMode(R_CAL,OUTPUT);
  digitalWrite(R_FAN,HIGH);
  digitalWrite(R_CAL,HIGH);
  dht2.begin();
  Serial.begin(9600);
}

void loop() {
  // Wait a few seconds between measurements.
  leer_dht2();
  control_temp();
  imprimir_ht();
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

  hum_int = dht.readHumidity();
  temp_int = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(hum_int) || isnan(temp_int)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void imprimir_ht(){
  Serial.print("temperatura interna: ");
  Serial.println(temp_int);
}

void control_temp(){

  if(temp_int > TEMP_MAX_FAN && !fan_flag){
    digitalWrite(R_FAN,LOW);
    fan_flag = true;
  }
  else if(temp_int <= TEMP_MIN_FAN && fan_flag){
    digitalWrite(R_FAN,HIGH);
    fan_flag = false;
  }

  if(temp_int < TEMP_MIN_CAL && !cal_flag){
    digitalWrite(R_CAL,LOW);
    cal_flag = true;
  }
  else if(temp_int >= TEMP_MAX_CAL && cal_flag){
    digitalWrite(R_CAL,HIGH);
    cal_flag = false;
  }
}


