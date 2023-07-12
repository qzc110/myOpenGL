#version 330 core

out vec4 frag_color;

uniform vec4 new_color;

void main()
{
   frag_color = new_color;
}