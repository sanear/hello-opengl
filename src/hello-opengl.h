#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../lib/linmath.h"

struct God {
  bool doPause;
  float pausedAtTime;
  float elapsedPauseTime;
  float nextTransX;
  float nextTransY;
  float nextTransZ;

  void resetCmds();
};

typedef struct Vertex
{
    vec2 pos; // 2-space position
    vec3 col; // rgb color w/ float values
} Vertex;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void error_callback(int error, const char* description);

int do_triangle();
