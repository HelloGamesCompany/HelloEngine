#include "Headers.h"
#include "InstanceRenderer.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "MeshRenderComponent.h"

InstanceRenderer::InstanceRenderer()
{
    instancedShader = new Shader("Resources/shaders/instanced.vertex.shader", "Resources/shaders/instanced.fragment.shader");
    perMeshShader = new Shader("Resources/shaders/basic.vertex.shader", "Resources/shaders/basic.fragment.shader");
}

InstanceRenderer::~InstanceRenderer()
{
    RELEASE(instancedShader);
    RELEASE(perMeshShader);
}

void InstanceRenderer::SetMeshInformation(ResourceMesh* resource)
{
    if (initialized) 
        LOG("Tried to call RenderManager::SetMeshInformation more than once in a single Render Manager instance.");
    
    // Set this RenderManager Mesh information.
    totalVertices = &resource->meshInfo.vertices;
    totalIndices = &resource->meshInfo.indices;
    this->resource = resource;

    CreateBuffers();
    CreateBasicBuffers();

    initialized = true;
}

void InstanceRenderer::Draw()
{
    if (!initialized) return; // This is placed here for security reasons. No RenderManager should be created without being initialized.
    if (meshes.empty())
    {
        LOG("A Render Manager is being updated without any meshes!");
        Application::Instance()->renderer3D->renderManager.DestroyRenderManager(resource->UID);
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
        {
            continue;
        }

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

    // Reset model matrices.
    modelMatrices.clear();
    textureIDs.clear();
    TextureManager::UnBindTextures();
}

uint InstanceRenderer::AddMesh()
{
    if (!initialized)
    {
        LOG("Trying to add mesh information into a RenderManager that has not been initialized yet!");
    }
    uint meshID = ++IDcounter; // We use a counter for easier debugging, but this could be an UUID.
    meshes[meshID].localAABB = resource->localAABB;
    meshes[meshID].resource = this->resource;

    // If our instance capacity is too low, reserve more memory inside the opengl buffer.
    if (instanceNum < meshes.size())
        ReallocateMoreMemory();

    return meshID;
}

void InstanceRenderer::DrawInstance(Mesh* mesh, bool useBasicShader)
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

void InstanceRenderer::CreateBuffers()
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

void InstanceRenderer::CreateBasicBuffers()
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

void InstanceRenderer::ReallocateMoreMemory()
{
    // Add instance num
    instanceNum += (int)(instanceNum * 0.5f);

    DestroyDynamicBuffers();
    CreateDynamicBuffers();
}

void InstanceRenderer::DestroyDynamicBuffers()
{
    glDeleteBuffers(1, &MBO);
    glDeleteBuffers(1, &TBO);
}

void InstanceRenderer::CreateDynamicBuffers()
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

