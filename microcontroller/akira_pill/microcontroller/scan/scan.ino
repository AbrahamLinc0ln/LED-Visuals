#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"
#include "pill_boundary_vec.h"

#define PIN 5
#define LED_COUNT 717
#define DELAY 5

unsigned long curr_time = 0;
unsigned long prev_time = 0;
unsigned long wait_time = 0;

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(115200);
  leds.begin();  // Call this to start up the LED leds.
  delay(10);
  leds.setBrightness(20); //adjust brightness here
  leds.show(); // Initialize all pixels to 'off'


}

void loop() {
  curr_time = millis();
  if(curr_time - prev_time > wait_time){
    
    scan();
    prev_time = curr_time;
  }
}

void scan(){
  for(int j = 0; j < LED_COUNT; j++) {
    leds.setPixelColor(j,0,0,0);
  }
  leds.show();
  // scan through all LEDs and print location
  int curr_time = 0;
  for(int j = 0; j < LED_COUNT; j++) {
    curr_time = millis();
    //setAll(0,0,0);
    if(inBlueZone(j)){
      leds.setPixelColor(j,0,128,128);
    }
    else if(inRedZone(j)){
      leds.setPixelColor(j,255,0,0);
    }
    else if (inWhiteZone(j)){
      leds.setPixelColor(j,255,255,255);      
    }
    // if(j>0){
    //   leds.setPixelColor(j-1,0,0,0);
    // }
    Serial.print("LED: ");
    Serial.println(j);
    leds.show();
    delay(DELAY);

  }
}

bool inRedZone(int jj){
  if((jj >= 45 && jj <= 54) ||
      (jj >= 75 && jj <= 84) ||
      (jj >= 106 && jj <= 116) ||
      (jj >= 139 && jj <= 149) ||
      (jj >= 173 && jj <= 183) ||
      (jj >= 207 && jj <= 217) ||
      (jj >= 241 && jj <= 251) ||
      (jj >= 276 && jj <= 286) ||
      (jj >= 311 && jj <= 321) ||
      (jj >= 346 && jj <= 356) ||
      (jj >= 381 && jj <= 391) ||
      (jj >= 416 && jj <= 426) ||
      (jj >= 451 && jj <= 461) ||
      (jj >= 486 && jj <= 496) ||
      (jj >= 520 && jj <= 530) ||
      (jj >= 554 && jj <= 564) ||
      (jj >= 587 && jj <= 588) ||
      (jj >= 595 && jj <= 597) ||
      (jj >= 619 && jj <= 620) ||
      (jj >= 627 && jj <= 629) ||
      (jj >= 650 && jj <= 651) ||
      (jj >= 658 && jj <= 659) ||
      (jj >= 679 && jj <= 687)){
    return 1;
  }
  else{
    return 0;
  }
}

bool inBlueZone(int jj){
  if((jj >= 29 && jj <= 41) ||
      (jj >= 57 && jj <= 72) ||
      (jj >= 87 && jj <= 103) ||
      (jj >= 119 && jj <= 136) ||
      (jj >= 152 && jj <= 170) ||
      (jj >= 186 && jj <= 204) ||
      (jj >= 220 && jj <= 238) ||
      (jj >= 254 && jj <= 273) ||
      (jj >= 289 && jj <= 308) ||
      (jj >= 324 && jj <= 343) ||
      (jj >= 359 && jj <= 378) ||
      (jj >= 394 && jj <= 413) ||
      (jj >= 429 && jj <= 448) ||
      (jj >= 464 && jj <= 483) ||
      (jj >= 499 && jj <= 517) ||
      (jj >= 533 && jj <= 551) ||
      (jj >= 567 && jj <= 569) ||
      (jj >= 582 && jj <= 584) ||
      (jj >= 600 && jj <= 601) ||
      (jj >= 615 && jj <= 616) ||
      (jj >= 632 && jj <= 633) ||
      (jj >= 645 && jj <= 647) ||
      (jj >= 662 && jj <= 675)){
        return 1;
  }
  else{
    return 0;
  }
}

bool inWhiteZone(int jj){
  if((jj >= 570 && jj <= 581) ||
      (jj >= 589 && jj <= 594) ||
      (jj >= 602 && jj <= 614) ||
      (jj >= 621 && jj <= 626) ||
      (jj >= 634 && jj <= 644) ||
      (jj >= 652 && jj <= 657)){
        return 1;
  }
  else{
    return 0;
  }
}
