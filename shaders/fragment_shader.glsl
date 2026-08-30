#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

void main(){
    float ambient_strength = 0.15f;
    vec3 ambient_color = objectColor * lightColor * ambient_strength;
    FragColor = vec4(ambient_color,1.0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
}
