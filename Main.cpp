#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./Include/Shader/Shader.hpp"
#include "./Include/stb_image/stb_image.h"

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
  
  float vertices[] = {
    -50.0f,-50.0f,0.0f, 0.0f,0.0f,
    50.0f,-50.0f,0.0f,  1.0f,0.0f,
    50.0f,50.0f,0.0f,   1.0f,1.0f,

    50.0f,50.0f,0.0f,   1.0f,1.0f,
    -50.0f,50.0f,0.0f,  0.0f,1.0f,
    -50.0f,-50.0f,0.0f, 0.0f,0.0f
  };
  
  float tilemap[WORLD_X][WORLD_Y] = {
    {1,1,1,1,1},
    {1,2,2,2,1},
    {1,2,3,2,1},
    {1,2,2,2,1},
    {1,1,1,1,1}
  };

  unsigned int VAO,VBO;

  glGenVertexArrays(1,&VAO);
  glGenBuffers(1,&VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  
  stbi_set_flip_vertically_on_load(true);

  unsigned int tile;
  glGenTextures(1,&tile);
  glBindTexture(GL_TEXTURE_2D,tile);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  int width,height,nrChannels;
  unsigned char *data = stbi_load("./Assets/Textures/Tileset.png",&width,&height,&nrChannels,0);
  if(data){
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  stbi_image_free(data);

  Shader shader = Shader("./Assets/Shaders/vert.glsl","./Assets/Shaders/frag.glsl");

  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  
  int tilesetTexUnit = 0;
  shader.use();
  shader.setValue("Tileset",tilesetTexUnit);
  
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
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,tile);

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

    glm::mat4 projection = glm::ortho((float)WIDTH,0.0f,(float)HEIGHT,0.0f);

    shader.use();

    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES,0,6);
    for(int x = 0; x < WORLD_X; x++){
      for(int y = 0; y < WORLD_Y; y++){
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(offsetX+(x*tileSize),offsetY+(y*tileSize),0.0f));
        decideTile(tilemap[x][y]);
        shader.setValue("model",model);
        shader.setValue("view",view);
        shader.setValue("projection",projection);
        shader.setValue("uv_x",uv_x);
        shader.setValue("uv_y",uv_y);
        shader.setValue("nxf",nxf);
        shader.setValue("nyf",nyf);
       
        if(tilemap[x][y] != 0)
          glDrawArrays(GL_TRIANGLES,0,6);

      }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
    
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  glfwDestroyWindow(window);

  glDeleteBuffers(1,&VBO);
  glDeleteVertexArrays(1,&VAO);
  glDeleteTextures(1,&tile);

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window,int width,int height){
  glViewport(0,0,width,height);
}
