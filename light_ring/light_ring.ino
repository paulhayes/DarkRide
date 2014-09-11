#include <Adafruit_NeoPixel.h>

#define lightsPin 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, lightsPin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop(){
  colorWipe(strip.Color(219, 1, 134), 50); 
  colorWipe(strip.Color(0, 151, 217), 50); 
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

