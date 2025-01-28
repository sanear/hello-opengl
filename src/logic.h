#include "input.h"

class Triangle {
public:
  float delX;
  float delY;
  float delZ;

  bool doPause;
  float pausedAt;
  float elapsedPause;

  void doUpdate(float time, InputState* inputState);
  void flipPause();
};
