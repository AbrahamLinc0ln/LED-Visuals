#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 5
#define LED_COUNT 717
#define DELAY 5
#define PI 3.146
#define EFFECTS 14
#define ROWS 22
#define BRIGHT 50

unsigned long curr_time = 0;
unsigned long prev_time = 0;
unsigned long wait_time = 5;
//unsigned long start_time = 0;
unsigned long effect_start_time = 0;
unsigned int effect_time_s = 2;
unsigned long effect_time_ms = effect_time_s*1000;

unsigned long curr_color = 0;

char serial_buff;

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// define some colors
uint32_t red = leds.Color(255, 0, 0);
uint32_t blue = leds.Color(0, 0, 255);

void setup() {
  Serial.begin(115200);
  leds.begin();  // Call this to start up the LED leds.
  leds.setBrightness(10); //adjust brightness here
  leds.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));
}

void loop() {
curr_time = millis();
effect_start_time = curr_time;
// choose effect
int effect = random(EFFECTS); 
// int effect = 4; 
// loop for the effect time

Serial.print("EFFECT: ");
Serial.print(effect);
Serial.println();

while(curr_time - effect_start_time < effect_time_ms){
  curr_time = millis();
  if(curr_time - prev_time > wait_time){
    runEffect(effect, effect_start_time, effect_time_s);
    prev_time = curr_time;
  }
}
}

void runEffect(int effect, int start_time, int period){
  switch(effect){
    // case 0: // Fade to Black
    //   fade2Black();
    //   break;
    case 0: // breathe
      breathe(start_time, period, 0);
      break;
    case 1: // fast breathe
      breathe(start_time, float(period)/2, 0);
      break;
    case 2: // scan bounce
      rowWaveBounce(start_time, float(period)/2, 10, 2.0, 0);
      break;
    case 3: //scan one way slow
      rowWaveBounce(start_time, float(period), 7, 2.5, 0);
      break;
    case 4: // breathe red
      breathe(start_time, float(period)/2, red);
      break;
    case 5: // breathe blue
      breathe(start_time, float(period)/2, blue);
      break;
    case 6: // breathe red super fast
      breathe(start_time, float(period)/4, red);
      break;
    case 7: // breathe blue super fast
      breathe(start_time, float(period)/4, blue);
      break;
    case 8: // breathe super fast
      breathe(start_time, float(period)/4, 0);
      break;
    case 9: // breathe blue
      breathe(start_time, float(period)/2, blue);
      break;
    case 10: // wave red
      rowWaveBounce(start_time, float(period)/2, 10, 2.0, red);
      break;
    case 11: //scan one way slow red
      rowWaveBounce(start_time, float(period), 7, 2.5, red);
      break;
    case 12: // wave red
      rowWaveBounce(start_time, float(period)/2, 10, 2.0, blue);
      break;
    case 13: //scan one way slow red
      rowWaveBounce(start_time, float(period), 7, 2.5, blue);
      break;
    default: //Do Nothing
      break;
  }
}

void fade2Black(){
  for(int jj=0;jj<LED_COUNT;jj++){
    uint32_t color = leds.getPixelColor(jj);
    uint8_t r = color >> 16;
    uint8_t g = color >> 8;
    uint8_t b = color;

    if(r>1){r = r-1;}
    if(g>1){g = g-1;}
    if(b>1){b = b-1;}
    
    leds.setPixelColor(jj, r, g, b);
  }
  leds.show();
}

void setAllColor(){
for(int jj = 0; jj < LED_COUNT; jj++){
    setBaseColor(jj);
  }
}
void breathe(int start_time, float period, uint32_t mono_color){
  // start_time = when the effect was triggered
  // curr_time = the current time
  // period =  speed of the oscillation in s
  uint32_t color = 0;
  float freq = 1.0/period;
  float brightness = 0.5*(-1*cos(2.0*PI*freq*(float(curr_time-start_time)/1000))+1);
  
  for(int jj = 0; jj < LED_COUNT; jj++){
    if(mono_color == 0){
      color = getZoneColor(jj);
    }
    else{
      color = getZoneMonoColor(jj,mono_color);
    }
    uint8_t r = color >> 16;
    uint8_t g = color >> 8;
    uint8_t b = color;

    r = r*brightness;
    b = b*brightness;
    g = g*brightness;

    leds.setPixelColor(jj,r,g,b);
  }
  // Serial.print(" BRIGHT: ");
  // Serial.print(brightness);
  // Serial.println();
  leds.show();
}

void rowWaveBounce(int start_time, int period, uint8_t range, float width, uint32_t mono_color){
  float freq = 1.0/period;
  float wave_center = (ROWS+range)*0.5*(cos(PI*freq*float(curr_time-start_time)/1000)+1)-(range)/2;
  uint32_t color = 0;
  
  for(int jj = 0; jj < LED_COUNT; jj++){
    if(mono_color == 0){
      color = getZoneColor(jj);
    }
    else{
      color = getZoneMonoColor(jj,mono_color);
    }
    uint8_t r = color >> 16;
    uint8_t g = color >> 8;
    uint8_t b = color;
    float row = getRow(jj);
    // float row_bright_scale = pow(1-(abs(row-wave_center)/(ROWS+width)),4);
    float row_bright_scale = pow(2.7,-1*pow(row-wave_center,2)/(2*width));

    // if(row_bright_scale < 0.25) row_bright_scale = 0;
  
    
    r = r*row_bright_scale;
    b = b*row_bright_scale;
    g = g*row_bright_scale;


    leds.setPixelColor(jj,r,g,b);

  }
  
  leds.show();
}

