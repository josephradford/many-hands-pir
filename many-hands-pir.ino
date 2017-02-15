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

  int outputState = 0;

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

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);

    // turn on first one
    
  digitalWrite(mhPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW
  outputState = 1;
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

      // generate a random pattern for turning on relays in sequence, and flash?
      int pauses[20]; // generate maximum number of pauses possible

      int hold[20]; // how long to hold each one
      int repeat[3]; // how many times to repeat each one
      // repeat is only successful when 7, 4 returned
      repeat[0] = (int)(random(6,7) / random(4,6)); 
      repeat[1] = (int)(random(6,7) / random(4,6)); 
      repeat[2] = (int)(random(6,7) / random(3,6)); // bit more likely

      int divider = (int)(random(6,7) / random(3,6));

      for(int i = 0; i < 20; i++) {
        pauses[i] = random(0, 200) / divider;
        hold[i] = random(800, 3000) / divider;
      }

      int ctr = 0;
      for(int i = 0; i < 3; i++) {
        for(int j = 0; j < repeat[i]; j++) {

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
          delay(hold[ctr]);

          // turn everything off          
          digitalWrite(mhPin1, LOW);    // turn the LED off by making the voltage LOW
          digitalWrite(mhPin2, LOW);    // turn the LED off by making the voltage LOW
          digitalWrite(mhPin3, LOW);    // turn the LED off by making the voltage LOW

          delay(pauses[ctr]);

          ctr++;
          
        }
      }


      
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
