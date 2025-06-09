#pragma once

#include "../Shader/Shader.hpp"
#include "../stb_image/stb_image.h"

#include <iostream>
#include <string>

class Texture{
  public:

    Texture(const std::string &path, GLenum flag);  
    Texture(const std::string &path, GLenum wrapS, GLenum wrapT, GLenum minF, GLenum magF, GLenum flag);
    ~Texture();

    unsigned int& getID();

    void setSamplerValue(Shader &shader,const std::string &name, int value);
    void assignTextureUnit(int value);

    void bind();
    void unbind();

  private:
    unsigned int ID;

    void loadTexture(const std::string &path, GLenum wrapS, GLenum wrapT, GLenum minF, GLenum magF, GLenum flag);
};
