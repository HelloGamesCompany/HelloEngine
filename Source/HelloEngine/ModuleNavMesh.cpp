#include "Headers.h"
#include "ModuleNavMesh.h"
#include "NavMeshBuilder.h"
#include "NavMeshImporter.h"

#include "InputGeom.h"
#include "Detour/DetourCommon.h"

#include "ModuleRenderer3D.h"
#include "ModuleLayers.h"

#include "NavAgentComponent.h"
#include "MeshRenderComponent.h"
#include "PhysicsComponent.h"
#include "Console.h";
#include "LayerEditor.h"

using CSL = Engine::Console;

NavMeshBuilder* ModuleNavMesh::navMeshBuilder = nullptr;
Pathfinder* ModuleNavMesh::pathfinder = nullptr;
InputGeom* ModuleNavMesh::geometry = nullptr;
BuildSettings* ModuleNavMesh::buildSettings = nullptr;

inline bool InRange(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1];
	const float dz = v2[2] - v1[2];
	return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
}

static bool GetSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
	const float minTargetDist, const dtPolyRef* path, const int pathSize,
	float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
	float* outPoints = 0, int* outPointCount = 0)
{
	// Find steer target.
	static const int MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS * 3];
	unsigned char steerPathFlags[MAX_STEER_POINTS];
	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	int nsteerPath = 0;
	navQuery->findStraightPath(startPos, endPos, path, pathSize,
		steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
	if (!nsteerPath)
		return false;

	if (outPoints && outPointCount)
	{
		*outPointCount = nsteerPath;
		for (int i = 0; i < nsteerPath; ++i)
			dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
	}


	// Find vertex far enough to steer to.
	int ns = 0;
	while (ns < nsteerPath)
	{
		// Stop at Off-Mesh link or when point is further than slop away.
		if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
			!InRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
			break;
		ns++;
	}
	// Failed to find good point to steer to.
	if (ns >= nsteerPath)
		return false;

	dtVcopy(steerPos, &steerPath[ns * 3]);
	steerPos[1] = startPos[1];
	steerPosFlag = steerPathFlags[ns];
	steerPosRef = steerPathPolys[ns];

	return true;
}

static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
	const dtPolyRef* visited, const int nvisited)
{
	int furthestPath = -1;
	int furthestVisited = -1;

	// Find furthest common polygon.
	for (int i = npath - 1; i >= 0; --i)
	{
		bool found = false;
		for (int j = nvisited - 1; j >= 0; --j)
		{
			if (path[i] == visited[j])
			{
				furthestPath = i;
				furthestVisited = j;
				found = true;
			}
		}
		if (found)
			break;
	}

	// If no intersection found just return current path. 
	if (furthestPath == -1 || furthestVisited == -1)
		return npath;

	// Concatenate paths.	

	// Adjust beginning of the buffer to include the visited.
	const int req = nvisited - furthestVisited;
	const int orig = rcMin(furthestPath + 1, npath);
	int size = rcMax(0, npath - orig);
	if (req + size > maxPath)
		size = maxPath - req;
	if (size)
		memmove(path + req, path + orig, size * sizeof(dtPolyRef));

	// Store visited
	for (int i = 0; i < req; ++i)
		path[i] = visited[(nvisited - 1) - i];

	return req + size;
}

// This function checks if the path has a small U-turn, that is,
// a polygon further in the path is adjacent to the first polygon
// in the path. If that happens, a shortcut is taken.
// This can happen if the target (T) location is at tile boundary,
// and we're (S) approaching it parallel to the tile edge.
// The choice at the vertex can be arbitrary, 
//  +---+---+
//  |:::|:::|
//  +-S-+-T-+
//  |:::|   | <-- the step can end up in here, resulting U-turn path.
//  +---+---+
static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
{
	if (npath < 3)
		return npath;

	// Get connected polygons
	static const int maxNeis = 16;
	dtPolyRef neis[maxNeis];
	int nneis = 0;

	const dtMeshTile* tile = 0;
	const dtPoly* poly = 0;
	if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
		return npath;

	for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
	{
		const dtLink* link = &tile->links[k];
		if (link->ref != 0)
		{
			if (nneis < maxNeis)
				neis[nneis++] = link->ref;
		}
	}

	// If any of the neighbour polygons is within the next few polygons
	// in the path, short cut to that polygon directly.
	static const int maxLookAhead = 6;
	int cut = 0;
	for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
		for (int j = 0; j < nneis; j++)
		{
			if (path[i] == neis[j]) {
				cut = i;
				break;
			}
		}
	}
	if (cut > 1)
	{
		int offset = cut - 1;
		npath -= offset;
		for (int i = 1; i < npath; i++)
			path[i] = path[i + offset];
	}

	return npath;
}

