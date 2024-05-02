#version 460 core
out vec4 fragmentColor;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform PointLight light;
uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 gDiffuse, vec3 gSpecular, float shininess) {
    // ambient
    vec3 ambient = light.ambient * 0.1 * gDiffuse;

    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * gDiffuse);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * gSpecular);

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
    vec2 texCoords = vec2(gl_FragCoord.x / 1200.0f, gl_FragCoord.y / 600.0f);
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 norm = texture(gNormal, texCoords).rgb;
    vec3 diffuse = texture(gAlbedo, texCoords).rgb;
    vec3 specular = texture(gSpecular, texCoords).rgb;
    float shininess = texture(gSpecular, texCoords).a;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = CalcPointLight(light, norm, fragPos, viewDir, diffuse, specular, shininess);
        
    fragmentColor = vec4(result, 1.0);
}
