#include "./TileMap.hpp"

TileMap::TileMap(GLFWwindow *window, const std::string &path, int ssRows, int ssCols, float tileSize){
  spriteSheetRows = ssRows;
  spriteSheetCols = ssCols;
  uv_x = 0.0f;
  uv_y = 0.0f;
  this->tileSize = tileSize;
  glfwGetWindowSize(window,&windowX,&windowY);

  loadMapData(path);
}
/*
void TileMap::loadMapData(const std::string &path){
  std::ifstream mapFile;
  std::string line;
  int rows = 0;
  int cols = 0;

  mapFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  try{
    mapFile.open(path);

    while(std::getline(mapFile,line)){
      std::vector<int> row;
      std::stringstream ss(line);

      std::string cell;

      while(std::getline(ss,cell,',')){
        int value = std::stoi(cell);
        row.push_back(value);
        
      }

      if(rows == 0){
        cols = row.size();
      }

      map.push_back(row);
      rows++;
    }

    mapFile.close();
    
    mapRows = rows;
    mapCols = cols;

    std::cout<<"Successfully Loaded Map Data!"<<std::endl;
    for(int i =0;i<rows;i++){
      for(int j = 0; j < cols;j++){
        std::cout<<map[i][j]<<" ";
      }
      std::cout<<std::endl;
    }
  }
  catch(const std::ifstream::failure &e){
    std::cout<<"ERROR: Reading map data!"<< e.what() << std::endl;
  }
}
*/
void TileMap::loadMapData(const std::string &path){
  std::ifstream mapFile(path);
  if (!mapFile.is_open()) {
    std::cout << "ERROR: Cannot open file: " << path << std::endl;
    return;
  }
  
  std::string line;
  int rows = 0;
  int cols = 0;
  
  while(std::getline(mapFile, line)) {
    if (line.empty()) continue;  // Skip empty lines
    
    std::vector<int> row;
    std::stringstream ss(line);
    std::string cell;
    
    while(std::getline(ss, cell, ',')) {
      try {
        int value = std::stoi(cell);
        row.push_back(value);
      } catch (const std::exception& e) {
        std::cout << "ERROR: Invalid number in map data: " << cell << std::endl;
        return;
      }
    }
    
    if (row.empty()) continue;  // Skip empty rows
    
    if (rows == 0) {
      cols = row.size();
    } else if (row.size() != cols) {
      std::cout << "ERROR: Inconsistent row size at row " << rows << std::endl;
      return;
    }
    
    map.push_back(row);
    rows++;
  }
  
  mapRows = rows;
  mapCols = cols;
  std::cout << "Successfully loaded " << rows << "x" << cols << " map!" << std::endl;
}

void TileMap::decideTile(int t){
  int x = t % spriteSheetCols;
  int y = t - x;

  uv_x = (float)x;
  uv_y = (float)y;
}

void TileMap::render(Shader &shader, Texture &texture){
  
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(0.0f,(float)windowX,0.0f,(float)windowY);
  
  float xOffset = (windowX/2.0f) - (tileSize*(mapCols/2.0f) - (tileSize/2.0f));
  float yOffset = (windowY/2.0f) - (tileSize*(mapRows/2.0f) - (tileSize/2.0f));

  texture.assignTextureUnit(0);
  shader.use();

  for(int y = 0; y < mapRows; y++){
    for(int x = 0; x < mapCols; x++){
      int flippedY = mapRows - 1 - y;
      model = glm::mat4(1.0f);
      model = glm::translate(model,glm::vec3(xOffset + (x*tileSize),yOffset + (flippedY*tileSize),0.0f));
      float nxf = (float)spriteSheetCols;
      float nyf = (float)spriteSheetRows;

      decideTile(map[y][x]);
      shader.setValue("model",model);
      shader.setValue("view",view);
      shader.setValue("projection",projection);
      shader.setValue("uv_x",uv_x);
      shader.setValue("uv_y",uv_y);
      shader.setValue("nxf",nxf);
      shader.setValue("nyf",nyf);

      if(map[y][x] != -1)
        glDrawArrays(GL_TRIANGLES,0,6);
    }
  }

  texture.unbind();
}
