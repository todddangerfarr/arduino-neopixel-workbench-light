// Shipping Table NeoPixel Light Controller
// By: Todd Farr 

#include <Adafruit_NeoPixel.h> 

#define ON_OFF           2
#define LEFT_LIGHT       5
#define RIGHT_LIGHT      6
#define MODE             8
#define BRIGHTNESS_DOWN  11
#define BRIGHTNESS_UP    12

#define RIGHT_LIGHT_LED_COUNT    36

// Varaibles 
int mode = 0; 
int powerState = 0;
int brightness = 75;    

// Constants 
const int MAX_BRIGHTNESS = 250; 
const int BUTTON_DEBOUNCE = 300; 
const int BRIGHTNESS_ADDER = 10;
const int NUM_MODES = 2; 

// Neopixel Class Instance
Adafruit_NeoPixel strip(RIGHT_LIGHT_LED_COUNT, RIGHT_LIGHT, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  // NeoPixel Setup 
  strip.begin(); 
  strip.show(); 
  strip.setBrightness(brightness); 

  // Pin Modes  
  pinMode(MODE, INPUT); 
  pinMode(ON_OFF, INPUT);
  pinMode(BRIGHTNESS_UP, INPUT);
  pinMode(BRIGHTNESS_DOWN, INPUT);
}

void loop() { 
  // ON/OFF Button Handling, need delays for momentary action
  if ((digitalRead(ON_OFF) == 1) && (powerState == 0)) {
    powerState = 1; 
    Serial.println("ON/OFF Pressed");
    Serial.println("Power State = 1"); 
    Serial.print("Mode: "); Serial.println(mode); 
    delay(BUTTON_DEBOUNCE);
  } else if ((digitalRead(ON_OFF) == 1) && (powerState == 1)) {
    powerState = 0; 
    Serial.println("ON/OFF Pressed");
    Serial.println("Power State = 0"); 
    Serial.print("Mode: "); Serial.println(mode); 
    delay(BUTTON_DEBOUNCE); 
  }

  // BRIGHTNESS Buttons Handling
  if (digitalRead(BRIGHTNESS_UP) == 1) {
    Serial.println("Brightness UP Pressed");
    if (brightness < MAX_BRIGHTNESS) {
      brightness += BRIGHTNESS_ADDER;
      strip.setBrightness(brightness);
      delay(BUTTON_DEBOUNCE); 
    }
  }
  if (digitalRead(BRIGHTNESS_DOWN) == 1) {
    Serial.println("Brightness DOWN Pressed");
    if (brightness >= 40) {
      brightness -= BRIGHTNESS_ADDER;
      strip.setBrightness(brightness);
      delay(BUTTON_DEBOUNCE); 
    }
  }

  // Mode Button Handling 
  if (digitalRead(MODE) == 1) { 
    Serial.println("Mode Button Pressed");
    if (++mode > (NUM_MODES-1)) mode = 0; 
    Serial.print("Mode: "); Serial.println(mode); 
    delay(BUTTON_DEBOUNCE); 
  }

  // Run Lights in whatever mode is selected
  if (powerState == 1) { // Lights should be on
    switch (mode) {
      case 0: 
        colorWipe(strip.Color(  0,   0,   0, 255), 10, 0); // True white (not RGB white)
        break; 
      case 1:
        rainbow(10); 
        break; 
       }
   } else {
   colorWipe(strip.Color(  0,   0,   0,   0), 10, 1); // Turn off the lights all of the lights...
  }
}
 
// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); 
    delay(wait); 
    if ((digitalRead(MODE) == 1) || (digitalRead(ON_OFF) == 1)) {
      delay(BUTTON_DEBOUNCE); 
      return;
    }  
  }
}

void colorWipe(uint32_t color, int wait, int dir) {
  for(int i=0; i<strip.numPixels(); i++) {
    if (dir == 0) {
      strip.setPixelColor(i, color);         
    } else {
      strip.setPixelColor((strip.numPixels() - 1) - i, color);
    }
    strip.show();                          
    delay(wait);                         
  }
}
