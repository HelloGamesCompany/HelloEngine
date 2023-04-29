#include "Headers.h"
#include "NavAgentComponent.h"
#include "ModuleNavMesh.h"
#include "GameObject.h"

ComponentAgent::ComponentAgent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::AGENT;
	agentProperties = new NavAgent();
	pathfinder = ModuleNavMesh::S_GetPathfinding();

	testPath = { 0,0,0 };
}

ComponentAgent::~ComponentAgent()
{
	agentProperties->path.clear();
	RELEASE(agentProperties);
}

void ComponentAgent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("NavAgent"))
	{
		if (ImGui::Button("CreatePath"))
			agentProperties->path = ModuleNavMesh::S_GetPathfinding()->CalculatePath(this, testPath);

		ImGui::SameLine();

		if (ImGui::Button("GoTo"))
			ModuleNavMesh::S_GetPathfinding()->MovePath(this);

		ImGui::DragFloat3("XYZ: ", testPath.ptr());
		ImGui::Separator();
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Text("Agent Properties");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushItemWidth(180);
		ImGui::DragFloat("Agent Radius", &agentProperties->radius, 0.1f, 0.f);
		ImGui::DragFloat("Agent Height", &agentProperties->height, 0.1f, 0.f);
		ImGui::DragFloat("Stop Height", &agentProperties->maxClimb, 0.1f, 0.f);
		ImGui::DragInt("Max Slope", &agentProperties->maxSlope, 1, 0);
		ImGui::PopItemWidth();
		ImGui::Dummy({ 0,10 });

		ImGui::Text("Steering");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushItemWidth(180);
		ImGui::DragFloat("Speed", &agentProperties->speed, 0.1f, 0.f);
		ImGui::DragFloat("Angular Speed", &agentProperties->angularSpeed, 1.f, 0.f);
		ImGui::DragFloat("Acceleration", &agentProperties->acceleration, 0.1f, 0.f);
		ImGui::DragFloat("Stopping Distance", &agentProperties->stoppingDistance, 0.1f, 0.f);
		ImGui::PopItemWidth();
		ImGui::Dummy({ 0,10 });

		ImGui::Text("Path Type");
		ImGui::Separator();

		ImGui::Spacing();

		if (ImGui::RadioButton("Smooth Path", agentProperties->pathType == PathType::SMOOTH))
			agentProperties->pathType = PathType::SMOOTH;
		ImGui::SameLine();
		if (ImGui::RadioButton("Straight Path", agentProperties->pathType == PathType::STRAIGHT))
			agentProperties->pathType = PathType::STRAIGHT;

		ImGui::Dummy({ 0,10 });
	}
	ImGui::PopID();
}

void ComponentAgent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["Enabled"] = _isEnabled;

	_j["Agent"]["Radius"] = agentProperties->radius;
	_j["Agent"]["Height"] = agentProperties->height;
	_j["Agent"]["MaxClimb"] = agentProperties->maxClimb;
	_j["Agent"]["MaxSlope"] = agentProperties->maxSlope;
	_j["Agent"]["Speed"] = agentProperties->speed;
	_j["Agent"]["AngularSpeed"] = agentProperties->angularSpeed;
	_j["Agent"]["Acceleration"] = agentProperties->acceleration;
	_j["Agent"]["StoppingDistance"] = agentProperties->stoppingDistance;

	// Path info
	/*
	_j["Agent"]["PathType"] = agentProperties->pathType;
	_j["Agent"]["NumPolys"] = agentProperties->m_npolys;
	_j["Agent"]["NumStraight"] = agentProperties->m_nstraightPath;
	_j["Agent"]["NumSmooth"] = agentProperties->m_nsmoothPath;

	if (agentProperties->m_npolys > 0)
	{

	}
	*/

	j["Components"].push_back(_j);
}

void ComponentAgent::DeSerialization(json& j)
{
	agentProperties->radius = j["Agent"]["Radius"];
	agentProperties->height = j["Agent"]["Height"];
	agentProperties->maxClimb = j["Agent"]["MaxClimb"];
	agentProperties->maxSlope = j["Agent"]["MaxSlope"];
	agentProperties->speed = j["Agent"]["Speed"];
	agentProperties->angularSpeed = j["Agent"]["AngularSpeed"];
	agentProperties->acceleration = j["Agent"]["Acceleration"];
	agentProperties->stoppingDistance = j["Agent"]["StoppingDistance"];

	bool enabled = j["Enabled"];
}
