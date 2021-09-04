#include "GyverEncoder.h"
//https://github.com/AlexGyver/GyverLibs#GyverEncoder
#include <Keypad.h>
#include <Adafruit_NeoPixel.h> 
#ifdef __AVR__
  #include <avr/power.h>
#endif
const byte ROWS = 4;
const byte COLS = 6;
char hexaKeys[ROWS][COLS] = {
  //6  7   4   5   2   3
  {'a','b','c','d','e','f'}, //10
  {'g','h','i','j','k','l'}, //16
  {'m','n','o','p','q','r'}, //9
  {'s','t','u','v','w','x'}  //8
};
byte rowPins[ROWS] =  {10, 16, 9, 8};
byte colPins[COLS] = {6, 7, /*4*/4, 5, 2, /*3*/3};


//initialize an instance of class NewKeypad
Keypad kpd = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

Encoder enc2(A1,A0,15,true);
Encoder enc1(A3,A2,14,true);
Adafruit_NeoPixel led(1, 1);

unsigned long loopCount;
unsigned long startTime;
String msg;

void setup(){
  pinMode(1, OUTPUT);
  digitalWrite(1, 1);
  Serial.begin(115200);
  while (!Serial) {};
  Serial.println("start ok");
  loopCount = 0;
  startTime = millis();
  msg = "";
led.begin();
   led.setPixelColor(0, led.Color(255,255,255));
   led.setPixelColor(1, led.Color(255,0,0));
   led.setPixelColor(2, led.Color(0,255,0));
   led.setPixelColor(3, led.Color(0,0,255));
  led.show();
}
  
void loop() {
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }

    enc1.tick();
    if (enc1.isRight()) Serial.println("1 Right");         // если был поворот
    if (enc1.isLeft()) Serial.println("1 Left");
    if (enc1.isPress()) Serial.println("1 Press");       

     enc2.tick();
    if (enc2.isRight()) Serial.println("2 Right");         // если был поворот
    if (enc2.isLeft()) Serial.println("2 Left");
    if (enc2.isPress()) Serial.println("2 Press");       

    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    msg = " PRESSED.";
                break;
                    case HOLD:
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    msg = " RELEASED.";
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
                Serial.print("Key ");
                Serial.print(kpd.key[i].kchar);
                Serial.println(msg);
            }
        }
    }
}  // End loop
