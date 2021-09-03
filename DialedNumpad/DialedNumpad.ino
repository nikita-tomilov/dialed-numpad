#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 6; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3', 'A', 'B'},
  {'4','5','6','7', 'C', 'D'},
  {'8','9','E','F', 'G', 'H'},
  {'I', 'J', 'K', 'L', 'M', 'N'}
};
byte rowPins[ROWS] =  {10, 16, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 4, 5, 2, 3};//connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(115200);
  while (!Serial) {};
  Serial.println("start ok");
}
  
void loop(){
 // Serial.println("test");
 // delay(1000);
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}
