//
// Code by McKey  Jan 2021    https://youtu.be/oXqDV9sT4F8
// RC light v1.1
// 
// 
#include <FastLED.h>
#define NUM_LEDS 60               // <=== Указываем количество светодиодов в ленте( сколько что бы работало) / Specify the number of LEDs in the tape(how many that would work)
CRGB leds[NUM_LEDS];
#define DATA_PIN 5                // pin D5, выход на ленту WS2812b / output to ws2812b tape
#define dlay 500
#define pulse1_pin1 3            // pin D3  вход с CH5 приёмника  / input from CH5 receive
//---------------------------------------------------
int ch1 = 0;
int prog = 1;
int last_prog = prog;
int steps = 0;
long nowMillis = 0;
long toRead = 0;
boolean next_step = false;
boolean lamp_on = false;
//-----------------------------------------------------
int pwmIn = 9;               // pin D9 это вход с CH3 приёмника  input from the CH3 receiver
int valMin = 980;            // minimum sensor value 
int valMax = 2000;           // maximum sensor value
int val = 0;
int mappedHue;
boolean switchLed = 0;
//-----------------настройки----------------------------
void setup() { 
  pinMode(pulse1_pin1, INPUT);
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
//-------------------------------------------------
   FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
   leds[0] = CRGB::White; 
   FastLED.show();
}
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }  
//--------------------цикл---------------------------
void loop() { 
  if (millis() - toRead>dlay) { 
    ch1 = pulseIn(pulse1_pin1, HIGH, 25000); 

    if (ch1 < 1100)        { if (prog!=1){prog=1; next_step=1;} 
    } else if (ch1 < 1400) { if (prog!=2){prog=2; steps=0;} 
    } else if (ch1 < 1900) { if (prog!=3){prog=3; steps=0;} 
    } else                   
    toRead = millis();
  }
  nowMillis = millis();
//============================================================ 
  if ((prog==1)and(next_step)) {
  }
//============================================================ 
  if (prog == 2) { 
      val = pulseIn(pwmIn,HIGH);
        mappedHue = map(val, valMin, valMax, 80, 255);
    // Set the i'th led to the chosen colour
        
        int numLedsToLight = map(val, valMin, valMax, 0, NUM_LEDS);

        val = constrain(val, 0, 255);

        // First, clear the existing led values
        FastLED.clear();
        for(int led = 0; led < numLedsToLight; led++) { 
            leds[led] = CHSV(mappedHue, 255, 255); 
        }
        FastLED.show();
 } 
//============================================================ 
  if (prog == 3) { 
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(15); 
  }
  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(15);    
    }
  }  
}
