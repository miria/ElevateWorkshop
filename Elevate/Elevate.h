/*
 * Elevate.h - Library for a simplified Arduino interface to
 * Adafruit's Flora Accelerometer and NeoPixel Jewel.
 * Created by Miria Grunick on April 7, 2015.
 * Released into the public domain.
 */

#ifndef Elevate_h
#define Elevate_h

#include "Arduino.h"
#include "Wire.h"
#include "../Adafruit_Sensor/Adafruit_Sensor.h"
#include "../Adafruit_LSM303DLHC/Adafruit_LSM303_U.h"
#include "../Adafruit_NeoPixel/Adafruit_NeoPixel.h"

class Accelerometer
{
public:
	Accelerometer();
	float getCompassReading();
	float getMotionReading();
private:
	Adafruit_LSM303_Accel_Unified* _accel;
	Adafruit_LSM303_Mag_Unified* _mag;
	float _xValues[100];
	float _yValues[100];
	float _zValues[100];
	int _idx;
};

class Light
{
public:
	Light(int pin);
	void setColor(int color);
	void setColor(int red, int green, int blue);
	void setBrightness(float brightness);
private:
	Adafruit_NeoPixel* _light;
	float _brightness;
	int _red;
	int _green;
	int _blue;
	void updateLight();
};

#endif
