//Stepper Motor Output
static const byte PUL = A0;
static const byte DIR = A1;
static const byte ENA = A2;
static const int stepDelay = 500; //Microsecond (400 minimum)

//Encoder Input
static const byte OE = 2;
static const byte SEL0 = 3;
static const byte SEL1 = 4;
static const byte ED0 = 5;
static const byte ED1 = 6;
static const byte ED2 = 7;
static const byte ED3 = 8;
static const byte CLOCKOUT = 9;
static const byte ED4 = 10;
static const byte ED5 = 11;
static const byte ED6 = 12;
static const byte ED7 = 13;
static const byte RESET = A5;

//Stepper Input

int encoderCorrection = 10;
int encoderSteps = 0;
long stepperSteps = 0;
int corrections = 0;
unsigned long lastCheck = 0;
int interval = 3000;
long input = 0;

void setup(){
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(OE, OUTPUT);
  pinMode(SEL0, OUTPUT);
  pinMode(SEL1, OUTPUT);
  pinMode(ED0, INPUT);
  pinMode(ED1, INPUT);
  pinMode(ED2, INPUT);
  pinMode(ED3, INPUT);
  pinMode(ED4, INPUT);
  pinMode(ED5, INPUT);
  pinMode(ED6, INPUT);
  pinMode(ED7, INPUT);
  pinMode(RESET, OUTPUT);
  
  Serial.begin(9600);
  digitalWrite(ENA, LOW);
  delay(500);
  
  digitalWrite(RESET, LOW);
  delay(10);
  digitalWrite(RESET, HIGH);
  
//pinMode (CLOCKOUT, OUTPUT);
//  TCCR1A = bit (COM1A0);
//  TCCR1B = bit (WGM12) | bit (CS10);
//  OCR1A =  0;
DDRB = _BV(DDB1);                  //set OC1A/PB1 as output (Arduino pin D9, DIP pin 15)
  TCCR1A = _BV(COM1A0);              //toggle OC1A on compare match
  OCR1A = 0;                         //top value for counter
  TCCR1B = _BV(WGM12) | _BV(CS10);
  digitalWrite(OE, HIGH);
}

void loop(){
  /*//while (Serial.available() == 0);
  //stepperSteps = stepperSteps + input;
  if (Serial.available() > 0){input = Serial.parseInt();}
  if(input > 0){
    Serial.println("stepping forward");
    stepperSteps = stepperSteps + input;
    stepper(input, 1);
  }else if(input < 0){
    Serial.println("stepping backwards");
    input = input * -1;
    stepperSteps = stepperSteps - input;
    stepper(input, 0);
  }
  input = 0;
  
  if(correct()){
  }else{
    Serial.println("error");
  }
  //Serial.println(lastCheck + interval);
  if(millis() - lastCheck >= interval){readEncoder(); lastCheck = millis();}
  */

  stepper(20, 0);
  delay(500);
  stepper(20,1);
}

boolean stepper(long steps, boolean dir){
  digitalWrite(DIR, dir);
  //delay(1);
  for(int i = 1; i <= steps; i++){
    digitalWrite(PUL, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(PUL, LOW);
    delayMicroseconds(stepDelay);
  }
  //delay(500);
  readEncoder();
  return true;
}

void readEncoder(){
  digitalWrite(OE, LOW);
  encoderSteps = 0;
  
  digitalWrite(SEL0, LOW);
  digitalWrite(SEL1, HIGH);
  delay(10);
  byte byte1 =
    digitalRead(ED0)|
    (digitalRead(ED1)<<1)|
    (digitalRead(ED2)<<2)|
    (digitalRead(ED3)<<3)|
    (digitalRead(ED4)<<4)|
    (digitalRead(ED5)<<5)|
    (digitalRead(ED6)<<6)|
    (digitalRead(ED7)<<7);
    
  digitalWrite(SEL0, HIGH);
  digitalWrite(SEL1, HIGH);
  delay(10);
  byte byte2 = 
    digitalRead(ED0)|
    (digitalRead(ED1)<<1)|
    (digitalRead(ED2)<<2)|
    (digitalRead(ED3)<<3)|
    (digitalRead(ED4)<<4)|
    (digitalRead(ED5)<<5)|
    (digitalRead(ED6)<<6)|
    (digitalRead(ED7)<<7);
  
  digitalWrite(SEL0, LOW);
  digitalWrite(SEL1, LOW);
  delay(10);  
  byte byte3 = 
    digitalRead(ED0)|
    (digitalRead(ED1)<<1)|
    (digitalRead(ED2)<<2)|
    (digitalRead(ED3)<<3)|
    (digitalRead(ED4)<<4)|
    (digitalRead(ED5)<<5)|
    (digitalRead(ED6)<<6)|
    (digitalRead(ED7)<<7);
  
  digitalWrite(SEL0, HIGH);
  digitalWrite(SEL1, LOW);
  delay(10);  
  byte byte4 = 
    digitalRead(ED0)|
    (digitalRead(ED1)<<1)|
    (digitalRead(ED2)<<2)|
    (digitalRead(ED3)<<3)|
    (digitalRead(ED4)<<4)|
    (digitalRead(ED5)<<5)|
    (digitalRead(ED6)<<6)|
    (digitalRead(ED7)<<7);
    
  encoderSteps = byte1;
  encoderSteps = encoderSteps <<8;
  encoderSteps = encoderSteps + byte2;
  encoderSteps = encoderSteps <<8;
  encoderSteps = encoderSteps + byte3;
  encoderSteps = encoderSteps <<8;
  encoderSteps = (encoderSteps + byte4)/7.2;
  
  digitalWrite(OE, HIGH);
  Serial.print(encoderSteps); Serial.print("  "); Serial.println(stepperSteps); 
}

boolean correct(){
  //Serial.println("correcting");
  if(encoderSteps + encoderCorrection < stepperSteps){
    Serial.println("not far enough");
    while(encoderSteps + encoderCorrection < stepperSteps){
      //Serial.println("not far enough");
      stepper(stepperSteps - encoderSteps,true);
      readEncoder();
      corrections++;
    }
  }
  else if(encoderSteps - encoderCorrection > stepperSteps){
    Serial.println("too far");
    while(encoderSteps - encoderCorrection > stepperSteps){
      //Serial.println("too far");
      stepper(encoderSteps - stepperSteps,false);
      readEncoder();
      corrections++;
    }
  }
  else{
    corrections = 0;
  }
  //Serial.println("exit correction");
  if(corrections < 10)return true;
  return false;
}
