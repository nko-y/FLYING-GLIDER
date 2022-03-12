#ifndef GROUND_RENDER_H
#define GROUND_RENDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>
#include "model.h"

#include "texture.h"
#include "shader.h"
#include "shadowmaprender.h"

#include <stb_image.h>

class GroundRender
{
public:
	std::vector<std::vector<glm::vec3>> mountData;
	glm::vec3 lightDir;
	glm::vec3 lightPos;
	float fog_Density;
	glm::vec3 fog_Color;

	GroundRender(Shader shader, Shader objShader, glm::vec3 lightDir, glm::vec3 lightPos);
	~GroundRender();
	void addTexture(Texture2D oneTexture, std::string name);
	void drawGround(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, ShadowMapRender* sm);
	void drawShadowMap(ShadowMapRender* sm, glm::mat4 model);
	void setFog(float fogd, glm::vec3 fogc);
// private:
	Shader shader;
	Shader objShader;
	Model treeModel;
	std::map<std::string, Texture2D> allTextures;

	unsigned int quadVAO;
	int groundWidth = 10;
	int groundHeight = 10;
	int widthNum = 500;
	int heightNum = 500;
	float* groundVertex;
	float toUnder = -5.0;

	unsigned int amount = 1000;
	glm::mat4* modelMatrices;
};

#endif