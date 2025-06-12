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

struct Bullet{
  glm::vec3 position;
  bool isCharging;
  bool isActive;
  float range;
};

void framebuffer_size_callback(GLFWwindow *window,int width,int height);

std::vector<Bullet> bullets;

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
  
  Texture bullet = Texture("./Assets/Textures/rasenshuriken.png",GL_RGBA);
  Shader bulletShader = Shader("./Assets/Shaders/bVert.glsl","./Assets/Shaders/bFrag.glsl");
  
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  
  tile.setSamplerValue(shader,"Tileset",0);
  spriteSheet.setSamplerValue(pShader,"Player",0);
  bullet.setSamplerValue(bulletShader,"rasengan",0);

  Player player = Player(9.0f,4.0f,9.0f);
  player.speed = 100.0f;
  
  TileMap tilemap = TileMap(window,"/home/b1n4ry/MyFolder/Projects/LiveToSurvive/Assets/mapData.csv",13,25,100.0f);
  
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  
  float nxf = 2.0f;
  float nyf = 3.0f;
  float uv_x = 0.0f;
  float uv_y = 0.0f;
  float frame_ps = 8.0f;
        float animTime = 0.0f;

  while(!glfwWindowShouldClose(window)){
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    static float lastShot = 0.0f;
    static bool spaceHeld = false;

    if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS && !spaceHeld){
      Bullet bullet;
      bullet.position = player.position;
      bullet.isActive = true;
      bullet.isCharging = true;
      bullet.range = 0.5f;
      bullets.push_back(bullet);
      spaceHeld = true;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
      // While holding, increase range of last bullet
      if(!bullets.empty() && bullets.back().isCharging){
          bullets.back().range += deltaTime * 2.0f; // adjust multiplier as needed
      }
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && spaceHeld){
      // On release, stop charging and let bullet fly
      if(!bullets.empty()){
          bullets.back().isCharging = false;
      }
      spaceHeld = false;
    }
    
    tilemap.playerPos = player.position - glm::vec3(960.0f,540.0f,0.0f);

    vao.bind();

    tilemap.render(shader,tile);

    player.update(window,deltaTime);
    player.render(pShader,spriteSheet);
    
    bulletShader.use();
    bullet.assignTextureUnit(0);
    glm::mat4 view = glm::translate(glm::mat4(1.0f),-(player.position-glm::vec3(999.0f,540.0f,0.0f)));
    glm::mat4 projection = glm::ortho(0.0f,1920.0f,0.0f,1080.0f);
    
    for(int i = 0; i < bullets.size();){
      Bullet &b = bullets[i];
      if(b.isActive){
        if(!b.isCharging){
          b.position += glm::vec3(500.0f,0.0f,0.0f) * deltaTime;
        }
        glm::mat4 model = glm::mat4(1.0f); 
        
        animTime += deltaTime;
        if(animTime > 1.0f/frame_ps){
          uv_x += 1.0f;

          if(uv_x >= nxf){
            uv_x = 0.0f;
            uv_y += 1.0f;
            if(uv_y >= nyf){
              uv_y = 0.0f;
            }
          }
          animTime = 0.0f;
        }

        model = glm::translate(model,b.position);
        model = glm::scale(model,glm::vec3(b.range));
        bulletShader.setValue("model",model);
        bulletShader.setValue("view",view);
        bulletShader.setValue("projection",projection);
        bulletShader.setValue("nxf",nxf);
        bulletShader.setValue("nyf",nyf);
        bulletShader.setValue("uv_x",uv_x);
        bulletShader.setValue("uv_y",uv_y);

        glDrawArrays(GL_TRIANGLES,0,6);
      }

      if(b.position.x > 1920.0f){
        b.isActive = false;
        bullets.erase(bullets.begin() + i);
      }
      else{
        i++;
      }
    }
    bullet.unbind();
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
