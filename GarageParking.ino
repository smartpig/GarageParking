#include <NewPing.h>            //include the NewPing library

//I/O pins
int bLED1 = 4;
int bLED2 = 5;
int bLED3 = 6;
int bLED4 = 7;
int gLED = 8;
int rLED = 9; 
int buttonPin = 10;
int echo = 11;
int trigger = 12;



//values to keep track of
int distance = 0;
int set = 0;
int upper = 0;             
int lower = 0;
int range = 3;                                   //range is +/- so it is double the value in inches
int count = 0;
boolean lastButton = LOW;          //keep track of button status
boolean currentButton = LOW;
int previous = 0;                              //keep track of distances to identify when car is parked
int current = 0;

NewPing sonar(0, 1, 500);       //constructor for NewPing sonar(triggerPin, echoPin, max_CM)

void setup()
{
pinMode(bLED1, OUTPUT);
pinMode(bLED2, OUTPUT);
pinMode(bLED3, OUTPUT);
pinMode(bLED4, OUTPUT);
pinMode(gLED, OUTPUT);
pinMode(rLED, OUTPUT);
pinMode(buttonPin, INPUT);
}
void loop()
{
  distance = sonar.ping_in();                  //detect distance in inches
  if(distance == 0) distance = 500;            //if no distance is read, set at max distance
  current = distance;                          //to check how long car has been parked in order to enter power save
 
  delay(100);                                  //Slow program down - save battery?
 
  currentButton = debounce(lastButton);        //reads button status and performs debounce, fixes unexpected button behavior
 
  if(lastButton == LOW && currentButton == HIGH)          //when button pushed, set desired distance for parking
  {
    set = distance;                                //new variable for distance measured
    flashGreen();                                  //flicker green LED 3 times
    flashGreen();
    flashGreen();
  }
 
  lastButton = currentButton;            //reset button status
 
  upper = set + range;                        //  +/- inch tolerance range 
  lower = set - range; 
 
  if(distance <= upper && distance >= lower)            //within set area, stop!
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
  if(distance > upper)
  { 
    count = 0;                            //if car moves slow enough, it may never reset the count in above code, we reset again, just in case
                                       
    if(distance >= set+120)              //sensor becomes innacurate at distances much further than 120 inches
    {                                                   //no car in garage turn LED off 
      off();
    }                                              
    if(distance < set+120 && distance > set+75)                  //car is detected
    {                                                            //if distance is less than set + 120 inches and greater than set +75 inches
      blue1();
     }
    if(distance <= set+75 && distance > set+50)                  //wihtin 75-50 inches of set distance, getting closer
    { 
      blue2();
    } 
    if(distance <= set+50 && distance > set+20)                  //wihtin 50-25 inches of set distance, almost there
    { 
      blue3();
    } 
    if(distance <= set+20 && distance > set+3)                  //wihtin 25-6 inches of set distance, get ready to stop
    { 
      blue4();
    } 

  }
  
  if(distance < lower)                                                           //car is too close, you must back up
  {
    count = 0;                                                                         //same situation for if car moves very slowly
    flashRed();
  }
 
  previous = distance;                                                        //update distances
}                                                                             //end of loop

//debounce method to correct for voltage spikes that cause unexpected behavior
boolean debounce(boolean last)
{
  boolean current = digitalRead(buttonPin);
  if (last != current)
  {
    delay(5);
    current = digitalRead(buttonPin);
  }
  return current;
}

//color methods;
void green()
{
   digitalWrite(gLED, LOW);                 //because of pull-up resistor circuit, code is inverted 
}
void red()
{
  digitalWrite(rLED, LOW);
}
void blue1()
{
  digitalWrite(bLED1, LOW); 
}
void blue2()
{
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
}
void blue3()
{
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
}
void blue4()
{
  digitalWrite(bLED1, LOW); 
  digitalWrite(bLED2, LOW); 
  digitalWrite(bLED3, LOW); 
  digitalWrite(bLED4, LOW); 
}
void flashRed()
{
  digitalWrite(rLED, LOW);                     //flash red LED
  delay(100);
  digitalWrite(rLED, HIGH);
  delay(100);
}
void flashGreen()
{
  digitalWrite(gLED, LOW);                  //flicker green LED when button pushed
  delay(50);
  digitalWrite(gLED,HIGH);
  delay(50);
}
void off()
{
  digitalWrite(gLED, HIGH);                  //turn off the LED
  digitalWrite(rLED, HIGH);
  digitalWrite(bLED1, HIGH);  
  digitalWrite(bLED2, HIGH);  
  digitalWrite(bLED3, HIGH);  
  digitalWrite(bLED4, HIGH);  
}
