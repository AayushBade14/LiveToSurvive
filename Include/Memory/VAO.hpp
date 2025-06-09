#pragma once

#include "./VBO.hpp"

class VAO{
  public:
    
    VAO(VBO &vbo);
    ~VAO();

    unsigned int& getID();
    
    void setVertexAttrib(int loc, int numVals, int stride, int start);

    void bind();
    void unbind();
  private:
  
    unsigned int ID;
};
