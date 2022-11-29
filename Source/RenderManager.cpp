#include "Headers.h"
#include "RenderManager.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

RenderManager::RenderManager()
{
    instancedShader = new Shader("Resources/shaders/instanced.vertex.shader", "Resources/shaders/instanced.fragment.shader");
    lineShader = new Shader("Resources/shaders/lines.vertex.shader", "Resources/shaders/lines.fragment.shader");
    localLineShader = new Shader("Resources/shaders/localLines.vertex.shader", "Resources/shaders/localLines.fragment.shader");
    perMeshShader = new Shader("Resources/shaders/basic.vertex.shader", "Resources/shaders/basic.fragment.shader");
}

RenderManager::~RenderManager()
{
    if (resource != nullptr)
        resource->Dereference();

    RELEASE(instancedShader);
    RELEASE(lineShader);
    RELEASE(localLineShader);
    RELEASE(perMeshShader);
}

uint RenderManager::SetMeshInformation(ResourceMesh* resource)
{
    if (initialized) LOG("Tried to call RenderManager::SetMeshInformation more than once in a single Render Manager instance.");
    
    // Set this RenderManager Mesh information.
    totalVertices = &resource->meshInfo.vertices;
    totalIndices = &resource->meshInfo.indices;
    //this->totalVertices.insert(totalVertices.begin(), resource->meshInfo.vertices.begin(), resource->meshInfo.vertices.end());
    //this->totalIndices.insert(totalIndices.begin(), resource->meshInfo.indices.begin(), resource->meshInfo.indices.end());

    CreateBuffers();
    CreateBasicBuffers();
    CreateNormalsDisplayBuffer();
    CreateAABB();

    Mesh firstMesh;
    firstMesh.localAABB = localAABB;

    initialized = true;

    return AddMesh(firstMesh); // Adds a copy of the original mesh into the mesh list.
}

void RenderManager::Draw()
{
    if (!initialized) return; // This is placed here for security reasons. No RenderManager should be created without being initialized.
    if (meshes.empty())
    {
        LOG("A Render Manager is being updated without any meshes!");
        return;
    }

    CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;

    for (auto& mesh : meshes)
    {
        if (currentCamera->isCullingActive)
        {
            if (!currentCamera->IsInsideFrustum(mesh.second.globalAABB))
            {
                mesh.second.outOfFrustum = true;
                continue;
            }
            else
                mesh.second.outOfFrustum = false;
        }
        else if (currentCamera->type != CameraType::SCENE)
        {
            mesh.second.outOfFrustum = false;
        }

        if (!mesh.second.Update())
            continue;
       
        // Check if this game camera is culling.
        // Check if the current mesh is inside the camera culling
        // If true, keep going. If false, go to next iteration.

        modelMatrices.push_back(mesh.second.modelMatrix); // Insert updated matrices
        textureIDs.push_back(mesh.second.OpenGLTextureID);
        mesh.second.OpenGLTextureID = -1; // Reset this, in case the next frame our texture ID changes to -1.
    }

    if (!modelMatrices.empty())
    {
        // Update View and Projection matrices
        instancedShader->Bind();

        instancedShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
        instancedShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());

        // Draw using Dynamic Geometry
        glBindVertexArray(VAO);

        // Update Model matrices
        glBindBuffer(GL_ARRAY_BUFFER, MBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, &modelMatrices.front(), modelMatrices.size() * sizeof(float4x4));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        // Update TextureIDs
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        void* ptr2 = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr2, &textureIDs.front(), textureIDs.size() * sizeof(float));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        for (int i = 0; i < TextureManager::bindedTextures; i++)
        {
            instancedShader->SetInt(("textures[" + std::to_string(i) + "]").c_str(), i);
        }

        // Draw
        glDrawElementsInstanced(GL_TRIANGLES, totalIndices->size(), GL_UNSIGNED_INT, 0, modelMatrices.size());
        glBindVertexArray(0);
    }

    // Drawing normals for every mesh instance
    // TODO: We can optimize this. Add every mesh that has to draw any debug primitive inside a vector. Iterate that vector every frame. 
    
    if (Application::Instance()->camera->currentDrawingCamera->type == CameraType::SCENE)
    {
        int index = 0;
        for (auto& mesh : meshes)
        {
            if (mesh.second.showVertexNormals) 
                DrawVertexNormals(mesh.second);
            if (mesh.second.showFaceNormals) 
                DrawFaceNormals(mesh.second);
            if (mesh.second.showAABB)
                DrawBoundingBoxAABB(mesh.second);
            if (mesh.second.showOBB)
                DrawBoundingBoxOBB(mesh.second);

            index++;
        }
    }

    // Reset model matrices.
    modelMatrices.clear();
    textureIDs.clear();
    TextureManager::UnBindTextures();
}