ModuleNavMesh::ModuleNavMesh(bool start_enabled)
{
    geometry = new InputGeom();
    buildSettings = new BuildSettings;
    pathfinder = new Pathfinder;
}

ModuleNavMesh::~ModuleNavMesh()
{
	RELEASE(geometry);
	RELEASE(navMeshBuilder);
    RELEASE(buildSettings);
    RELEASE(pathfinder);
}

bool ModuleNavMesh::Start()
{
    return true;
}

UpdateStatus ModuleNavMesh::Update(float dt)
{
    return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleNavMesh::Save()
{
	if (!navMeshBuilder)
		return false;

	// Get current scene name and convert to the navmesh path
	std::string navMeshPath;
	navMeshPath = LayerEditor::S_GetCurrentSceneName();
	navMeshPath = "Assets/NavMeshes/" + navMeshPath + ".nav";

	return NavMeshImporter::SaveNavMesh(navMeshPath.c_str(), navMeshBuilder->GetNavMesh(), buildSettings);
}

void ModuleNavMesh::Load()
{
	std::string navMeshPath;
	navMeshPath = LayerEditor::S_GetCurrentSceneName();
	navMeshPath = "Assets/NavMeshes/" + navMeshPath + ".nav";

	dtNavMesh* navMesh = nullptr;

	if (!NavMeshImporter::LoadNavMesh(navMeshPath.c_str(), navMesh, buildSettings))
	{
		Console::S_Log("PROBLEM LOADING THE NAVMESH: There isn't a NavMesh baked for this scene!", LogType::WARNING);
		return;
	}

	ResetNavMeshes();

	rcVcopy(geometry->m_meshBMin, buildSettings->navMeshBMin);
	rcVcopy(geometry->m_meshBMax, buildSettings->navMeshBMax);

	if (!navMeshBuilder)
		navMeshBuilder = new NavMeshBuilder();

	navMeshBuilder->HandleMeshChanged(geometry, buildSettings);

	navMeshBuilder->SetNavMesh(navMesh);

	navMeshBuilder->BuildAllTiles();

	pathfinder->Init(navMeshBuilder);
}

void ModuleNavMesh::CheckNavMeshIntersection(LineSegment raycast, int clickedMouseButton)
{
	if (navMeshBuilder == nullptr)
		return;

	if (geometry->getChunkyMesh() == nullptr && navMeshBuilder->GetNavMesh() == nullptr)
	{
		//return;
		BakeNavMesh();
		CSL::S_Log("No chunky mesh set, one has been baked to avoid crashes", LogType::WARNING);
	}

	float hitTime;

	pathfinder->rayCast[0] = raycast.a;
	pathfinder->rayCast[1] = raycast.b;

	if (geometry->raycastMesh(raycast.a.ptr(), raycast.b.ptr(), hitTime))
		pathfinder->hitPosition = raycast.a + (raycast.b - raycast.a) * hitTime;
}

void ModuleNavMesh::ResetNavMeshes()
{
	RELEASE(geometry);

	RELEASE(navMeshBuilder);

	geometry = new InputGeom();

	navMeshBuilder = new NavMeshBuilder();
}

bool ModuleNavMesh::IsWalkable(float x, float z, float3& hitPoint)
{
	float3 upperPoint = float3(x, 20.0f, z);
	float3 lowerPoint = float3(x, -20, z);
	float hitTime;

	bool walkable = geometry->raycastMesh(upperPoint.ptr(), lowerPoint.ptr(), hitTime);

	hitPoint = upperPoint + (lowerPoint - upperPoint) * hitTime;

	return walkable;
}

bool ModuleNavMesh::CleanUp()
{
    return true;
}

void ModuleNavMesh::BakeNavMesh()
{
	ResetNavMeshes();

	std::vector<GameObject*>* gameObjects = ModuleLayers::rootGameObject->GetAllChildren();

	// Merge all meshes
	for (auto gameObject : *gameObjects)
	{
		if (gameObject->IsStatic())
			AddGameObjectToNavMesh(gameObject);
	}

	// Build navMesh
	if (geometry->getMesh()->vertices.size() > 0 && geometry->getMesh()->indices.size() > 0)
	{
		geometry->SetChunkyMesh();

		if (navMeshBuilder == nullptr)
			navMeshBuilder = new NavMeshBuilder();

		navMeshBuilder->HandleMeshChanged(geometry, buildSettings);
		navMeshBuilder->HandleSettings();
		navMeshBuilder->HandleBuild();

		pathfinder->Init(navMeshBuilder);
	}
	
	Save();
}

void ModuleNavMesh::AddGameObjectToNavMesh(GameObject* objectToAdd)
{
	MeshRenderComponent* meshRenderer = objectToAdd->GetComponent<MeshRenderComponent>();

	if (!meshRenderer)
		return;

	ResourceMesh* mesh = meshRenderer->GetResource();

	if (!mesh)
		return;

	float4x4 globalTransform = objectToAdd->transform->GetGlobalMatrix();

	geometry->MergeToMesh(mesh, globalTransform);
}

Pathfinder::Pathfinder() : m_navQuery(nullptr),
m_navMesh(nullptr), m_navMeshBuilder(nullptr)
{
}

Pathfinder::~Pathfinder()
{
    m_navMesh = nullptr;
    m_navQuery = nullptr;
    m_navMeshBuilder = nullptr;
}

void Pathfinder::Init(NavMeshBuilder* builder)
{
    if (!builder)
        return;

    m_navMeshBuilder = builder;
    m_navMesh = builder->GetNavMesh();
    m_navQuery = builder->GetNavMeshQuery();
}

std::vector<float3> Pathfinder::CalculatePath(ComponentAgent* agent, float3 destination)
{
	std::vector<float3> calculatedPath;

	NavAgent* agentProp = agent->agentProperties;
	float3 origin = agent->GetGameObject()->GetComponent<TransformComponent>()->GetGlobalPosition();

	dtStatus status;
	const float m_polyPickExt[3] = { 2,4,2 };

	if (m_navQuery == nullptr)
		return calculatedPath;

	status = m_navQuery->findNearestPoly(origin.ptr(), m_polyPickExt, &m_filter, &agentProp->m_startRef, nullptr);

	if (dtStatusFailed(status) || (status & DT_STATUS_DETAIL_MASK))
	{
		CSL::S_Log("Could not find a near poly to start path", LogType::ERR);
		return calculatedPath;
	}

	status = m_navQuery->findNearestPoly(destination.ptr(), m_polyPickExt, &m_filter, &agentProp->m_endRef, nullptr);

	if (dtStatusFailed(status) || (status & DT_STATUS_DETAIL_MASK))
	{
		CSL::S_Log("Could not find a near poly to end path", LogType::ERR);
		return calculatedPath;
	}

	if (agentProp->pathType == PathType::SMOOTH)
	{
		if (agentProp->m_startRef && agentProp->m_endRef)
		{
			status = m_navQuery->findPath(agentProp->m_startRef, agentProp->m_endRef, origin.ptr(), destination.ptr(), &m_filter, agentProp->m_polys, &agentProp->m_npolys, MAX_POLYS);

			agentProp->m_nsmoothPath = 0;

			if (agentProp->m_npolys)
			{
				// Iterate over the path to find smooth path on the detail mesh surface.
				dtPolyRef polys[MAX_POLYS];
				memcpy(polys, agentProp->m_polys, sizeof(dtPolyRef) * agentProp->m_npolys);
				int npolys = agentProp->m_npolys;

				float iterPos[3], targetPos[3];
				m_navQuery->closestPointOnPoly(agentProp->m_startRef, origin.ptr(), iterPos, 0);
				m_navQuery->closestPointOnPoly(polys[npolys - 1], destination.ptr(), targetPos, 0);

				static const float STEP_SIZE = 0.5f;
				static const float SLOP = 0.01f;

				agentProp->m_nsmoothPath = 0;

				dtVcopy(&agentProp->m_smoothPath[agentProp->m_nsmoothPath * 3], iterPos);
				agentProp->m_nsmoothPath++;

				// Move towards target a small advancement at a time until target reached or
				// when ran out of memory to store the path.
				while (npolys && agentProp->m_nsmoothPath < MAX_SMOOTH)
				{
					// Find location to steer towards.
					float steerPos[3];
					unsigned char steerPosFlag;
					dtPolyRef steerPosRef;

					if (!GetSteerTarget(m_navQuery, iterPos, targetPos, SLOP,
						polys, npolys, steerPos, steerPosFlag, steerPosRef))
						break;

					bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
					bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

					// Find movement delta.
					float delta[3], len;
					dtVsub(delta, steerPos, iterPos);
					len = dtMathSqrtf(dtVdot(delta, delta));
					// If the steer target is end of path or off-mesh link, do not move past the location.
					if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
						len = 1;
					else
						len = STEP_SIZE / len;
					float moveTgt[3];
					dtVmad(moveTgt, iterPos, delta, len);

					// Move
					float result[3];
					dtPolyRef visited[16];
					int nvisited = 0;
					m_navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &m_filter,
						result, visited, &nvisited, 16);

					npolys = fixupCorridor(polys, npolys, MAX_POLYS, visited, nvisited);
					npolys = fixupShortcuts(polys, npolys, m_navQuery);

					float h = 0;
					m_navQuery->getPolyHeight(polys[0], result, &h);
					result[1] = h;
					dtVcopy(iterPos, result);

					// Handle end of path and off-mesh links when close enough.
					if (endOfPath && InRange(iterPos, steerPos, SLOP, 1.0f))
					{
						// Reached end of path.
						dtVcopy(iterPos, targetPos);
						if (agentProp->m_nsmoothPath < MAX_SMOOTH)
						{
							dtVcopy(&agentProp->m_smoothPath[agentProp->m_nsmoothPath * 3], iterPos);
							agentProp->m_nsmoothPath++;
						}
						break;
					}
					else if (offMeshConnection && InRange(iterPos, steerPos, SLOP, 1.0f))
					{
						// Reached off-mesh connection.
						float startPos[3], endPos[3];

						// Advance the path up to and over the off-mesh connection.
						dtPolyRef prevRef = 0, polyRef = polys[0];
						int npos = 0;
						while (npos < npolys && polyRef != steerPosRef)
						{
							prevRef = polyRef;
							polyRef = polys[npos];
							npos++;
						}
						for (int i = npos; i < npolys; ++i)
						{
							polys[i - npos] = polys[i];
						}

						npolys -= npos;

						// Handle the connection.
						dtStatus status = m_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
						if (dtStatusSucceed(status))
						{
							if (agentProp->m_nsmoothPath < MAX_SMOOTH)
							{
								dtVcopy(&agentProp->m_smoothPath[agentProp->m_nsmoothPath * 3], startPos);
								agentProp->m_nsmoothPath++;
								// Hack to make the dotted path not visible during off-mesh connection.
								if (agentProp->m_nsmoothPath & 1)
								{
									dtVcopy(&agentProp->m_smoothPath[agentProp->m_nsmoothPath * 3], startPos);
									agentProp->m_nsmoothPath++;
								}
							}
							// Move position at the other side of the off-mesh link.
							dtVcopy(iterPos, endPos);
							float eh = 0.0f;
							m_navQuery->getPolyHeight(polys[0], iterPos, &eh);
							iterPos[1] = eh;
						}
					}

					// Store results.
					if (agentProp->m_nsmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&agentProp->m_smoothPath[agentProp->m_nsmoothPath * 3], iterPos);
						agentProp->m_nsmoothPath++;
					}
				}
			}

			if (dtStatusFailed(status) || (status & DT_STATUS_DETAIL_MASK) || agentProp->m_nstraightPath == 0)
			{	
				CSL::S_Log("Could not create smooth path", LogType::ERR);
				calculatedPath.clear();
				return calculatedPath;
			}
		}
		else
		{
			agentProp->m_npolys = 0;
			agentProp->m_nsmoothPath = 0;
		}
	}
	else if (agentProp->pathType == PathType::STRAIGHT)
	{
		if (agentProp->m_startRef && agentProp->m_endRef)
		{
			status = m_navQuery->findPath(agentProp->m_startRef, agentProp->m_endRef, origin.ptr(),
				destination.ptr(), &m_filter, agentProp->m_polys, &agentProp->m_npolys, MAX_POLYS);

			agentProp->m_nstraightPath = 0;

			if (agentProp->m_npolys)
			{
				// In case of partial path, make sure the end point is clamped to the last polygon.
				float epos[3];
				dtVcopy(epos, destination.ptr());
				if (agentProp->m_polys[agentProp->m_npolys - 1] != agentProp->m_endRef)
					m_navQuery->closestPointOnPoly(agentProp->m_polys[agentProp->m_npolys - 1], destination.ptr(), epos, 0);

				m_navQuery->findStraightPath(origin.ptr(), epos, agentProp->m_polys, agentProp->m_npolys,
					agentProp->m_straightPath, agentProp->m_straightPathFlags,
					agentProp->m_straightPathPolys, &agentProp->m_nstraightPath, MAX_POLYS);
			}

			if (dtStatusFailed(status) || (status & DT_STATUS_DETAIL_MASK) || agentProp->m_nstraightPath == 0) 
			{	
				CSL::S_Log("Could not create straight path", LogType::ERR);
				calculatedPath.clear();
				return calculatedPath;
			}
		}
		else
		{
			agentProp->m_npolys = 0;
			agentProp->m_nstraightPath = 0;
		}
	}

	calculatedPath.resize(agentProp->m_nstraightPath);

	memcpy(calculatedPath.data(), agentProp->m_straightPath, sizeof(float) * agentProp->m_nstraightPath * 3);
	
	if (calculatedPath.size() > 0)
		calculatedPath.erase(calculatedPath.begin());

	agentProp->path = calculatedPath;

	return calculatedPath;
}

