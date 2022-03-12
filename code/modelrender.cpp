#include "modelrender.h"

ModelRender::ModelRender(Shader shader, glm::vec3 lightDir, glm::vec3 lightPos) {
	this->shader = shader;
	this->lightDir = lightDir;
	this->lightPos = lightPos;
	for (int i = 0; i < 4; i++) {
		boundingBox.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	shader.setInt("shadowMap", 1);
}

void ModelRender::addModel(std::string modelPath, std::string modelName) {
	allModels[modelName] = Model(modelPath.c_str());
}

void ModelRender::drawModel(std::string modelName, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, ShadowMapRender* sm) {
	shader.use();
	shader.setVec3("light.direction", lightDir);
	shader.setVec3("light.lightPos", lightPos);
	shader.setVec3("viewPos", cameraPos);
	shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("material.shininess", 64.0f);
	shader.setVec3("light.direction", lightDir);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	shader.setMat4("lightSpaceMatrix", sm->lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sm->depthMap);
	boundingBox = allModels[modelName].getCC(model);
	allModels[modelName].Draw(shader);
}
void ModelRender::TransFromName(std::string modelName)
{
	allModels[modelName].Translate();
}

glm::vec3 ModelRender::getTransFromName(std::string modelName)
{
	return allModels[modelName].Position;
}
void ModelRender::ProcessKeyboardMovementfromName(std::string modelName, Model_Movement direction, float deltaTime)
{
	Model model = allModels[modelName];
	if (direction == MODELROLLUP) {
		allModels[modelName].Roll += model.RotateSpeed;
		allModels[modelName].Yaw += model.RotateSpeed * (allModels[modelName].Roll / 15.0);
		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (allModels[modelName].Roll > 45.0f)
			allModels[modelName].Roll = 45.0f;
		if (allModels[modelName].Roll < -45.0f)
			allModels[modelName].Roll = -45.0f;
	}
	if (direction == MODELROLLDOWN) {
		allModels[modelName].Roll -= model.RotateSpeed;
		allModels[modelName].Yaw += model.RotateSpeed * (allModels[modelName].Roll / 15.0);
		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (allModels[modelName].Roll > 45.0f)
			allModels[modelName].Roll = 45.0f;
		if (allModels[modelName].Roll < -45.0f)
			allModels[modelName].Roll = -45.0f;
	}
	if (direction == MODELPITCHUP) {
		allModels[modelName].Pitch += model.RotateSpeed;
		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (allModels[modelName].Pitch > 30.0f)
			allModels[modelName].Pitch = 30.0f;
		if (allModels[modelName].Pitch < -30.0f)
			allModels[modelName].Pitch = -30.0f;
	}
	if (direction == MODELPITCHDOWN) {
		allModels[modelName].Pitch -= model.RotateSpeed;
		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (allModels[modelName].Pitch > 30.0f)
			allModels[modelName].Pitch = 30.0f;
		if (allModels[modelName].Pitch < -30.0f)
			allModels[modelName].Pitch = -30.0f;
	}
	allModels[modelName].updateVec();
}
void ModelRender::ResetRotateFromName(std::string modelName)
{
	double pitch = allModels[modelName].Pitch, roll = allModels[modelName].Roll, speed = allModels[modelName].RotateSpeed;
	if (pitch > 0) {
		allModels[modelName].Pitch -= speed;
	}
	else if (pitch < 0) {
		allModels[modelName].Pitch += speed;
	}
	else {
		allModels[modelName].Pitch = 0.0f;
	}
	if (roll > 0) {
		allModels[modelName].Roll -= speed;
	}
	else if (roll < 0) {
		allModels[modelName].Roll += speed;
	}
	else {
		allModels[modelName].Roll= 0.0f;
	}
	allModels[modelName].Yaw += speed * (allModels[modelName].Roll / 15.0);
	allModels[modelName].updateVec();
}
glm::vec3 ModelRender::GetAngleFromName(std::string modelName)
{
	Model model = allModels[modelName];
	glm::vec3 angle = glm::vec3(model.Pitch, model.Yaw, model.Roll);
	return angle;
}
glm::vec3 ModelRender::GetFrontFromName(std::string modelName)
{
	return allModels[modelName].Front;
}
glm::vec3 ModelRender::GetRightFromName(std::string modelName)
{
	return allModels[modelName].Right;
}
glm::vec3 ModelRender::GetUpFromName(std::string modelName)
{
	return allModels[modelName].Up;
}

