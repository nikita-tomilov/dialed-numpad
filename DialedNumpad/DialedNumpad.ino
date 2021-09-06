#include <Keypad.h>
#undef KEY_H //it is a include guard in this lib but actual const in hid-project lib

//https://github.com/AlexGyver/GyverLibs#GyverEncoder
#include "GyverEncoder.h"

//https://github.com/NicoHood/HID/wiki/Consumer-API
#include "HID-Project.h"

#include "EventDef.h"
#include "MatrixKeyboardDef.h"
#include "KeycodesDef.h"

Encoder enc2(A1, A0, 15, true);
Encoder enc1(A3, A2, 14, true);

void setup(){
  pinMode(1, OUTPUT);
  digitalWrite(1, 1);
  Serial.begin(115200);
  while (!Serial) {};
  Serial.println("start ok");
  BootKeyboard.begin();
}
  
void loop() {
  requestEncoders();     
  if (kpd.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {
      if (kpd.key[i].stateChanged) {
        keyEvent(kpd.key[i].kchar, kpd.key[i].kstate);
      }
    }
  }
}

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
  KeyEvent e = encEvents[eventNo][encoderNo];
  sendKey(e, KEYP_PRESS);
}

void keyEvent(char keyChar, int keyState) {
  int row, col;
  findRowColTransposed(keyChar, &row, &col);
  printff("coord %d %d state %d\n", row, col, keyState);
  emitKey(row, col, keyState);
}

void emitKey(int row, int col, int keyState) {
  KeyEvent e = keyEventsLayer0[row][col];
  if (keyState == PRESSED) {
    sendKey(e, KEYP_DOWN);
  } else if (keyState == RELEASED) {
    sendKey(e, KEYP_UP);
  }
}

void sendKey(KeyEvent e, KeyPressType t) {
  switch (t) {
    case KEYP_PRESS:
      if (e.type == K) {
        BootKeyboard.write(KeyboardKeycode(e.keyCode));
      } else if (e.type == M) {
        Consumer.write(e.keyCode);
      }
      break;
    case KEYP_DOWN:
      if (e.type == K) {
        BootKeyboard.press(KeyboardKeycode(e.keyCode));
      } else if (e.type == M) {
        Consumer.press(e.keyCode);
      } else if (e.type == FN) {
        Serial.println("FN ON");
      }
      break;
    case KEYP_UP:
      if (e.type == K) {
        BootKeyboard.release(KeyboardKeycode(e.keyCode));
      } else if (e.type == M) {
        Consumer.release(e.keyCode);
      } else if (e.type == FN) {
        Serial.println("FN OFF");
      }
      break;
  }
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
