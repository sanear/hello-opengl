#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "input.h"
#include "graphics.h"

using namespace std;

// Text of GLSL code for simple vertex and fragment shaders
// Evidently, this will sometimes be the time when we convert
// application x,y,z to "normalized device coordinates", which
// are (understandably) scaled to [-1.0, 1.0] and (bafflingly)
// centered at (0.5,0.5,0.5)
// ... I am guessing that we do that transformation in the
// shader in order to push as much vertex-level computation as
// possible to the GPU
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

static const Vertex centerTriangle[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error %i: %s\n", error, description);
}

// TODO implementation
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    cout << "Got ESC key from user; closing" << endl;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  // TODO wire up input and stuff
  //handleKeys(key, action);
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

void setupVertexBuffer(GLuint* vertexBuffer) {
  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(centerTriangle), centerTriangle, GL_STATIC_DRAW);
}

void setupShader(GLuint* shader, const char* shaderText, string kind) {
  glShaderSource(*shader, 1, &shaderText, NULL);
  glCompileShader(*shader);
  logShaderError(*shader, kind);
}

void setupVertexShader(GLuint* vShader, const char* shader_text) {
  setupShader(vShader, shader_text, "VERTEX");
}

void setupFragmentShader(GLuint* fShader, const char* shader_text) {
  setupShader(fShader, shader_text, "FRAGMENT");
}

void setupProgram(const GLuint* program, const GLuint* shaders[], int shaderCount) {
  for (int i = 0; i < shaderCount; i++) {
    glAttachShader(*program, *shaders[i]);
  }
}

GLFWwindow* initGlfwWindow() {
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
  GLFWwindow* window = glfwCreateWindow(1280, 960, "TRIANGLE", NULL, NULL);
  if (!window) {
    cout << "Failed to create glfw window!" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Callback for input handling
  glfwSetKeyCallback(window, key_callback);

  // Bind window to context
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

  // vsync
  glfwSwapInterval(1);
  return window;
}

void mystifyingLinAlg(const GLuint* program, GLuint* vertex_array, GLint* mvp_location) {
  // mvp = model*view*projection, a matrix multiplication for camera/3d world stuff
  // vPos = vector of positions
  // vCol = vector of colors
  GLint mvp_loc = glGetUniformLocation(*program, "MVP");
  mvp_location = &mvp_loc;
  const GLint vpos_location = glGetAttribLocation(*program, "vPos");
  const GLint vcol_location = glGetAttribLocation(*program, "vCol");

  glGenVertexArrays(1, vertex_array);
  glBindVertexArray(*vertex_array);
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void*) offsetof(Vertex, pos));
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void*) offsetof(Vertex, col));
}

void rotationMatrix(mat4x4 m, mat4x4 p, mat4x4 mvp, float ratio) {
  mat4x4_identity(m);
  mat4x4_rotate_Z(m, m, (float) glfwGetTime());
  mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  mat4x4_mul(mvp, p, m);
}

void doEverything() {
  cout << "Initializing window..." << endl;
  GLFWwindow* window = initGlfwWindow();
  
  GLuint vertex_buffer;
  setupVertexBuffer(&vertex_buffer);

  cout << "Initializing shaders..." << endl;
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  setupVertexShader(&vertex_shader, vertex_shader_text);
  setupFragmentShader(&fragment_shader, fragment_shader_text);
  const GLuint *shaders[] = {&vertex_shader, &fragment_shader};

  cout << "Creating program..." << endl;
  const GLuint program = glCreateProgram();
  setupProgram(&program, shaders, 2);

  GLuint vertex_array;
  GLint mvp_location;
  mystifyingLinAlg(&program, &vertex_array, &mvp_location);

  // Main loop!
  // TODO: this should live separate from the graphics right?
  cout << "Starting render loop..." << endl;
  while(!glfwWindowShouldClose(window)) {
    // TODO: get input from input layer..?

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4 m, v, mvp;  // don't need to be pointers, b/c arrays
    rotationMatrix(m, v, mvp, ratio);

    glUseProgram(program); // TODO every iteration?
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
  cout << "Done." << endl;
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
