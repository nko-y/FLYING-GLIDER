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
    float type;//�������ͣ��ַ������͵ڶ�������
    glm::vec3 position;
    glm::vec3 velocity;
    float lifetimeMills;//����
    float alpha;//alphaͨ��
    float size;//���ӵ㾫���С
    float life;//����
};

class FlameRender
{
public:
	Shader shader;
	FlameRender(Shader shader) {
		this->shader = shader;
	}
};

