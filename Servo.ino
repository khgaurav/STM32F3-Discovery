#include <Servo.h> 
 
int servoPin = 9;
 
Servo servo2;  
Servo servo1;  
 
int angle = 0;   // servo position in degrees 
 
void setup() 
{ 
  servo2.attach(10); 
  servo1.attach(servoPin); 
  servo2.write(180);
servo1.write(0);
  Serial.begin(38400);

} 
 
 
void loop() 
{ 
//servo2.write(0);
//servo1.write(0);
//delay(10000000);

if(Serial.read()==97)
  {   
      servo2.write(180);
  servo1.write(0);
  delay(1000);
    // scan from 0 to 180 degrees
      for(angle = 0; angle < 180; angle++)  
      {                                  
        servo1.write(angle);
        if(Serial.available()>0 && Serial.read()=='f')
        {
          Serial.println(angle);
          break;
        }
        delay(30);                   
      } 
      // now scan back from 180 to 0 degrees
      for(angle = 180; angle > 0; angle--) 
      {                                           
            servo2.write(angle);
           if(Serial.available()>0 && Serial.read()=='f')
        {
          Serial.println(180+map(angle,180,0,0,180));
          break;
        }
        delay(30);
      }
      for(angle = 180; angle > 0; angle--)    
      {                                           
            servo1.write(angle);
            if(Serial.available()>0 && Serial.read()=='f')
        {
          Serial.println(180+angle);
          break;
        }
        delay(30); 
        
      }
     for(angle = 0; angle < 180; angle++)    
      {                                           
            servo2.write(angle);
if(Serial.available()>0 && Serial.read()=='f')
        {
          Serial.println(map(angle,180,0,0,180));
          break;
        }
        delay(30);
        
      }
     Serial.println('n'); 
  }
}

