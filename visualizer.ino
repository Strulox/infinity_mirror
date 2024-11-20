#include <FastLED.h>

#define NUM_LEDS 300
#define DATA_PIN 6
#define EXPLOSION_LIMIT 20
#define ANALOG_PIN A0
#define HALF (NUM_LEDS / 2)
#define BASE_LEVEL 270
#define QUARTER (NUM_LEDS/4)
#define THIRD_QUARTER QUARTER*3

enum MODE { INSIDE_OUT, OUTSIDE_IN, LEVELS, MULTI_LEVELS, RANDOM, DEBUG, EXPLOSION, ALTERNATE, MUSIC_LINE};

unsigned long last_event = 0UL;
unsigned long last_mode_change = 0UL;
CRGB leds[NUM_LEDS];
MODE currentMode = MUSIC_LINE;
int highestLevel = 0;
int lowestLevel = 0;
int bright = 255;
int rBright = 0;
bool reverse = false;

int colours[4] = {50, 94, 136, 165};

void setup() { 
  FastLED.addLeds<WS2812B,DATA_PIN,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(84);
  randomSeed(analogRead(A5));
}

//Helper functions
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i] = CHSV(0,0,0); } }

int setRandomColour() {
  return (rand() % sizeof(colours) / sizeof(colours[0]) -1);
}

//Lighting Modes----
void quarters(int val) {
  if(val <= BASE_LEVEL)
    return 0;
  int mVal = val - BASE_LEVEL;
  int hue = 60; // Green - Red
  for(int i = 0; i <= (mVal/10); ++i) {
      leds[HALF+i] = CHSV(hue++, 255,255);
      leds[HALF-i] = CHSV(hue++, 255,255);
      leds[QUARTER+i] = CHSV(hue++, 255,255);
      leds[QUARTER-i] = CHSV(hue++, 255,255);
      leds[THIRD_QUARTER+i] = CHSV(hue++, 255,255);
      leds[THIRD_QUARTER-i] = CHSV(hue++, 255,255);
      FastLED.show();
  } 
}

void outsideIn(int val) {
  if(val <= BASE_LEVEL)
    return 0;
  int mVal = val - BASE_LEVEL;
  if(mVal > NUM_LEDS)
    mVal = NUM_LEDS;

  int hue = 60;
  for(int i = 0; i <=mVal; ++i) {
    leds[i] = CHSV(hue++, 255,255);
    leds[NUM_LEDS-i] = CHSV(hue++, 255,255);
    FastLED.show();
  }
}

void lightUpAndFade(int val, int hue) {
  if(val <= BASE_LEVEL)
    return 0;
  int mVal = (val - BASE_LEVEL);
  if(mVal > NUM_LEDS)
    mVal = NUM_LEDS;

  for(int i = 0; i <= mVal/5; ++i) {
      leds[i] = CHSV(hue++, 255,255);
      FastLED.show();
  }
}

void multiLightUpAndFade(int val, int hue) {
  if(val <= BASE_LEVEL)
    return 0;
  int mVal = ((val - BASE_LEVEL)/2);

  for(int i = 0; i <= mVal; ++i) {
    leds[i] = CHSV(hue++, 255, 255);
    leds[HALF+i] = CHSV(hue++, 255, 255);
  }
  FastLED.show();
}


void explosion(int val, int hue) {
    if(val <= BASE_LEVEL)
      return;
    //Issue here, where it could be going out of bounds.
    int mVal = NUM_LEDS - ((val - BASE_LEVEL)+EXPLOSION_LIMIT) *2;
    if (mVal <= EXPLOSION_LIMIT)
      return;

    for(int i = 0; i <= EXPLOSION_LIMIT; ++i) {
      leds[mVal + i - EXPLOSION_LIMIT - 1] = CHSV(hue,255,255-(i*12));
      leds[mVal - i + EXPLOSION_LIMIT + 1] = CHSV(hue,255,255-(i*12));
      FastLED.show();
    }
    for(int i = 0; i <= EXPLOSION_LIMIT; ++i) {
      leds[mVal + i] = CHSV(0,0,0);
      leds[mVal - i] = CHSV(0,0,0);
      FastLED.show();
    }
}

