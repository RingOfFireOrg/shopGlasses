#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 8

// This code started from the AdaFruit NeoPixel sample code. The stuff that is written well came from there.
//   Code that looks a little wonky is probably bent from the good start by Glen to fit his needs.
//   Code that looks like bailing wire and duct tape probably was written by Glen (several times) until the
//     compiler gave up and did what I wanted it to just to get me to go away.

// **********************    IMPORTANT!!  *************************
//
// Since the State variable is read as a byte, I have taken over Pin 0 and 1.
//  You don't care, except the Near selector switch is grounding out the RX or TX
//   unless it is in HARD TOOLS position. You will get an error when trying to load
//   code unless you put that switchi in HARD TOOLS -or- disconnect the selector
//   switch from the header pins.
//
//  Sorry.
//
// ************** Glen. ***********************************

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(45, PIN, NEO_GRB + NEO_KHZ400);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Define some color codes by name to make it easier to call and revise later on
uint32_t Red = strip.Color(255, 0, 0);
uint32_t Green = strip.Color(0,0,255);
uint32_t Yellow = strip.Color(255, 0, 90);
uint32_t Off = strip.Color(0, 0, 0);


// The State variable is defined thus:
// Bit 7 is currently unused, Or... might be a poorly documented, last minute add, party mode ;-)
// Bit 6 is the ESTOP button,
// Bit 5 is the FAR HardTools input,
// Bit 4 is the FAR SoftTools input,
// Bit 3 is the FAR NoTools input,
// Bit 2 is the NEAR HardTools input,
// Bit 1 is the NEAR SoftTools input,
// Bit 0 is the NEAR NoTools input,
// Bit 0, 1, 2, and 3, 4, 5, are connected to a rotary selector switch, which will ground only one input
//    at a time. Bit 6 is connected to a momentary push button which will ground the E-Stop input while it
//    is being pressed.
//  Switch (two used): https://smile.amazon.com/gp/product/B00PZST5TE/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1
// LED's are broken into five sections of NINE pixels (eighteen LED's) each. These are set up like traffic lights
//   with a set of three pixels for each color (Red, Yellow, Green). To keep wiring simpler, and the colors in the
//   right order, the 

//   New code for the BFF space August 14, 2021. With only one shop area, we are using the two controlers in two locations to
//     control all the LED's at once rather than two separate independant spaces. 

  byte State = PIND;
  int ESTOP = 6;


void setup() {
  pinMode(0,INPUT_PULLUP);
  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  DDRD = B00000000;

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(9,OUTPUT);
  
}

void loop() {
/*  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50);
  colorWipe(strip.Color(0, 255, 0), 50);
  colorWipe(strip.Color(Green, 50);
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue
  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
*/
  State = PIND;		//Read bits 0 through 7 into the State variable.
  if (digitalRead(ESTOP) == LOW){
    // The ESTOP button has been pushed, lights go red, don't exit this state until ESTOP is high and 
	//  NO Tools are selected in both areas.
	while (State != 246){
	  AllRed();
	  State = PIND;	//Read the inputs so we have a chance to exit
	}
  }
  switch (State){
	case 246:		//No E-STOP, FAR No tools, NEAR N0 Tools: output FAR Green, NEAR Green
	  FarGreen();
	  NearGreen();
    break;

	case 245:		//No E-STOP, FAR No tools, NEAR SOFT Tools: output FAR GREEN, NEAR YELLOW **** Revised 8/14/21 for single build space
	  //FarGreen();
	  FarYellow();
	  NearYellow();
    break;

	case 243:		//No E-STOP, FAR No tools, NEAR HARD Tools: output FAR YELLOW, NEAR YELLOW
	  FarYellow();
	  NearYellow();
    break;

	case 238:		//No E-STOP, FAR SOFT tools, NEAR N0 Tools: output FAR YELLOW, NEAR Green **** Revised 8/14/21 for single build space
	  FarYellow();
	  //NearGreen();
	  NearYellow();
    break;

	case 237:		//No E-STOP, FAR SOFT tools, NEAR SOFT Tools: output FAR YELLOW, NEAR YELLOW
	  FarYellow();
	  NearYellow();
    break;

	case 235:		//No E-STOP, FAR SOFT tools, NEAR HARD Tools: output FAR YELLOW, NEAR YELLOW
	  FarYellow();
	  NearYellow();
    break;

	case 222:		//No E-STOP, FAR HARD tools, NEAR N0 Tools: output FAR YELLOW, NEAR YELLOW
	  FarYellow();
	  NearYellow();
    break;

	case 221:		//No E-STOP, FAR HARD tools, NEAR SOFT Tools: output FAR YELLOW, NEAR YELLOW
	  FarYellow();
	  NearYellow();
    break;

	case 219:		//No E-STOP, FAR HARD tools, NEAR HARD Tools: output FAR YELLOW, NEAR YELLOW
	  FarYellow();
	  NearYellow();
    break;

	case 118:		//Party Mode!!!!! Bit 7 low, No E-STOP, FAR No Tools, NEAR No Tools: Get Funky!
	  rainbowCycle(2);
    break;


	default:		//Should only get here during state transistions - Clear LEDS
      colorWipe(strip.Color(0, 0, 0), 5);
	  strip.setPixelColor(0, Red);
	  strip.show();

    }  
  }

