int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30; 

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 0;  

boolean lockLow = true;
boolean takeLowTime;  

int mhPin1 = 4;
int mhPin2 = 5;
int mhPin3 = 6;

#define MAX_FLASHES 10
#define TEST_ROUTINES 1
#define START_ROUTINE_IDX 0
#define NUM_ROUTINES 10

struct flash_type{
  int port;       // 0 is red, 1 is orange, 2 is yellow
  int hold;       // time while activated
  int pause;      // time after activating finished
};

struct flashRoutine_type{
  flash_type flash[MAX_FLASHES]; // max length of a routine
};

flashRoutine_type flashRoutines[NUM_ROUTINES];


void setup() {
  // put your setup code here, to run once:
  pinMode(mhPin1, OUTPUT);
  pinMode(mhPin2, OUTPUT);
  pinMode(mhPin3, OUTPUT);

  // turn everything off to start
  digitalWrite(mhPin1, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW

  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);



  // set up the defaults for anything that gets forgotten or doesn't need to be changed
  for(int i = 0; i < NUM_ROUTINES; i++) {
    for (int j = 0; j < MAX_FLASHES; j++) {
      flashRoutines[i].flash[j].port = 0;
      flashRoutines[i].flash[j].pause = 0;
      flashRoutines[i].flash[j].hold= 0;
      
    }
  }

  // set up the varying routines we will perform

  setupRoutines();  

  if (TEST_ROUTINES == 1) {
    for (int i = START_ROUTINE_IDX; i < NUM_ROUTINES; i++) {
      runRoutine(i);
      delay(2000);
    }
  }

  
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void runRoutine(int routine_idx) {

  for(int i = 0; i < MAX_FLASHES; i++) {
    if (flashRoutines[routine_idx].flash[i].hold == 0) {
      // skip
    }
    else {
      if (flashRoutines[routine_idx].flash[i].port == 0) {
        Serial.print(millis());
        Serial.println("turn on first relay"); 
        digitalWrite(mhPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW
      }
      else if (flashRoutines[routine_idx].flash[i].port == 1) {
        Serial.print(millis());
        Serial.println("turn on second relay"); 
        digitalWrite(mhPin2, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(mhPin1, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW
      }
      else {
        Serial.print(millis());
        Serial.println("turn on third relay"); 
        digitalWrite(mhPin3, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(mhPin1, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW 
      }
  
      // hold the light on
      delay(flashRoutines[routine_idx].flash[i].hold);
  
      // turn everything off    
        Serial.println("turn everything off");       
      digitalWrite(mhPin1, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW
  
      delay(flashRoutines[routine_idx].flash[i].pause);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(pirPin) == HIGH) {
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    if(lockLow){  
      //makes sure we wait for a transition to LOW before any further output is made:
      lockLow = false;            
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis()/1000);
      Serial.println(" sec"); 

      
      // seed radnom number generator
      randomSeed(analogRead(0));
    
      // select one of the constructed routines
      int routine_idx = random(0,NUM_ROUTINES-1);
    
      runRoutine(routine_idx);      
    }
    takeLowTime = true;
  }
  
  if(digitalRead(pirPin) == LOW){       
    digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
    
    if(takeLowTime){
      lowIn = millis();          //save the time of the transition from high to LOW
      takeLowTime = false;       //make sure this is only done at the start of a LOW phase
    }
    //if the sensor is low for more than the given pause, 
    //we assume that no more motion is going to happen
    if(!lockLow && millis() - lowIn > pause){  
      //makes sure this block of code is only executed again after 
      //a new motion sequence has been detected
      lockLow = true;                        
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause)/1000);
      Serial.println(" sec");
      delay(50);
    }
  }  
}

void setupRoutines()
{
  /////////
flashRoutines[0].flash[0].port = 0;
flashRoutines[0].flash[0].hold = 900;
flashRoutines[0].flash[0].pause = 0;

flashRoutines[0].flash[1].port = 1;
flashRoutines[0].flash[1].hold = 900;
flashRoutines[0].flash[1].pause = 0;

flashRoutines[0].flash[2].port = 2;
flashRoutines[0].flash[2].hold = 1700;
flashRoutines[0].flash[2].pause = 0;


/////
flashRoutines[1].flash[0].port = 0;
flashRoutines[1].flash[0].hold = 293;
flashRoutines[1].flash[0].pause = 483;

flashRoutines[1].flash[1].port = 1;
flashRoutines[1].flash[1].hold = 328;
flashRoutines[1].flash[1].pause = 438;

flashRoutines[1].flash[2].port = 2;
flashRoutines[1].flash[2].hold = 1054;
flashRoutines[1].flash[2].pause = 635;

flashRoutines[1].flash[3].port = 2;
flashRoutines[1].flash[3].hold = 131;
flashRoutines[1].flash[3].pause = 0;



//////////
flashRoutines[2].flash[0].port = 0;
flashRoutines[2].flash[0].hold = 212;
flashRoutines[2].flash[0].pause = 555;

flashRoutines[2].flash[1].port = 1;
flashRoutines[2].flash[1].hold = 203;
flashRoutines[2].flash[1].pause = 1361;

flashRoutines[2].flash[2].port = 0;
flashRoutines[2].flash[2].hold = 140;
flashRoutines[2].flash[2].pause = 98;

flashRoutines[2].flash[3].port = 1;
flashRoutines[2].flash[3].hold = 140;
flashRoutines[2].flash[3].pause = 115;

flashRoutines[2].flash[4].port = 2;
flashRoutines[2].flash[4].hold = 168;
flashRoutines[2].flash[4].pause = 294;

flashRoutines[2].flash[5].port = 2;
flashRoutines[2].flash[5].hold = 177;
flashRoutines[2].flash[5].pause = 0;

////////
flashRoutines[3].flash[0].port = 0;
flashRoutines[3].flash[0].hold = 149;
flashRoutines[3].flash[0].pause = 186;

flashRoutines[3].flash[1].port = 1;
flashRoutines[3].flash[1].hold = 168;
flashRoutines[3].flash[1].pause = 472;

flashRoutines[3].flash[2].port = 0;
flashRoutines[3].flash[2].hold = 96;
flashRoutines[3].flash[2].pause = 79;

flashRoutines[3].flash[3].port = 1;
flashRoutines[3].flash[3].hold = 158;
flashRoutines[3].flash[3].pause = 222;

flashRoutines[3].flash[4].port = 0;
flashRoutines[3].flash[4].hold = 140;
flashRoutines[3].flash[4].pause = 134;

flashRoutines[3].flash[5].port = 1;
flashRoutines[3].flash[5].hold = 166;
flashRoutines[3].flash[5].pause = 402;

flashRoutines[3].flash[6].port = 0;
flashRoutines[3].flash[6].hold = 86;
flashRoutines[3].flash[6].pause = 80;

flashRoutines[3].flash[7].port = 1;
flashRoutines[3].flash[7].hold = 95;
flashRoutines[3].flash[7].pause = 71;

flashRoutines[3].flash[8].port = 2;
flashRoutines[3].flash[8].hold = 1000;
flashRoutines[3].flash[8].pause = 0;

////
flashRoutines[4].flash[0].port = 0;
flashRoutines[4].flash[0].hold = 132;
flashRoutines[4].flash[0].pause = 213;

flashRoutines[4].flash[1].port = 0;
flashRoutines[4].flash[1].hold = 132;
flashRoutines[4].flash[1].pause = 518;

flashRoutines[4].flash[2].port = 1;
flashRoutines[4].flash[2].hold = 78;
flashRoutines[4].flash[2].pause = 106;

flashRoutines[4].flash[3].port = 2;
flashRoutines[4].flash[3].hold = 132;
flashRoutines[4].flash[3].pause = 0;

//////
flashRoutines[5].flash[0].port = 0;
flashRoutines[5].flash[0].hold = 80;
flashRoutines[5].flash[0].pause = 80;

flashRoutines[5].flash[1].port = 1;
flashRoutines[5].flash[1].hold = 80;
flashRoutines[5].flash[1].pause = 80;

flashRoutines[5].flash[2].port = 2;
flashRoutines[5].flash[2].hold = 80;
flashRoutines[5].flash[2].pause = 80;

flashRoutines[5].flash[3].port = 0;
flashRoutines[5].flash[3].hold = 80;
flashRoutines[5].flash[3].pause = 80;

flashRoutines[5].flash[4].port = 1;
flashRoutines[5].flash[4].hold = 80;
flashRoutines[5].flash[4].pause = 80;

flashRoutines[5].flash[5].port = 2;
flashRoutines[5].flash[5].hold = 70;
flashRoutines[5].flash[5].pause = 200;

flashRoutines[5].flash[6].port = 1;
flashRoutines[5].flash[6].hold = 80;
flashRoutines[5].flash[6].pause = 80;

flashRoutines[5].flash[7].port = 2;
flashRoutines[5].flash[7].hold = 80;
flashRoutines[5].flash[7].pause = 0;



//////
flashRoutines[6].flash[0].port = 0;
flashRoutines[6].flash[0].hold = 222;
flashRoutines[6].flash[0].pause = 384;

flashRoutines[6].flash[1].port = 1;
flashRoutines[6].flash[1].hold = 212;
flashRoutines[6].flash[1].pause = 385;

flashRoutines[6].flash[2].port = 2;
flashRoutines[6].flash[2].hold = 202;
flashRoutines[6].flash[2].pause = 866;

flashRoutines[6].flash[3].port = 0;
flashRoutines[6].flash[3].hold = 168;
flashRoutines[6].flash[3].pause = 402;

flashRoutines[6].flash[4].port = 1;
flashRoutines[6].flash[4].hold = 194;
flashRoutines[6].flash[4].pause = 385;

flashRoutines[6].flash[5].port = 2;
flashRoutines[6].flash[5].hold = 194;
flashRoutines[6].flash[5].pause = 446;

flashRoutines[6].flash[6].port = 1;
flashRoutines[6].flash[6].hold = 141;
flashRoutines[6].flash[6].pause = 142;

flashRoutines[6].flash[7].port = 2;
flashRoutines[6].flash[7].hold = 167;
flashRoutines[6].flash[7].pause = 0;

////////////////
flashRoutines[7].flash[0].port = 0;
flashRoutines[7].flash[0].hold = 204;
flashRoutines[7].flash[0].pause = 447;

flashRoutines[7].flash[1].port = 1;
flashRoutines[7].flash[1].hold = 239;
flashRoutines[7].flash[1].pause = 430;

flashRoutines[7].flash[2].port = 2;
flashRoutines[7].flash[2].hold = 1539;
flashRoutines[7].flash[2].pause = 1377;

flashRoutines[7].flash[3].port = 0;
flashRoutines[7].flash[3].hold = 61;
flashRoutines[7].flash[3].pause = 0;

flashRoutines[7].flash[4].port = 1;
flashRoutines[7].flash[4].hold = 53;
flashRoutines[7].flash[4].pause = 0;

flashRoutines[7].flash[5].port = 2;
flashRoutines[7].flash[5].hold = 48;
flashRoutines[7].flash[5].pause = 0;


  ///////// repeat
flashRoutines[8].flash[0].port = 0;
flashRoutines[8].flash[0].hold = 900;
flashRoutines[8].flash[0].pause = 0;

flashRoutines[8].flash[1].port = 1;
flashRoutines[8].flash[1].hold = 900;
flashRoutines[8].flash[1].pause = 0;

flashRoutines[8].flash[2].port = 2;
flashRoutines[8].flash[2].hold = 1700;
flashRoutines[8].flash[2].pause = 0;

///////
flashRoutines[9].flash[0].port = 0;
flashRoutines[9].flash[0].hold = 105;
flashRoutines[9].flash[0].pause = 213;

flashRoutines[9].flash[1].port = 0;
flashRoutines[9].flash[1].hold = 131;
flashRoutines[9].flash[1].pause = 268;

flashRoutines[9].flash[2].port = 1;
flashRoutines[9].flash[2].hold = 122;
flashRoutines[9].flash[2].pause = 186;

flashRoutines[9].flash[3].port = 1;
flashRoutines[9].flash[3].hold = 123;
flashRoutines[9].flash[3].pause = 697;

flashRoutines[9].flash[4].port = 0;
flashRoutines[9].flash[4].hold = 78;
flashRoutines[9].flash[4].pause = 106;

flashRoutines[9].flash[5].port = 1;
flashRoutines[9].flash[5].hold = 78;
flashRoutines[9].flash[5].pause = 366;

flashRoutines[9].flash[6].port = 2;
flashRoutines[9].flash[6].hold = 78;
flashRoutines[9].flash[6].pause = 0;


}


