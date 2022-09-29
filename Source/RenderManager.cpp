#include "Headers.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
    CreateBuffers();

    Mesh cube;
    cube.InitAsCube({ 5.0f,-4.0f,5.0f }, { 10.0f,1.0f,10.0f });

    Mesh cube2;
    cube2.InitAsCube({ 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f });

    meshes.push_back(cube);
    meshes.push_back(cube2);
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
    for (int i = 0; i < meshes.size(); i++)
    {
        uint size = totalVertices.size(); // Used so we know where the indices that are about to be pushed must start. 
                                          // (if X vertices were already pushed, our indices should start at indexNum + X)
        meshes[i].Update();
        totalVertices.insert(totalVertices.end(), meshes[i]._vertices->begin(), meshes[i]._vertices->end());

        for (int j = 0; j < meshes[i]._indices->size(); j++)
        {
            totalIndices.push_back(meshes[i]._indices->at(j) + size);
        }
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    std::cout << totalVertices.size() * sizeof(float3) << std::endl;
    memcpy(ptr, &totalVertices.front(), totalVertices.size() * sizeof(float3));
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    void* ptr2 = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    std::cout << totalIndices.size() * sizeof(uint) << std::endl;
    memcpy(ptr2, &totalIndices.front(), totalIndices.size() * sizeof(uint));
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

   
    glDrawElements(GL_TRIANGLES, totalIndices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
    totalVertices.clear();
    totalIndices.clear();

    //for (int i = 0; i < totalVertices.size(); i++)
    //{
    //    std::cout << "Vertex " << i << ": x " << totalVertices[i].x << " y: " << totalVertices[i].y << " z: " << totalVertices[i].z << std::endl;
    //}

    //for (int i = 0; i < totalIndices.size(); i++)
    //{
    //    std::cout << "Index " << i << ": " << totalIndices[i] << std::endl;
    //}
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 1000/*NUM OF INDICES*/,nullptr, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}
