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
void logUniformError(GLint uniform_location, string uniform_name, GLuint program);

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
    "in vec3 vCol;\n"
    "in vec2 vPos;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char *rgb_fragment_shader_text =
    "#version 330 core\n"
    "in vec3 color;\n"
    "out vec4 fragment;\n"
    "void main()\n"
    "{\n"
    "    fragment = vec4(color, 1.0);\n"
    "}\n";

static const char *uniform_fragment_shader_text =
    "#version 330 core\n"
    "uniform float uniColor;\n"
    "in vec3 color;\n"
    "out vec4 fragment;\n"
    "void main()\n"
    "{\n"
    "    fragment = vec4(uniColor * color, 1.0);\n"
    "}\n";

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

void setupShader(GLuint *shader, const char *shaderText) {
  glShaderSource(*shader, 1, &shaderText, NULL);
  glCompileShader(*shader);
  logShaderError(*shader);
}

void setupVertexShader(GLuint *shader, const char *text) {
  *shader = glCreateShader(GL_VERTEX_SHADER);
  setupShader(shader, text);
}

void setupFragmentShader(GLuint *shader, const char *text) {
  *shader = glCreateShader(GL_FRAGMENT_SHADER);
  setupShader(shader, text);
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
  GLuint vertex_shader, fragment_shader, uniform_fragment_shader;
  setupVertexShader(&vertex_shader, vertex_shader_text);
  setupFragmentShader(&fragment_shader, rgb_fragment_shader_text);
  setupFragmentShader(&uniform_fragment_shader, uniform_fragment_shader_text);
  const GLuint *shaders[] = {&vertex_shader, &fragment_shader};
  const GLuint *shaders_2[] = {&vertex_shader, &uniform_fragment_shader};

  cout << "Creating program..." << endl;
  GLuint program, uniform_program;
  setupShaderProgram(&program, shaders, 2);
  setupShaderProgram(&uniform_program, shaders_2, 2);

  // Setup buffer and array objects
  cout << "Setting up vertex array and buffer objects..." << endl;
  GLuint vertex_buffers[3];
  glGenBuffers(3, vertex_buffers);

  GLuint vertex_arrays[3];
  glGenVertexArrays(3, vertex_arrays);

  // Get attribute locations
  // vPos = vector of positions
  const GLint vpos_location = glGetAttribLocation(program, "vPos");
  // vCol = vector of colors
  const GLint vcol_location = glGetAttribLocation(program, "vCol");

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
  GLint mvp_location = glGetUniformLocation(program, "MVP");
  logUniformError(mvp_location, "MVP", program);

  // For uniform program
  GLint uniform_mvp_location = glGetUniformLocation(uniform_program, "MVP");
  logUniformError(uniform_mvp_location, "MVP", uniform_program);
  GLint uniColor_location = glGetUniformLocation(uniform_program, "uniColor");
  logUniformError(uniColor_location, "uniColor", uniform_program);

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
        glUseProgram(uniform_program);
        glUniformMatrix4fv(uniform_mvp_location, 1, GL_FALSE,
                           (const GLfloat *)&mvp);

        glUniform1f(uniColor_location, triangle.colorMultiplier);
      } else {
        glUseProgram(program);
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

void logUniformError(GLint uniform_location, string uniform_name, GLuint program) {
  if (uniform_location < 0) {
      cout << "ERROR::UNIFORM::Failed to find location of uniform " << uniform_name << " in program " << program << endl;
  }
}
