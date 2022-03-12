#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"

#include "resource_manager.h"
#include "skyboxrender.h"
#include "modelrender.h"
#include "groundrender.h"
#include "shadowmaprender.h"
#include "Fog.h"
#include "./snow/Snow.h"


class Game
{
public:
	unsigned int Width, Height;
	bool Keys[1024];
	bool KeysProcessed[1024];
	Camera camera;
	glm::vec3 lightDir;
	glm::vec3 lightPos;

	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render(float deltaTime);
	bool isCrash = false;
	void crashTest(std::vector<glm::vec3> recV, GroundRender* ground);
	void explodeRender(float deltaTime); //
};

#endif