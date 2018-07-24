#define R_FAN 7
#define R_CAL 8
#define PIN_TEMP A0
#define HISTERESIS 50

#define TEMP_OBJ_FAN 700
#define TEMP_MAX_FAN TEMP_OBJ_FAN+HISTERESIS
#define TEMP_MIN_FAN  TEMP_OBJ_FAN-HISTERESIS

#define TEMP_OBJ_CAL 300
#define TEMP_MAX_CAL TEMP_OBJ_CAL+HISTERESIS
#define TEMP_MIN_CAL  TEMP_OBJ_CAL-HISTERESIS

int temp;
bool fan_flag = false;
bool cal_flag = false;

void control_temp();

void setup() {
  // put your setup code here, to run once:
  pinMode(R_FAN,OUTPUT);
  pinMode(R_CAL,OUTPUT);
  pinMode(PIN_TEMP,INPUT);
  digitalWrite(R_FAN,HIGH);
  digitalWrite(R_CAL,HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  temp = analogRead(PIN_TEMP);
  control_temp();
  Serial.print("TEMP: ");
  Serial.println(temp);
  delay(10);

}

void control_temp(){

  if(temp > TEMP_MAX_FAN && !fan_flag){
    digitalWrite(R_FAN,LOW);
    fan_flag = true;
  }
  else if(temp <= TEMP_MIN_FAN && fan_flag){
    digitalWrite(R_FAN,HIGH);
    fan_flag = false;
  }

  if(temp < TEMP_MIN_CAL && !cal_flag){
    digitalWrite(R_CAL,LOW);
    cal_flag = true;
  }
  else if(temp >= TEMP_MAX_CAL && cal_flag){
    digitalWrite(R_CAL,HIGH);
    cal_flag = false;
  }
  
}

