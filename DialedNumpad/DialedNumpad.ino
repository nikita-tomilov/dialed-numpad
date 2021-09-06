#include <Keypad.h>
#undef KEY_H //it is a include guard in this lib but actual const in hid-project lib

//https://github.com/AlexGyver/GyverLibs#GyverEncoder
#include "GyverEncoder.h"

//https://github.com/NicoHood/HID/wiki/Consumer-API
#include "HID-Project.h"

#include "EventDef.h"
#include "MatrixKeyboardDef.h"

Encoder enc2(A1, A0, 15, true);
Encoder enc1(A3, A2, 14, true);

unsigned long loopCount;
unsigned long startTime;
String msg;

KeyEvent encRotateLeftEvents[2] = { {M, MEDIA_VOLUME_DOWN}, {K, KEY_UP} };
KeyEvent encRotateRightEvents[2] = { {M, MEDIA_VOLUME_UP}, {K, KEY_DOWN} };
KeyEvent encPressEvents[2] = { {M, MEDIA_PLAY_PAUSE}, {K, KEY_ENTER} };
KeyEvent* encEvents[3] = { encRotateLeftEvents, encRotateRightEvents, encPressEvents };

void setup(){
  pinMode(1, OUTPUT);
  digitalWrite(1, 1);
  Serial.begin(115200);
  while (!Serial) {};
  Serial.println("start ok");
  loopCount = 0;
  startTime = millis();
  msg = "";
  BootKeyboard.begin();
}
  
void loop() {
    loopCount++;
    if ((millis() - startTime) > 5000) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount / 5);
        startTime = millis();
        loopCount = 0;
    }

    requestEncoders();     

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
                  keyEvent(kpd.key[i].kchar, PRESSED);
                  break;
                case RELEASED:
                  keyEvent(kpd.key[i].kchar, RELEASED);
                  break;
                }
            }
        }
    }
}  // End loop

void requestEncoders() {
  enc1.tick();
  if (enc1.isLeft())  encoderEvent(0, 0);
  if (enc1.isRight()) encoderEvent(0, 1);
  if (enc1.isPress()) encoderEvent(0, 2);
  enc2.tick();
  if (enc2.isLeft())  encoderEvent(1, 0);
  if (enc2.isRight()) encoderEvent(1, 1);
  if (enc2.isPress()) encoderEvent(1, 2);
}

void encoderEvent(int encoderNo, int eventNo) {
  printff("%d %d\n", encoderNo, eventNo);
  KeyEvent e = encEvents[eventNo][encoderNo];
  printff("%d %d\n", e.type, e.keyCode);
  if (e.type == K) {
    BootKeyboard.write(KeyboardKeycode(e.keyCode));
  } else if (e.type == M) {
    Consumer.write(e.keyCode);
  } else {
    printff("unknown key type %d\n", e.type);
  }
}

void keyEvent(char keyChar, int keyState) {
  printff("key %c state %d\n", keyChar, keyState);
  int row, col;
  findRowColTransposed(keyChar, &row, &col);
  printff("coord %d %d\n", row, col);
}

size_t printff(const char *format, ...) {
    const uint8_t MAX_STRING_SIZE = 64;      
    char buf[MAX_STRING_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buf, MAX_STRING_SIZE, format, args);
    va_end(args);
    return Serial.print(buf);
}
