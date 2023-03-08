#include "Headers.h"
#include "BillBoardComponent.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"


BillBoardComponent::BillBoardComponent(GameObject* gameObject) : Component(gameObject)
{

	_type = Type::BILLBOARD;
	app = Application::Instance();

	currentBBoard = "No Align";
}

BillBoardComponent::~BillBoardComponent()
{

}

void BillBoardComponent::OnEnable()
{
}

void BillBoardComponent::OnDisable()
{
}

Quat BillBoardComponent::GetBBRotation(Particle& particle)
{

	switch (typeofBBoard)
	{
	case BILLBOARDTYPE::NO_ALIGN:

		rotation = Quat::identity;
		break;
	case BILLBOARDTYPE::SCREENALIGN:

		rotation = ScreenAlignBBoard();

		break;
	case BILLBOARDTYPE::WORLDALIGN:

		rotation = WorldAlignBBoard(particle);

		break;
	case BILLBOARDTYPE::AXISALIGN:

		rotation = AxisAlignBBoard(particle);

		break;
	default:
		break;
	}

	return rotation;
}

Quat BillBoardComponent::ScreenAlignBBoard()
{

	//GET INFO ABOUT CAM AXIS
	float3 activecamfront = app->camera->currentDrawingCamera->cameraFrustum.front;
	//Vector UP is the same as the cam
	float3 activecamup = app->camera->currentDrawingCamera->cameraFrustum.up;

	//Z-AXIS MUST BE INVERTED 
	zBBoardAxis = -activecamfront;
	//Y-AXIS KEEPS THE SAME VALUE
	yBBoardAxis = activecamup;

	//COMPUTE CROSS PRODUCT IN ORDER TO GET THE REMAINING AXIS
	xBBoardAxis = yBBoardAxis.Cross(zBBoardAxis).Normalized();

	//Gather the axis into a 3x3 matrix
	float3x3 rotBBoard;
	rotBBoard.Set(xBBoardAxis.x, xBBoardAxis.y, xBBoardAxis.z, yBBoardAxis.x, yBBoardAxis.y, yBBoardAxis.z, zBBoardAxis.x, zBBoardAxis.y, zBBoardAxis.z);

	return rotBBoard.Inverted().ToQuat();

}

Quat BillBoardComponent::WorldAlignBBoard(Particle& particle)
{

	//Vector from gameobject to cam
	zBBoardAxis = (app->camera->currentDrawingCamera->cameraFrustum.pos - particle.position).Normalized();

	//Vector UP is the same as the cam

	yBBoardAxis = app->camera->currentDrawingCamera->cameraFrustum.up;

	//COMPUTE CROSS PRODUCT IN ORDER TO GET THE REMAINING AXIS

	xBBoardAxis = yBBoardAxis.Cross(zBBoardAxis).Normalized();

	//COMPUTE Y AXIS AGAIN IN ORDER TO BE SURE THAT THE ANGLE BETWEEN Z AND Y IS 90 degrees

	yBBoardAxis = zBBoardAxis.Cross(xBBoardAxis).Normalized();

	//Gather the axis into a 3x3 matrix
	float3x3 rotBBoard;
	rotBBoard.Set(xBBoardAxis.x, xBBoardAxis.y, xBBoardAxis.z, yBBoardAxis.x, yBBoardAxis.y, yBBoardAxis.z, zBBoardAxis.x, zBBoardAxis.y, zBBoardAxis.z);

	return rotBBoard.Inverted().ToQuat();

}

Quat BillBoardComponent::AxisAlignBBoard(Particle& particle)
{

	//Vector from gameobject to cam

	zBBoardAxis = (app->camera->currentDrawingCamera->cameraFrustum.pos - particle.position).Normalized();

	//Vector UP is the same as the cam
	yBBoardAxis = { 0.0f,1.0f,0.0f };

	//COMPUTE CROSS PRODUCT IN ORDER TO GET THE REMAINING AXIS

	xBBoardAxis = { 1.0f,0.0f,0.0f };

	//COMPUTE Y AXIS AGAIN IN ORDER TO BE SURE THAT THE ANGLE BETWEEN Z AND Y IS 90 degrees

	//yBBoardAxis = zBBoardAxis.Cross(xBBoardAxis).Normalized();

	//Gather the axis into a 3x3 matrix
	float3x3 rotBBoard = float3x3::identity;
	rotBBoard.Set(xBBoardAxis.x, xBBoardAxis.y, xBBoardAxis.z, yBBoardAxis.x, yBBoardAxis.y, yBBoardAxis.z, zBBoardAxis.x, zBBoardAxis.y, zBBoardAxis.z);

	 
	return rotBBoard.Inverted().ToQuat();
}

#ifdef STANDALONE

void BillBoardComponent::OnEditor()
{
	
	if (ImGui::CollapsingHeader("BillBoard: ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginMenu("Select your BillBoard"))
		{

			if (ImGui::MenuItem("Screen Align BillBoard"))
			{
				typeofBBoard = BILLBOARDTYPE::SCREENALIGN;
				currentBBoard = BBtype[0];
			}
			else if (ImGui::MenuItem("World Align BillBoard"))
			{
				typeofBBoard = BILLBOARDTYPE::WORLDALIGN;
				currentBBoard = BBtype[1];
			}
			else if (ImGui::MenuItem("Axis Align BillBoard"))
			{
				typeofBBoard = BILLBOARDTYPE::AXISALIGN;
				currentBBoard = BBtype[2];
			}
			else if (ImGui::MenuItem("No Align BillBoard"))
			{
				typeofBBoard = BILLBOARDTYPE::NO_ALIGN;
				rotation = Quat::identity;
				currentBBoard = BBtype[3];
			}

			


			ImGui::End();
		}

		ImGui::TextColored(ImVec4(0, 1, 0, 1), currentBBoard.c_str());
	}

}

#endif


void BillBoardComponent::Serialization(json& j)
{
}

void BillBoardComponent::DeSerialization(json& j)
{
}
