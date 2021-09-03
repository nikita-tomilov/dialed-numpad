/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

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

unsigned long loopCount;
unsigned long startTime;
String msg;

void setup(){
  Serial.begin(115200);
  while (!Serial) {};
  Serial.println("start ok");
  loopCount = 0;
  startTime = millis();
  msg = "";
}
  
void loop() {
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }

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