void Pathfinder::RenderPath(ComponentAgent* agent)
{
}

bool Pathfinder::MovePath(ComponentAgent* agent)
{
	if (!agent->agentProperties->path.empty() &&
		MoveTo(agent, agent->agentProperties->path[0]))
	{
		agent->agentProperties->path.erase(agent->agentProperties->path.begin());

		if (agent->agentProperties->path.empty())
		{
			agent->GetGameObject()->GetComponent<PhysicsComponent>()->SetVelocity({ 0,0,0 });
			return true;
		}
	}

	return false;
}

bool Pathfinder::MoveTo(ComponentAgent* agent, float3 destination)
{
	PhysicsComponent* physComponent = agent->GetGameObject()->GetComponent<PhysicsComponent>();
	
	float3 origin = agent->GetGameObject()->GetComponent<TransformComponent>()->GetGlobalPosition();
	
	float3 direction = destination - origin;
	
	float2 destination2D = { destination.x, destination.z };
	
	float totalheight = math::Abs(direction.y);
	
	direction.Normalize();

	physComponent->_physBody->body->activate(true);

	//Movement
	physComponent->SetVelocity(direction * agent->agentProperties->speed);

	//Rotation
	SmoothLookAt(physComponent->_physBody->body, { direction.x, direction.z }, { origin.x, origin.z }, agent->agentProperties->angularSpeed * DEGTORAD);
	//LookAt(rigidBody, { direction.x, direction.z }, { origin.x, origin.z });

	if (destination2D.Distance({ origin.x, origin.z }) < MAX_ERROR * agent->agentProperties->speed &&
		totalheight < MAX_ERROR + (agent->agentProperties->height / 2))
		return true;

	return false;
}

