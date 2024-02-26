#version 460 core
out vec4 fragmentColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define POINT_LIGHTS_NO 1

in vec3 fragPos;
in vec3 normal;

uniform Material material;
uniform DirLight moonlight;
uniform PointLight pointLights[POINT_LIGHTS_NO];
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    // ambient
    //vec3 ambient = light.ambient * material.ambient;
    vec3 ambient = light.ambient * 0.1 * material.diffuse;
    
    // diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // ambient
    //vec3 ambient = light.ambient * material.ambient;
    vec3 ambient = light.ambient * 0.1 * material.diffuse;

    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * (d * d));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = CalcDirLight(moonlight, norm, viewDir);
    for (int i = 0; i < POINT_LIGHTS_NO; ++i) {
        if (pointLights[i].position != vec3(0.0)) {
            result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
        }
    }

    fragmentColor = vec4(result, 1.0f);
}
