#include "logic.h"
#include <iostream>

using namespace std;

void Triangle::doUpdate(float currentTime, InputState *inputState) {
  // cout << "Start state: (" << delX << "," << delY << "," << delZ
  //      << "), pause=" << doPause << " elapsedPause=" << elapsedPause << endl;
  if (inputState->spacePressed) {
    doPause = !doPause;
  }
  if (doPause) {
    elapsedPause = currentTime - inputState->spacePressedAt;
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
