#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./Include/Shader/Shader.hpp"
#include "./Include/Texture/Texture.hpp"
#include "./Include/stb_image/stb_image.h"

#include "./Include/Memory/VBO.hpp"
#include "./Include/Memory/VAO.hpp"

#include "./Player/Player.hpp"

#include <iostream>

#define WIDTH 1920
#define HEIGHT 1080
#define TITLE "LiveToSurvive"

#define WORLD_X 5
#define WORLD_Y 5

float uv_x = 0.0f;
float uv_y = 0.0f;

void framebuffer_size_callback(GLFWwindow *window,int width,int height);
 
void decideTile(int t){
  if(t == 2){
    uv_x = 0.0f;
    uv_y = 13.0f;
  }
  else if(t == 1){
    uv_x = 0.0f;
    uv_y = 12.0f;
  }
  else if(t == 3){
    uv_x = 9.0f;
    uv_y = 0.0f;
  }
  else if(t == 4){
    uv_y = 7.0f;
    uv_x = 4.0f;
  }
  else if(t == 5){
    uv_y = 7.0f;
    uv_x = 5.0f;
  }
  else if(t == 6){
    uv_y = 11.0f;
    uv_x = 18.0f;
  }
  else if(t == 7){
    uv_y = 10.0f;
    uv_x = 18.0f;
  }
}

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
  
  float tilemap[WORLD_X][WORLD_Y] = {
    {1,4,5,6,1},
    {1,2,2,7,1},
    {1,2,2,2,1},
    {1,2,2,2,1},
    {1,1,1,1,1}
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
  
  float animationTime = 0.0f;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  float frame_ps = 8.0f;
  float nxf = 25.0f;
  float nyf = 13.0f;
  
  float tileSize = 100.0f;
  float offsetX = 760.0f;
  float offsetY = 340.0f;

  while(!glfwWindowShouldClose(window)){
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    tile.assignTextureUnit(0);
    
    animationTime += deltaTime;
    if(animationTime >= 1.0f/frame_ps){
      uv_x += 1.0f;

      if(uv_x >= nxf){
        uv_x = 0.0f;
        uv_y += 1.0f;

        if(uv_y >= nyf){
          uv_y = 0.0f;
        }
      }

      animationTime = 0.0f;
    }

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 projection = glm::ortho(0.0f,(float)WIDTH,0.0f,(float)HEIGHT);

    shader.use();

    vao.bind();
    for(int y = 0; y < WORLD_Y; y++){
      for(int x = 0; x < WORLD_X; x++){
        int flippedY = WORLD_Y - 1 - y;
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(offsetX+(x*tileSize),offsetY+(flippedY*tileSize),0.0f));
        decideTile(tilemap[y][x]);
        shader.setValue("model",model);
        shader.setValue("view",view);
        shader.setValue("projection",projection);
        shader.setValue("uv_x",uv_x);
        shader.setValue("uv_y",uv_y);
        shader.setValue("nxf",nxf);
        shader.setValue("nyf",nyf);
       
        if(tilemap[y][x] != 0)
          glDrawArrays(GL_TRIANGLES,0,6);

      }
    }
    
    player.update(window,deltaTime);
    player.render(pShader,spriteSheet);

    tile.unbind();

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
