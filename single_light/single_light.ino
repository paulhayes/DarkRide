#include <Adafruit_NeoPixel.h>

#define lightsPin 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, lightsPin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop(){
  colorWipe(strip.Color(219, 1, 134), 1200); 
  colorWipe(strip.Color(0, 151, 217), 1200); 
  colorWipe(strip.Color(168, 168, 168), 1200); 
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

