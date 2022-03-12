#include "groundrender.h"

GroundRender::GroundRender(Shader shader, Shader objShader, glm::vec3 lightDir, glm::vec3 lightPos) {
	this->shader = shader;
	this->objShader = objShader;
	this->lightDir = lightDir;
	this->lightPos = lightPos;

	int hm_width, hm_height, hm_nrChannels;
	unsigned char* data = stbi_load("resources/ground/Heightmap.png", &hm_width, &hm_height, &hm_nrChannels, 0);

	//生成地形数据
	float widthStep = 1.0 * groundWidth / widthNum;
	float heightStep = 1.0 * groundHeight / heightNum;
	int zStepWidth = 1.0 * hm_width / (widthNum+1);
	int zStepHeight = 1.0 * hm_height / (heightNum+1);
	float zScale = 1 / 100.0;
	float textureScale = 1/10.0;
	groundVertex = new float[widthNum*heightNum*6*14];
	int count = 0;
	for (int i = 0; i < heightNum; i++) {
		std::vector<glm::vec3> oneLineData;
		for (int j = 0; j < widthNum; j++) {
			float nowX = j * widthStep;
			float nextX = (j + 1) * widthStep;
			float nowY = i * heightStep;
			float nextY = (i + 1) * heightStep;
			float nownowz = data[zStepHeight * i * hm_width * 4 + zStepWidth * j * 4]*zScale+toUnder;
			float nownextz = data[zStepHeight * i * hm_width * 4 + zStepWidth * (j+1) * 4]*zScale+toUnder;
			float nextnowz = data[zStepHeight * (i+1) * hm_width * 4 + zStepWidth * j * 4]*zScale+toUnder;
			float nextnextz = data[zStepHeight * (i+1) * hm_width * 4 + zStepWidth * (j+1) * 4]*zScale+toUnder;

			glm::vec3 pos1(nowX, nownowz, nowY);
			glm::vec2 uv1(nowX * textureScale, nowY * textureScale);
			glm::vec3 pos2(nextX, nownextz, nowY);
			glm::vec2 uv2(nextX * textureScale, nowY * textureScale);
			glm::vec3 pos3(nextX, nextnextz, nextY);
			glm::vec2 uv3(nextX * textureScale, nextY * textureScale);
			glm::vec3 pos4(nowX, nextnowz, nextY);
			glm::vec2 uv4(nowX * textureScale, nextY * textureScale);

			glm::vec3 tangent1, bitangent1;
			glm::vec3 tangent2, bitangent2;

			glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1;
			glm::vec3 norm1 = glm::cross(edge2, edge1);
			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;
			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			edge1 = pos3 - pos1;
			edge2 = pos4 - pos1;
			glm::vec3 norm2 = glm::cross(edge2, edge1);
			deltaUV1 = uv3 - uv1;
			deltaUV2 = uv4 - uv1;
			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			//first
			groundVertex[count++] = nowX;
			groundVertex[count++] = nownowz;
			groundVertex[count++] = nowY;
			groundVertex[count++] = norm1.x;
			groundVertex[count++] = norm1.y;
			groundVertex[count++] = norm1.z;
			groundVertex[count++] = nowX*textureScale;
			groundVertex[count++] = nowY * textureScale;
			groundVertex[count++] = tangent1.x;
			groundVertex[count++] = tangent1.y;
			groundVertex[count++] = tangent1.z;
			groundVertex[count++] = bitangent1.x;
			groundVertex[count++] = bitangent1.y;
			groundVertex[count++] = bitangent1.z;
			//second
			groundVertex[count++] = nextX;
			groundVertex[count++] = nownextz;
			groundVertex[count++] = nowY;
			groundVertex[count++] = norm1.x;
			groundVertex[count++] = norm1.y;
			groundVertex[count++] = norm1.z;
			groundVertex[count++] = nextX * textureScale;
			groundVertex[count++] = nowY * textureScale;
			groundVertex[count++] = tangent1.x;
			groundVertex[count++] = tangent1.y;
			groundVertex[count++] = tangent1.z;
			groundVertex[count++] = bitangent1.x;
			groundVertex[count++] = bitangent1.y;
			groundVertex[count++] = bitangent1.z;
			//third
			groundVertex[count++] = nextX;
			groundVertex[count++] = nextnextz;
			groundVertex[count++] = nextY;
			groundVertex[count++] = norm1.x;
			groundVertex[count++] = norm1.y;
			groundVertex[count++] = norm1.z;
			groundVertex[count++] = nextX * textureScale;
			groundVertex[count++] = nextY * textureScale;
			groundVertex[count++] = tangent1.x;
			groundVertex[count++] = tangent1.y;
			groundVertex[count++] = tangent1.z;
			groundVertex[count++] = bitangent1.x;
			groundVertex[count++] = bitangent1.y;
			groundVertex[count++] = bitangent1.z;
			//third
			groundVertex[count++] = nextX;
			groundVertex[count++] = nextnextz;
			groundVertex[count++] = nextY;
			groundVertex[count++] = norm2.x;
			groundVertex[count++] = norm2.y;
			groundVertex[count++] = norm2.z;
			groundVertex[count++] = nextX * textureScale;
			groundVertex[count++] = nextY * textureScale;
			groundVertex[count++] = tangent2.x;
			groundVertex[count++] = tangent2.y;
			groundVertex[count++] = tangent2.z;
			groundVertex[count++] = bitangent2.x;
			groundVertex[count++] = bitangent2.y;
			groundVertex[count++] = bitangent2.z;
			//four
			groundVertex[count++] = nowX;
			groundVertex[count++] = nextnowz;
			groundVertex[count++] = nextY;
			groundVertex[count++] = norm2.x;
			groundVertex[count++] = norm2.y;
			groundVertex[count++] = norm2.z;
			groundVertex[count++] = nowX * textureScale;
			groundVertex[count++] = nextY * textureScale;
			groundVertex[count++] = tangent2.x;
			groundVertex[count++] = tangent2.y;
			groundVertex[count++] = tangent2.z;
			groundVertex[count++] = bitangent2.x;
			groundVertex[count++] = bitangent2.y;
			groundVertex[count++] = bitangent2.z;
			//first
			groundVertex[count++] = nowX;
			groundVertex[count++] = nownowz;
			groundVertex[count++] = nowY;
			groundVertex[count++] = norm2.x;
			groundVertex[count++] = norm2.y;
			groundVertex[count++] = norm2.z;
			groundVertex[count++] = nowX * textureScale;
			groundVertex[count++] = nowY * textureScale;
			groundVertex[count++] = tangent2.x;
			groundVertex[count++] = tangent2.y;
			groundVertex[count++] = tangent2.z;
			groundVertex[count++] = bitangent2.x;
			groundVertex[count++] = bitangent2.y;
			groundVertex[count++] = bitangent2.z;

			oneLineData.push_back(pos1);
		}
		mountData.push_back(oneLineData);
	}
	unsigned int VBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* widthNum * heightNum * 6 * 14, groundVertex, GL_STATIC_DRAW);
	glBindVertexArray(quadVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.ambient", 1);
	shader.setInt("material.normal", 2);
	shader.setInt("material.displacement", 3);
	shader.setInt("shadowMap", 4);
	//shader.setInt("groundgrass", 0);

	//treeModel
	this->treeModel = Model("resources/objects/tree/treeA.obj");
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());
	for (int i = 0; i < amount; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 tempPos = mountData[rand() % heightNum][rand() % widthNum];
		tempPos.z -= 0.001;
		model = glm::translate(model, tempPos);
		float scale = (rand() % 20) / 10000.0f + 0.004;
		model = glm::scale(model, glm::vec3(scale));
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 0.6f, 0.0f));
		modelMatrices[i] = model;
	}
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < treeModel.meshes.size(); i++) {
		unsigned int VAO = treeModel.meshes[i].VAO;
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
	objShader.setInt("shadowMap", 1);
}

