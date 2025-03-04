#ifndef SHADER_H
#define SHADER_H

#include <string>

using namespace std;

class Texture {
public:
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0)

    GLuint texture;

    Texture(const char *texturePath, int width, int height, int nrChannels);

}

#endif
