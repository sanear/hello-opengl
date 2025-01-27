#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "input.h"
#include "graphics.h"

using namespace std;

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void logShaderError(GLuint shader);
void logShaderProgramError(GLuint shaderProgram);

// Text of GLSL code for simple vertex and fragment shaders
const char* vertex_shader_text =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main() {\n"
  "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\n";

const char* fragment_shader_text =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n";

const float vertices[] = {
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
  0.0f, 0.5f, 0.0f
};


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

  // Callback for framebuffer sizing
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Callback for input handling
  glfwSetKeyCallback(window, key_callback);

  // Bind window to context
  glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

  // vsync
  glfwSwapInterval(1);
  return window;
}

void setupShader(GLuint* shader, const char* shaderText, GLuint shader_type) {
  *shader = glCreateShader(shader_type);
  glShaderSource(*shader, 1, &shaderText, NULL);
  glCompileShader(*shader);
  logShaderError(*shader);
}

void setupVertexShader(GLuint* shader) {
  setupShader(shader, vertex_shader_text, GL_VERTEX_SHADER);
}

// TODO: why tf didn't this work with a pointer to the now-returned value?
void setupFragmentShader(GLuint* shader) {
  setupShader(shader, fragment_shader_text, GL_FRAGMENT_SHADER);
}

void setupShaderProgram(GLuint* shaderProgram, const GLuint* shaders[], int shaderCount) {
  *shaderProgram = glCreateProgram();
  for (int i = 0; i < shaderCount; i++) {
    cout << "Attaching shader " << *shaders[i] << " to program " << *shaderProgram << endl;
    glAttachShader(*shaderProgram, *shaders[i]);
    // TODO Once attached, no longer needed?
    // glDeleteShader(*shaderProgram);
  }
  glLinkProgram(*shaderProgram);
  logShaderProgramError(*shaderProgram);
}

void doEverything() {
  cout << "Initializing window..." << endl;
  GLFWwindow* window = initGlfwWindow();
  
  cout << "Initializing shaders..." << endl;
  GLuint vertex_shader;
  setupVertexShader(&vertex_shader);
  GLuint fragment_shader;
  setupFragmentShader(&fragment_shader);
  const GLuint *shaders[] = {&vertex_shader, &fragment_shader};

  cout << "Creating program..." << endl;
  GLuint shaderProgram;
  setupShaderProgram(&shaderProgram, shaders, 2);
  cout << "Got program " << shaderProgram << endl;
  // GLuint shaderProgram = glCreateProgram();
  // for (int i=0; i < 2; i++) {
  //   cout << "Attaching shader " << *shaders[i] << " to program " << shaderProgram << endl;
  //   glAttachShader(shaderProgram, *shaders[i]);
  //   glDeleteShader(*shaders[i]);
  // }

  // glLinkProgram(shaderProgram);
  // logShaderProgramError(shaderProgram);

  cout << "Linking vertex attributes..." << endl;
  GLuint vertex_buffer_object, vertex_array_object;
  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);

  glBindVertexArray(vertex_array_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);

  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  cout << "Starting render loop..." << endl;
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cout << "Done." << endl;
  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error %i: %s\n", error, description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    cout << "Got ESC key from user; closing" << endl;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  // TODO wire up input and stuff
  //handleKeys(key, action);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void logShaderError(GLuint shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::" << shader << "::COMPILATION_FAILED\n" << infoLog << endl;
  }
}

void logShaderProgramError(GLuint shaderProgram) {
  GLint success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    cout << "ERROR::SHADER::" << shaderProgram << "::COMPILATION_FAILED\n" << infoLog << endl;
  }
}
