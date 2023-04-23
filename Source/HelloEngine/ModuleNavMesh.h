#pragma once

#include "Module.h"
#include "Recast/Recast.h"
#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

#define MAX_POLYS 256
#define MAX_SMOOTH 2048
#define MAX_ERROR 0.05f

class GameObject;
class InputGeom;
class NavMeshBuilder;
class ComponentAgent;
class btRigidBody;

enum class PathType
{
	SMOOTH,
	STRAIGHT
};

enum class AgentType
{
	ENEMY,
	CHARACTER_1,
};

struct NavAgent
{
	NavAgent();

	float radius = 0.0f;
	float height = 0.0f;
	float maxClimb = 0.0f;
	int maxSlope = 0;

	float speed = 0.0f;
	float angularSpeed = 0.0f;
	float acceleration = 0.0f;
	float stoppingDistance = 0;

	std::vector<float3> path;
	PathType pathType = PathType::STRAIGHT;

	//Path Calculations Variables
	dtPolyRef m_startRef = 0;
	dtPolyRef m_endRef = 0;

	int m_npolys = 0;
	dtPolyRef m_polys[MAX_POLYS];
	dtPolyRef m_parent[MAX_POLYS];

	int m_nsmoothPath = 0;
	float m_smoothPath[MAX_SMOOTH * 3];

	int m_nstraightPath = 0;
	float m_straightPath[MAX_POLYS * 3];
	unsigned char m_straightPathFlags[MAX_POLYS];
	dtPolyRef m_straightPathPolys[MAX_POLYS];
};

class Pathfinder : public Module
{
public:
	Pathfinder();
	~Pathfinder();

	void Init(NavMeshBuilder* builder);
	std::vector<float3> CalculatePath(ComponentAgent* agent, float3 destination);
	void RenderPath(ComponentAgent* agent);
	bool MovePath(ComponentAgent* agent);
	bool MoveTo(ComponentAgent* agent, float3 destination);
	bool LookAt(btRigidBody* rigidBody, float2 direction2D, float2 origin2D);
	bool SmoothLookAt(btRigidBody* rigidBody, float2 direction2D, float2 origin2D, float speed);

public:
	dtNavMesh* m_navMesh = nullptr;
	dtNavMeshQuery* m_navQuery = nullptr;
	dtQueryFilter m_filter;
	NavMeshBuilder* m_navMeshBuilder = nullptr;

	float3 hitPosition = float3::zero;
	float3 rayCast[2] = { float3::zero, float3::zero };
};

struct BuildSettings
{
	// Cell size in world units
	float cellSize;
	// Cell height in world units
	float cellHeight;
	// Agent height in world units
	float agentHeight;
	// Agent radius in world units
	float agentRadius;
	// Agent max climb in world units
	float agentMaxClimb;
	// Agent max slope in degrees
	float agentMaxSlope;
	// Region minimum size in voxels.
	// regionMinSize = sqrt(regionMinArea)
	float regionMinSize;
	// Region merge size in voxels.
	// regionMergeSize = sqrt(regionMergeArea)
	float regionMergeSize;
	// Edge max length in world units
	float edgeMaxLen;
	// Edge max error in voxels
	float edgeMaxError;
	float vertsPerPoly;
	// Detail sample distance in voxels
	float detailSampleDist;
	// Detail sample max error in voxel heights.
	float detailSampleMaxError;
	// Partition type, see SamplePartitionType
	int partitionType;
	// Size of the tiles in voxels
	float tileSize;
	// Bounds of the area to mesh
	float navMeshBMin[3];
	float navMeshBMax[3];
};

class ModuleNavMesh : public Module
{
public:
	ModuleNavMesh(bool start_enabled = true);

	~ModuleNavMesh();

	bool Start() override;

	UpdateStatus Update(float dt);

	static bool Save();

	static void Load();

	static void CheckNavMeshIntersection(LineSegment raycast, int clickedMouseButton);

	static void ResetNavMeshes();

	static bool IsWalkable(float x, float z, float3& hitPoint);

	bool CleanUp() override;

	static void BakeNavMesh();

	static void AddGameObjectToNavMesh(GameObject* objectToAdd);

	static NavMeshBuilder* GetNavMeshBuilder() { return navMeshBuilder; };
	static InputGeom* GetInputGeom() { return geometry; };
	static BuildSettings* GetBuildSettings() { return buildSettings; };
	static Pathfinder* GetPathfinding() { return pathfinder; };

private:
	static NavMeshBuilder* navMeshBuilder;
	static Pathfinder* pathfinder;
	static InputGeom* geometry;
	static BuildSettings* buildSettings;
};
