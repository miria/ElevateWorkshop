/*
 * Elevate.cpp - Library for a simplified Arduino interface to
 * Adafruit's Flora Accelerometer and NeoPixel Jewel.
 * Created by Miria Grunick on April 7, 2015.
 * Released into the public domain.
 */

#include "Arduino.h"
#include "Wire.h"
#include "../Adafruit_Sensor/Adafruit_Sensor.h"
#include "../Adafruit_LSM303DLHC/Adafruit_LSM303_U.h"
#include "../Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#include "Elevate.h"


Accelerometer::Accelerometer() {
	_mag = Adafruit_LSM303_Mag_Unified(12345);
	_accel = Adafruit_LSM303_Accel_Unified(54321);
	if (!_mag.begin()) {
		while (1)
			;
	}
	if (!_accel.begin()) {
		while (1)
			;
	}

	int i = 0;
	while (i < 100) {
		_xValues[i] = 0.0;
		_yValues[i] = 0.0;
		_zValues[i] = 0.0;
		i++;
	}
}

float Accelerometer::getCompassReading() {
	sensors_event_t event;
	_mag.getEvent(&event);
	float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / 3.14159;
	if (heading < 0) {
		heading = 360 + heading;
	}
	return heading;

}

float Accelerometer::getMotionReading() {
	sensors_event_t event;
	_accel.getEvent(&event);

	float xAccel = event.acceleration.x;
	float yAccel = event.acceleration.y;
	float zAccel = event.acceleration.z;

	_xValues[_idx] = xAccel;
	_yValues[_idx] = yAccel;
	_zValues[_idx] = zAccel;

	_idx++;
	if (_idx >= 100) {
		_idx = 0;
	}

	float sumX = 0.0;
	float sumY = 0.0;
	float sumZ = 0.0;

	int i = 0;
	while (i < 100) {
		sumX += _xValues[i];
		sumY += _yValues[i];
		sumZ += _zValues[i];
		i++;
	}

	float reading = max(abs((sumX / 100.0f)),
			max(abs((sumY / 100.0f)), abs((sumZ / 100.0f))));

	// Need to normalize this to between 0 and 1. We will say that the "max"
	// measurement is 6g's.
	if (reading >= 6.0f) {
		return 1.0;
	}
	return reading / 6.0f;
}

Light::Light(int pin) {
	_light = Adafruit_NeoPixel(7, pin, 0x01 + 0x00);
	_light.begin();
	_light.show();
	_brightness = 100.0;
}

void Light::setColor(int color) {
	switch (color / 128) {
	case 0:
		_red = ((127 - color % 128) * 2 * _brightness);
		_green = (color % 128) * 2 * _brightness;
		_blue = 0;
		break;
	case 1:
		_red = 0;
		_green = (127 - color % 128) * 2 * _brightness;
		_blue = (color % 128) * 2 * _brightness;
		break;
	case 2:
		_red = color % 128 * 2 * _brightness;
		_green = 0;
		_blue = (127 - color % 128) * 2 * _brightness;
		break;
	}
}
void Light::setColor(int red, int green, int blue) {
	_red = red;
	_green = green;
	_blue = blue;

}
void Light::setBrightness(float brightness) {
	_brightness = brightness;
	updateLight();
}

void Light::updateLight() {
	for (int i = 0; i < 7; i++) {
		_light.setPixelColor(i, _red, _green, _blue);
	}
	_light.show();

}

