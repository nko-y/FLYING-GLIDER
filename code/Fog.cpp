#include "Fog.h"

void Fog::setShader(Shader& shader, std::string Name, GLboolean UseShader) {
	if (UseShader)
		shader.use();
	shader.setFloat(Name + ".Density", m_Density);
	shader.setVec3(Name + ".Color", m_Color);
}
void Fog::setGroundRender(GroundRender* shader) {
	shader->setFog(m_Density, m_Color);
}