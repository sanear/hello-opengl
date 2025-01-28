struct InputState {
  bool wHeld;
  bool aHeld;
  bool sHeld;
  bool dHeld;
  bool spaceHeld;
  float spacePressedAt;
};

void handleKeys(int key, int action, InputState *inputState);
