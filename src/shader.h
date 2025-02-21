#ifndef SHADER_H
#define SHADER_H

#include <string>

using namespace std;

class Shader {
public:
    // program ID
    GLuint id;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();

    // Setters for uniform values
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
};

#endif
