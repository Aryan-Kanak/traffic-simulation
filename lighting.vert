#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normMatrix;

void main()
{
    fragPos = vec3(model * vec4(aPosition, 1.0));
    normal = normMatrix * aNormal;
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
