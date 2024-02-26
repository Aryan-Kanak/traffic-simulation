#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpecular;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 fragPos;
in vec3 normal;

uniform Material material;

void main() {
    gPosition = fragPos;
    gNormal = normalize(normal);
    gAlbedo.rgb = material.diffuse;
    gSpecular.rgb = material.specular;
    gSpecular.a = material.shininess;
}
