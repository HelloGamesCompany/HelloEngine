#include "Headers.h"
#include "LayerEditor.h"
#include "Application.h"
#include "QuickSave.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ImGuizmo/ImGuizmo.h"

#include "ImWindowConfiguration.h"
#include "ImWindowAbout.h"
#include "ImWindowConsole.h"
#include "ImWindowProject.h"
#include "ImWindowHierarchy.h"
#include "ImWindowInspector.h"
#include "ImWindowQuickSave.h"
#include "ImWindowScene.h"
#include "ImWindowGame.h"
#include "ImWindowPerformanceTest.h"
#include "ImWindowResources.h"

#include "ModuleLayers.h"
#include "LayerGame.h"

#include "ModuleFiles.h"
#include "TransformComponent.h"

#include "ModuleCommand.h"

LayerEditor::LayerEditor()
{
	selectedGameObject = nullptr;
}

LayerEditor::~LayerEditor()
{
}

void LayerEditor::Start()
{
	// Get application
	_app = Application::Instance();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGuiContext* imGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGuizmo::SetImGuiContext(imGuiContext);
	ImGuizmo::SetOrthographic(false);
	// Setup Dear ImGui style
	
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 10.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.0f;
		style.Colors[ImGuiCol_Border] = ImVec4(255, 1, 1, 255);
	}

	// Setup ImGui style
	{
		style.Colors[ImGuiCol_ScrollbarBg] = style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1f, 0.1f, 0.1f, 1);

		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1);

		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 1);

		style.Colors[ImGuiCol_FrameBg] = style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_Header] =
			style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_TabUnfocused] = style.Colors[ImGuiCol_Tab] =
			ImVec4(0.25f, 0.25f, 0.25f, 1);

		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1);

		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_HeaderHovered] = style.Colors[ImGuiCol_FrameBgActive] =
			style.Colors[ImGuiCol_DockingPreview] = style.Colors[ImGuiCol_TabActive] = style.Colors[ImGuiCol_SliderGrab] =
			ImVec4(0.4f, 0.4f, 0.4f, 1);

		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_SliderGrabActive] =
			style.Colors[ImGuiCol_HeaderActive] = style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_CheckMark] =
			ImVec4(0.65f, 0.65f, 0.65f, 1);

		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1);
	}
	
	// Setup font
	char* buf = nullptr;

	uint bufSize = ModuleFiles::S_Load("Resources/Editor/Fonts/font.ttf", &buf);

	// ImGui will delete buf memory when close application, so no need realease here
	io.Fonts->AddFontFromMemoryTTF(buf, bufSize, 14.0f, NULL);

	// Init OpenGL
	const char* glsl_version = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(_app->window->window, _app->renderer3D->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Create ImGui editor windows
	{
		_imWindows[(uint)ImWindowID::CONFIGURATION] = configuration = new ImWindowConfiguration();		
		_imWindows[(uint)ImWindowID::ABOUT] = new ImWindowAbout();
		_imWindows[(uint)ImWindowID::CONSOLE] = new ImWindowConsole();
		_imWindows[(uint)ImWindowID::PROJECT] = new ImWindowProject();
		_imWindows[(uint)ImWindowID::QUICKSAVE] = new ImWindowQuickSave();
		_imWindows[(uint)ImWindowID::INSPECTOR] = new ImWindowInspector();
		_imWindows[(uint)ImWindowID::HIERARCHY] = new ImWindowHierarchy();
		_imWindows[(uint)ImWindowID::SCENE] = new ImWindowScene();
		_imWindows[(uint)ImWindowID::GAME] = new ImWindowGame();
		_imWindows[(uint)ImWindowID::PERFORMANCE] = new ImWindowPerformanceTest();
		_imWindows[(uint)ImWindowID::RESOURCES] = new ImWindowResources();
	}

	// Get layer game for future implementation
	_game = (LayerGame*)_app->layers->layers[(uint)LayersID::GAME];

	// Reserve space for popUpMessages
	_popUpMessages.reserve(20);
}

void LayerEditor::PreUpdate()
{

}

void LayerEditor::Update()
{
#ifdef STANDALONE

	if(_app->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		ModuleCommand::S_DeleteGameObject(selectedGameObject);
	}

#endif // !STANDALONE
}

