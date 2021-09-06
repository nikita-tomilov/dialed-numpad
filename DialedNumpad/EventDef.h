enum KbdType { K, M, FN, MOUSE, NOK };

struct KeyEvent {
  enum KbdType type;
  int keyCode;
};

enum KeyPressType { KEYP_DOWN, KEYP_UP, KEYP_PRESS };