GroundRender::~GroundRender() {
	glDeleteVertexArrays(1, &this->quadVAO);
}

void GroundRender::setFog(float fogd, glm::vec3 fogc)
{
	fog_Density = fogd;
	fog_Color = fogc;
}

void GroundRender::addTexture(Texture2D oneTexture, std::string name) {
	allTextures[name] = oneTexture;
}

void GroundRender::drawGround(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, ShadowMapRender* sm) {

	objShader.use();
	objShader.setVec3("light.direction", lightDir);
	objShader.setVec3("light.lightPos", lightPos);
	objShader.setVec3("viewPos", cameraPos);
	//std::cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
	objShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	objShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	objShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	objShader.setFloat("material.shininess", 64.0f);
	objShader.setVec3("light.direction", lightDir);
	objShader.setVec3("viewPos", cameraPos);
	objShader.setMat4("projection", projection);
	objShader.setMat4("view", view);
	objShader.setMat4("model", model);
	objShader.setMat4("lightSpaceMatrix", sm->lightSpaceMatrix);
	objShader.setFloat("fog.Density", fog_Density*0.8);
	objShader.setVec3("fog.Color", fog_Color);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sm->depthMap);
	for (unsigned int i = 0; i < treeModel.meshes.size()-1; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, treeModel.textures_loaded[i].id);
		glBindVertexArray(treeModel.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, treeModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}


	shader.use();
	shader.setFloat("fog.Density", fog_Density);
	shader.setVec3("fog.Color", fog_Color);
	shader.setVec3("light.direction", lightDir);
	shader.setVec3("light.lightPos", lightPos);
	shader.setVec3("viewPos", cameraPos);
	shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("material.shininess", 64.0f);
	shader.setMat4("lightSpaceMatrix", sm->lightSpaceMatrix);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, allTextures["groundColor"].ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, allTextures["groundAmbient"].ID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, allTextures["groundNormal"].ID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, allTextures["groundHeight"].ID);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, sm->depthMap);

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, widthNum * heightNum * 6 * 14);
}

void GroundRender::drawShadowMap(ShadowMapRender* sm, glm::mat4 model) {
	sm->shader.use();
	sm->shader.setMat4("lightSpaceMatrix", sm->lightSpaceMatrix);
	sm->shader.setMat4("model", model);
	sm->shader.setBool("isInstance", true);
	glBindFramebuffer(GL_FRAMEBUFFER, sm->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (unsigned int i = 0; i < treeModel.meshes.size() - 1; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, treeModel.textures_loaded[i].id);
		glBindVertexArray(treeModel.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, treeModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}

	sm->shader.setBool("isInstance", false);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, widthNum * heightNum * 6 * 14);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}