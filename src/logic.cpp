#include "logic.h"

#include <iostream>

void Triangle::doUpdate(float currentTime, InputState *inputState) {
  std::cout << "Start state: (" << delX << "," << delY << "," << delZ
       << "), pause=" << doPause << " elapsedPause=" << elapsedPause << std::endl;
  if (inputState->spaceHeld) {
    doPause = !doPause;
    if (!doPause) {
      elapsedPause += currentTime - inputState->spacePressedAt;
    }
  }
  if (inputState->wHeld) {
    delY += 0.02f;
  }
  if (inputState->aHeld) {
    delX -= 0.02f;
  }
  if (inputState->sHeld) {
    delY -= 0.02f;
  }
  if (inputState->dHeld) {
    delX += 0.02f;
  }
}
