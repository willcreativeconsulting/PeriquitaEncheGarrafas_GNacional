#define motor_enable_IN1  2
#define motor_enable_IN2  3
#define motor_enable_IN3  4

#define TIME_ON_SEC 10*1000
#define TIME_OFF_SEC 25*1000
#define TIME_PAUSE_SEC 30*1000

#define MAX_CYCLE 50

static int timer_count = 0;

static unsigned long time_off;
static unsigned long timer;
static unsigned long timer_g;
static unsigned long counter_g;
static unsigned long cicle_g;
static unsigned long timer_local;

enum states 
{ 
  HALT,
  ON,
  OFF
};

static enum states motorState = HALT;


void setup() 
{    
  timer = millis();
  timer_g = millis();

  counter_g = 0;
  cicle_g = 0;

  time_off = TIME_OFF_SEC;
    
  //Serial that will be use to debug
  Serial.begin(9600);
     
  pinMode(motor_enable_IN1, OUTPUT);
  pinMode(motor_enable_IN2, OUTPUT);
  pinMode(motor_enable_IN3, OUTPUT);

  digitalWrite(motor_enable_IN1, 1);
  digitalWrite(motor_enable_IN2, 1);
  digitalWrite(motor_enable_IN3, 1);
}

  
void loop() 
{   
  timer_local = millis();
  
  switch(motorState)
  {
    case HALT:
      counter_g = 0;
      Serial.println("");
      Serial.print(cicle_g);
      Serial.print("-Motor OFF:");
      motorState = OFF;
      digitalWrite(motor_enable_IN1, 1);
      digitalWrite(motor_enable_IN2, 1);
      digitalWrite(motor_enable_IN3, 1);
      break;
    case ON:
      if(timer_local > timer + TIME_ON_SEC)
      {
        counter_g = 0;
        Serial.println("");
        Serial.print(cicle_g);
        Serial.print("-Motor OFF:");
        timer = millis();
        motorState = OFF;
        digitalWrite(motor_enable_IN1, 1);
        digitalWrite(motor_enable_IN2, 1);      
        digitalWrite(motor_enable_IN3, 1);
      }     
      break;
    case OFF:
      if(timer_local > timer + time_off)
      {
        counter_g = 0;
        cicle_g++;
        timer = millis();

        if((cicle_g > MAX_CYCLE) && (timer_count < 2))
        {
          timer_count++;
          time_off = TIME_PAUSE_SEC;

          Serial.println("");
          Serial.print(cicle_g);
          Serial.print("-Motor OFF :");
          
          Serial.println("");
          Serial.print("MAX CYCLE REACHED - Waiting 30s :");

          motorState = OFF;
        }
        else
        {
          motorState = ON;
          digitalWrite(motor_enable_IN1, 0);
          digitalWrite(motor_enable_IN2, 0);
          digitalWrite(motor_enable_IN3, 0);

          time_off = TIME_OFF_SEC;

          if(timer_count >= 2)
          {
            timer_count = 0;
            cicle_g = 0;
          }
          
          Serial.println("");
          Serial.print(cicle_g);
          Serial.print("-Motor ON :");
        }
      }     
      break;
  }

  if(timer_local > timer_g + 1000)
  {
    counter_g++;
    Serial.print(" ");
    Serial.print(counter_g);
    timer_g = millis();
  }
}


