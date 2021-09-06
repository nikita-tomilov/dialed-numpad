enum KbdType { K, M };

struct KeyEvent {
  enum KbdType type;
  int keyCode;
};
