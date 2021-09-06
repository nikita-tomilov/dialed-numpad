const byte ROWS = 4;
const byte COLS = 6;
char hexaKeys[ROWS][COLS] = {
  //6  7   4   5   2   3     pinNo
  {'a','b','c','d','e','f'}, //10
  {'g','h','i','j','k','l'}, //16
  {'m','n','o','p','q','r'}, //9
  {'s','t','u','v','w','x'}  //8
};
byte rowPins[ROWS] = {10, 16, 9, 8};
byte colPins[COLS] = {6, 7, 4, 5, 2, 3};

Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void findRowCol(char keyChar, int* row, int* col) {
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (hexaKeys[r][c] == keyChar) {
        *row = r;
        *col = c;
        return;
      }
    }
  }
  *row = -1;
  *col = -1;
}

//since actual kbd is 4 wide 6 tall, and due to wiring kbdlib
//has to think it is 6 wide 4 tall, the coords of a key being
//pressed have to be transposed
void findRowColTransposed(char keyChar, int* row, int* col) {
  int r, c;
  findRowCol(keyChar, &r, &c);
  *row = COLS - c - 1;
  *col = r;
}
