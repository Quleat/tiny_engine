#include<glm/glm.hpp>
#include<string>
#include"shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
public:
    std::vector<Vertex>  verticies;
    std::vector<unsigned int> indicies;
    std::vector<Texture> textures;

    Mesh(
        std::vector<Vertex> verticies, 
        std::vector<unsigned int> indicies, 
        std::vector<Texture> textures);
    void Draw(Shader& shader);
private:
    unsigned int VAO{0}, VBO{0}, EBO{0};

    void setupMesh();
};
