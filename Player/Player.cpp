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
  isMoving = false;

  if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
    position.y += speed * dt;
    isMoving = true;
    playerState = State::TOP;
  }
  if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
    position.y -= speed * dt;
    isMoving = true;
    playerState = State::DOWN;
  } 
  if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){
    position.x -= speed * dt;
    isMoving = true;
    playerState = State::LEFT;
  }
  if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){
    position.x += speed * dt;
    isMoving = true;
    playerState = State::RIGHT;
  }
  
  if(!isMoving){
    playerState = State::IDLE;
  }
  
  // handle animation
  handlePlayerState();
  if(playerState != State::IDLE){
    animTime += dt;
    if(animTime >= 1.0f/frame_ps){
      uv_x += 1.0f;
      if(uv_x >= nxf){
        uv_x = 0.0f;
      }
      animTime = 0.0f;
    }
  }
  else{
    uv_y = 1.0f;
    uv_x = 0.0f;
  }
}

void Player::handlePlayerState(){
  switch (playerState) {
    case State::TOP: uv_y = 3.0f; break;
    case State::DOWN: uv_y = 1.0f; break;
    case State::LEFT: uv_y = 2.0f; break;
    case State::RIGHT: uv_y = 0.0f; break;
    default: uv_y = 0.0f; break;
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