uint RenderManager::AddMesh(Mesh& mesh)
{
    if (!initialized)
    {
        LOG("Trying to add mesh information into a RenderManager that has not been initialized yet!");
    }
    uint meshID = ++IDcounter;
    meshes[meshID] = mesh;
    meshes[meshID].localAABB = localAABB;

    // If our instance capacity is too low, reserve more memory inside the opengl buffer.
    if (instanceNum < meshes.size())
        ReallocateMoreMemory();

    return meshID;
}

void RenderManager::DrawInstance(Mesh* mesh, bool useBasicShader)
{
    if (useBasicShader)
    {
        if (mesh->textureID != -1) 
        {
            glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        perMeshShader->Bind();
        perMeshShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
        perMeshShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
        perMeshShader->SetMatFloat4v("model", &mesh->modelMatrix.v[0][0]);
    }

    glBindVertexArray(BasicVAO);

    glDrawElements(GL_TRIANGLES, totalIndices->size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderManager::CreateBuffers()
{
    // Create Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * totalVertices->size(), &totalVertices->at(0), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    // vertex texture coords
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Create Index Buffer Object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * totalIndices->size(), &totalIndices->at(0), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Create dynamic buffers based on current instanceNum.
    CreateDynamicBuffers();
}

void RenderManager::CreateBasicBuffers()
{
    // Create Vertex Array Object
    glGenVertexArrays(1, &BasicVAO);
    glBindVertexArray(BasicVAO);

    // Create Vertex Buffer Object
    glGenBuffers(1, &BasicVBO);

    glBindBuffer(GL_ARRAY_BUFFER, BasicVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * totalVertices->size(), &totalVertices->at(0), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glGenBuffers(1, &BasicIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BasicIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * totalIndices->size(), &totalIndices->at(0), GL_STATIC_DRAW);

    glBindVertexArray(0);


}

void RenderManager::CreateNormalsDisplayBuffer()
{
    { 
        vertexNormalsDisplay.resize(totalVertices->size() * 2);

        float lineMangitude = 0.5f;

        int j = 0;
        for (int i = 0; i < totalVertices->size() * 2; i++)
        {
            if (i % 2 == 0)
            {
                vertexNormalsDisplay[i] = totalVertices->at(j).position;
            }
            else
            {
                vertexNormalsDisplay[i] = totalVertices->at(j).position + (totalVertices->at(j).normals * lineMangitude);
                j++;
            }

        }

        glGenVertexArrays(1, &VertexLineVAO);
        glBindVertexArray(VertexLineVAO);

        // Create Vertex Buffer Object
        glGenBuffers(1, &VertexLineVBO);

        glBindBuffer(GL_ARRAY_BUFFER, VertexLineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * vertexNormalsDisplay.size(), &vertexNormalsDisplay[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

        glBindVertexArray(0);
    }

    {
        faceNormalsDisplay.resize((totalIndices->size() / 3) * 2); // 3 vertices make a face; we need 2 points to display 1 face normal. 

        float lineMangitude = 0.5f;

        int k = 0;
        int l = 0;

        int iterations = faceNormalsDisplay.size() / 2;
        for (int i = 0; i < iterations; i++)
        {
            float3 faceCenter = { 0,0,0 };
            for (int j = 0; j < 3; j++)
            {
                faceCenter += totalVertices->at(totalIndices->at(k++)).position;
            }
            faceCenter /= 3;
            faceNormalsDisplay.push_back(faceCenter);
            
            float3 normalsDir = { 0,0,0 };
            for (int j = 0; j < 3; j++)
            {
                normalsDir += totalVertices->at(totalIndices->at(l++)).normals;
            }
            normalsDir /= 3;
            normalsDir.Normalize();
            faceNormalsDisplay.push_back(faceCenter + (normalsDir * lineMangitude));
        }

        glGenVertexArrays(1, &FaceLineVAO);
        glBindVertexArray(FaceLineVAO);

        // Create Vertex Buffer Object
        glGenBuffers(1, &FaceLineVBO);

        glBindBuffer(GL_ARRAY_BUFFER, FaceLineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * faceNormalsDisplay.size(), &faceNormalsDisplay[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

        glBindVertexArray(0);
    }
}

void RenderManager::CreateAABB()
{
    localAABB.SetNegativeInfinity();
   
    std::vector<float3> vertexPositions;
    vertexPositions.resize(totalVertices->size());
    for (int i = 0; i < totalVertices->size(); i++)
    {
        vertexPositions[i] = totalVertices->at(i).position;
    }
    localAABB.Enclose(&vertexPositions[0], totalVertices->size());

    boxIndices.push_back(0);    // 1
    boxIndices.push_back(1);    // 2
    boxIndices.push_back(0);    // 3
    boxIndices.push_back(2);    // 4
    boxIndices.push_back(2);    // 5
    boxIndices.push_back(3);    // 6
    boxIndices.push_back(1);    // 7
    boxIndices.push_back(3);    // 8
    boxIndices.push_back(0);    // 9
    boxIndices.push_back(4);    // 10
    boxIndices.push_back(4);    // 11
    boxIndices.push_back(5);    // 12
    boxIndices.push_back(4);    // 13
    boxIndices.push_back(6);    // 14
    boxIndices.push_back(6);    // 15
    boxIndices.push_back(7);    // 16
    boxIndices.push_back(7);    // 17
    boxIndices.push_back(5);    // 18
    boxIndices.push_back(1);    // 19
    boxIndices.push_back(5);    // 20
    boxIndices.push_back(3);    // 21
    boxIndices.push_back(7);    // 22
    boxIndices.push_back(2);    // 23
    boxIndices.push_back(6);    // 24


    // Set up buffer for OBB lines.
    glGenVertexArrays(1, &OBBLineVAO);
    glBindVertexArray(OBBLineVAO);

    glGenBuffers(1, &OBBIndexO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OBBIndexO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(), &boxIndices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &OBBLineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, OBBLineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 8, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

    glBindVertexArray(0);

    // Set up buffer for AABB lines.
    glGenVertexArrays(1, &AABBLineVAO);
    glBindVertexArray(AABBLineVAO);

    glGenBuffers(1, &AABBIndexO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBIndexO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(), &boxIndices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &AABBLineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, AABBLineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 8, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

    glBindVertexArray(0);
}

void RenderManager::DrawVertexNormals(Mesh& mesh)
{
    lineShader->Bind();
    lineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
    lineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
    lineShader->SetFloat4("lineColor", 0.36f, 0.75f, 0.72f, 1.0f);

    lineShader->SetMatFloat4v("model", &mesh.modelMatrix.v[0][0]);

    glBindVertexArray(VertexLineVAO);

    glDrawArrays(GL_LINES, 0, vertexNormalsDisplay.size());

    glBindVertexArray(0);

}

void RenderManager::DrawFaceNormals(Mesh& mesh)
{
    lineShader->Bind();
    lineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
    lineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
    lineShader->SetFloat4("lineColor", 0.75f, 0.36f, 0.32f, 1.0f);

    lineShader->SetMatFloat4v("model", &mesh.modelMatrix.v[0][0]);

    glBindVertexArray(FaceLineVAO);

    glDrawArrays(GL_LINES, 0, faceNormalsDisplay.size());

    glBindVertexArray(0);

}

void RenderManager::DrawBoundingBoxAABB(Mesh& mesh)
{
    float3 AABBPoints[8];

    mesh.globalAABB.GetCornerPoints(AABBPoints);

    glBindVertexArray(AABBLineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, AABBLineVBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &AABBPoints[0], 8 * sizeof(float3));
    glUnmapBuffer(GL_ARRAY_BUFFER);

    localLineShader->Bind();
    localLineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
    localLineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
    localLineShader->SetFloat4("lineColor", 0.0f, 1.0f, 0.0f, 1.0f);

    glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void RenderManager::DrawBoundingBoxOBB(Mesh& mesh)
{
    float3 OBBPoints[8];

    mesh.globalOBB.GetCornerPoints(OBBPoints);

    glBindVertexArray(OBBLineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, OBBLineVBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &OBBPoints[0], 8 * sizeof(float3));
    glUnmapBuffer(GL_ARRAY_BUFFER);

    localLineShader->Bind();
    localLineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
    localLineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
    localLineShader->SetFloat4("lineColor", 1.0f, 0.0f, 0.0f, 1.0f);

    glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void RenderManager::ReallocateMoreMemory()
{
    // Add instance num
    instanceNum += (int)(instanceNum * 0.5f);

    DestroyDynamicBuffers();
    CreateDynamicBuffers();
}

void RenderManager::DestroyDynamicBuffers()
{
    glDeleteBuffers(1, &MBO);
    glDeleteBuffers(1, &TBO);
}

void RenderManager::CreateDynamicBuffers()
{
    // Create Model Matrix buffer object
    glGenBuffers(1, &MBO);

    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float4x4) * instanceNum, nullptr, GL_DYNAMIC_DRAW); // TODO: This buffer size should dynamicaly change

    glBindVertexArray(VAO);

    // You can't pass an entire matrix, so we go row by row.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)sizeof(float4));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 2));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 3));

    // Set instancing interval
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    // Create TextureID buffer object
    glGenBuffers(1, &TBO);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * instanceNum, nullptr, GL_DYNAMIC_DRAW); // TODO: This buffer size should dynamically change

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);
}

