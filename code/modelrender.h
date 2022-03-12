#ifndef MODEL_RENDER_H
#define MODEL_RENDER_H

#include <GL/glew.h>
#include "model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

#include <map>
#include <vector>
#include <string>

#include "shadowmaprender.h"

enum Model_Movement {
	MODELPITCHUP,
	MODELPITCHDOWN,
	MODELROLLUP,
	MODELROLLDOWN
};

class ModelRender
{
public:
	std::vector<glm::vec3> boundingBox;
	glm::vec3 lightDir;
	glm::vec3 lightPos;

	ModelRender(Shader shader, glm::vec3 lightDir, glm::vec3 lightPos);
	void addModel(std::string modelPath, std::string modelName);
	void drawModel(std::string modelName, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, ShadowMapRender* sm);
	void TransFromName(std::string modelName);
	void ProcessKeyboardMovementfromName(std::string modelName, Model_Movement direction, float deltaTime);
	void ResetRotateFromName(std::string modelName);
	glm::vec3 GetAngleFromName(std::string modelName);
	glm::vec3 getTransFromName(std::string modelName);
	glm::vec3 GetFrontFromName(std::string modelName);
	glm::vec3 GetRightFromName(std::string modelName);
	glm::vec3 GetUpFromName(std::string modelName);
private:
	Shader shader;
	std::map<std::string, Model> allModels;
};

#endif
