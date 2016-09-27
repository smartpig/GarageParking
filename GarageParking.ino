//I/O pins
int bLED1 = 4;
int bLED2 = 5;
int bLED3 = 6;
int bLED4 = 7;
int gLED = 8;
int rLED = 9; 
int buttonPin = 3;

//Digital pin 11 for reading in the pulse width from the MaxSonar device.

//This variable is a constant because the pin will not change throughout execution of this code.

const int pwPin = 11;



//variables needed to store values

long pulse, inches, cm;


//values to keep track of
int set = 20; // target distance of parked car - change to make closer or further from wall
int upper = 24;    //upper range of parking target         
int lower = 16;    // lower range of parking target
int count = 0;
int range = 4;
int previous = 0;                              //keep track of distances to identify when car is parked
int current = 0;
int buttonState = 0;          //keep track of button status
int lastButtonState = 0;


void setup()
{
pinMode(bLED1, OUTPUT);
pinMode(bLED2, OUTPUT);
pinMode(bLED3, OUTPUT);
pinMode(bLED4, OUTPUT);
pinMode(gLED, OUTPUT);
pinMode(rLED, OUTPUT);
pinMode(buttonPin, INPUT);
Serial.begin(9600);
}


void loop()
{
  pinMode(pwPin, INPUT);



  //Used to read in the pulse that is being sent by the MaxSonar device.

  //Pulse Width representation with a scale factor of 147 uS per Inch.



  pulse = pulseIn(pwPin, HIGH);

  //.039 mm to an inch

  inches = pulse * .039;

  //change inches to centimetres

  cm = pulse * .1;

  Serial.print(pulse);

  Serial.print("uS, ");

  Serial.print(inches);

  Serial.print("in, ");

  Serial.print(cm);

  Serial.print("cm");

  Serial.print(" Count: ");

  Serial.print(count);

  Serial.print(" Set: ");

  Serial.print(set);

  Serial.println();
  
  current = inches;                          //to check how long car has been parked in order to enter power save
 
  delay(100);                                  //Slow program down - save battery?
 
  
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, Change Set distance
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // went from off to on:
      set = (pulseIn(pwPin, HIGH) * .039);
      Serial.print("New Set Point:  ");
      Serial.println(set);
      Serial.print("Changing Upper from: ");
      Serial.print(upper);
      Serial.print(" to: ");
      Serial.println(set + range);
      Serial.print("Changing Lower from: ");
      Serial.print(lower);
      Serial.print(" to: ");
      Serial.println(set - range);
      upper = set + range;                        //  +/- inch tolerance range 
      lower = set - range; 
      flashGreen();
      delay(2000);
    } 
    
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
 
  
if(inches <= upper && inches >= lower)            //within set area, stop!
  {
    //distance fluctuates some resulting in false movement detection
    if(current == previous || current == previous + 1 || current == previous - 1 )
    {
      count++;                           //increase count if car has not moved
    }
    else
    {
      count = 0;                          //resent count if there is a significat change in distance
    }
   
    // if distance doest change for 250 cycles (30 sec) turn off LED -> power saving mode
    if(count >= 250)
    {
      off();
      count = 250;                        //dont increment forever
    }
    else
    {
      green();                                   // within range, stop. green LEDs on
    }
   
  }
  if(inches > upper)
  { 
    count = 0;                            //if car moves slow enough, it may never reset the count in above code, we reset again, just in case
                                       
    if(inches >= set+120)              //sensor becomes innacurate at distances much further than 120 inches
    {                                                   //no car in garage turn LED off 
      off();
    }                                              
    if(inches < set+100 && inches > set+75)                  //car is detected
    {                                                            //if distance is less than set + 120 inches and greater than set +75 inches
      blue1();
     }
    if(inches <= set+75 && inches > set+50)                  //wihtin 75-50 inches of set distance, getting closer
    { 
      blue2();
    } 
    if(inches <= set+50 && inches > set+20)                  //wihtin 50-25 inches of set distance, almost there
    { 
      blue3();
    } 
    if(inches <= set+20 && inches > set+4)                  //wihtin 25-6 inches of set distance, get ready to stop
    { 
      blue4();
    } 

  }
  
  if(inches < lower)                                                           //car is too close, you must back up
  {
    count = 0;                                                                         //same situation for if car moves very slowly
    flashRed();
  }
 
  previous = inches;                                                        //update distances
}                                                                             //end of loop


//color methods;
void green()
{
  digitalWrite(gLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW);
   digitalWrite(gLED, HIGH);                  
}
void red()
{
  digitalWrite(gLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW);
  digitalWrite(rLED, HIGH);
}
void blue1()
{
  digitalWrite(gLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW);
  digitalWrite(bLED1, HIGH); 
}
void blue2()
{
  digitalWrite(gLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW);
  digitalWrite(bLED1, HIGH); 
  digitalWrite(bLED2, HIGH); 
}
void blue3()
{
  digitalWrite(gLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW);
  digitalWrite(bLED1, HIGH); 
  digitalWrite(bLED2, HIGH); 
  digitalWrite(bLED3, HIGH); 
}
void blue4()
{
  digitalWrite(gLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW);
  digitalWrite(bLED1, HIGH); 
  digitalWrite(bLED2, HIGH); 
  digitalWrite(bLED3, HIGH); 
  digitalWrite(bLED4, HIGH); 
}
void flashRed()
{
  digitalWrite(gLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW); 
  digitalWrite(rLED, HIGH);                     //flash red LED
  delay(100);
  digitalWrite(rLED, LOW);
  delay(100);
}

void flashGreen()
{
  digitalWrite(rLED, LOW);
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW); 
  digitalWrite(gLED, HIGH);                     //flash green LED
  delay(100);
  digitalWrite(gLED, LOW);
  delay(100);
  digitalWrite(gLED, HIGH);                     //flash green LED
  delay(100);
  digitalWrite(gLED, LOW);
  delay(100);
  digitalWrite(gLED, HIGH);                     //flash green LED
  delay(100);
  digitalWrite(gLED, LOW);
  delay(100);
  digitalWrite(gLED, HIGH);                     //flash green LED
  delay(100);
  digitalWrite(gLED, LOW);
  delay(100);
}

void off()
{
  digitalWrite(gLED, LOW);                  //turn off the LED
  digitalWrite(rLED, LOW);
  digitalWrite(bLED1, LOW);  
  digitalWrite(bLED2, LOW);  
  digitalWrite(bLED3, LOW);  
  digitalWrite(bLED4, LOW);  
}
