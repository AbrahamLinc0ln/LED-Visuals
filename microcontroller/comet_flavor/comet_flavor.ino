#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 5
#define LED_COUNT 150

byte r = 0;
byte g = 0;
byte b = 0;

byte r_cyn = 0;
byte g_cyn = 150;
byte b_cyn = 70;

byte r_green = 154;
byte g_green = 250;
byte b_green = 0;

byte r_vlt = 50;
byte g_vlt = 100;
byte b_vlt = 139;

byte r_mag = 100;
byte g_mag = 0;
byte b_mag = 139;

byte r_gr = 135;
byte g_gr = 125;
byte b_gr = 15;

byte r_pnk = 255;
byte g_pnk = 20;
byte b_pnk = 143;

byte r_ryl = 65;
byte g_ryl = 105;
byte b_ryl = 225;

unsigned long curr_time = 0;
unsigned long prev_time = 0;
unsigned long wait_time = 0;

long speed_delay_var = 19;
long speed_delay_mean = 20;
long speed_delay = 0;

long size_var = 30;
long size_mean = 64;
long tail_size = 0;

unsigned int rand_color = 0;

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

//  Serial.println(color);
//  Serial.println(r);
//  Serial.println(g);
//  Serial.println(b);

  randomSeed(analogRead(5));

}

void loop() {
  // start a meteor based on random time
  curr_time = millis();
  if(curr_time - prev_time > wait_time){
    speed_delay = speed_delay_mean + random(-1*speed_delay_var,speed_delay_var);
    tail_size = size_mean + random(-1*size_var,size_var);

    rand_color = random(5);

    switch(rand_color){
      case 0:
        r = r_cyn;
        g = g_cyn;
        b = b_cyn;
        break;
      case 1:
        r = r_mag;
        g = g_mag;
        b = b_mag;
        break;
      case 2:
        r = r_gr;
        g = g_gr;
        b = b_gr;
        break;
      case 3:
        r = r_pnk;
        g = g_pnk;
        b = b_pnk;
        break;
      case 4:
        r = r_ryl;
        g = g_ryl;
        b = b_ryl;
        break;
    }

    
    meteorRain(r, g, b, 1, tail_size, true, speed_delay, true);
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
    
    r=(r<=10)? 0 : (int) r-(r*(fadeValue+random(5))/256);
    g=(g<=10)? 0 : (int) g-(g*(fadeValue+random(5))/256);
    b=(b<=10)? 0 : (int) b-(b*(fadeValue+random(5))/256);
   
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
