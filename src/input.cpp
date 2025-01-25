#include <GLFW/glfw3.h>

struct InputState {
  bool wHeld;
  bool aHeld;
  bool sHeld;
  bool dHeld;
};

static InputState inputState = {
  false,
  false,
  false,
  false
};

// void handleKeys(int key, int action) {
//     switch (key) {
//   case GLFW_KEY_SPACE:
//     if (action == GLFW_PRESS) {
//       if (!triangle.doPause) {
//         triangle.pausedAtTime = glfwGetTime();
//       } else {
//         triangle.elapsedPauseTime += glfwGetTime() - triangle.pausedAtTime;
//       }
//       triangle.doPause = !triangle.doPause;
//     }
//     break;
//   case GLFW_KEY_W:
//     if (action == GLFW_PRESS) { // TODO: Repeat is a bad experience
//       inputState.wHeld = true;
//     } else if (action == GLFW_RELEASE) {
//       inputState.wHeld = false;
//     }
//     break;
//   case GLFW_KEY_A:
//     if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//       inputState.aHeld = true;
//     } else if (action == GLFW_RELEASE) {
//       inputState.aHeld = false;
//     }
//     break;
//   case GLFW_KEY_S:
//     if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//       inputState.sHeld = true;
//     } else if (action == GLFW_RELEASE) {
//       inputState.sHeld = false;
//     }
//     break;
//   case GLFW_KEY_D:
//     if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//       inputState.dHeld = true;
//     } else if (action == GLFW_RELEASE) {
//       inputState.dHeld = false;
//     }
//     break;
//   }
// }
