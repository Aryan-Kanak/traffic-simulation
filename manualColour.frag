#version 460 core
out vec4 fragmentColor;

in vec3 vertexColour;

uniform vec3 colour;

void main()
{
    fragmentColor = vec4(colour, 1.0f);
}
