#version 460 core

out vec4 FragColor;

in vec2 TexCoord;
uniform float uv_x;
uniform float uv_y;

uniform float nxf;
uniform float nyf;

uniform sampler2D rasengan;
void main(){
 // FragColor = vec4(1.0);
  float x = 1.0/nxf;
  float y = 1.0/nyf;

  vec4 tex = texture(rasengan,vec2(TexCoord.x * x, TexCoord.y * y) + vec2(uv_x * x, uv_y * y));

  if(tex.a < 0.5)
    discard;

  FragColor = tex;
}
