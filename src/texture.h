#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    unsigned char *data;
    int width, height, nrChannels;

    GLuint texture;

    Texture(const char *texturePath);
};

#endif
