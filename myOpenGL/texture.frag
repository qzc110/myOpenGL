#version 330 core

out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
   frag_color = mix(texture(texture1, tex_coord), texture(texture2, vec2(-tex_coord.x + 1, tex_coord.y)), 0.2);
}