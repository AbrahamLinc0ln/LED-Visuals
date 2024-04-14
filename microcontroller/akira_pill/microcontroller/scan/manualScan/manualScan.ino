#include <Adafruit_NeoPixel.h>

#define PIN 5
#define LED_COUNT 717
#define DELAY 5

unsigned long curr_time = 0;
unsigned long prev_time = 0;
unsigned long wait_time = 0;

char serial_buff;

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
    leds.setPixelColor(j,255,0,0);
    if(j>0){
      leds.setPixelColor(j-1,0,0,0);
    }
    Serial.print("LED: ");
    Serial.println(j);
    leds.show();
    while (Serial.available() < 1){}
    serial_buff = Serial.read();
    delay(DELAY);

  }
}
