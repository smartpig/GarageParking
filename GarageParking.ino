//I/O pins
int bLED1 = 4;
int bLED2 = 5;
int bLED3 = 6;
int bLED4 = 7;
int gLED = 8;
int rLED = 9; 

//Digital pin 11 for reading in the pulse width from the MaxSonar device.

//This variable is a constant because the pin will not change throughout execution of this code.

const int pwPin = 11;



//variables needed to store values

long pulse, inches, cm;


//values to keep track of
int settarget = 12; // target distance of parked car - change to make closer or further from wall
int upper = 14;    //upper range of parking target         
int lower = 10;    // lower range of parking target
int count = 0;
int previous = 0;                              //keep track of distances to identify when car is parked
int current = 0;


void setup()
{
pinMode(bLED1, OUTPUT);
pinMode(bLED2, OUTPUT);
pinMode(bLED3, OUTPUT);
pinMode(bLED4, OUTPUT);
pinMode(gLED, OUTPUT);
pinMode(rLED, OUTPUT);
Serial.begin(9600);
}


void loop()
{
  pinMode(pwPin, INPUT);



  //Used to read in the pulse that is being sent by the MaxSonar device.

  //Pulse Width representation with a scale factor of 147 uS per Inch.



  pulse = pulseIn(pwPin, HIGH);

  //147uS per inch

  inches = pulse / 147;

  //change inches to centimetres

  cm = inches * 2.54;



  Serial.print(inches);

  Serial.print("in, ");

  Serial.print(cm);

  Serial.print("cm");

  Serial.println();
  
  current = inches;                          //to check how long car has been parked in order to enter power save
 
  delay(100);                                  //Slow program down - save battery?
 
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
    if(inches < set+120 && inches > set+75)                  //car is detected
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
    if(inches <= set+20 && inches > set+2)                  //wihtin 25-6 inches of set distance, get ready to stop
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

void off()
{
  digitalWrite(gLED, HIGH);                  //turn off the LED
  digitalWrite(rLED, HIGH);
  digitalWrite(bLED1, HIGH);  
  digitalWrite(bLED2, HIGH);  
  digitalWrite(bLED3, HIGH);  
  digitalWrite(bLED4, HIGH);  
}
