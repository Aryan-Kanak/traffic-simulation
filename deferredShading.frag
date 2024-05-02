#version 460 core
out vec4 fragColor;

in vec2 texCoords;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform DirLight moonlight;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 gDiffuse, vec3 gSpecular, float shininess) {
    // ambient
    vec3 ambient = light.ambient * 0.1 * gDiffuse;
    
    // diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * gDiffuse);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * gSpecular);

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 norm = texture(gNormal, texCoords).rgb;
    vec3 diffuse = texture(gAlbedo, texCoords).rgb;
    vec3 specular = texture(gSpecular, texCoords).rgb;
    float shininess = texture(gSpecular, texCoords).a;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = CalcDirLight(moonlight, norm, viewDir, diffuse, specular, shininess);

    fragColor = vec4(result, 1.0);
}
