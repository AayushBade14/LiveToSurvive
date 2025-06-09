#include "./VAO.hpp"

VAO::VAO(VBO& vbo){
  glGenVertexArrays(1,&ID);
  bind();

  vbo.bind();

  unbind();
}

VAO::~VAO(){
  glDeleteVertexArrays(1,&ID);
}

unsigned int& VAO::getID(){
  return ID;
}

void VAO::setVertexAttrib(int loc, int numValues, int stride, int start){
  bind();

  glVertexAttribPointer(loc,numValues,GL_FLOAT,GL_FALSE,stride * sizeof(float),(void*)(start * sizeof(float)));
  glEnableVertexAttribArray(loc);

  unbind();
}

void VAO::bind(){
  glBindVertexArray(ID);
}

void VAO::unbind(){
  glBindVertexArray(0);
}
