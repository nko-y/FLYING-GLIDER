#ifndef SKYBOX_RENDER_H
#define SKYBOX_RENDER_H

#include <stb_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "texture.h"
#include "shader.h"

class SkyboxRender
{
public:
	SkyboxRender(Shader shader, std::vector<std::string> faces);
	~SkyboxRender();
	void drawSkyBox(glm::mat4 view, glm::mat4 projection);
private:
	Shader shader;
	unsigned int textureID;
	unsigned int skyboxVAO, skyboxVBO;
};

#endif