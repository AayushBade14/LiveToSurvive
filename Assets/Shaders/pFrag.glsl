#version 460 core

out vec4 FragOut;

in vec2 TexCoord;

uniform sampler2D Player;

uniform float uv_x;
uniform float uv_y;

uniform float nxf;
uniform float nyf;

void main(){
  
  float x = 1.0/nxf;
  float y = 1.0/nyf;
  
  vec4 tex = texture(Player,vec2(TexCoord.x * x,TexCoord.y * y) + vec2(uv_x * x, uv_y * y));

  if(tex.a < 0.5)
    discard;

  FragOut = tex;
}
  

