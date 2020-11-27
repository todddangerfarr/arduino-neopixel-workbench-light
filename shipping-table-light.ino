// Shipping Table NeoPixel Light Controller
// By: Todd Farr 

#include <Adafruit_NeoPixel.h> 

#define FIRST_LIGHT      6
#define SECOND_LIGHT     5
#define ON_OFF           2
#define BRIGHTNESS_UP    12
#define BRIGHTNESS_DOWN  11

#define FIRST_LIGHT_LED_COUNT    36

// Varaibles 
int powerState = 0; 
int onOffButton; 
int brightnessUpButton; 
int brightnessDownButton; 
int brightness = 75; 

// Constants 
const int MAX_BRIGHTNESS = 200; 
const int BUTTON_DELAY = 300; 
const int BRIGHTNESS_ADDER = 20;

// Neopixel Class Instance
Adafruit_NeoPixel strip(FIRST_LIGHT_LED_COUNT, FIRST_LIGHT, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  // NeoPixel Setup 
  strip.begin(); 
  strip.show(); 
  strip.setBrightness(brightness); 

  // Pin Modes
  pinMode(ON_OFF, INPUT);
  pinMode(BRIGHTNESS_UP, INPUT);
  pinMode(BRIGHTNESS_DOWN, INPUT);
}

void loop() {
  // Inputs
  onOffButton = digitalRead(ON_OFF);
  brightnessUpButton = digitalRead(BRIGHTNESS_UP);
  brightnessDownButton = digitalRead(BRIGHTNESS_DOWN);

  // ON/OFF Button Handling, need delays for momentary action
  if ((onOffButton == 1) && (powerState == 0)) {
    powerState = 1; 
    Serial.println("Power State = 1"); 
    delay(BUTTON_DELAY);
  } else if ((onOffButton == 1) && (powerState == 1)) {
    powerState = 0; 
    Serial.println("Power State = 0"); 
    delay(BUTTON_DELAY); 
  }

  // BRIGHTNESS Buttons Handling
  if (brightnessUpButton == 1) {
    Serial.println("Brightness UP Pressed");
    if (brightness < MAX_BRIGHTNESS) {
      brightness += BRIGHTNESS_ADDER;
      strip.setBrightness(brightness);
      delay(BUTTON_DELAY); 
    }
  }
  if (brightnessDownButton == 1) {
    Serial.println("Brightness UP Pressed");
    if (brightness >= 40) {
      brightness -= BRIGHTNESS_ADDER;
      strip.setBrightness(brightness);
      delay(BUTTON_DELAY); 
    }
  }
  
  if (powerState == 1) {
    // rainbowFade2White(3, 3, 1);
    colorWipe(strip.Color(  0,   0,   0, 255), 10); // True white (not RGB white)
  } else {
    colorWipe(strip.Color(  0,   0,   0,   0), 10); // Turn off the lights all of the lights...
  }

  if (onOffButton == 1) {
    Serial.println("ON/OFF Pressed"); 
  }
  
}
 

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
