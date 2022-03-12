#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

#include <map>
#include <vector>
#include <string>

struct FlameParticle
{
    float type;//粒子类型，分发射器和第二级粒子
    glm::vec3 position;
    glm::vec3 velocity;
    float lifetimeMills;//年龄
    float alpha;//alpha通道
    float size;//粒子点精灵大小
    float life;//寿命
};

class FlameRender
{
public:
	Shader shader;
	FlameRender(Shader shader) {
		this->shader = shader;
	}
};

