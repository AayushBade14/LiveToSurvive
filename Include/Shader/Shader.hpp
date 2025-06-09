#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader{
  public:
    // constructors & destructors
    Shader(const std::string &vertexPath,const std::string &fragmentPath);
    ~Shader();
    
    // getters
    unsigned int& getID();
    
    // setters
    template <typename T>
    void setValue(const std::string &name,T &value) const{
      unsigned int loc = glGetUniformLocation(ID,name.c_str());

      if constexpr(std::is_same<T,int>::value){
        glUniform1i(loc,value);
      }
      else if constexpr(std::is_same<T,float>::value){
        glUniform1f(loc,value);
      }
      else if constexpr(std::is_same<T,bool>::value){
        glUniform1i(loc,(int)value);
      }
      else if constexpr(std::is_same<T,glm::vec2>::value){
        glUniform2fv(loc,1,&value[0]);
      }
      else if constexpr(std::is_same<T,glm::vec3>::value){
        glUniform3fv(loc,1,&value[0]);
      }
      else if constexpr(std::is_same<T,glm::mat4>::value){
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(matrix));
      }
    }
    
    // other methods
    void use();

  private:
    unsigned int ID; // shader program id
    
    // helper methods
    const std::string& LoadFile(const std::string &path);
    unsigned int& compileShader(const std::string &code,bool isVertex);
    void createShaderProgram(unsigned int &vertex,unsigned int &fragment);
};
