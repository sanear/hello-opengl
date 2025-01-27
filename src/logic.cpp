#include <iostream>
#include "logic.h"

using namespace std;

void Triangle::doUpdate(float currentTime, InputState* inputState) {
  cout << "Start state: (" << delX << "," << delY << "," << delZ << "), pause=" << doPause << "elapsedPause=" << elapsedPause << endl;
  if (doPause) {
    elapsedPause += currentTime - inputState->spacePressedAt;
    if (inputState->spacePressed) {
      doPause = false;
    }
  } else {
    if (inputState->spacePressed) {
      doPause = true;
      elapsedPause += currentTime - inputState->spacePressedAt;
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
  cout << "End state: (" << delX << "," << delY << "," << delZ << "), pause=" << doPause << "elapsedPause=" << elapsedPause << endl;
}
