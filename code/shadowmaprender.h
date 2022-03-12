#ifndef SHADOW_MAP_RENDER
#define SHADOW_MAP_RENDER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShadowMapRender
{
public:
	unsigned int depthMapFBO;
    unsigned int depthMap;
	glm::vec3 lightPos;
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	Shader shader;
	ShadowMapRender(Shader shader, glm::vec3 lightPos, unsigned int SHADOW_WIDTH, unsigned int SHADOW_HEIGHT);
	~ShadowMapRender();
};

#endif

