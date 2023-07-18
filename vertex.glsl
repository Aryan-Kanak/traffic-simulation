#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;

out vec3 vertexColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexColour = colour;
}