void randomLEDs(int val) {
  if(val < BASE_LEVEL + 50)
    return;
  int randomLED = random(EXPLOSION_LIMIT + 1, NUM_LEDS - EXPLOSION_LIMIT);
  double mVal = ( (double)(val) / (double)highestLevel);
  for(int i = 0; i <= EXPLOSION_LIMIT; ++i) {
      leds[randomLED + i - EXPLOSION_LIMIT - 1] = CHSV(floor(mVal * 255.0) ,255,255-(i*12));
      leds[randomLED - i + EXPLOSION_LIMIT + 1] = CHSV(floor(mVal * 255.0),255,255-(i*12));
      FastLED.show();
    }
    for(int i = 0; i <= EXPLOSION_LIMIT; ++i) {
      leds[randomLED + i] = CHSV(0,0,0);
      leds[randomLED - i] = CHSV(0,0,0);
      FastLED.show();
    }
}


void musicLine(int val) {

  if(val < BASE_LEVEL) {
    return;
  }
  //get a percentage the sounds is over Base_level
  double mVal = ( (double)(val - BASE_LEVEL) / (double)NUM_LEDS) * 100;
  //Light up the whole line
  for(int i = 1; i <= NUM_LEDS; ++i) {
    if(i == ( (mVal/100) * NUM_LEDS) && i > EXPLOSION_LIMIT ) {
        for(int ii = 0; ii <= EXPLOSION_LIMIT; ++ii) {
          //change it so if the pixel is coloured again, it changes to a deeper colour / more red color.
          leds[i+ii] = CHSV(150,255,255);
          leds[i-ii] = CHSV(150,255,255);
          FastLED.show();
        }
    }
    else 
      leds[i] = CHSV(0, 0, 0);
  }
  FastLED.show();
}

void alternate(int val) {
  if(val <= BASE_LEVEL+50)
    return;
        
  for(int i = 0; i <= NUM_LEDS; ++i) {
    if(reverse) {
      bright++;
      rBright--;
    }
    else {
      bright--;
      rBright++;
    }
    if(bright == 0 || rBright ==0)
        reverse = !reverse;
    if(i%2==0) {
      leds[i] = CHSV(140, 255, bright);
    }
    else {
      // leds[i] = CHSV(70, 255, rBright);
      leds[i]=CHSV(130, 255, rBright);
    } 
  }
  FastLED.show(); 
}

//Main loop
void loop() { 
  int val = 0;
  int hue = 0;
  if (millis() - last_mode_change > 8000UL) {
      currentMode = (rand() % 9);
      last_mode_change = millis();
      hue = setRandomColour();
  }

  if (millis() - last_event > 100UL) {
      val = analogRead(ANALOG_PIN);
      last_event = millis();
  }

  if(val > highestLevel || highestLevel == 0)
    highestLevel = val;
  if(val < lowestLevel || lowestLevel == 0)
    lowestLevel = val;

  switch (currentMode) {
    case LEVELS:
      lightUpAndFade(val, colours[hue]);
      break;
    case MULTI_LEVELS:
      multiLightUpAndFade(val, colours[hue]);
      break;
    case INSIDE_OUT:
      quarters(val);
      break;
    case OUTSIDE_IN:
      outsideIn(val);
      break;
    // case DEBUG:
    //   for(int i = 0; i < NUM_LEDS; ++i) {
    //       Serial.print("Hue value is: ");
    //       Serial.println(i);
    //       leds[i] = CHSV(i, 255,255);
    //       FastLED.show();
    //   }
    //   break;
    case ALTERNATE:
      alternate(val);
      break;
    case RANDOM:
      randomLEDs(val); 
      break;
    case EXPLOSION:
      explosion(val, colours[hue]);
      break;
    case MUSIC_LINE:
      musicLine(val);
      break;
    default:
      musicLine(val);
      break;
  }
  fadeall();
}


