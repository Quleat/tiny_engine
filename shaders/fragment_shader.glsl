#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main(){
    float ambient_strength = 0.15f;
    float specular_strength = 0.5f;

    vec3 ambient_color = objectColor * lightColor * ambient_strength;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse_color = max(dot(norm, lightDir), 0.0) * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specular_strength * spec * lightColor;

    vec3 result = (ambient_color+diffuse_color+specular)*objectColor;
    FragColor = vec4(result,1.0);
}
