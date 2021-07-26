#include "mesh.h"

Mesh::Mesh(std::string path)
{
    loadMesh(path);
    setupMesh();
}

void Mesh::loadMesh(std::string path)
{
    std::ifstream gltfFile(path);
    json j = json::parse(gltfFile);

    unsigned int nrOfVerts = j["accessors"][0]["count"].get<unsigned int>();
    unsigned int indicesIndex = j["meshes"][0]["primitives"][0]["indices"].get<unsigned int>();
    unsigned int nrOfIndices = j["accessors"][indicesIndex]["count"].get<unsigned int>();

    std::string URI = j["buffers"][0]["uri"].get<std::string>();
    unsigned int URIbyteLength = j["buffers"][0]["byteLength"].get<unsigned int>();

    //Vertexes
    for (unsigned int i = 0; i < nrOfVerts; i++)
    {
        Vertex v;
        //v.Position = URI.substr(0, 288);
        this->vertices.push_back(v);
    }

    // Indices
    this->indices.push_back(nrOfIndices);

}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //Vertex Texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    //Draw the fuggin mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}