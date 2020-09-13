// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(64); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.begin(115200);
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop()
{
  goto shimmer;
  alloff();
  delay (300); //time in ms gate is off between dial cycles

  for (int chevcount = 0; chevcount < 6; chevcount += 2)
  {
    for (int frameindex = 0; frameindex < strip.numPixels(); frameindex++)
    {
      greenframe(frameindex, chevcount);
      delay(100);
    }

    for (int frameindex = 0; frameindex < strip.numPixels(); frameindex++)
    {
      greenframe(strip.numPixels() - frameindex, chevcount + 1);
      delay(100);
    }
  }
shimmer:
  //for (int frameindex = 0; frameindex < strip.numPixels(); frameindex++)
  {
    greenframe(0, 6);
    delay(500);
  }

  for (int orangeness = 255; orangeness > 0; orangeness--)
  {
    uint32_t color = strip.Color(orangeness, orangeness, 255);
    allonecolor(color);
    delay (3);
  }


  for (int tick = 0; tick < 3000; tick++)
  {
    //   int orangeness = 25 + sin(radians(tick * 0.5)) * 25;
    // int blue = 225 + sin(radians(tick * 1.3)) * 25;
    //  uint32_t color = strip.Color(orangeness, orangeness, blue);
    // allonecolor(color);
    shimmerframe(tick);
    delay (1);
  }

  // uint32_t colorblue = strip.Color(0, 0, 255);
  // allonecolor(colorblue);
  //delay (1000 * 32); //set final time to (1000*60*38)





  alloff ();
  delay (500);
}

void allonecolor (uint32_t color)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void alloff ()
{
  uint32_t color = strip.Color(0, 0, 0);
  allonecolor (color);
}

void greenframe (int frameindex, int chevcount)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {
    int chev = chevpattern (i, chevcount);
    int gr = nonlinearbrightness((i + frameindex) % 6) - chev;
    uint32_t color = strip.Color(chev,   max(0, gr),   0);
    strip.setPixelColor(i, color);
    //Serial.println("hello "+String(gr));
  }
  strip.show();
}

// return integer between 0 and 30
int bluepattern (int i)
{
  switch (i)
  {
    case 0: return 15;
    case 1: return 15;
    case 2: return 6;
    case 3: return 17;
    case 4: return 20;
    case 5: return 10;
    case 6: return 5;
    case 7: return 15;
    case 8: return 20;
    case 9: return 30;
    case 10: return 25;
    case 11: return 15;
    case 12: return 30;
    case 13: return 25;
    case 14: return 18;
    case 15: return 24;
    case 16: return 30;
    case 17: return 26;
    case 18: return 14;
    case 19: return 15;
    case 20: return 10;
    default: return 0;
  }
}

void shimmerframe (int frameindex)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {
    int chev = chevpattern (i, 6);
    //int gr = nonlinearbrightness((i + frameindex) % 6) - chev;


    uint32_t color1 = shimmerframeinner1 (chev, i, frameindex);
    uint32_t color2 = shimmerframeinner2 (chev, i, frameindex);
    uint32_t color = mixcolor (color1, color2);
    strip.setPixelColor(i, color);
    // Serial.println("hello " + pcolor(color) + " " + String (a) + " " + String (b)); //uncomment to make animations run in correct time
  }
  strip.show();
}

uint32_t shimmerframeinner1 (int chev, int i, int frameindex)
{
  int blu = 75 + frameindexpatternspeed(i, 0.3 * frameindex, 40, 6);
  int orangeness = frameindexpatternspeed(i, -0.2 * frameindex, 35, 3);
  uint32_t color = scolor(chev + orangeness,   orangeness,   blu);
  return color;
}

uint32_t shimmerframeinner2 (int chev, int i, int frameindex)
{
  int blu = 75 + frameindexpatternspeed(i, -0.5 * frameindex, 60, 6);
  int orangeness = frameindexpatternspeed(i, 0.15 * frameindex, 55, 3);
  uint32_t color = scolor(chev + orangeness,   orangeness,   blu);
  return color;
}


int frameindexpatternspeed (int i, double frameindex, int mod, int scale )
{
  int s = ( int(i + frameindex) % mod) ;
  int orangeness = bluepattern(abs(s)) * scale;
  return orangeness;
}

uint32_t scolor(int r, int g, int b)
{
  return strip.Color(colorclamp(r), colorclamp(g), colorclamp(b));
}

uint32_t mixcolor (uint32_t color_1, uint32_t color_2)
{
  uint8_t r1 = ( color_1 ) >> 16;
  uint8_t g1 = ( color_1 ) >> 8;
  uint8_t b1 = ( color_1 ) >> 0;

  uint8_t r2 = ( color_2 ) >> 16;
  uint8_t g2 = ( color_2 ) >> 8;
  uint8_t b2 = ( color_2 ) >> 0;

  uint8_t r3 = ((r1 + r2) / 2);
  uint8_t g3 = ((g1 + g2) / 2);
  uint8_t b3 = ((b1 + b2) / 2);

  return scolor(r3, g3, b3);
}

String pcolor (uint32_t color)
{
  uint8_t r = ( color ) >> 16;
  uint8_t g = ( color ) >> 8;
  uint8_t b = ( color ) >> 0;
  return "" + String(r) + " " + String (g) + " " + String(b);
}

int colorclamp(int v)
{
  return max(0, min(255, v));
}


int chevpattern (int i, int chevcount)
{
  if (i + 2 > chevcount * 10)
  {
    return 0;
  }
  switch (i % 10)
  {
    case 0: return 150;
    case 1: return 255;
    case 2: return 150;
    default: return 0;
  }
}

int nonlinearbrightness(int i)
{
  switch (i)
  {
    case 0: return 32;
    case 1: return 150;
    case 2: return 64;
    case 3: return 32;
    case 4: return 4;
    case 5: return 0;


  }
}

// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
    strip.setPixelColor(i, strip.Color(0,   0,   0));
    strip.show();
    delay(wait);
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
