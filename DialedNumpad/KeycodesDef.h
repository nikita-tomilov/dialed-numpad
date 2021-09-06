KeyEvent encRotateLeftEvents[2] = { {M, MEDIA_VOLUME_DOWN}, {K, KEY_UP} };
KeyEvent encRotateRightEvents[2] = { {M, MEDIA_VOLUME_UP}, {K, KEY_DOWN} };
KeyEvent encPressEvents[2] = { {M, MEDIA_PLAY_PAUSE}, {K, KEY_ENTER} };
KeyEvent* encEvents[3] = { encRotateLeftEvents, encRotateRightEvents, encPressEvents };



KeyEvent keyEventsLayer0[6][4] = { 
  { {K, KEY_F13}, {K, KEY_F14}, {K, KEY_F15}, {K, KEY_F16} },
  { {K, KEY_NUM_LOCK}, {K, KEY_LEFT}, {K, KEY_RIGHT}, {K, KEYPAD_ADD} },
  { {K, KEYPAD_7}, {K, KEYPAD_8}, {K, KEYPAD_9}, {K, KEYPAD_SUBTRACT} },
  { {K, KEYPAD_4}, {K, KEYPAD_5}, {K, KEYPAD_6}, {K, KEYPAD_MULTIPLY} },
  { {K, KEYPAD_1}, {K, KEYPAD_2}, {K, KEYPAD_3}, {K, KEYPAD_DIVIDE} },
  { {FN, 0/*FN*/}, {K, KEYPAD_0}, {K, KEYPAD_DOT}, {K, KEYPAD_ENTER} },
};
