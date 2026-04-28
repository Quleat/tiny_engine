#include<glm/glm.hpp>

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
};
