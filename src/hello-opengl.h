#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../lib/linmath.h"

using namespace std;

// TODO: I know essentially for a fact that using god objects for flow control is wrong
// I just need to figure out what the "appropriate" structure here to separate concerns is
// Ideally, the control flow of the game would be (almost) entirely decoupled from the graphics
struct TriangleData {
  bool doPause;
  float pausedAtTime;
  float elapsedPauseTime;
  float offsetX;
  float offsetY;
  float offsetZ;
};

struct InputState {
  bool wHeld;
  bool aHeld;
  bool sHeld;
  bool dHeld;
};

typedef struct Vertex
{
    vec2 pos; // 2-space position
    vec3 col; // rgb color w/ float values
} Vertex;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void error_callback(int error, const char* description);

int do_triangle();
