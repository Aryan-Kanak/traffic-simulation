#version 460 core
out vec4 fragmentColor;

in vec3 vertexColour;

void main()
{
    fragmentColor = vec4(vertexColour, 1.0f);
}
