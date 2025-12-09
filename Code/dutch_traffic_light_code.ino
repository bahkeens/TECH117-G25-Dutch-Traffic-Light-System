// NR = 10,NY = 9,NG = 8;
// WR = 13,WY = 12,WG = 11;
// SR = 4,SY = 3,SG = 2;
// ER = 7,EY = 6,EG = 5;
// Variables will change:
int color1 = 3, color2 = 3, color3 = 3, color4 = 3;  // used to choose signal state

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0,previousMillis1 = 0;  // will store last time LED was updated

// constants won't change:
long interval1 = 0,interval2 = 0;  // interval at which to blink (milliseconds)
//const long RDint1 = 160, RDint2 = 640, Chngint = 3000;
int SW1,SW2,SW3,SW4;

void setup() {
  // set the digital pin as output:
  pinMode(13, OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
  color1 = 3;
  color2 = 3;
  color3 = 3;
  color4 = 3;
  digitalWrite(10,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(7,HIGH);
}

void loop() {
  // here is where you'd put code that needs to be running all the time.
  Serial.print(interval1);
  Serial.println(interval2);
  //read inputs
  SW1 = digitalRead(A0); //northbound sensor 
  SW2 = digitalRead(A1); //eastbound sensor
  SW3 = digitalRead(A2); //southbound sensor
  SW4 = digitalRead(A3); //westbound sensor
  
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval1) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
 if (SW4 == 1 || color1 != 3){ // westbound logic
    if (color1 == 3 && color2 == 3 && color4 == 3) {    // conflicting with northbound and southbound
      color1 = 1; // green
      interval1 = 5000;
      }else if (color1 == 1){
      color1 = 2; // yellow
      interval1 = 3790;
    }else{
      color1 = 3; //red
      interval1 = 640;
      }
       if(color1 == 1){
   digitalWrite(13,LOW);  //westbound green on
   digitalWrite(12,LOW);
   digitalWrite(11,HIGH);
    }else if(color1 == 2){
     digitalWrite(13,LOW);  //westbound yellow on
     digitalWrite(12,HIGH);
     digitalWrite(11,LOW);
      }else{
        digitalWrite(13,HIGH); //westbound red on
        digitalWrite(12,LOW);
        digitalWrite(11,LOW);
        }
 }
 if (SW1 == 1 || color2 != 3){ // northbound logic
    if (color2 == 3 && color1 == 3 && color3 == 3) { // conflicting with westbound and eastbound
      color2 = 1; // green
      interval1 = 5000;
    }else if (color2 == 1){
      color2 = 2; // yellow
      interval1 = 3790;
    }else{
      color2 = 3; //red
      interval1 = 640;
      }
  
 
    // set the signal with the color variable:
   
        if(color2 == 1){
   digitalWrite(10,LOW);  //northbound green on
   digitalWrite(9,LOW);
   digitalWrite(8,HIGH);
    }else if(color2 == 2){
     digitalWrite(10,LOW);  //northbound yellow on
     digitalWrite(9,HIGH);
     digitalWrite(8,LOW);
      }else{
        digitalWrite(10,HIGH); //northbound red on
        digitalWrite(9,LOW);
        digitalWrite(8,LOW);
        }
      
 }
  }  
  if (currentMillis - previousMillis1 >= interval2) {
    // save the last time you blinked the LED
    previousMillis1 = currentMillis; 
  
  if (SW2 == 1 || color3 != 3){ // eastbound logic 
    if (color3 == 3 && color2 == 3 && color4 == 3) {    // conflicting with northbound and southbound
      color3 = 1; // green
      interval2 = 5000;
    }else if (color3 == 1){
      color3 = 2; // yellow
      interval2 = 3790;
    }else{
      color3 = 3; //red
      interval2 = 640;
      }
      if(color3 == 1){
   digitalWrite(7,LOW);  //eastbound green on
   digitalWrite(6,LOW);
   digitalWrite(5,HIGH);
    }else if(color3 == 2){
     digitalWrite(7,LOW);  //eastbound yellow on
     digitalWrite(6,HIGH);
     digitalWrite(5,LOW);
      }else{
        digitalWrite(7,HIGH); //eastbound red on
        digitalWrite(6,LOW);
        digitalWrite(5,LOW);
        }
  }
    if (SW3 == 1 || color4 != 3){ // southbound logic 
    if (color4 == 3 && color1 == 3 && color3 == 3) {
      color4 = 1; // green
      interval2 = 5000;
    }else if (color4 == 1){
      color4 = 2; // yellow
      interval2 = 3790;
    }else{
      color4 = 3; //red
      interval2 = 640;
      }
      if(color4 == 1){
   digitalWrite(4,LOW);  //southbound green on
   digitalWrite(3,LOW);
   digitalWrite(2,HIGH);
    }else if(color4 == 2){
     digitalWrite(4,LOW);  //southbound yellow on
     digitalWrite(3,HIGH);
     digitalWrite(2,LOW);
      }else{
        digitalWrite(4,HIGH); //southbound red on
        digitalWrite(3,LOW);
        digitalWrite(2,LOW);
        }
  }
  }
}