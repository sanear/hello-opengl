#include "input.h"
#include <vector>

class Triangle {
  void construct();

public:
  float delX;
  float delY;
  float delZ;

  std::vector<float> pauseEvents;
  float elapsedPaused;
  float colorMultiplier;

  Triangle();
  Triangle(float currentTime);

  void doUpdate(float time, InputState *inputState);
  void flipPause();
};