void LayerEditor::PostUpdate()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// ImGuizmo
	ImGuizmo::BeginFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	DrawMenuBar();

	DrawPopUpMessages();

	// Draw Every windows
    for (int i = 0; i < (uint)ImWindowID::MAX; i++)
    {
        if (_imWindows[i]->isEnabled) _imWindows[i]->Update();
    }

	Application::Instance()->renderer3D->modelRender.OnEditor();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void LayerEditor::CleanUp()
{
	for (int i = 0; i < (uint)ImWindowID::MAX; i++)
	{
		RELEASE(_imWindows[i]);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void LayerEditor::SetSelectGameObject(GameObject* g)
{
	if (selectedGameObject)
		selectedGameObject->isSelected = false;

	selectedGameObject = g;
	
	if (selectedGameObject)
		selectedGameObject->isSelected = true;

	ImWindowInspector* inspector = (ImWindowInspector*)_imWindows[(uint)ImWindowID::INSPECTOR];

	inspector->SelectGameObject(g);
}

void LayerEditor::AddPopUpMessage(std::string message)
{
	_popUpMessages.emplace_back(message);
}

void LayerEditor::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1));

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.05f, 0.05f, 0.05f, 1));

		if (ImGui::BeginMenu("Application"))
		{
			if (ImGui::MenuItem("Close Appplication"))
			{
				_app->Exit();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			for (int i = 0; i < (uint)ImWindowID::MAX; i++)
			{
				ImGui::MenuItem(_imWindows[i]->windowName.c_str(), (const char*)0, &_imWindows[i]->isEnabled);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("Check our Github!"))
			{
				ShellExecute(0, 0, "https://github.com/HelloGamesCompany/HelloEngine", 0, 0, SW_SHOW);
			}
			ImGui::EndMenu();
		}
		ImGui::PopStyleColor(2);

		ImGui::EndMainMenuBar();
	}

	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = ImGui::GetFrameHeight();

	if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
		if (ImGui::BeginMenuBar()) {
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			int totalButtonsWidth = 100;
			ImGui::SetCursorPos(ImVec2(contentRegion.x / 2 - totalButtonsWidth, 0));
			if (ImGui::Button("Play"))
				_game->Play();
			if (ImGui::Button("Pause"))
				_game->Pause();			
			if (ImGui::Button("Stop"))
				_game->Stop();
			if (ImGui::Button("One frame"))
				_game->OneFrame();
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

}

void LayerEditor::DrawPopUpMessages()
{
	float fadeInFix = 0.4f; // 0 - 0.4 do fadeIn
	float fadeOutFix = 0.8f; // 0.8 - 1.0 do fadeOut

	bool fadeIn = false;
	bool fadeOut = false;
	bool hovering = false; // We need this variable to Push/Pop in the correct moment.

	int width = _app->window->width;
	int height = _app->window->height;

	std::string id = "popUpMessage";

	ImGuiWindowFlags popupFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs;

	if (_popUpMessages.empty())
		return;

	for (size_t i = 0; i < _popUpMessages.size(); i++)
	{
		_popUpMessages[i].currentMessageTime += (1 / ImGui::GetIO().Framerate);

		if (_popUpMessages[i].currentMessageTime >= _messageTime)
		{
			_popUpMessages.erase(_popUpMessages.begin() + i--);
		
			continue;
		}

		fadeIn = _popUpMessages[i].currentMessageTime <= _messageTime * fadeInFix;
		fadeOut = _popUpMessages[i].currentMessageTime >= _messageTime * fadeOutFix;

		if (fadeIn)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, _popUpMessages[i].currentMessageTime / (_messageTime * fadeInFix));
		}
		else if (fadeOut)
		{
			float result = 1.0f - ((_popUpMessages[i].currentMessageTime - (_messageTime * fadeOutFix)) / (1.0f - fadeOutFix));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, result > 0 ? result : 0.0f);
		}

		ImGui::SetNextWindowSize(ImVec2(width * 0.3f, height * 0.15f));
		ImGui::SetNextWindowPos(ImVec2(width * 0.35f, (height * 0.425f) - 30.0f * _popUpMessages[i].currentMessageTime));

		id += std::to_string(i);

		if (_popUpMessages[i].hovered)
		{
			if (fadeIn || fadeOut)
				ImGui::PopStyleVar();
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.25f);
			hovering = true;
		}
		else
			hovering = false;

		if (ImGui::Begin(id.c_str(),0, popupFlags))
		{
			float2 temp = float2(width * 0.3f, height * 0.15f);
			temp += float2(width * 0.35f, (height * 0.425f) - 30.0f * _popUpMessages[i].currentMessageTime);
			ImVec2 bottomRight = ImVec2(temp.x, temp.y);

			if (ImGui::IsMouseHoveringRect(ImVec2(width * 0.35f, (height * 0.425f) - 30.0f * _popUpMessages[i].currentMessageTime),
				bottomRight))
			{
				_popUpMessages[i].hovered = true;
			}
			else
				_popUpMessages[i].hovered = false;

			ImVec2 textDimensions = ImGui::CalcTextSize(_popUpMessages[i].message.c_str());

			ImGui::SetWindowFontScale(1.0f);
			ImGui::SetCursorPos(ImVec2((width * 0.3f - textDimensions.x) * 0.5f, (height * 0.15f - textDimensions.y) * 0.5f));
			ImGui::Text(_popUpMessages[i].message.c_str());
		}
		if (fadeIn || fadeOut || hovering) ImGui::PopStyleVar();
		ImGui::End();

		
	}
}