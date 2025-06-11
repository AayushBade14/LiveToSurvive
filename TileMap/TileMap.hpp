#pragma once

#include "../Include/Shader/Shader.hpp"
#include "../Include/Texture/Texture.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class TileMap{
  public:
    TileMap(GLFWwindow *window, const std::string &path, int ssRows, int ssCols, float tileSize);
    ~TileMap() = default;
    
    float uv_x,uv_y;
    float tileSize;
    int windowX, windowY;
    glm::vec3 playerPos;
    void render(Shader &shader, Texture &texture);
  private:
    
    int spriteSheetRows;
    int spriteSheetCols;

    int mapRows;
    int mapCols;

    std::vector<std::vector<int>> map;

    void loadMapData(const std::string &path);
    void decideTile(int t);
};
