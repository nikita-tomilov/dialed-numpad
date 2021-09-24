#include <Keypad.h>
#undef KEY_H //it is a include guard in this lib but actual const in hid-project lib

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include "HMouse.h"

//https://github.com/AlexGyver/GyverLibs#GyverEncoder
#include "GyverEncoder.h"

//https://github.com/NicoHood/HID/wiki/Consumer-API
#include "HID-Project.h"

#include "EventDef.h"
#include "MatrixKeyboardDef.h"
#include "KeycodesDef.h"

Encoder enc2(A1, A0, 15, true);
Encoder enc1(A3, A2, 14, true);
Adafruit_NeoPixel pixels(4, 0, NEO_GRB + NEO_KHZ800);
int layerIndex = 0;

void setup(){
  Serial.begin(115200);
  //while (!Serial) {};
  Serial.println("start ok");
  BootKeyboard.begin();

  pixels.begin();
  pixels.clear();
  pixels.show();
  setLed(0, 0, 0, 150); delay(200);
  setLed(1, 150, 150, 0); delay(200);
  setLed(2, 0, 150, 0); delay(200);
  setLed(3, 150, 0, 0); delay(200);
  pixels.clear();
  pixels.show();

  Serial.println("finita");
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
  setLedIfKbd(0, LED_CAPS_LOCK);
  setLedIfKbd(1, LED_SCROLL_LOCK);
  setLedIfKbd(2, LED_SHIFT);
  setLedIfKbd(3, LED_NUM_LOCK);
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
  KeyEvent e = (layerIndex == 0) ? keyEventsLayer0[row][col] : keyEventsLayer1[row][col];
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
      } else if (e.type == MOUSE) {
        //horizontal scroll
        HMouse.move(0, 0, 0, e.keyCode);
      }
      break;
    case KEYP_DOWN:
      if (e.type == K) {
        BootKeyboard.press(KeyboardKeycode(e.keyCode));
      } else if (e.type == M) {
        Consumer.press(e.keyCode);
      } else if (e.type == FN) {
        Serial.println("FN ON");
        layerIndex = 1;
      }
      break;
    case KEYP_UP:
      if (e.type == K) {
        BootKeyboard.release(KeyboardKeycode(e.keyCode));
      } else if (e.type == M) {
        Consumer.release(e.keyCode);
      } else if (e.type == FN) {
        Serial.println("FN OFF");
        layerIndex = 0;
      }
      break;
  }
}

void setLed(int ledIdx, int r, int g, int b) {
  pixels.setPixelColor(ledIdx, pixels.Color(r, g, b));
  pixels.show();
}

void setLedIfKbd(int ledIdx, uint8_t flag) {
  if (BootKeyboard.getLeds() & flag)
    setLed(ledIdx, 0, 32, 64);
  else
    setLed(ledIdx, 0, 0, 0);
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
