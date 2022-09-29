#include "Headers.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
    CreateBuffers();

    for (int i = 0; i < 100; i++)
    {
        Mesh cube;
        cube.InitAsCube({ 0.0f + i,0.0f,0.0f }, { 1.0f,1.0f,1.0f });
        meshes.push_back(cube);
    }
}

RenderManager::~RenderManager()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].CleanUp();
    }
}

void RenderManager::Draw()
{
    // Update all meshes, so their vertices and indices buffers are up to date with their current position and transform.
    for (int i = 0; i < meshes.size(); i++)
    {
        uint size = totalVertices.size(); // Used so we know where the indices that are about to be pushed must start. 
                                          // (if X vertices were already pushed, our indices should start at indexNum + X)
        meshes[i].Update();
        totalVertices.insert(totalVertices.end(), meshes[i]._vertices->begin(), meshes[i]._vertices->end()); // Insert updated vertices

        for (int j = 0; j < meshes[i]._indices->size(); j++)
        {
            totalIndices.push_back(meshes[i]._indices->at(j) + size); // Insert indices adding the current size, so they point to the same mesh's vertices.
        }
    }

    // Draw using Dynamic Geometry
    glBindVertexArray(VAO);

    // Update Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    std::cout << totalVertices.size() * sizeof(float3) << std::endl;
    memcpy(ptr, &totalVertices.front(), totalVertices.size() * sizeof(float3));
    glUnmapBuffer(GL_ARRAY_BUFFER);

    // Update Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    void* ptr2 = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    std::cout << totalIndices.size() * sizeof(uint) << std::endl;
    memcpy(ptr2, &totalIndices.front(), totalIndices.size() * sizeof(uint));
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    // Draw
    glDrawElements(GL_TRIANGLES, totalIndices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
    // Reset vertices and indices.
    totalVertices.clear();
    totalIndices.clear();
}

void RenderManager::AddMesh(Mesh newMesh)
{
    meshes.push_back(newMesh);
}

void RenderManager::CreateBuffers()
{
    // Create Vertex Array Object
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object
    glCreateBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 1000, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);
  
    // Create Index Buffer Object
    glCreateBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 4000/*NUM OF INDICES*/,nullptr, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}
