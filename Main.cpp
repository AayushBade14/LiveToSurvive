#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./Include/Shader/Shader.hpp"
#include "./Include/Texture/Texture.hpp"
#include "./Include/stb_image/stb_image.h"

#include "./Include/Memory/VBO.hpp"
#include "./Include/Memory/VAO.hpp"

#include "./Player/Player.hpp"
#include "./TileMap/TileMap.hpp"

#include <iostream>

#define WIDTH 1920
#define HEIGHT 1080
#define TITLE "LiveToSurvive"

void framebuffer_size_callback(GLFWwindow *window,int width,int height);

int main(void){
  if(glfwInit()<0){
    std::cerr<<"ERROR: Init_glfw!"<<std::endl;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH,
                                        HEIGHT,
                                        TITLE,
                                        NULL,
                                        NULL);

  if(!window){
    std::cerr<<"ERROR: Creating window!"<<std::endl;
    glfwTerminate();
  }
  
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cerr<<"ERROR: Init_glad!"<<std::endl;
    glfwTerminate();
    glfwDestroyWindow(window);
  }
  
  std::vector<float> vertices = {
    -50.0f,-50.0f,0.0f, 0.0f,0.0f,
    50.0f,-50.0f,0.0f,  1.0f,0.0f,
    50.0f,50.0f,0.0f,   1.0f,1.0f,

    50.0f,50.0f,0.0f,   1.0f,1.0f,
    -50.0f,50.0f,0.0f,  0.0f,1.0f,
    -50.0f,-50.0f,0.0f, 0.0f,0.0f
  };
  
  VBO vbo = VBO(vertices,GL_STATIC_DRAW);
  VAO vao = VAO(vbo);

  vao.setVertexAttrib(0,3,5,0);
  vao.setVertexAttrib(1,2,5,3);

  Texture tile = Texture("./Assets/Textures/Tileset.png",GL_RGBA);
  Shader shader = Shader("./Assets/Shaders/vert.glsl","./Assets/Shaders/frag.glsl");
  
  Texture spriteSheet = Texture("./Assets/Textures/Player.png",GL_RGBA);
  Shader pShader = Shader("./Assets/Shaders/pVert.glsl","./Assets/Shaders/pFrag.glsl");

  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  
  tile.setSamplerValue(shader,"Tileset",0);
  spriteSheet.setSamplerValue(pShader,"Player",0);
  
  Player player = Player(9.0f,4.0f,9.0f);
  player.speed = 100.0f;
  
  TileMap tilemap = TileMap(window,"/home/b1n4ry/MyFolder/Projects/LiveToSurvive/Assets/mapData.csv",13,25,100.0f);
  
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  while(!glfwWindowShouldClose(window)){
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    

    vao.bind();

    tilemap.render(shader,tile);

    player.update(window,deltaTime);
    player.render(pShader,spriteSheet);
    
    vao.unbind();

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  glfwDestroyWindow(window);

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window,int width,int height){
  glViewport(0,0,width,height);
}
