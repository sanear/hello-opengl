#include <GLFW/glfw3.h>
#include "input.h"

// TODO: this could be a class
void handleKeys(int key, int action, InputState* inputState) {
  switch (key) {
  case GLFW_KEY_SPACE:
    if (action == GLFW_PRESS) {
      inputState->spaceHeld = true;
      inputState->spacePressedAt = glfwGetTime();
    } else if (action == GLFW_RELEASE) {
      inputState->spaceHeld = false;
    }
    break;
  case GLFW_KEY_W:
    if (action == GLFW_PRESS) {
      inputState->wHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState->wHeld = false;
    }
    break;
  case GLFW_KEY_A:
    if (action == GLFW_PRESS) {
      inputState->aHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState->aHeld = false;
    }
    break;
  case GLFW_KEY_S:
    if (action == GLFW_PRESS) {
      inputState->sHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState->sHeld = false;
    }
    break;
  case GLFW_KEY_D:
    if (action == GLFW_PRESS) {
      inputState->dHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState->dHeld = false;
    }
    break;
  }
}
