#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "graphics.h"
#include "logic.h"

using namespace std;

void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
void logShaderError(GLuint shader);
void logShaderProgramError(GLuint shaderProgram);

// Text of GLSL code for simple vertex and fragment shaders
// Evidently, this will sometimes be the time when we convert
// application x,y,z to "normalized device coordinates", which
// are (understandably) scaled to [-1.0, 1.0] and (bafflingly)
// centered at (0.5,0.5,0.5)
// ... I am guessing that we do that transformation in the
// shader in order to push as much vertex-level computation as
// possible to the GPU
static const char *vertex_shader_text =
    "#version 330 core\n"
    "uniform mat4 MVP;\n"
    "in vec2 vPos;\n"
    "in vec3 vCol;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char *fragment_shader_text = "#version 330 core\n"
                                          "in vec3 color;\n"
                                          "out vec4 fragment;\n"
                                          "void main()\n"
                                          "{\n"
                                          "    fragment = vec4(color, 1.0);\n"
                                          "}\n";

static struct InputState inputState = {false, false, false, false, 0.0f};
static Triangle triangle = Triangle();
static const Vertex triangles[3][3] = {{{{-0.3f, 0.f}, {1.f, 0.f, 0.f}},
                                        {{0.3f, 0.f}, {0.f, 1.f, 0.f}},
                                        {{0.f, 0.6f}, {0.f, 0.f, 1.f}}},
                                       {{{-0.6f, -0.4f}, {1.f, 0.f, 0.f}},
                                        {{0.f, 0.4f}, {0.f, 1.f, 0.f}},
                                        {{-0.3f, 0.f}, {0.f, 0.f, 1.f}}},
                                       {{{0.f, -0.4f}, {1.f, 0.f, 0.f}},
                                        {{0.6f, -0.4f}, {0.f, 1.f, 0.f}},
                                        {{0.3f, 0.f}, {0.f, 0.f, 1.f}}}};

void setupShader(GLuint *shader, const char *shaderText) {
  glShaderSource(*shader, 1, &shaderText, NULL);
  glCompileShader(*shader);
  logShaderError(*shader);
}

void setupVertexShader(GLuint *shader) {
  *shader = glCreateShader(GL_VERTEX_SHADER);
  setupShader(shader, vertex_shader_text);
}

void setupFragmentShader(GLuint *shader) {
  *shader = glCreateShader(GL_FRAGMENT_SHADER);
  setupShader(shader, fragment_shader_text);
}

void setupShaderProgram(GLuint *shaderProgram, const GLuint *shaders[],
                        int shaderCount) {
  *shaderProgram = glCreateProgram();
  for (int i = 0; i < shaderCount; i++) {
    cout << "Attaching shader " << *shaders[i] << " to program "
         << *shaderProgram << endl;
    glAttachShader(*shaderProgram, *shaders[i]);
  }
  glLinkProgram(*shaderProgram);
  logShaderProgramError(*shaderProgram);
  for (int i = 0; i < shaderCount; i++) {
    glDeleteShader(*shaders[i]);
  }
}

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
  GLFWwindow *window = glfwCreateWindow(1280, 960, "TRIANGLE", NULL, NULL);
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

// TODO: What if we just do each of these two processes once for each triangle..?
void setupVertexBuffer(GLuint *vertex_buffer, const Vertex* triangle) {
  glGenBuffers(1, vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), triangle, GL_STATIC_DRAW);
}

// TODO: Do we need to do this entire process for each triangle...
// Does this mean that the shaders have to be rewritten knowing exactly how many to draw???
// Surely they're a per-triangle configuration...
void setupVertexArrayObject(const GLuint *program, GLuint *vertex_array,
                            GLint *mvp_location) {
  // Get the location of MVP based on the vertex shader code
  *mvp_location = glGetUniformLocation(*program, "MVP");
  // Get the location of vpos and vcol based on the vertex shader code
  const GLint vpos_location = glGetAttribLocation(*program, "vPos");
  const GLint vcol_location = glGetAttribLocation(*program, "vCol");

  // TODO: all of the below seems to be predicated on the drawing of one triangle
  // Generate vertex array object
  glGenVertexArrays(1, vertex_array);
  // Bind vertex array object to context or whatever
  glBindVertexArray(*vertex_array);
  // Enable the attribute stored at vpos_location (i.e., vpos); same for vcol
  glEnableVertexAttribArray(vpos_location);
  glEnableVertexAttribArray(vcol_location);
  // This is grabbing the first triangle by position... but where are the other two???
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, pos));
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, col));
}

void rotationMatrix(mat4x4 m, mat4x4 p, mat4x4 mvp, float delX, float delY,
                    float delZ, float ratio, float elapsedPause) {
  mat4x4_identity(m);
  mat4x4_translate(m, delX, delY, delZ);
  mat4x4_rotate_Z(m, m, (float)glfwGetTime() - elapsedPause);
  mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  mat4x4_mul(mvp, p, m);
}

void doEverything() {
  cout << "Initializing window..." << endl;
  GLFWwindow *window = initGlfwWindow();

  cout << "Initializing shaders..." << endl;
  GLuint vertex_shader, fragment_shader;
  setupVertexShader(&vertex_shader);
  setupFragmentShader(&fragment_shader);
  const GLuint *shaders[] = {&vertex_shader, &fragment_shader};

  cout << "Creating program..." << endl;
  GLuint program;
  setupShaderProgram(&program, shaders, 2);

  cout << "Setting up vertex buffer and array objects..." << endl;
  GLuint vertex_arrays[3];
  GLint mvp_locations[3];
  GLuint vertexBuffers[3];
  for (int i = 0; i < 3; i++) {
    // TODO: Annoying that I can't do this in one line without decaying to int*
    Vertex t[3];
    for (int j = 0; j < 3; j++) {
      t[j] = triangles[i][j];
    }
    // const Vertex *t = triangles[i]; // TODO: I don't understand why I can't use array type here
    setupVertexBuffer(&vertexBuffers[i], t);

    setupVertexArrayObject(&program, &vertex_arrays[i], &mvp_locations[i]);
  }
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

    glUseProgram(program);
    for (int i = 0; i < 3; i++) {
      glUniformMatrix4fv(mvp_locations[i], 1, GL_FALSE, (const GLfloat *)&mvp);
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

void logShaderError(GLuint shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::" << shader << "::COMPILATION_FAILED\n"
         << infoLog << endl;
  }
}

void logShaderProgramError(GLuint shaderProgram) {
  GLint success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    cout << "ERROR::SHADER_PROGRAM::" << shaderProgram
         << "::COMPILATION_FAILED\n"
         << infoLog << endl;
  }
}
