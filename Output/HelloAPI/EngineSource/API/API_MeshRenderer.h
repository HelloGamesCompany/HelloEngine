#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class MeshRenderComponent;
class DragBoxMeshRenderer;

namespace API
{
    class TO_API API_MeshRenderer
    {
    public:
        API_MeshRenderer();
        ~API_MeshRenderer();

        API_GameObject GetGameObject();

        void ChangeMesh(uint meshResourceUID);

        void SetActive(bool active);

    private:
        MeshRenderComponent* GetComponent();
        void SetComponent(MeshRenderComponent* component);
    private:
        MeshRenderComponent* _meshRenderer = nullptr;

        friend class API_GameObject;
        friend class DragBoxMeshRenderer;
    };
}