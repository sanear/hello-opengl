#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

// Stolen from glfw examples
// TODO: not this
#include "../lib/linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "../lib/models.h"  // TODO: impossible to imagine this being correct

using namespace std;

typedef struct Vertex
{
    vec2 pos; // 2-space position
    vec3 col; // rgb color w/ float values
} Vertex;

static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

// TODO: this is arcane
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



void lilGuyPoc() {
  ALilGuy *guy = new ALilGuy("Ava", 0, 0);

  cout << endl;
  cout << "There's a guy! " << guy->info() << endl;
  int delX = 16;
  int delY = 32;
  cout << "They're moving " << delX << " units right and " << delY << " units up!" << endl;
  float dist = guy->move(delX, delY);
  cout << "They moved " << dist << " units!" << endl;
  cout << guy->info() << endl;
}


// glfw callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
  // error unused?
}

int main() {

  // OO hello world
  lilGuyPoc();

  // Initialize glfw
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    cout << "glfw could not initialize!" << endl;
    return 1;
  }

  // Create a glfw window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "TRIANGLE", NULL, NULL);
  if (!window) {
    cout << "Failed to create glfw window!" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  // Start using window
  glfwMakeContextCurrent(window);
  //gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  //gladLoadGL();

  // This is vsync. Default is 0, which means you just swap as fast as you render
  // TODO: what unit is this?
  glfwSwapInterval(1);

  // TODO: OpenGL error checks have been omitted for brevity

  // TODO: parse this copypasta
  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // TODO: Get your lsp working so you can drill into this source
  // Fairly certain most of this is about shadow and stuff that I probably don't do right now
  const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);

  const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
  glCompileShader(fragment_shader);

  // Link the shaders up to a program object
  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  // TODO: parse
  // mvp?
  // vpos = vertex position?
  // col might not be color...
  const GLint mvp_location = glGetUniformLocation(program, "MVP");
  const GLint vpos_location = glGetAttribLocation(program, "vPos");
  const GLint vcol_location = glGetAttribLocation(program, "vCol");

  // I hate wrapper types... why can't I use a regular int...
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
  while (!glfwWindowShouldClose(window)) {
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
    mat4x4_rotate_Z(m, m, (float) glfwGetTime());
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    // prefixes have switched from glfw to just gl...
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // TODO: They told me about this but we're not using it
    // double time = glfwGetTime();

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
