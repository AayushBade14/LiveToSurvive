#include "./Shader.hpp"

Shader::Shader(const std::string &vertexPath,const std::string &fragmentPath){
  const std::string vCode = loadFile(vertexPath);
  const std::string fCode = loadFile(fragmentPath);

  unsigned int vertex = compileShader(vCode,true);
  unsigned int fragment = compileShader(fCode,false);

  createShaderProgram(vertex,fragment);
}

Shader::~Shader(){
  glDeleteProgram(ID);
}

unsigned int& Shader::getID(){
  return ID;
}

const std::string& loadFile(const std::string &path){
  std::ifstream shaderFile;
  std::string code;

  shaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  try{
    shaderFile.open(path);
    
    std::stringstream stream;
    
    stream << shaderFile.rdbuf();

    shaderFile.close();
    
    code = stream.str();
  }
  catch(const std::ifstream::failure &e){
    std::cerr<<"ERROR: Reading shader file -> PATH: ' "<<path<<" '"<<std::endl;
  }

  return code;
}

unsigned int& compileShader(const std::string &code,bool isVertex){
  unsigned int shader;
  
  int success;
  char infoLog[512];
  
  std::string log;
  
  if(isVertex){
    shader = glCreateShader(GL_VERTEX_SHADER);
    log = "VERTEX_SHADER";
  }
  else{
    shader = glCreateShader(GL_FRAGMENT_SHADER);
    log = "FRAGMENT_SHADER";
  }

  glShaderSource(shader,1,&code,NULL);
  glCompileShader(shader);

  glGetShaderiv(shader,GL_COMPILE_SHADER,&success);
  if(!success){
    glGetShaderInfoLog(shader,512,NULL,infoLog);
    std::cerr<<"ERROR: Compiling "<< log << " REASON: "<<infoLog<<std::endl;
  }

  return shader;
}

void Shader::createShaderProgram(unsigned int &vertex,unsigned int &fragment){
  int success;
  char infoLog[512];

  ID = glCreateProgram();
  glAttachShader(ID,vertex);
  glAttachShader(ID,fragment);
  glLinkProgram(ID);
  
  glGetProgramiv(ID,GL_LINK_PROGRAM,&success);
  if(!success){
    glGetProgramInfoLog(ID,512,NULL,infoLog);
    std::cerr<<"ERROR: Linking shader program. REASON: "<<infoLog<<std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use(){
  glUseProgram(ID);
}
