#include "Headers.h"
#include "LayerGame.h"

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

	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, 36/*NUM OF INDICES*/, GL_UNSIGNED_INT, 0);
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
}
