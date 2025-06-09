#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <string>

class VBO{
  public:
    VBO(std::vector<float> &vertices,GLenum method);
    ~VBO();
    
    unsigned int& getID();

    void bind();
    void unbind();

  private:
    
    unsigned int ID;
};