// Fill the dots from Start to End with a color
void colorSet(uint32_t c, uint8_t Start, uint8_t End ) {
  for(uint16_t i=Start; i<End; i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

// set all indicators to red (E-stop)
void AllRed( ) {
	  strip.setPixelColor(8, Red);
	  strip.setPixelColor(7, Red);
	  strip.setPixelColor(6, Red);
	  strip.setPixelColor(5, Off);
	  strip.setPixelColor(4, Off);
	  strip.setPixelColor(3, Off);
	  strip.setPixelColor(2, Off);
	  strip.setPixelColor(1, Off);
	  strip.setPixelColor(0, Off);
	  
	  strip.setPixelColor(44, Red);
	  strip.setPixelColor(43, Red);
	  strip.setPixelColor(42, Red);
	  strip.setPixelColor(41, Off);
	  strip.setPixelColor(40, Off);
	  strip.setPixelColor(39, Off);
	  strip.setPixelColor(38, Off);
	  strip.setPixelColor(37, Off);
	  strip.setPixelColor(36, Off);
	  
	  strip.setPixelColor(17, Red);
	  strip.setPixelColor(16, Red);
	  strip.setPixelColor(15, Red);
	  strip.setPixelColor(14, Off);
	  strip.setPixelColor(13, Off);
	  strip.setPixelColor(12, Off);
	  strip.setPixelColor(11, Off);
	  strip.setPixelColor(10, Off);
	  strip.setPixelColor( 9, Off);
	  
	  strip.setPixelColor(26, Red);
	  strip.setPixelColor(25, Red);
	  strip.setPixelColor(24, Red);
	  strip.setPixelColor(23, Off);
	  strip.setPixelColor(22, Off);
	  strip.setPixelColor(21, Off);
	  strip.setPixelColor(20, Off);
	  strip.setPixelColor(19, Off);
	  strip.setPixelColor(18, Off);

	  strip.setPixelColor(35, Red);
	  strip.setPixelColor(34, Red);
	  strip.setPixelColor(33, Red);
	  strip.setPixelColor(32, Off);
	  strip.setPixelColor(31, Off);
	  strip.setPixelColor(30, Off);
	  strip.setPixelColor(29, Off);
	  strip.setPixelColor(28, Off);
	  strip.setPixelColor(27, Off);

	  
	  strip.show();
}

// set the Near indicators to Green (no tools)
void NearGreen( ) {
	  strip.setPixelColor(8, Off);
	  strip.setPixelColor(7, Off);
	  strip.setPixelColor(6, Off);
	  strip.setPixelColor(5, Off);
	  strip.setPixelColor(4, Off);
	  strip.setPixelColor(3, Off);
	  strip.setPixelColor(2, Green);
	  strip.setPixelColor(1, Green);
	  strip.setPixelColor(0, Green);
	  
	  strip.setPixelColor(44, Off);
	  strip.setPixelColor(43, Off);
	  strip.setPixelColor(42, Off);
	  strip.setPixelColor(41, Off);
	  strip.setPixelColor(40, Off);
	  strip.setPixelColor(39, Off);
	  strip.setPixelColor(38, Green);
	  strip.setPixelColor(37, Green);
	  strip.setPixelColor(36, Green);
	  
	  strip.show();
}

// set the Near indicators to Yellow (Glasses required)
void NearYellow( ) {
	  strip.setPixelColor(8, Off);
	  strip.setPixelColor(7, Off);
	  strip.setPixelColor(6, Off);
	  strip.setPixelColor(5, Yellow);
	  strip.setPixelColor(4, Yellow);
	  strip.setPixelColor(3, Yellow);
	  strip.setPixelColor(2, Off);
	  strip.setPixelColor(1, Off);
	  strip.setPixelColor(0, Off);
	  
	  strip.setPixelColor(44, Off);
	  strip.setPixelColor(43, Off);
	  strip.setPixelColor(42, Off);
	  strip.setPixelColor(41, Yellow);
	  strip.setPixelColor(40, Yellow);
	  strip.setPixelColor(39, Yellow);
	  strip.setPixelColor(38, Off);
	  strip.setPixelColor(37, Off);
	  strip.setPixelColor(36, Off);
	  
	  strip.show();
}
 
// set the Far indicators to Green (no tools)
void FarGreen( ) {
	  strip.setPixelColor(17, Off);
	  strip.setPixelColor(16, Off);
	  strip.setPixelColor(15, Off);
	  strip.setPixelColor(14, Off);
	  strip.setPixelColor(13, Off);
	  strip.setPixelColor(12, Off);
	  strip.setPixelColor(11, Green);
	  strip.setPixelColor(10, Green);
	  strip.setPixelColor( 9, Green);
	  
	  strip.setPixelColor(26, Off);
	  strip.setPixelColor(25, Off);
	  strip.setPixelColor(24, Off);
	  strip.setPixelColor(23, Off);
	  strip.setPixelColor(22, Off);
	  strip.setPixelColor(21, Off);
	  strip.setPixelColor(20, Green);
	  strip.setPixelColor(19, Green);
	  strip.setPixelColor(18, Green);

	  strip.setPixelColor(35, Off);
	  strip.setPixelColor(34, Off);
	  strip.setPixelColor(33, Off);
	  strip.setPixelColor(32, Off);
	  strip.setPixelColor(31, Off);
	  strip.setPixelColor(30, Off);
	  strip.setPixelColor(29, Green);
	  strip.setPixelColor(28, Green);
	  strip.setPixelColor(27, Green);

	  strip.show();
}

// set the Far indicators to Yellow (Glasses required)
void FarYellow( ) {
	  strip.setPixelColor(17, Off);
	  strip.setPixelColor(16, Off);
	  strip.setPixelColor(15, Off);
	  strip.setPixelColor(14, Yellow);
	  strip.setPixelColor(13, Yellow);
	  strip.setPixelColor(12, Yellow);
	  strip.setPixelColor(11, Off);
	  strip.setPixelColor(10, Off);
	  strip.setPixelColor( 9, Off);
	                      
	  strip.setPixelColor(26, Off);
	  strip.setPixelColor(25, Off);
	  strip.setPixelColor(24, Off);
	  strip.setPixelColor(23, Yellow);
	  strip.setPixelColor(22, Yellow);
	  strip.setPixelColor(21, Yellow);
	  strip.setPixelColor(20, Off);
	  strip.setPixelColor(19, Off);
	  strip.setPixelColor(18, Off);
                          
	  strip.setPixelColor(35, Off);
	  strip.setPixelColor(34, Off);
	  strip.setPixelColor(33, Off);
	  strip.setPixelColor(32, Yellow);
	  strip.setPixelColor(31, Yellow);
	  strip.setPixelColor(30, Yellow);
	  strip.setPixelColor(29, Off);
	  strip.setPixelColor(28, Off);
	  strip.setPixelColor(27, Off);

	  strip.show();
}
 

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
