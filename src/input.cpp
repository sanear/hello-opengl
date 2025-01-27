#include <iostream>
#include <GLFW/glfw3.h>
#include "input.h"

using namespace std;

// TODO: this could be a class
void handleKeys(int key, int action, InputState* inputState) {
    switch (key) {
  case GLFW_KEY_SPACE:
    if (action == GLFW_PRESS) {
      cout << "Got space." << endl;
      inputState->spacePressed = true;
      inputState->spacePressedAt = glfwGetTime();
    } else {
      inputState->spacePressed = false;
    }
    break;
  case GLFW_KEY_W:
    if (action == GLFW_PRESS) {
      cout << "W press." << endl;
      inputState->wHeld = true;
    } else if (action == GLFW_RELEASE) {
      cout << "W release." << endl;
      inputState->wHeld = false;
    }
    break;
  case GLFW_KEY_A:
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      cout << "A press." << endl;
      inputState->aHeld = true;
    } else if (action == GLFW_RELEASE) {
      cout << "A release." << endl;
      inputState->aHeld = false;
    }
    break;
  case GLFW_KEY_S:
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      cout << "S press." << endl;
      inputState->sHeld = true;
    } else if (action == GLFW_RELEASE) {
      cout << "S release." << endl;
      inputState->sHeld = false;
    }
    break;
  case GLFW_KEY_D:
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      cout << "D press." << endl;
      inputState->dHeld = true;
    } else if (action == GLFW_RELEASE) {
      cout << "D release." << endl;
      inputState->dHeld = false;
    }
    break;
  }
}
