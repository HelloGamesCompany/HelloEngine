#include "Headers.h"
#include "LayerGame.h"
#include "Application.h"
#include "ModuleWindow.h"

float vertices[] = {
-1.0f,1.0f,0.0f,
  -1.0f,-1.0f,0.0f,
  1.0f,1.0f,0.0f,
  1.0f,-1.0f,0.0f,
  -1.0f,1.0f,-2.0f,
  -1.0f,-1.0f,-2.0f,
  1.0f,1.0f,-2.0f,
  1.0f,-1.0f,-2.0f
};

unsigned int indices[] = {
  0, 2, 3, 0, 3, 1,
  2, 6, 7, 2, 7, 3,
  6, 4, 5, 6, 5, 7,
  4, 0, 1, 4, 1, 5,
  0, 4, 6, 0, 6, 2,
  1, 5, 7, 1, 7, 3,
};

LayerGame::LayerGame()
{
	
}

LayerGame::~LayerGame()
{
}

void LayerGame::Start()
{
	VertexBufferArraySetUp();
}

void LayerGame::PreUpdate()
{
}

void LayerGame::Update()
{
}

void LayerGame::PostUpdate()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, 36/*NUM OF INDICES*/, GL_UNSIGNED_INT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//void* pixels = malloc(Application::Instance()->window->width * Application::Instance()->window->height * 3 * sizeof(float));

	//glGetTexImage(GL_TEXTURE_2D, textureColorbuffer, 0, GL_RGB, pixels);
}

void LayerGame::CleanUp()
{
}

void LayerGame::VertexBufferArraySetUp()
{
	// Create Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Create Index Buffer Object
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * 36/*NUM OF INDICES*/, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	// Create Vertex Buffer Object
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// generate texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Application::Instance()->window->width, Application::Instance()->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// create render buffer object
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Application::Instance()->window->width, Application::Instance()->window->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}
