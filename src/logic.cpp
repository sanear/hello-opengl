#include "logic.h"

#include <iostream>

float COLOR_MULT_RATE = 0.05;

Triangle::Triangle() {
  construct();
}

Triangle::Triangle(float currentTime) {
  construct();
  pauseEvents.push_back(currentTime);
}

void Triangle::construct() {
  delX = 0.0f;
  delY = 0.0f;
  delZ = 0.0f;

  pauseEvents.reserve(32*sizeof(float)); // TODO: totally arbitrary
  elapsedPaused = 0.0f;
  colorMultiplier = 0.5f;
}

void Triangle::doUpdate(float currentTime, InputState *inputState) {
  // Handle pause input
  if (inputState->spacePressedAt > pauseEvents.back()) {
    pauseEvents.push_back(currentTime);
    inputState->spacePressedAt = 0.0f; // TODO: ick, but works
  }

  if (pauseEvents.size() % 2 == 0) { // <=> unpaused
    if (colorMultiplier > 0.0f) {
      colorMultiplier -= COLOR_MULT_RATE;
    }
  } else {
    if (colorMultiplier < 20.0f) {
      colorMultiplier += COLOR_MULT_RATE;
    }
  }

  // Advance pause timer
  // We need this to be correct every frame, so we have to update it
  // every frame.
  // We want to do a pairwise subtraction to establish the windows during
  // which the triangle is paused.
  // TODO: Surely this doesn't need to be recalculated every time...
  // Could cache the completed windows and just update the current one
  // (if paused, otherwise hold steady)
  elapsedPaused = 0.0f;
  for (int i = 0; i < pauseEvents.size(); i += 2) {
    if (i < pauseEvents.size() - 1) {
      elapsedPaused += pauseEvents[i+1] - pauseEvents[i];
    } else {
      elapsedPaused += currentTime - pauseEvents[i];
    }
  }
  // Handle translation input
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
