#include "./Player.hpp"

Player::Player(float nxf, float nyf, float frame_ps) : nxf(nxf), nyf(nyf), frame_ps(frame_ps){
  speed = 5.0f;
  animTime = 0.0f;
  uv_x = 0.0f;
  uv_y = 0.0f;
  position = glm::vec3(0.0f);
} 

void Player::update(GLFWwindow *window,float dt){
  // handle movement controls
  if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
    position.y += speed * dt;
  }
  if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
    position.y -= speed * dt;
  } 
  if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){
    position.x -= speed * dt;
  }
  if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){
    position.x += speed * dt;
  }

  // handle animation
  animTime += dt;
  if(animTime >= 1.0f/frame_ps){
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
}

void Player::render(Shader& shader, Texture& texture){
  texture.assignTextureUnit(0);
  shader.use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(0.0f,1920.0f,0.0f,1080.0f);
  model = glm::translate(model,position);

  shader.setValue("model",model);
  shader.setValue("view",view);
  shader.setValue("projection",projection);
  shader.setValue("uv_x",uv_x);
  shader.setValue("uv_y",uv_y);
  shader.setValue("nxf",nxf);
  shader.setValue("nyf",nyf);

  glDrawArrays(GL_TRIANGLES,0,6);

  texture.unbind();
}
