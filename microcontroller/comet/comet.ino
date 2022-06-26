#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 5
#define LED_COUNT 150

byte r = 0;
byte g = 0;
byte b = 0;


unsigned long curr_time = 0;
unsigned long prev_time = 0;
unsigned long wait_time = 0;

unsigned int dir = 0;

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);
  leds.begin();  // Call this to start up the LED leds.
  leds.setBrightness(70); //adjust brightness here
  leds.show(); // Initialize all pixels to 'off'

//  r = (CYAN & 0xFF0000) >> 32;
//  g = (CYAN & 0x00FF00) >> 16;
//  b = (CYAN & 0x0000FF);
  // define colors here. figure out way to read from a single hex variable later
  r = 0;
  g = 150;
  b = 70;

//  Serial.println(color);
//  Serial.println(r);
//  Serial.println(g);
//  Serial.println(b);
}

void loop() {
  // start a meteor based on random time
  curr_time = millis();
  if(curr_time - prev_time > wait_time){
    meteorRain(r, g, b, 1, 64, true, 25, true);
    wait_time = round(random(3000));
    prev_time = curr_time;
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay, bool randDir) {  
  setAll(0,0,0);
  
  if(!randDir){
    // alternate direction
    for(int i = 0; i < LED_COUNT+20; i++) {     
      // fade brightness all LEDs one step
      for(int j=0; j<LED_COUNT; j++) {
        if((!meteorRandomDecay) || (random(10)>5)) {
          fadeToBlack(j, meteorTrailDecay );        
        }
      }
     
      // draw meteor
      for(int j = 0; j < meteorSize; j++) {
        if( ( i-j <LED_COUNT) && (i-j>=0) ) {
          setPixel(i-j, red, green, blue);
        }
      }
      showStrip();
      delay(SpeedDelay);
    }
    // switch direction
    for(int i = LED_COUNT; i > -20; i--) { 
      // fade brightness all LEDs one step
      for(int j=0; j<LED_COUNT; j++) {
        if( (!meteorRandomDecay) || (random(10)>5) ) {
          fadeToBlack(j, meteorTrailDecay );        
        }
      }
     
      // draw meteor
      for(int j = 0; j < meteorSize; j++) {
        if((i+j < LED_COUNT) && (i+j>=0) ) {
          setPixel(i+j, red, green, blue);
        }
      }
     
      showStrip();
      delay(SpeedDelay);
    }
  }
  else{
    // randomize direction
    dir = round(random(2));
    if(dir == 1){
      for(int i = 0; i < LED_COUNT+20; i++) {     
      // fade brightness all LEDs one step
      for(int j=0; j<LED_COUNT; j++) {
        if( (!meteorRandomDecay) || (random(10)>5) ) {
          fadeToBlack(j, meteorTrailDecay );        
        }
      }
      // draw meteor
      for(int j = 0; j < meteorSize; j++) {
        if( ( i-j <LED_COUNT) && (i-j>=0) ) {
          setPixel(i-j, red, green, blue);
        }
      }
      showStrip();
      delay(SpeedDelay);      
      }
    }
    else{
      for(int i = LED_COUNT; i > -20; i--) { 
      // fade brightness all LEDs one step
        for(int j=0; j<LED_COUNT; j++) {
          if( (!meteorRandomDecay) || (random(10)>5) ) {
            fadeToBlack(j, meteorTrailDecay );        
          }
        }
       
        // draw meteor
        for(int j = 0; j < meteorSize; j++) {
          if((i+j < LED_COUNT) && (i+j>=0) ) {
            setPixel(i+j, red, green, blue);
          }
        }
        showStrip();
        delay(SpeedDelay);
      }  
    }     
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = leds.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    leds.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}


void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   leds.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   leds.setPixelColor(Pixel, leds.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < LED_COUNT; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
