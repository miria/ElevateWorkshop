
#include <Elevate.h>
#include <Wire.h>

Accelerometer accel = Accelerometer();
Light light = Light(10);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  // Returns a number between 0.0 and 360.0
  float heading = accel.getCompassReading();
  if (heading > 0.0) {
      // Returns a number between 0.0 and 1.0
      float movement = accel.getMotionReading();
  
      Serial.print("Accelerometer: "); Serial.println(movement);
      Serial.print("Compass: "); Serial.println(heading);
      
      // Takes a number between 0.0 and 1.0;
      light.setBrightness(movement);
      // Takes a number between 0.0 and 360.0;
      light.setColor(heading);
 
      delay(100);
   }
   
}