void setBaseColor(int jj){
  curr_color = getZoneColor(jj);
  byte r = curr_color >> 16;
  byte g = curr_color >> 8;
  byte b = curr_color;

  leds.setPixelColor(jj,r,g,b);
}

uint32_t getRainbowColor(int jj){
  if(inBlueZone(jj) || inRedZone(jj)){
    uint32_t color = Wheel(Wheel(((jj * 256 / leds.numPixels())) & 255));
    return color;
  }
  else{
    return 0;
  }
}

uint32_t getZoneColor(int jj){
  if(inBlueZone(jj)){
    return 0x0000FF;
  }
  else if(inRedZone(jj)){
    return 0xFF0000;
  }
  else if(inWhiteZone(jj)){
    return 0xFFFFFF;
  }
  else{
    return 0;
  }
}

uint32_t getZoneMonoColor(int jj, uint32_t color){
  if(inBlueZone(jj)){
    return color;
  }
  else if(inRedZone(jj)){
    return color;
  }
  else if(inWhiteZone(jj)){
    return 0;
  }
  else{
    return 0;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
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

int getRow(int jj){
  if(jj <= 26){
    return 0;
  }
  else if(jj <= 55){
    return 1;
  }
  else if(jj <= 85){
    return 2;
  }
  else if(jj <= 117){
    return 3;
  }
  else if(jj <= 150){
    return 4;
  }
  else if(jj <= 184){
    return 5;
  }
  else if(jj <= 218){
    return 6;
  }
  else if(jj <= 252){
    return 7;
  }
  else if(jj <= 287){
    return 8;
  }
  else if(jj <= 322){
    return 9;
  }
  else if(jj <= 357){
    return 10;
  }
  else if(jj <= 392){
    return 11;
  }
  else if(jj <= 427){
    return 12;
  }
  else if(jj <= 462){
    return 13;
  }
  else if(jj <= 497){
    return 14;
  }
  else if(jj <= 531){
    return 15;
  }
  else if(jj <= 565){
    return 16;
  }
  else if(jj <= 598){
    return 17;
  }
  else if(jj <= 630){
    return 18;
  }
  else if(jj <= 660){
    return 19;
  }
  else if(jj <= 689){
    return 20;
  }
  else{
    return 21;
  }
}

int getCol(int pix){
  int row = getRow(pix);
  int col = 0;

  switch(row){
    case 0:
    col = round((31.0-5.0)*(pix-0)/26+5.0);
    break;

    case 1:
    col = round((32.0-4.0)*(pix-27)/(55-27)+4.0);
    break;
    
    case 2:
    col = round((32.0-3.0)*(pix-56)/(85-56)+3.0);
    break;
    
    case 3:
    col = round((33.0-2.0)*(pix-86)/(117-86)+2.0);
    break;
    
    case 4:
    col = round((34.0-2.0)*(pix-118)/(150-118)+2.0);
    break;
    
    case 5:
    col = round((35.0-1.0)*(pix-151)/(184-151)+1.0);
    break;
    
    case 6:
    col = round((35.0-1.0)*(pix-185)/(218-185)+1.0);
    break;
    
    case 7:
    col = round((35.0-1.0)*(pix-219)/(252-219)+1.0);
    break;
    
    case 8:
    col = round((35.0-1.0)*(pix-253)/(287-253)+1.0);
    break;
    
    case 9:
    col = round((35.0-1.0)*(pix-288)/(322-288)+1.0);
    break;
    
    case 10:
    col = round((35.0-1.0)*(pix-323)/(357-323)+1.0);
    break;
    
    case 11:
    col = round((35.0-1.0)*(pix-358)/(392-358)+1.0);
    break;
    
    case 12:
    col = round((35.0-1.0)*(pix-393)/(427-393)+1.0);
    break;
    
    case 13:
    col = round((35.0-1.0)*(pix-428)/(462-428)+1.0);
    break;
    
    case 14:
    col = round((35.0-1.0)*(pix-463)/(497-463)+1.0);
    break;
    
    case 15:
    col = round((35.0-1.0)*(pix-498)/(531-498)+1.0);
    break;
    
    case 16:
    col = round((34.0-2.0)*(pix-532)/(565-532)+2.0);
    break;
    
    case 17:
    col = round((34.0-2.0)*(pix-566)/(598-566)+2.0);
    break;
    
    case 18:
    col = round((33.0-2.0)*(pix-599)/(630-599)+2.0);
    break;
    
    case 19:
    col = round((33.0-3.0)*(pix-631)/(660-631)+3.0);
    break;
    
    case 20:
    col = round((32.0-4.0)*(pix-661)/(689-661)+4.0);
    break;
    
    case 21:
    col = round((31.0-5.0)*(pix-690)/(690-716)+5.0);
    break;
    
    default:
    break;
  }

  col--;

  return col;
}