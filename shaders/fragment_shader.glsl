#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main(){
    //ambient
    vec3 ambient_color = lightColor * material.ambient;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse_color = max(dot(norm, lightDir), 0.0) * lightColor;
    diffuse_color *= material.diffuse;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
    vec3 specular_color = material.specular * spec * lightColor;

    vec3 result = ambient_color+diffuse_color+specular_color;
    FragColor = vec4(result,1.0);
}
