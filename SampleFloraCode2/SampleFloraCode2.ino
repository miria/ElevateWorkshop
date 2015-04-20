
//#include <Bounce.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>
#include "SPI.h"

const int PIXEL_PIN = 6;
const int NUM_ACCEL_READINGS_AVG = 10;
const int NUM_PIXELS = 7;
const float MAX_GS = 5.0;

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


float maxBrightness = 1.0;
float readingsX[NUM_ACCEL_READINGS_AVG];
float readingsY[NUM_ACCEL_READINGS_AVG];
float readingsZ[NUM_ACCEL_READINGS_AVG];

int curIdx = 0;
int buttonState = 0;


void setup() 
{
  
  Serial.begin(9600);

  if(!mag.begin())
  {
    while(1);
  }
  if(!accel.begin())
  {
    while(1);
  }
  
  // Initialize the readings arrays
  for(int i=0; i<NUM_ACCEL_READINGS_AVG;i++) 
  {
    readingsX[i] = 0.0;
    readingsY[i] = 0.0;
    readingsZ[i] = 0.0;
  }  
  pixels.begin();
  pixels.show(); 
}

// Brightness is determined by accelerometer delta, color is
// determined my compass heading
void loop() 
{
	//useOnlyCompass();
	//useOnlyAccel();
	useCompassWithAccel();
  	//delay(100);
}

void useOnlyCompass()
{
	
	pixels.setBrightness(30);

	float heading = getCompassHeading();
	int colors[3];
  	getColors(heading, colors);

  	for (int i=0; i < NUM_PIXELS; i++) {
      		pixels.setPixelColor(i, colors[0], colors[1], colors[2]);
  	}
  	pixels.show();
  	Serial.print("Compass heading: "); Serial.println(heading); 
  
}

void useOnlyAccel()
{
	float accelDeltas[3];
	getAccelerometerData(accelDeltas);
  
	float maxDelta = 0;
	for (int i=0; i < 3; i++) {
		if (accelDeltas[i] > maxDelta) {
			maxDelta = accelDeltas[i];
		}
	}

	float brightness = getBrightness(maxDelta);

	Serial.println(maxDelta);

	for (int i=0; i < NUM_PIXELS; i++) {
		pixels.setPixelColor(i, brightness, brightness, brightness);
	}
	pixels.show();

}

void useCompassWithAccel()
{
	float heading = getCompassHeading();
	int colors[3];
  	getColors(heading, colors);

  
	float accelDeltas[3];
	getAccelerometerData(accelDeltas);
  
	float maxDelta = 0;
	for (int i=0; i < 3; i++) {
		if (accelDeltas[i] > maxDelta) {
			maxDelta = accelDeltas[i];
		}
	}

	static float delta_smooth;
	delta_smooth = (maxDelta + delta_smooth * 9) / 10;
	float brightness = getBrightness(maxDelta);

	Serial.print(maxDelta);
	Serial.print(" ");
  	Serial.print("Compass heading: "); Serial.println(heading); 

	for (int i=0; i < NUM_PIXELS; i++) {
		pixels.setPixelColor(i, colors[0]*brightness/256, colors[1]*brightness/256, colors[2]*brightness/256);
	}
	pixels.show();

}

float getCompassHeading() 
{
  sensors_event_t event;
  mag.getEvent(&event);
  float Pi = 3.14159;
  // Calculate the angle of the vector y,x  
  float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;
  // Normalize to 0-360
  if (heading < 0) 
  {
    heading = 360 + heading;
  }

  return heading;
}


void getColors(int WheelPos, int colors[])
{
  switch(WheelPos / 128)
  {
    case 0:
      colors[0] = ((127 - WheelPos % 128)*2*maxBrightness);   //Red down
      colors[1] = (WheelPos % 128)*2*maxBrightness;      // Green up
      colors[2] = 0;                  //blue off
      break; 
    case 1:
      colors[1] = (127 - WheelPos % 128)*2*maxBrightness;  //green down
      colors[2] = (WheelPos % 128)*2*maxBrightness;      //blue up
      colors[0] = 0;                  //red off
      break; 
    case 2:
      colors[2] = (127 - WheelPos % 128)*2*maxBrightness;  //blue down 
      colors[0] = WheelPos % 128*2*maxBrightness;      //red up
      colors[1] = 0;                  //green off
      break; 
  }

}

void getAccelerometerData(float deltas[])
{
  sensors_event_t event; 
  accel.getEvent(&event);
  
  float xAccel = event.acceleration.x;
  float yAccel = event.acceleration.y;
  float zAccel = event.acceleration.z;
 
  float sumX = 0.0;
  float sumY = 0.0;
  float sumZ = 0.0;
  
  for (int i=0; i<NUM_ACCEL_READINGS_AVG; i++) 
  {
     sumX += readingsX[i]; 
     sumY += readingsY[i]; 
     sumZ += readingsZ[i]; 
  }
  deltas[0] = (sumX/NUM_ACCEL_READINGS_AVG) / xAccel;
  deltas[0] = abs(deltas[0]);
  deltas[1] = (sumY/NUM_ACCEL_READINGS_AVG) / yAccel;
  deltas[1] = abs(deltas[1]);
  deltas[2] = (sumZ/NUM_ACCEL_READINGS_AVG) / zAccel;
  deltas[2] = abs(deltas[2]);
  
  readingsX[curIdx] = xAccel;
  readingsY[curIdx] = yAccel;
  readingsZ[curIdx] = zAccel;
  
  curIdx++;
  if (curIdx >= NUM_ACCEL_READINGS_AVG) {
    curIdx = 0;
  }
}

float getBrightness(float delta) 
{  
  // 5gs is the "max" force we will measure.
  
  float brightness = min(abs(delta - 1.0), MAX_GS);
  // return brightness / MAX_GS; 
  //return (brightness / MAX_GS) *20; // for useOnlyAccel()
  return (brightness / MAX_GS) *50; // for useCompassWithAccel()
}
