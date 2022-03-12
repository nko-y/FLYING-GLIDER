#include "game.h"

SkyboxRender* skyboxItem;
ModelRender* modelItem;
Snow::Snow* snowItem;
// mountain
GroundRender* groundItem;
ShadowMapRender* shadowItem;
float Sdt = 0.0f;

// Fog
const float fogDensity = 0.2f;
const glm::vec3 fogColor1 = glm::vec3(0.604f, 0.655f, 0.718f);
const glm::vec3 fogColor2 = glm::vec3(0.631f, 0.651f, 0.698f);

Game::Game(unsigned int width, unsigned int height) {
	this->Width = width;
	this->Height = height;
    Camera camera();
}

Game::~Game() {

}

void Game::Init() {
    snowItem = new Snow::Snow();
    lightDir = glm::vec3(-1.2f, -1.0f, -2.0f);
    lightPos = glm::vec3(6.0f, 5.0f, 10.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Load Skybox
    ResourceManager::LoadShader("skybox.vs", "skybox.fs", nullptr, "skyboxShader");
    //Load ground
    ResourceManager::LoadShader("groundgrass.vs", "groundgrass.fs", nullptr, "grassShader");
    ResourceManager::LoadShader("groundInstance.vs", "groundInstance.fs", nullptr, "instanceShader");
    //Load shadowMap
    ResourceManager::LoadShader("shadowmap.vs", "shadowmap.fs", nullptr, "shadowShader");
    ResourceManager::LoadShader("debug_quad.vs", "debug_quad_depth.fs", nullptr, "debugDepthShader");
    // -------------------------------------------------------
    ResourceManager::LoadShader("glider.vs", "glider.fs", nullptr, "gliderShader");
    //
    ResourceManager::LoadShader("flameloading.vs", "flameloading.fs", nullptr, "flameShader");

    //Model
    //ResourceManager::LoadShader("modelloading.vs", "modelloading.fs", nullptr, "modelShader");



    //Load Skybox texture
    std::vector<std::string> faces;
    faces.push_back(std::string("resources/skybox/right.jpg"));
    faces.push_back(std::string("resources/skybox/left.jpg"));
    faces.push_back(std::string("resources/skybox/top.jpg"));
    faces.push_back(std::string("resources/skybox/bottom.jpg"));
    faces.push_back(std::string("resources/skybox/front.jpg"));
    faces.push_back(std::string("resources/skybox/back.jpg"));
    skyboxItem = new SkyboxRender(ResourceManager::GetShader("skyboxShader"), faces);

    //Load ShadowMap
    shadowItem = new ShadowMapRender(ResourceManager::GetShader("shadowShader"), lightPos, Width, Height);

    //Load Ground
    ResourceManager::LoadTexture("resources/ground/ambient.jpg", "groundAmbient");
    ResourceManager::LoadTexture("resources/ground/color.jpg","groundColor");
    ResourceManager::LoadTexture("resources/ground/height.jpg", "groundHeight");
    ResourceManager::LoadTexture("resources/ground/normal.jpg", "groundNormal");
    groundItem = new GroundRender(ResourceManager::GetShader("grassShader"), ResourceManager::GetShader("instanceShader"), lightDir, lightPos);
    groundItem->addTexture(ResourceManager::GetTexture("groundColor"), "groundColor");
    groundItem->addTexture(ResourceManager::GetTexture("groundColor"), "groundAmbient");
    groundItem->addTexture(ResourceManager::GetTexture("groundColor"), "groundHeight");
    groundItem->addTexture(ResourceManager::GetTexture("groundColor"), "groundNormal");

    //Load all model
    modelItem = new ModelRender(ResourceManager::GetShader("gliderShader"), lightDir, lightPos);
    modelItem->addModel("resources/objects/glider1/glider1.obj", "glider1");

    //Load flame

    // Fog
    Fog fog(fogDensity, fogColor1);
    fog.setGroundRender(groundItem);
}
//key
void Game::ProcessInput(float dt) {
    if (this->Keys[GLFW_KEY_W]) {
        modelItem->ProcessKeyboardMovementfromName("glider1", MODELPITCHUP, dt);
        Sdt = 0;
    }
        
    if (this->Keys[GLFW_KEY_S]) {
        modelItem->ProcessKeyboardMovementfromName("glider1", MODELPITCHDOWN, dt);
        Sdt = 0;
    }
    if (this->Keys[GLFW_KEY_A]) {
        modelItem->ProcessKeyboardMovementfromName("glider1", MODELROLLDOWN, dt);
        Sdt = 0;
    }
    if (this->Keys[GLFW_KEY_D]) {
        modelItem->ProcessKeyboardMovementfromName("glider1", MODELROLLUP, dt);
        Sdt = 0;
    }
}

void Game::Update(float dt) {
    // flying
    modelItem->TransFromName("glider1");
    crashTest(modelItem->boundingBox, groundItem);
    return;
}

void Game::Render(float deltaTime) {
    if (Sdt > 0) {
        modelItem->ResetRotateFromName("glider1");
    }
    Sdt++;
    //首先绘制shadowmap
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    groundItem->drawShadowMap(shadowItem, model);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绘制 glider
    glm::vec3 trans = modelItem->getTransFromName("glider1") + glm::vec3(5.2f, -2.8f, 2.8f);
    glm::vec3 angle = modelItem->GetAngleFromName("glider1");
    //std::cout << angle.x << " " << angle.y << " " << angle.z << std::endl;
    projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, trans);
    //std::cout << trans.x << " " << trans.y << " " << trans.z << std::endl;
    // std::cout << modelItem->GetFrontFromName("glider1").x << " " << modelItem->GetFrontFromName("glider1").y << " " << modelItem->GetFrontFromName("glider1").z << std::endl;
    camera.Forward(trans, modelItem->GetFrontFromName("glider1"), modelItem->GetUpFromName("glider1"));
    camera.setAngle(angle.y, angle.x-30);
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
    model = glm::rotate(model, glm::radians(angle.x), modelItem->GetRightFromName("glider1"));
    model = glm::rotate(model, glm::radians(90-angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelItem->drawModel("glider1", model, view, projection, camera.Position, shadowItem);

    //绘制地形
    projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    model = glm::mat4(1.0f);
    groundItem->drawGround(model, view, projection, camera.Position, shadowItem);

    //最后渲染天空盒
    projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    skyboxItem->drawSkyBox(view, projection);

    // snow
    model = glm::mat4(1.0f);
    // model = glm::translate(model, trans - glm::vec3(0.0f, 50.0f, 0.0f));
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 2000.0f);
    model = glm::translate(model, trans - glm::vec3(0.0f, 200.0f, 0.0f));
    //floor.render(model,view,projection);
    snowItem->Render(deltaTime, model, view, projection);
}

void Game::crashTest(std::vector<glm::vec3> recV, GroundRender* ground) {
    std::vector<float> xv;
    std::vector<float> yv;
    std::vector<float> zv;
    float calX = 0;
    float calY = 0;
    float calZ = 0;
    for (glm::vec3 onePoint : recV) {
        xv.push_back(onePoint.x);
        yv.push_back(onePoint.y);
        zv.push_back(onePoint.z);
        calX += onePoint.x;
        calY += onePoint.y;
        calZ += onePoint.z;
    }
    glm::vec3 gravy = glm::vec3(calX / 4, calY / 4, calZ / 4);
    float maxLen = -1;
    for (glm::vec3 onePoint : recV) {
        float tempLen = glm::length(onePoint - gravy);
        if (tempLen > maxLen) maxLen = tempLen;
    }
    float maxX = *(std::max_element(std::begin(xv), std::end(xv)));
    float minX = *(std::min_element(std::begin(xv), std::end(xv)));
    float maxZ = *(std::max_element(std::begin(zv), std::end(zv)));
    float minZ = *(std::min_element(std::begin(zv), std::end(zv)));
    if (maxX >= ground->groundWidth - 0.02) maxX = 0;
    if (minX < 0) minX = 0;
    if (maxZ >= ground->groundHeight - 0.02) maxZ = 0;
    if (minZ < 0) minZ = 0;
    int fromX = minX / ground->groundWidth * ground->widthNum;
    int toX = maxX / ground->groundWidth * ground->widthNum;
    int fromZ = minZ / ground->groundHeight * ground->heightNum;
    int toZ = maxZ / ground->groundHeight * ground->heightNum;
    for (int i = fromZ; i < toZ; i++) {
        for (int j = fromX; j < toX; j++) {
            glm::vec3 p = ground->mountData[i][j];
            float theLength = glm::length(p - gravy);
            if (theLength < maxLen * 3) {
                isCrash = true;
                return;
            }
        }
    }
}
void Game::explodeRender(float deltaTime)
{
    if (Sdt > 0) {
        modelItem->ResetRotateFromName("glider1");
    }
    Sdt++;
    //首先绘制shadowmap
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    groundItem->drawShadowMap(shadowItem, model);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绘制地形
    projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    model = glm::mat4(1.0f);
    groundItem->drawGround(model, view, projection, camera.Position, shadowItem);

    //绘制火焰


    //最后渲染天空盒
    projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    skyboxItem->drawSkyBox(view, projection);

    glm::vec3 trans = modelItem->getTransFromName("glider1") + glm::vec3(5.2f, -2.8f, 2.8f);
    model = glm::mat4(1.0f);
    // model = glm::translate(model, trans - glm::vec3(0.0f, 50.0f, 0.0f));
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 2000.0f);
    model = glm::translate(model, trans - glm::vec3(0.0f, 200.0f, 0.0f));
    //floor.render(model,view,projection);
    snowItem->Render(deltaTime, model, view, projection);
}