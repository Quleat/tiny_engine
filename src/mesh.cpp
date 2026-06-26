#include "mesh.h"

Mesh::Mesh(
    std::vector<Vertex> verticies,
    std::vector<unsigned int> indicies,
    std::vector<Texture> textures)
: verticies(verticies),
    indicies(indicies),
    textures(textures)
{ setupMesh(); }

void Mesh::Draw(Shader& shader)
{   }

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        verticies.size() * sizeof(Vertex),
        &verticies[0],
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indicies.size() * sizeof(unsigned int),
        &indicies[0],
        GL_STATIC_DRAW);

    //Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 
                          GL_FALSE, sizeof(Vertex), (void*) 0);

    //Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);

}
