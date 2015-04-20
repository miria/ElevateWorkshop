/** Turn on one blue pixel at position '0'
 *
 * This demonstrates the pixel strip definition, the setup and main loop
 * code for the FLORA and a NeoPixel ring.
 *
 */
#include <Adafruit_NeoPixel.h>

#define PIN 		6	
#define NUM_PIXELS	7

/*
 * The pixel ring, strip, or jewel is defined "at compile" time.
 * Since the hardware probably doesn't do that often, it is ok that
 * the number of pixels and pin wired to the pixels is fixed.  
*/

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
	NUM_PIXELS,            // how many pixels on strip?
	PIN,                   // output pin?  NEO_GRB +
	NEO_KHZ800            // type of pixels?
);

/*
 * This function is called continuously once the FLORA is running.
 * It doesn't need to ever return, but if it does it will be called
 * again.
 *
 * The pixels.setPixelColor() function takes four arguments:
 * 1. The pixel number (0 is the very first)
 * 2. The red intensity (0 is off, 255 is max)
 * 3. The green intensity
 * 4. The blue intensity
 */
void loop() {
	// Turn on the first pixel blue
	pixels.setPixelColor(0, 0, 0, 255);
	pixels.show();

}

// This is code was adapted from code from Adafruit
// Makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
	 uint16_t i, j;

	for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
		for(i=0; i< pixels.numPixels(); i++) {
			pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    		}
		pixels.show();
		delay(wait);
	}
}

// This is code from Adafruit
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	if(WheelPos < 85) {
		return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	} else if(WheelPos < 170) {
		WheelPos -= 85;
		return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	} else {
		WheelPos -= 170;
		return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}

/*
 * This function is called when the FLORA "boots up" and is
 * responsible for initializing the hardware, setting any globals
 * and getting ready to run.
 */
void setup() {
	pixels.begin();
	pixels.setBrightness(50);
}


