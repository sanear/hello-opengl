#include <glad/glad.h>

#include "./texture.h"
#include "../lib/stb_image.h" // TODO: relative path

// TODO: does it figure out the width & height? Stopping here.
Texture::Texture(const char *texturePath, int width, int height, int nrChannels) {
    data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
