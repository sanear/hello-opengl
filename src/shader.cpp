#include <glad/glad.h>

#include "./shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

string readFile(const char *path);
GLint compileShader(const char *shaderCode, GLenum shader_type);

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  glCreateShader(GL_VERTEX_SHADER);
  cout << "Reading shader code from file..." << endl;
  string vertexCode = readFile(vertexPath);
  string fragmentCode = readFile(fragmentPath);
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  cout << "Compiling shaders..." << endl;
  glCreateShader(GL_VERTEX_SHADER);
  cout << "test over..." << endl;
  GLint vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
  GLint fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

  // DRY => less memory reuse, in this case
  cout << "Linking shader program..." << endl;
  int success;
  char infoLog[512];
  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(id, 512, NULL, infoLog);
      cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

GLint compileShader(const char *shaderCode, GLenum shader_type) {
  unsigned int shader;
  int success;
  char infoLog[512];

  cout << "Creating shader, type " << shader_type << endl;
  shader = glCreateShader(shader_type);
  cout << "Got id " << shader << endl;
  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);
  cout << "Shader compiled." << endl;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::" << shader_type << "COMPILATION_FAILED\n"
         << infoLog << endl;
  }
  return shader;
}

string readFile(const char *path) {
  string code;
  ifstream file;
  file.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    file.open(path);
    stringstream stream;
    stream << file.rdbuf();
    file.close();
    code = stream.str();
  } catch (ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:" << path << endl;
  }
  return code;
}
