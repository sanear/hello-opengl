#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

// Stolen from glfw examples
// TODO: not this
#include "../lib/linmath.h"
#include "hello-opengl.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

using namespace std;

static TriangleData triangle = {
  false, // true on frame 1, won't draw until unpaused
  0.f,
  0.f,
  0.f,
  0.f,
  0.f
};

static InputState inputState = {
  false,
  false,
  false,
  false
};


static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};



// Text of GLSL code for simple vertex and fragment shaders
// Evidently, this will sometimes be the time when we convert
// application x,y,z to "normalized device coordinates", which
// are (understandably) scaled to [-1.0, 1.0] and (bafflingly)
// centered at (0.5,0.5,0.5)
// ... I am guessing that we do that transformation in the
// shader in order to push as much vertex-level computation as
// possibl to the GPU
static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

// glfw callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  switch (key) {
  case GLFW_KEY_ESCAPE:
    if (action == GLFW_PRESS) {
      cout << "Got ESC key from user; closing" << endl;
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    break;
  case GLFW_KEY_SPACE:
    if (action == GLFW_PRESS) {
      if (!triangle.doPause) {
        triangle.pausedAtTime = glfwGetTime();
      } else {
        triangle.elapsedPauseTime += glfwGetTime() - triangle.pausedAtTime;
      }
      triangle.doPause = !triangle.doPause;
    }
    break;
  case GLFW_KEY_W:
    if (action == GLFW_PRESS) { // TODO: Repeat is a bad experience
      inputState.wHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState.wHeld = false;
    }
    break;
  case GLFW_KEY_A:
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      inputState.aHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState.aHeld = false;
    }
    break;
  case GLFW_KEY_S:
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      inputState.sHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState.sHeld = false;
    }
    break;
  case GLFW_KEY_D:
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      inputState.dHeld = true;
    } else if (action == GLFW_RELEASE) {
      inputState.dHeld = false;
    }
    break;
  }
}

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error %i: %s\n", error, description);
}

void logShaderError(GLuint shader, string type) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" <<
      infoLog << endl;
  }
}

int do_triangle() {
// Initialize glfw
  cout << "Initializing glfw..." << endl;
  glfwSetErrorCallback(error_callback);
  // glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE); // TODO: desirable in some ways, but disables input and window selection
  if (!glfwInit()) {
    cout << "glfw could not initialize!" << endl;
    exit(EXIT_FAILURE);
  }

  // Create a glfw window
  cout << "Creating glfw window hints..." << endl;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  cout << "Creating glfw window..." << endl;
  GLFWwindow* window = glfwCreateWindow(1280, 960, "TRIANGLE", NULL, NULL);
  if (!window) {
    cout << "Failed to create glfw window!" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  // Start using window
  cout << "Making context current (window)..." << endl;
  glfwMakeContextCurrent(window);
  //
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  // gladLoadGL();

  // This is vsync. Default is 0, which means you just swap as fast as you render
  // TODO: what unit is this?
  cout << "Setting swap interval..." << endl;
  glfwSwapInterval(1);

  // TODO: OpenGL error checks have been omitted for brevity

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);
  logShaderError(vertex_shader, "VERTEX");

  const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
  glCompileShader(fragment_shader);
  logShaderError(vertex_shader, "VERTEX");

  // Link the shaders up to a program object
  cout << "Creating program..." << endl;
  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  cout << "Linking program to GL..." << endl;
  glLinkProgram(program);

  // TODO: parse
  // mvp = model*view*projection, a matrix multiplication for camera/3d world stuff
  // vPos = vector of positions
  // vCol = vector of colors
  const GLint mvp_location = glGetUniformLocation(program, "MVP");
  const GLint vpos_location = glGetAttribLocation(program, "vPos");
  const GLint vcol_location = glGetAttribLocation(program, "vCol");

  // I hate wrapper types... why can't I use a regular int...
  // TODO: I have a feeling that these are forcing the triangle's position to stay constant.
  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void*) offsetof(Vertex, pos));
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void*) offsetof(Vertex, col));

  // Control loop
  cout << "Starting main loop..." << endl;
  while (!glfwWindowShouldClose(window)) {
    // TODO: obviously this belongs in an updateState method that lives with the position data
    float keyDelta = 0.01f;
    // Process input state changes
    if (inputState.wHeld) {
      triangle.offsetY += keyDelta;
    }
    if (inputState.aHeld) {
      triangle.offsetX -= keyDelta;
    }
    if (inputState.sHeld) {
      triangle.offsetY -= keyDelta;
    }
    if (inputState.dHeld) {
      triangle.offsetX += keyDelta;
    }

    // do stuff
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float) height;

    // Viewport is whole window
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    // The Linear Algebra Part
    mat4x4 m, p, mvp;
    mat4x4_identity(m);
    if (!triangle.doPause) {
      mat4x4_translate_in_place(m, triangle.offsetX, triangle.offsetY, 0.f);
      mat4x4_rotate_Z(m, m, (float) glfwGetTime() - triangle.elapsedPauseTime);
      mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      mat4x4_mul(mvp, p, m);
    }

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Once back buffer is rendered, bring to front (this is how we advance frames)
    glfwSwapBuffers(window);

    // Could use glfwWaitEvents instead - for the slide puzzle game, this might actually
    // make sense, because the screen will stay static a lot
    glfwPollEvents();
  }

  // "finally"
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
