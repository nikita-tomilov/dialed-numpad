enum KbdType { BOOT, MEDIA };

struct KeyEvent {
  enum KbdType type;
  int keyCode;
};
