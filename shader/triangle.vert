#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 fragmentColor;

void main(){
    gl_Position = projection * view * model *  vec4(position, 1.0);
    fragmentColor = vec3(1.0f,0.0f,1.0f);
}