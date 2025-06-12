#pragma once

#include "../Include/Shader/Shader.hpp"
#include "../Include/Texture/Texture.hpp"

#include <GLFW/glfw3.h>

enum State{
  IDLE,
  TOP,
  DOWN,
  LEFT,
  RIGHT
};


class Player{
  public:
    float nxf, nyf;
    float uv_x, uv_y;
    float frame_ps, animTime;
    float speed;
    glm::vec3 position;
    
    Player(float nxf,float nyf,float frame_ps);
    ~Player() = default;

    void update(GLFWwindow *window,float dt);
    void render(Shader &shader, Texture &texture);
    
  private:
    bool isMoving = false;
    State playerState = State::IDLE;
    void handlePlayerState();
};  
