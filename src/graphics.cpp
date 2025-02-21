#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "graphics.h"
#include "logic.h"
#include "shader.h"

using namespace std;

void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
void logUniformError(GLint uniform_location, string uniform_name, GLuint program);

// TODO: obviously this doesn't go here
bool START_PAUSED = true;
Triangle initTriangle(bool start_paused) {
  if (START_PAUSED) {
    return Triangle(glfwGetTime());
  } else {
    return Triangle();
  }
}

static struct InputState inputState = {false, false, false, false, 0.0f};
static Triangle triangle = initTriangle(START_PAUSED);

const float sqrt_48 = 0.69282032f;

static const Vertex triangles[3][3] = {{{{-0.4f, 0.f}, {1.f, 0.f, 0.f}},
                                        {{0.f, sqrt_48}, {0.f, 1.f, 0.f}},
                                        {{0.4f, 0.f}, {0.f, 0.f, 1.f}}},
                                       {{{-0.8f, -sqrt_48}, {1.f, 0.f, 0.f}},
                                        {{-0.4f, 0.f}, {0.f, 1.f, 0.f}},
                                        {{0.f, -sqrt_48}, {0.f, 0.f, 1.f}}},
                                       {{{0.f, -sqrt_48}, {1.f, 0.f, 0.f}},
                                        {{0.4f, 0.f}, {0.f, 1.f, 0.f}},
                                        {{0.8f, -sqrt_48}, {0.f, 0.f, 1.f}}}};

GLFWwindow *initGlfwWindow() {
  // Set callback for errors and initialize glfw
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    cout << "glfw could not initialize!" << endl;
    exit(EXIT_FAILURE);
  }

  // Set up window version and params
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create window object
  GLFWwindow *window = glfwCreateWindow(1600, 1200, "TRIANGLE", NULL, NULL);
  if (!window) {
    cout << "Failed to create glfw window!" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Callback for input handling
  glfwSetKeyCallback(window, key_callback);

  // Bind window to context
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // vsync
  glfwSwapInterval(1);
  return window;
}

void rotationMatrix(mat4x4 m, mat4x4 p, mat4x4 mvp, float delX, float delY,
                    float delZ, float ratio, float elapsedPause) {
  mat4x4_identity(m);
  mat4x4_translate(m, delX, delY, delZ);
  mat4x4_rotate_Z(m, m, 0.5 * ((float)glfwGetTime() - elapsedPause));
  mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  mat4x4_mul(mvp, p, m);
}

void doEverything() {
  cout << "Initializing window..." << endl;
  GLFWwindow *window = initGlfwWindow();

  cout << "Initializing shaders..." << endl;
  Shader shader = Shader("./shaders/vertex.vs", "./shaders/rgb_fragment.fs");
  Shader uniformShader = Shader("./shaders/vertex.vs", "./shaders/uniform_fragment.fs");

  // Setup buffer and array objects
  cout << "Setting up vertex array and buffer objects..." << endl;
  GLuint vertex_buffers[3];
  glGenBuffers(3, vertex_buffers);

  GLuint vertex_arrays[3];
  glGenVertexArrays(3, vertex_arrays);

  // Get attribute locations
  // vPos = vector of positions
  const GLint vpos_location = glGetAttribLocation(shader.id, "vPos");
  // vCol = vector of colors
  const GLint vcol_location = glGetAttribLocation(shader.id, "vCol");

  // Bind arrays &co for each of the three triangles
  for (int i = 0; i < 3; i++) {
    cout << "Binding vao and vbo " << i << endl;
    glBindVertexArray(vertex_arrays[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles[i]), triangles[i],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, col));
    glEnableVertexAttribArray(vcol_location);
  }

  // Get uniform locations
  // For standard program
  GLint mvp_location = glGetUniformLocation(shader.id, "MVP");
  logUniformError(mvp_location, "MVP", shader.id);

  // For uniform program
  GLint uniform_mvp_location = glGetUniformLocation(uniformShader.id, "MVP");
  logUniformError(uniform_mvp_location, "MVP", uniformShader.id);
  GLint uniColor_location = glGetUniformLocation(uniformShader.id, "uniColor");
  logUniformError(uniColor_location, "uniColor", uniformShader.id);

  // Main loop!
  // TODO: this should live separate from the graphics right?
  cout << "Starting render loop..." << endl;
  while (!glfwWindowShouldClose(window)) {
    // Update logic state
    triangle.doUpdate(glfwGetTime(), &inputState);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4 m, v, mvp; // don't need to be pointers, b/c arrays
    rotationMatrix(m, v, mvp, triangle.delX, triangle.delY, triangle.delZ,
                   ratio, triangle.elapsedPaused);

    for (int i = 0; i < 3; i++) {
      if (i == 1) {
        glUseProgram(uniformShader.id);
        glUniformMatrix4fv(uniform_mvp_location, 1, GL_FALSE,
                           (const GLfloat *)&mvp);

        glUniform1f(uniColor_location, triangle.colorMultiplier);
      } else {
        glUseProgram(shader.id);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
      }
      glBindVertexArray(vertex_arrays[i]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
  cout << "Done." << endl;
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void error_callback(int error, const char *description) {
  fprintf(stderr, "Error %i: %s\n", error, description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    cout << "Got ESC key from user; closing" << endl;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  handleKeys(key, action, &inputState);
}

void logUniformError(GLint uniform_location, string uniform_name, GLuint program) {
  if (uniform_location < 0) {
      cout << "ERROR::UNIFORM::Failed to find location of uniform " << uniform_name << " in program " << program << endl;
  }
}
