#include "input.h"
#include <vector>

class Triangle {
public:
  float delX;
  float delY;
  float delZ;

  std::vector< float > pauseEvents;
  float elapsedPaused;

  Triangle();

  void doUpdate(float time, InputState* inputState);
  void flipPause();
};