bool Pathfinder::LookAt(btRigidBody* rigidBody, float2 direction2D, float2 origin2D)
{
	if (origin2D.Normalized().AngleBetween(direction2D) >= MAX_ERROR)
	{
		float2 axis = { 0, 1 };
		float angle = axis.AngleBetween(direction2D);

		if (angle != inf)
		{
			if (direction2D.x < 0) 
				angle *= -1;

			Quat temp = Quat::RotateY(angle);

			rigidBody->getWorldTransform().setRotation(btQuaternion(temp.x, temp.y, temp.z, temp.w));

			return true;
		}
	}

	return false;
}

bool Pathfinder::SmoothLookAt(btRigidBody* rigidBody, float2 direction2D, float2 origin2D, float speed)
{
	if (origin2D.Normalized().AngleBetween(direction2D) >= MAX_ERROR * speed)
	{
		float2 axis = { 0, 1 };
		float angle = axis.AngleBetween(direction2D);

		if (angle != inf)
		{
			if (direction2D.x < 0) 
				angle *= -1;
			
			btTransform quat = rigidBody->getWorldTransform();

			Quat temp = Quat::RotateY(angle);

			quat.setRotation(btQuaternion(temp.x,temp.y,temp.z,temp.w));

			btQuaternion btTemp = rigidBody->getWorldTransform().getRotation();

			btQuaternion btTemp2 = quat.getRotation();

			temp = math::Lerp(Quat(btTemp.x(), btTemp.y(), btTemp.z(), btTemp.w()),
				Quat(btTemp2.x(), btTemp2.y(), btTemp2.z(), btTemp2.w()), speed * EngineTime::GameDeltaTime());

			rigidBody->getWorldTransform().setRotation(btQuaternion(temp.x, temp.y, temp.z, temp.w));

			return true;
		}
	}
	return false;
}

NavAgent::NavAgent()
{
	const BuildSettings* settings = Application::Instance()->navMesh->GetBuildSettings();
	radius = settings->agentRadius;
	height = settings->agentHeight;
	maxClimb = settings->agentMaxClimb;
	maxSlope = settings->agentMaxSlope;

	speed = 1.0f;
	angularSpeed = 1.0f;
	acceleration = 1.0f;
	stoppingDistance = 0;
}