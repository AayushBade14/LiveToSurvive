#include "./Texture.hpp"

void Texture::loadTexture(const std::string &path, GLenum wrapS, GLenum wrapT, GLenum minF, GLenum magF, GLenum flag){
  
  stbi_set_flip_vertically_on_load(true);

  glGenTextures(1,&ID);
  bind();

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapS);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minF);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magF);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(path.c_str(),&width,&height,&nrChannels,0);
  if(data){
    glTexImage2D(GL_TEXTURE_2D,0,flag,width,height,0,flag,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  stbi_image_free(data);
  unbind();
}

Texture::Texture(const std::string &path,GLenum flag){
  loadTexture(path,GL_REPEAT,GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR,flag);
}

Texture::Texture(const std::string &path, GLenum wrapS, GLenum wrapT, GLenum minF, GLenum magF, GLenum flag){
  loadTexture(path,wrapS,wrapT,minF,magF,flag);
}

Texture::~Texture(){
  unbind();
  glDeleteTextures(1,&ID);
}

unsigned int& Texture::getID(){
  return ID;
}

void Texture::setSamplerValue(Shader &shader,const std::string &name, int value){
  shader.use();
  shader.setValue(name,value);
}

void Texture::assignTextureUnit(int value){
  glActiveTexture(GL_TEXTURE0 + value);
  bind();
}

void Texture::bind(){
  glBindTexture(GL_TEXTURE_2D,ID);
}

void Texture::unbind(){
  glBindTexture(GL_TEXTURE_2D,0);
}
