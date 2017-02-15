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


struct flashRoutine_type{
  int repeat;     // max is 5
  int pauses[5];  // time after activating
  int hold[5];    // time while activated
};

struct flashRoutines_type{
  flashRoutine_type flashRoutine[3]; // for each relay
};

#define TEST_ROUTINES 1
#define START_ROUTINE_IDX 1

#define NUM_ROUTINES 10
flashRoutines_type flashRoutines[NUM_ROUTINES];


int curRoutineIdx; // for testing only
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
    for (int j = 0; j < 3; j++) {
      flashRoutines[i].flashRoutine[j].repeat = 1;
      for (int k = 0; k < 5; k++) {
        flashRoutines[i].flashRoutine[j].pauses[k] = 0;
        flashRoutines[i].flashRoutine[j].hold[k] = 1000;
      }
    }
  }

  // set up the varying routines we will perform

  // 0 is default

  // 1 is double flash
  flashRoutines[1].flashRoutine[2].repeat = 2;
  flashRoutines[1].flashRoutine[2].pauses[0] = 100;
  flashRoutines[1].flashRoutine[2].hold[0] = 400;
  flashRoutines[1].flashRoutine[2].hold[1] = 400;

  // 2
  flashRoutines[2].flashRoutine[0].repeat    = 2;
  flashRoutines[2].flashRoutine[0].hold[0]   = 800;
  flashRoutines[2].flashRoutine[0].pauses[0] = 800;
  flashRoutines[2].flashRoutine[0].hold[1]   = 800;
  flashRoutines[2].flashRoutine[0].pauses[1] = 800;
  flashRoutines[2].flashRoutine[1].repeat    = 1;
  flashRoutines[2].flashRoutine[1].hold[0]   = 500;
  flashRoutines[2].flashRoutine[1].pauses[0] = 2000;
  flashRoutines[2].flashRoutine[2].repeat    = 4;
  flashRoutines[2].flashRoutine[2].hold[0]   = 200;
  flashRoutines[2].flashRoutine[2].pauses[0] = 800;
  flashRoutines[2].flashRoutine[2].hold[1]   = 200;
  flashRoutines[2].flashRoutine[2].pauses[1] = 2000;
  flashRoutines[2].flashRoutine[2].hold[2]   = 200;
  flashRoutines[2].flashRoutine[2].pauses[2] = 200;
  flashRoutines[2].flashRoutine[2].hold[3]   = 200;
  flashRoutines[2].flashRoutine[2].pauses[3] = 200;

  curRoutineIdx = START_ROUTINE_IDX;

  if (TEST_ROUTINES == 1) {
    runRoutine(curRoutineIdx++);
    delay(2000);
    runRoutine(curRoutineIdx++);
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


  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < flashRoutines[routine_idx].flashRoutine[i].repeat; j++) {

      if (i == 0) {
        Serial.print(millis()/1000);
        Serial.println("turn on first relay"); 
        digitalWrite(mhPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW
      }
      else if (i == 1) {
        Serial.print(millis()/1000);
        Serial.println("turn on second relay"); 
        digitalWrite(mhPin2, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(mhPin1, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW
      }
      else if (i == 2) {
        Serial.print(millis()/1000);
        Serial.println("turn on third relay"); 
        digitalWrite(mhPin3, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(mhPin1, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW 
      }

      // hold the light on
      delay(flashRoutines[routine_idx].flashRoutine[i].hold[j]);

      // turn everything off          
      digitalWrite(mhPin1, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW

      delay(flashRoutines[routine_idx].flashRoutine[i].pauses[j]);

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
    
      // when testing
      routine_idx = curRoutineIdx;
      curRoutineIdx++;
      if (curRoutineIdx >= NUM_ROUTINES) {
        curRoutineIdx = 0;
      }

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
