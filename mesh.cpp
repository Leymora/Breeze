#include "mesh.h"

Mesh::Mesh(char* path)
{
    loadMesh(path);
    setupMesh();
}

void Mesh::loadMesh(char* path)
{
    //std::ifstream gltfFile(path);
    //json j = json::parse(gltfFile);

    //unsigned int nrOfVerts = j["accessors"][0]["count"].get<unsigned int>();
    //unsigned int indicesIndex = j["meshes"][0]["primitives"][0]["indices"].get<unsigned int>();
    //unsigned int nrOfIndices = j["accessors"][indicesIndex]["count"].get<unsigned int>();

    //std::string URI = j["buffers"][0]["uri"].get<std::string>();
    //unsigned int URIbyteLength = j["buffers"][0]["byteLength"].get<unsigned int>();

    
	char buffer0[840];
	FILE *fh;
    char bufferView0[288];
    unsigned int bufferView0Data = 0; 

    //glGenBuffers(1, &buffer0Data);

    fh = fopen(path, "r");
    fread(buffer0, sizeof(char), 840, fh);

    memcpy(bufferView0, buffer0, 288); // bufferView0 now has the buffer data for vertex positions (I think)
    std::cout << bufferView0 << std::endl;

    
    for (int i = 0; i < 288; i++)
    {
        std::stringstream str(std::to_string(bufferView0[i]));
        //bufferView0Data <<= 8;
        //bufferView0Data |= bufferView0[i];
        str >> bufferView0Data;
    }
    std::cout << bufferView0Data << std::endl;

   

    glBindBuffer(GL_ARRAY_BUFFER, bufferView0Data);
    glBufferData(GL_ARRAY_BUFFER, 288, bufferView0, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8, (void*)0);

    std::cout << buffer0 << std::endl;

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, );
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float), "AACAvwAAgL8AAIA/AACAvwAAgL8AAIA/AACAvwAAgL8AAIA/AACAvwAAgD8AAIA/AACAvwAAgD8AAIA/AACAvwAAgD8AAIA/AACAvwAAgL8AAIC/AACAvwAAgL8AAIC/AACAvwAAgL8AAIC/AACAvwAAgD8AAIC/AACAvwAAgD8AAIC/AACAvwAAgD8AAIC/AACAPwAAgL8AAIA/AACAPwAAgL8AAIA/AACAPwAAgL8AAIA/AACAPwAAgD8AAIA/AACAPwAAgD8AAIA/AACAPwAAgD8AAIA/AACAPwAAgL8AAIC/AACAPwAAgL8AAIC/AACAPwAAgL8AAIC/AACAPwAAgD8AAIC/AACAPwAAgD8AAIC/AACAPwAAgD8AAIC/AACAvwAAAAAAAACAAAAAAAAAgL8AAACAAAAAAAAAAAAAAIA/AACAvwAAAAAAAACAAAAAAAAAAAAAAIA/AAAAAAAAgD8AAACAAACAvwAAAAAAAACAAAAAAAAAgL8AAACAAAAAAAAAAAAAAIC/AACAvwAAAAAAAACAAAAAAAAAAAAAAIC/AAAAAAAAgD8AAACAAAAAAAAAgL8AAACAAAAAAAAAAAAAAIA/AACAPwAAAAAAAACAAAAAAAAAAAAAAIA/AAAAAAAAgD8AAACAAACAPwAAAAAAAACAAAAAAAAAgL8AAACAAAAAAAAAAAAAAIC/AACAPwAAAAAAAACAAAAAAAAAAAAAAIC/AAAAAAAAgD8AAACAAACAPwAAAAAAAACAAADAPgAAgD8AAAA+AACAPgAAwD4AAAAAAAAgPwAAgD8AACA/AAAAAAAAYD8AAIA+AADAPgAAQD8AAAA+AAAAPwAAwD4AAEA/AAAgPwAAQD8AACA/AABAPwAAYD8AAAA/AADAPgAAgD4AAMA+AACAPgAAwD4AAIA+AAAgPwAAgD4AACA/AACAPgAAID8AAIA+AADAPgAAAD8AAMA+AAAAPwAAwD4AAAA/AAAgPwAAAD8AACA/AAAAPwAAID8AAAA/AAADAAkAAAAJAAYACAAKABUACAAVABMAFAAXABEAFAARAA4ADQAPAAQADQAEAAIABwASAAwABwAMAAEAFgALAAUAFgAFABAA", GL_STATIC_DRAW);
	// glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, (void*)0);

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